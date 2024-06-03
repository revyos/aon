# -*- coding:utf-8 -*-
#
# Copyright (C) 2015-2020 Alibaba Group Holding Limited
from __future__ import print_function
import os
import shutil
import pickle

from .ip import *
from .log import logger
from .tools import *
from .chip_info import *
from .chip_soc import *
from .package import *

class BoardComponents:
    def __init__(self, chip_info, csi_rtos, prj_board):
        self.chip_info = chip_info
        self.board_config = None
        self.chip_vendor = chip_info.vendor.chip_vendor
        self.chip_board = prj_board
        if self.chip_board.name == None:
            self.chip_board.name = chip_info.vendor.chip + '_evb'
        if self.chip_board.gcc_ld == None:
            self.chip_board.gcc_ld = 'gcc_mem.ld'

        self.csi_rtos_board_path = os.path.join(csi_rtos, 'boards')

        # scanning yoc all ips
        self.boards = IPBox()
        board_info_yamls = self.__search_board_info_yaml(
            ['.'])
        for filename in board_info_yamls:
            ip = IP(filename)

            if not self.boards.add(ip):
                logger.error('ip `%s` is multiple (ip path: %s)' %
                             (ip.name,  ip.path))
                exit(0)
    def dump(self, outpath):
        filename = os.path.join(outpath, 'package.yaml')
        board_pkg = BoardPackage(self.chip_info)
        board_pkg.package.hw_info.ld_script = os.path.join('./', self.chip_board.gcc_ld)
        board_pkg.save(filename)
        if self.chip_board.name != None and self.chip_board.name != '':
            csi_board = self.boards.get(self.chip_board.name, self.chip_vendor)
            if csi_board != None:
                csi_board.data = csi_board.versions['v1.0']
                copy(csi_board.data.drv_path, outpath)
                remove(os.path.join(outpath, 'info.yaml'))
                for v in os.listdir(outpath):
                    if '.ld' in v and v != self.chip_board.gcc_ld:
                        remove(os.path.join(outpath, v))
            else:
                logger.error('Generate board component error, can\'t find: ', self.chip_board.name)
                exit(0)
        board_yaml = os.path.join(outpath, 'board_config.yaml')
        if not file_is_exit(board_yaml):
            write_file(self.board_config_yaml(), board_yaml)

        if self.board_config == None:
            pass
        else:
            pass
    def __board_config_h(self):
        pass

    def board_config_yaml(self):
        if self.chip_info.device_attr == None:
            return ''
        text = 'board_config:\n'
        item = 	'name: {0:20} idx: {1:20} func: {2:20} pin: {3:20} pin_func: {4:20}'
        chip_io = IoctrlTcip(self.chip_info.devices, self.chip_info.device_attr)
        for v in self.chip_info.devices:
            if v.name in ['GPIO']:
                continue
            pin_map = chip_io.pinmap(v.name)
            for vv in pin_map:
                text += '    -{' + item.format(vv.dev_name + ',', str(vv.idx) + ',', vv.func + ',', vv.pin_name + ',', vv.pin_func) + '}\n'
        return text

    def __search_board_info_yaml(self, subpath=[]):
        paths = []
        if subpath:
            for sub in subpath:
                p = os.path.join(self.csi_rtos_board_path, sub)
                if os.path.exists(p):
                    paths.append(p)
        else:
            paths.append(self.csi_rtos_board_path)

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


