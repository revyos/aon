# -*- coding:utf-8 -*-
#
# Copyright (C) 2015-2020 Alibaba Group Holding Limited
from __future__ import print_function
import os
import shutil
import pickle
import datetime

from .ip import *
from .log import logger
from .tools import *
from .chip_info import *
from .soc_tcip import *
from .device_tcip import *
from .csi import *
from .chip_soc import *
from .chip_components import *
from .board_component import *
from .projects import *
from .template import *

class Sdk:
    def __init__(self,    csi_path, csi_rtos_path, soc_yaml, project_yaml):
        self.chip_info = ChipInfo(soc_yaml)
        self.prj_info = ChipProjectInfo(project_yaml)
        self.chip = ChipComponents(csi_path, csi_rtos_path, self.chip_info)
        self.board = BoardComponents(self.chip_info, csi_rtos_path, self.prj_info.board)
        self.csi_path = csi_path
        self.rtos_path = csi_rtos_path
        self.soc_yaml = soc_yaml
        current = datetime.datetime.now()
        current = datetime.datetime.strftime(current,'%Y-%m-%d-%H_%M_%S')
        self.sdk_data = str(current)

    def socns_dump(self, outpath, sdk_name = '', tests=''):
        if not sdk_name:
            sdk_name = self.chip_info.vendor.chip + '_evb_' + self.prj_info.sdk_version

        sdk_path = os.path.join(outpath, sdk_name)
        txt = 'yoc_version: V7.2\ncommit: 0'
        write_file(txt, os.path.join(sdk_path, '.yoc'))
        sdk_project_path = os.path.join(sdk_path, 'projects')
        sdk_cmp_path = os.path.join(sdk_path, 'components')
        sdk_chip_cmp_path = os.path.join(sdk_cmp_path, 'chips/chip_' + self.chip_info.vendor.chip)
        sdk_doc_path = os.path.join(sdk_path, 'docs')
        sdk_board_path = os.path.join(sdk_path, 'boards/' + self.chip_info.vendor.chip + '_evb')

        self.project = Projects(self.rtos_path, sdk_cmp_path,self.chip_info, self.prj_info)
        copy(self.soc_yaml, sdk_chip_cmp_path)
        self.chip.components(sdk_chip_cmp_path)
        self.chip.depends(sdk_cmp_path)
        self.project.common(sdk_project_path)
        self.project.drivers(sdk_project_path)
        self.project.kernel(sdk_project_path)
        if tests == 'autotest':
            self.project.testcase(sdk_project_path)
            self.chip.create_drivers_files_list(os.path.join(sdk_project_path, 'testcase/drivers_file.yaml'))
        self.board.dump(sdk_board_path)
        self.sdk_license(sdk_path)
        self.sdk_readme(sdk_path)
        self.chip.doc(sdk_doc_path)

        zip_path(sdk_path, sdk_path + '_' + self.sdk_data + '.zip')
        cmd = 'find ' + sdk_path + ' -name "*.o" | xargs rm -f'
        os.system(cmd)
        cmd = 'find ' + sdk_path + ' -name "*.d" | xargs rm -f'
        os.system(cmd)

    def cdk_dump(self, outpath, sdk_name = ''):
        if not sdk_name:
            sdk_name = self.chip_info.vendor.chip + '_evb_' + self.prj_info.sdk_version + '_cdk'
        else:
            sdk_name += '_cdk'

        sdk_path = os.path.join(outpath, sdk_name)
        sdk_project_path = os.path.join(sdk_path, 'projects')
        sdk_cmp_path = os.path.join(sdk_path, 'components')
        sdk_chip_cmp_path = os.path.join(sdk_cmp_path, 'chips/chip_' + self.chip_info.vendor.chip + '/v1.0')
        sdk_doc_path = os.path.join(sdk_path, 'docs')
        sdk_board_path = os.path.join(sdk_path, 'boards/' + self.chip_info.vendor.chip + '_evb' + '/v1.0')
        self.project = Projects(self.rtos_path, sdk_cmp_path,self.chip_info, self.prj_info)
        self.chip.components(sdk_chip_cmp_path)
        self.chip.depends(sdk_cmp_path, 'v1.0')
        self.project.common(sdk_project_path, True)
        self.project.drivers(sdk_project_path, True)
        self.project.kernel(sdk_project_path, True)
        self.board.dump(sdk_board_path)
        self.sdk_license(sdk_path)
        self.sdk_readme(sdk_path)
        self.chip.doc(sdk_doc_path)
        cmd = 'find ' + sdk_path + ' -name "*.o" | xargs rm -f'
        os.system(cmd)
        cmd = 'find ' + sdk_path + ' -name "*.d" | xargs rm -f'
        os.system(cmd)
        zip_path(sdk_path, sdk_path + '_' + self.sdk_data + '.zip')
    def dump_chip_cmp(self, outpath):
        self.chip.components(outpath)

    def dump_doc(self, outpath):
        self.chip.doc(outpath)

    def sdk_license(self, outpath):
        filename = os.path.join(outpath, 'LICENSE')
        write_file(sdk_license_template, filename)

    def sdk_readme(self, outpath):
        txt = self.chip_info.vendor.chip + ' sdk\nversion:' + self.chip_info.vendor.version
        filename = os.path.join(outpath, 'README.txt')
        write_file(txt, filename)
