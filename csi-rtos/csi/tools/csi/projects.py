# -*- coding:utf-8 -*-
#
# Copyright (C) 2015-2020 Alibaba Group Holding Limited
from __future__ import print_function
import os, sys, stat
import shutil
import pickle

from yaml.events import SequenceStartEvent

from .ip import *
from .log import logger
from .tools import *
from .chip_info import *
from .package import *
from .cdk_prj import *
from .template import *
from .pin_scv import *

class PrjBoardInfo(object):
    def __init__(self, dic):
        self.name = None
        self.gcc_ld = None
        for k,v in dic.items():
            self.__dict__[k] = v

class ChipProjectInfo(object):
    def __init__(self, filename):
        prj_yaml = yaml_load(filename)
        self.common = ''
        self.drivers = ''
        self.kernel = ''
        self.sdk_version = ''
        self.define = []
        self.board = PrjBoardInfo({})
        for k,v in prj_yaml.items():
            if k in ['yaml_file_type']:
                continue
            if k in ['define', 'yaml_file_type', 'sdk_version', 'download_to_flash']:
                self.__dict__[k] = v
                continue
            if k in ['board']:
                self.board = PrjBoardInfo(v)
                continue
            if k not in ['common', 'drivers', 'kernel']:
                print('project.yaml type error: ', k)
                continue

            self.__dict__[k] = v

