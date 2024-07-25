# -*- coding:utf-8 -*-
#
# Copyright (C) 2015-2020 Alibaba Group Holding Limited
from __future__ import print_function
import os
import shutil
import pickle
import re
import subprocess

from .ip import *
from .log import logger
from .tools import *
from .chip_info import *
from .soc_tcip import *
from .device_tcip import *
from .csi import *
from .chip_soc import *
from .package import *
from .qeum import *

class ChipComponents:
    def __init__(self,    csi_path, csi_rtos_path, chip_info):
        csi = CSI(csi_path)
        self.csi_ips = csi.ips
        self.csi_path = csi_path
        self.csi_rtos_path = csi_rtos_path
        self.chip_info = chip_info
        self.drv_ips = csi.chip_dev(self.chip_info.devices)
        self.cpu_ips = csi.chip_cpu(self.chip_info.cpu)
        self.ips = self.drv_ips + self.cpu_ips
        self.chip_soc = None
        self.chip_name = chip_info.vendor.chip
        self.chip_vendor = chip_info.vendor.chip_vendor

    def create_drivers_files_list(self, path):
        file_list = {}
        for v in self.drv_ips:
            file_list[v.name.lower()] = os.listdir(v.data.drv_path)
        yaml_dump(file_list, path)

    def components(self, outpath):
        chip_cmp_drv = os.path.join(outpath, 'drivers')
        chip_cmp_inc = os.path.join(outpath, 'include')
        chip_cmp_sys = os.path.join(outpath, 'sys')
        chip_cmp_drv_ll = os.path.join(chip_cmp_drv, 'll')

        for v in self.drv_ips:
           # if v.name in ["USB", "usb"]:
           #     drv_dir = ["device", "host"]
           #     for vv in drv_dir:
           #         src_path = os.path.join(v.data.drv_path, vv)
           #         copy_type(src_path, chip_cmp_drv, '.c')
           #         copy_type(src_path, chip_cmp_drv, '.S')
           #         copy_type(src_path, chip_cmp_inc, '.h')
           #     continue
            if v.data.drv_common != None:
                copy_type(v.data.drv_common, chip_cmp_drv, '.c')
                copy_type(v.data.drv_common, chip_cmp_inc, '.h')
            logger.info('cp %s, %s' %(v.data.drv_path, outpath))
            copy_type(v.data.drv_path, chip_cmp_drv, '.S')
            copy_type(v.data.drv_path, chip_cmp_drv, '.c')
            copy_type(v.data.drv_path, chip_cmp_inc, '.h')

        for v in self.cpu_ips:
            copy_type(v.data.drv_path, chip_cmp_sys, '.S')
            copy_type(v.data.drv_path, chip_cmp_sys, '.c')
            copy_type(v.data.drv_path, chip_cmp_sys, '.h')

        ##tcip sys_freq ...##
        drv_common = os.path.join(self.csi_path, 'drivers/common')
        copy(drv_common, chip_cmp_sys)
        csi_chip = self.csi_ips.get(self.chip_name, self.chip_vendor)
        qeum_xml = Qeum(self.chip_info, os.path.join(outpath, self.chip_name + '.xml'))
        qeum_xml.save()
        if csi_chip != None:
            csi_chip.data = csi_chip.versions['v1.0']
            copy_type(csi_chip.data.drv_path, chip_cmp_sys, '.S')
            copy_type(csi_chip.data.drv_path, chip_cmp_sys, '.c')
            copy_type(csi_chip.data.drv_path + '/include', chip_cmp_sys, '.h')
            copy_type(csi_chip.data.drv_path, outpath, '.xml')
        else:
            self.chip_soc = ChipSoc(self.chip_info)
            write_file(self.chip_soc.soc_h, os.path.join(chip_cmp_sys, 'soc.h'))
            write_file(self.chip_soc.device_c, os.path.join(chip_cmp_sys, 'devices.c'))

        package_name = os.path.join(outpath, 'package.yaml')
        package = chip_package(self.chip_info)
        if self.chip_info.cflag != None:
            package.append_cflag(self.chip_info.cflag)

        for v in self.chip_info.define:
            package.package.defconfig[v] = 1

        package.save(package_name)


        for f in os.listdir(chip_cmp_drv):
            if '_ll.c' in f:
                movefile(os.path.join(chip_cmp_drv, f), os.path.join(chip_cmp_drv_ll, f))

    def doc(self, outpath):
        self.__chip_doc_dump(outpath)
        self.__chip_api_doc_dump(outpath)

    def depends(self, outpath, sub_path = ''):
        self.__def_component_dump(outpath, sub_path)
        self.__csi_cmponent_dump(outpath, sub_path)

    def __def_component_dump(self, cmp_path, sub_path = ''):
        csi_def_cmp = {'libs': ['console', 'minilibc', 'mm']}
        for ip in self.drv_ips:
            if not ip.depends:
                continue
            for v in ip.depends:
                if v not in csi_def_cmp['libs']:
                    csi_def_cmp['libs'].append(v)

        #driver depends components
        for k, v in csi_def_cmp.items():
            root_path = os.path.join(self.csi_path, k)
            for vv in v:
                src = os.path.join(root_path, vv)
                dst = os.path.join(cmp_path, vv + '/' + sub_path)
                copy(src, dst)

        #kernel components
        src = os.path.join(self.csi_path, 'csi_kernel')
        for v in os.listdir(src):
            if v in ['include', 'ucos3']:
                continue
            kernel = os.path.join(src, v)
            if not dir_is_exit(kernel):
                continue
            dst = os.path.join(cmp_path, v + '/' + sub_path)
            copy(kernel, dst)
            copy(src + '/include', dst)

            cpu_num = re.sub("\D", "", self.chip_info.cpu[0].name)
            pkg_txt = read_file(os.path.join(dst, 'package.yaml'))

            if v == 'rhino':
                if self.chip_info.cpu[0].arch in ['csky', 'CSKY']:
                    pkg_txt = pkg_txt.replace('<tspend>', '')

                if self.chip_info.cpu[0].name in ['e906fd']:
                    cpu_num = 'rv32fd_32gpr'
                    pkg_txt = pkg_txt.replace('<tspend>', 'tspend')
                if self.chip_info.cpu[0].name in ['e906']:
                    cpu_num = 'rv32_32gpr'
                    pkg_txt = pkg_txt.replace('<tspend>', 'tspend')

            elif v == 'freertos':
                tspend = 'tspend'
                security = 'security'
                if self.chip_info.cpu[0].name in ['e906fd']:
                    cpu_num = 'rv32fd_32gpr'
                if self.chip_info.cpu[0].name in ['e906']:
                    cpu_num = 'rv32_32gpr'
                if self.chip_info.cpu[0].name in ['ck804ef']:
                    cpu_num = '804df'
                if self.chip_info.cpu[0].name in ['ck804f']:
                    cpu_num = '804f'

                if 'pangu' in self.chip_name:
                    pkg_txt = pkg_txt.replace('<tspend>', 'non_tspend')
                else:
                    pkg_txt = pkg_txt.replace('<tspend>', tspend)

                if self.chip_info.cpu[0].arch in ['csky', 'CSKY']:
                    if 'pangu' in self.chip_name:
                        pkg_txt = pkg_txt.replace('<security>', 'non_security')
                    else:
                        pkg_txt = pkg_txt.replace('<security>', security)
                elif self.chip_info.cpu[0].arch in ['riscv', 'RISCV']:
                    pkg_txt = pkg_txt.replace('<security>', '')

            pkg_txt = pkg_txt.replace('<arch>', self.chip_info.cpu[0].arch)
            pkg_txt = pkg_txt.replace('<cpu_num>', cpu_num)
            write_file(pkg_txt, os.path.join(dst, 'package.yaml'))


        csi_rtos_def_cmp = {}
        for k, v in csi_rtos_def_cmp.items():
            root_path = os.path.join(self.csi_rtos_path, k)
            for vv in v:
                src = os.path.join(root_path, vv)
                dst = os.path.join(cmp_path, vv + '/' + sub_path)
                copy(src, dst)

    def __csi_cmponent_dump(self, cmp_path, sub_path = ''):
        src_path = os.path.join(self.csi_path, 'csi')
        dst_path = os.path.join(cmp_path, 'csi' + '/' + sub_path)
        copy(src_path, dst_path)
        rm_file = os.path.join(dst_path, 'Kconfig')
        remove(rm_file)
        rm_file = os.path.join(dst_path, 'include/drv')
        remove(rm_file)
        rm_file = os.path.join(dst_path, 'docs')
        remove(rm_file)
        csi_heard_path = os.path.join(src_path, 'include/drv')
        dst_path = os.path.join(dst_path, 'include/drv')
        for ip in self.ips:
            h_file = os.path.join(csi_heard_path, ip.name.lower() + '.h')
            copy(h_file, dst_path)
            if ip.name in ['USI']:
                copy(os.path.join(csi_heard_path, 'usi_iic.h'), dst_path)
                copy(os.path.join(csi_heard_path, 'usi_usart.h'), dst_path)
                copy(os.path.join(csi_heard_path, 'usi_spi.h'), dst_path)
            if ip.name in ['USI_SPI']:
                copy(os.path.join(csi_heard_path, 'spi.h'), dst_path)
            if ip.name in ['USI_IIC']:
                copy(os.path.join(csi_heard_path, 'iic.h'), dst_path)

        com_h = ['common.h', 'dev_tag.h', 'clk.h', 'errno.h', 'intc.h', 'porting.h', \
                 'pin.h', 'irq.h', 'tick.h', 'io.h', 'list.h', 'cache_porting.h', 'ringbuffer.h', 'pm.h']
        for v in com_h:
            h_file = os.path.join(csi_heard_path, v)
            copy(h_file, dst_path)

    def __chip_doc_dump(self, outpath = '.'):
        return
        doc_txt = '<font  size=50>' +self.chip_info.vendor.chip + ' data sheet</font>\n\n[TOC]'
        for v in self.drv_ips:
            if not file_is_exit(v.data.doc):
                continue
            doc_txt += read_file(v.data.doc)
            if dir_is_exit(v.data.pictures):
                dst = os.path.join(outpath, os.path.basename(v.data.pictures))
                logger.info('cp %s, %s' %(v.data.pictures, dst))
                copy(v.data.pictures, dst)

        file_name = self.chip_info.vendor.chip + '_data_sheet.md'
        write_file(doc_txt, os.path.join(outpath, file_name))

    def __chip_api_doc_dump(self, outpath = '.'):
        drv_md = '* [%s](%s)\n'
        doc_txt = '# Summary\n'
        doc_txt += drv_md %('COMMON', 'COMMON.md')
        api_doc_path = os.path.join(self.csi_path, 'csi/docs/drv')
        copy(api_doc_path, os.path.join(outpath, 'api_docs'))
        for ip in self.drv_ips:
            if ip.name in ['USI_SPI']:
                ip.name = 'SPI'
            if ip.name in ['USI_IIC']:
                ip.name = 'IIC'
            if file_is_exit(os.path.join(api_doc_path, ip.name.upper() + '.md')):
                doc_txt += drv_md %(ip.name.upper(), ip.name.upper() + '.md')
            else:
                logger.warn("Can't find %s.md!!!" %(ip.name.upper()))

        def_doc = ['PIN', 'PM']
        for v in def_doc:
            doc_txt += drv_md %(v, v + '.md')

        file_name = self.chip_info.vendor.chip + '_api.pdf'
        file_name = os.path.join(outpath, file_name)
        write_file(doc_txt, os.path.join(os.path.join(outpath, 'api_docs/SUMMARY.md')))
        logger.info('create pdf...')
        cmd = 'gitbook pdf '+ os.path.join(outpath, 'api_docs') + ' ' + file_name
        logger.info(cmd)

        try:
          if os.system(cmd) != 0:
              raise Exception(cmd + ' does not exist')
        except:
            logger.warn("create pdf failed, plases install gitbook:\n%s" \
                        %("sudo apt-get install npm\nsudo npm install gitbook -g\nsudo npm install gitbook-cli -g"))

        remove(os.path.join(outpath, 'api_docs'))