class Projects:
    def __init__(self, csi_rtos_path,   sdk_cmp_path, chip_info, prj_info):
        self.chip_info = chip_info
        self.csi_rtos = csi_rtos_path
        self.test_path = os.path.join(csi_rtos_path, 'csi-test')
        self.sdk_cmp_path = sdk_cmp_path
        self.exmaple_path = os.path.join(csi_rtos_path, 'examples')
        self.utilities = os.path.join(csi_rtos_path, 'tools/utilities/' + chip_info.vendor.chip)
        if not dir_is_exit(self.utilities):
            self.utilities = None
        self.chip_proj = prj_info
        self.projects = IPBox()
        project_info_yamls = self.__search_prj_info_yaml(['.'])
        for filename in project_info_yamls:
            ip = IP(filename)
            if not self.projects.add(ip):
                logger.error('ip `%s` is multiple (ip path: %s)' %
                             (ip.name,  ip.path))
                exit(0)
    def common(self, path, cdk = None):
        for k,v in self.chip_proj.common.items():
            outpath = os.path.join(path, k)
            has_sub = True
            if len(v) < 2 and k == v[0]:
                has_sub = False
            for vv in v:
                project = self.projects.get(vv, 'thead')
                if project == None:
                    logger.warn("can't find project: %s , %s " %(vv, 'thead'))
                    continue
                drv_path = outpath
                if has_sub:
                    drv_path = os.path.join(outpath, vv)
                project.data = project.versions['v1.0']
                logger.info("prj cp %s -> %s" %(project.data.drv_path, drv_path))
                self.__prjfile_copy(project.data.drv_path, drv_path, vv, [], cdk)


    def drivers(self, path, cdk = None):
        path = os.path.join(path, 'drivers')
        for k,v in self.chip_proj.drivers.items():
            outpath = os.path.join(path, k)
            has_sub = True
            if len(v) < 2 and k == v[0]:
                has_sub = False
            for vv in v:
                project = self.projects.get(vv, 'thead')
                if project == None:
                    logger.warn("can't find project: %s , %s " %(vv, 'thead'))
                    continue
                drv_path = outpath
                if has_sub:
                    drv_path = os.path.join(outpath, vv)
                project.data = project.versions['v1.0']
                logger.info("driver prj cp %s -> %s" %(project.data.drv_path, drv_path))
                self.__prjfile_copy(project.data.drv_path, drv_path, vv, [], cdk)

    def kernel(self, path, cdk = None):
        path = os.path.join(path, 'kernel')
        for k,v in self.chip_proj.kernel.items():
            outpath = os.path.join(path, k)
            has_sub = True
            if len(v) < 2 and k == v[0]:
                has_sub = False
            for vv in v:
                project = self.projects.get(vv, 'thead')
                if project == None:
                    logger.warn("can't find project: %s , %s " %(vv, 'thead'))
                    continue
                drv_path = outpath
                if has_sub:
                    drv_path = os.path.join(outpath, vv)
                project.data = project.versions['v1.0']
                logger.info("kernel prj cp %s -> %s" %(project.data.drv_path, drv_path))
                depends = []
                depends.append({k: 'V7.2.2'})
                self.__prjfile_copy(project.data.drv_path, drv_path, vv, depends, cdk)

    def testcase(self, path):
        files = os.listdir(self.test_path)
        path = os.path.join(path, 'testcase')
        for f in files:
            if f in ['dirver', 'Kconfig', 'tests.mk']:
                continue
            src = os.path.join(self.test_path, f)
            if dir_is_exit(src):
                dst = os.path.join(path, f)
            else:
                dst = path
            logger.info('testcase: cp %s -> %s' %(src, dst))
            copy(src, dst)


        drv_case = os.path.join(self.test_path, 'driver')
        for v in self.chip_info.devices:
            if v.is_driver == 0:
                continue
            src = os.path.join(drv_case, v.name.lower())
            dst = os.path.join(path, 'driver/' + v.name.lower())
            logger.info('testcase: cp %s -> %s' %(src, dst))
            copy(src, dst)

        prj_com = os.path.join(self.exmaple_path, 'common')
        copy(prj_com, path)
        write_file(' ', os.path.join(path, 'csi_config.h'))
        def_package = os.path.join(self.test_path, 'package')
        test_pkg = example_package(def_package, self.chip_info)
        for v in self.chip_proj.define:
            test_pkg.package.defconfig[v] = 1
        test_pkg.save(os.path.join(path, 'package'))

        flash_driver = self.chip_info.vendor.chip + '_flash.elf'
        makefile = scons_makefile_template.replace('<flash.elf>', flash_driver)

        if self.chip_info.cpu[0].arch in ['riscv', 'RISCV']:
            makefile = makefile.replace('<gdbinit>', '-x .gdbinit')
        else:
            makefile = makefile.replace('<gdbinit>', '')

        write_file(makefile, os.path.join(path, 'Makefile'))

        make_txt = '#!/bin/bash\ncd $(dirname $(readlink -f "$0"))\nmake\n'
        write_file(make_txt, os.path.join(path, 'make'))
        os.chmod(os.path.join(path, 'make'), stat.S_IRWXU|stat.S_IRWXG|stat.S_IRWXO)
        make_txt = '#!/bin/bash\ncd $(dirname $(readlink -f "$0"))\nmake flash\n'
        write_file(make_txt, os.path.join(path, 'makeflash'))
        os.chmod(os.path.join(path, 'makeflash'), stat.S_IRWXU|stat.S_IRWXG|stat.S_IRWXO)
        self.__test_tools(path)

    def __test_tools(self, outpath):
        test_tools_path = os.path.join(self.csi_rtos, 'csi-test/test_tools/')
        file_conf = files_conf_template.replace('<chip_name>', self.chip_info.vendor.chip)
        dst = os.path.join(outpath, 'test_tools')
        copy(test_tools_path, dst)
        write_file(file_conf, os.path.join(dst, 'files_conf.yaml'))

    def __prjfile_copy(self, src_path, outpath, prjname, prj_depends = [], create_cdk = None):
        prjsrc = os.path.join(outpath, 'src')
        prjinc = os.path.join(outpath, 'include')
        copy_type(src_path, prjsrc, '.c')
        copy_type(src_path, prjinc, '.h')
        write_file(' ', os.path.join(prjinc, 'csi_config.h'))
        prj_com = os.path.join(self.exmaple_path, 'common')
        copy(os.path.join(prj_com, 'board_init.c'), prjsrc)
        copy(os.path.join(prj_com, 'board_init.h'), prjinc)

        pkgsrc = os.path.join(src_path, 'package.yaml')
        example_pkg = example_package(pkgsrc, self.chip_info)
        for v in self.chip_proj.define:
            example_pkg.package.defconfig[v] = 1
        example_pkg.add_depend(prj_depends)
        example_pkg.save(os.path.join(outpath, 'package.yaml'))
        mkdir(os.path.join(prjinc, 'sys'))
        if self.chip_info.cpu[0].arch in ['riscv', 'RISCV']:
            copy(os.path.join(self.csi_rtos, 'include/sys'), os.path.join(prjinc, 'sys'))

        if create_cdk != None:
            self.__cdk_prj(prjname, example_pkg.get_depends(), outpath)
            return

        write_file(svc_chip_config_template, os.path.join(prjsrc, 'chip_config.c'))
        write_file(scons_sconstruct_template, os.path.join(outpath, 'SConstruct'))
        flash_driver = self.chip_info.vendor.chip + '_flash.elf'

        makefile = scons_makefile_template.replace('<flash.elf>', flash_driver)
        if self.chip_info.cpu[0].arch in ['riscv', 'RISCV']:
            makefile = makefile.replace('<gdbinit>', '-x .gdbinit')
        else:
            makefile = makefile.replace('<gdbinit>', '')

        write_file(makefile, os.path.join(outpath, 'Makefile'))

    def __cdk_prj(self, prjname, depends, outpath):
        cdk_prj_file = os.path.join(outpath, 'project.cdkproj')
        write_file(cdkprj_template, cdk_prj_file)
        cdk = CdkPrj(cdk_prj_file)
        cdk.prj_name = prjname
        cdk.cpu_name = self.chip_info.cpu[0].name
        cdk.cpu_number = str(self.chip_info.cpu[0].idx)
        cdk.chip_name = self.chip_info.vendor.chip
        cdk.depend_packages = depends
        cdk.flash_tools = './utilities/' + self.chip_info.vendor.chip + '_flash_cdk.elf'
        cdk.down_to_flash = 'no'
        if self.chip_proj.download_to_flash == True:
            cdk.down_to_flash = 'yes'

        cdk.depend_path = os.path.relpath(self.sdk_cmp_path, outpath)
        self.sdk_cmp_path = os.path.abspath(self.sdk_cmp_path)
        board_path = os.path.abspath(os.path.join(self.sdk_cmp_path, "../"))
        cdk.board_path = os.path.relpath(board_path, outpath)
        cdk.dump(cdk_prj_file)
        if dir_is_exit(self.utilities):
            copy(self.utilities, os.path.join(outpath, 'utilities'))
        else:
            utilities = ['pre_build.sh', 'aft_build.sh', 'flash.init', 'gdb.init', 'flash.elf']
            for v in utilities:
                dst = os.path.join(outpath, 'utilities/' + v)
                write_file(' ', dst)

        write_file(svc_chip_config_template, os.path.join(outpath, 'chip_config.c'))

        if self.chip_info.device_attr == None or \
           'ioc' not in self.chip_info.device_attr.keys():
            return
        pin_json = CdkPinJson(self.chip_info)
        pin_json.save(os.path.join(outpath, 'utilities/pin_name.json'))
        cdk_svc_file = os.path.join(outpath, 'utilities/chip_config.svc')
        write_file(svc_xml_template, cdk_svc_file)
        pin_svc = CdkPinSvc(self.chip_info, cdk_svc_file)
        pin_svc.save(cdk_svc_file)
        copy(self.csi_rtos + '/tools/utilities/chip_config.dll', os.path.join(outpath, 'utilities/'))

    def __search_prj_info_yaml(self, subpath=[]):
        paths = []
        if subpath:
            for sub in subpath:
                p = os.path.join(self.exmaple_path, sub)
                if os.path.exists(p):
                    paths.append(p)
        else:
            paths.append(self.exmaple_path)

        package_list = []
        while paths:
            path = paths[0]
            filename = os.path.join(path, 'info.yaml')
            if os.path.isfile(filename):
                package_list.append(filename)
            else:
                files = os.listdir(path)
                for file in files:
                    p = os.path.join(path, file)
                    if os.path.isdir(p):
                        paths.append(p)
            del paths[0]
        return package_list


