# -*- coding:utf-8 -*-
#
# Copyright (C) 2015-2020 Alibaba Group Holding Limited
from __future__ import print_function
import os
import shutil
import pickle

#from .log import logger
from .tools import *
import xml.etree.ElementTree as ET
from .template import *

class CdkPackage:
    def __init__(self):
        self.ID = ''
        self.Path = ''
        self.Version = 'V1.0'
        self.IsBasic = "false"
    def attrib(self):
        val = {}
        for k, v in self.__dict__.items():
            val[k] = v
        return val

class CdkPrj:
    def __init__(self, cdk_file):
        self.prj_xml = XmlTree(cdk_file)
        self.prj_name = 'project'
        self.cpu_name = ''
        self.cpu_number = '0'
        self.chip_name = ''
        self.depend_path = ''
        self.board_path = ''
        self.depend_packages = []
        self.flash_tools = ''
        self.down_to_flash = 'no'

        self.__packages = self.prj_xml.find('Packages')
        self.__cpu = self.prj_xml.find('CPU')
        self.__cpu_number = self.prj_xml.find('CPUNumber')
        self.__chip = self.prj_xml.find('Chip')
        self.__board = self.prj_xml.find('Board')
        self.__down_to_flash = self.prj_xml.find('DownloadToFlash')
        self.__flash_tools = self.prj_xml.find('Algorithms')
        self.__flash_tools.text = 'utilities/flash.elf'
        self.__prj_name = self.prj_xml.find('Project')
        if self.__chip == None or self.__board == None:
            print('cdkprj error!')
            exit(-1)

        self.define = ''

    def __cdk_project_name(self):
        self.__prj_name.attrib = {'Name': self.prj_name, 'Version':'1'}

    def __cdk_depends(self):
        pkg = CdkPackage()
        for p in self.depend_packages:
            for k, vv in p.items():
                pkg.ID = k
                pkg.Path = self.depend_path
            if pkg.ID == 'chip_' + self.chip_name or \
               pkg.ID == self.chip_name + '_evb':
               continue
            iteam = ET.Element('Package')
            iteam.attrib = pkg.attrib()
            self.__packages.append(iteam)

    def __cdk_chip(self):
        iteam = ET.Element('Package')
        chip_pkg = CdkPackage()
        chip_pkg.ID = 'chip_' + self.chip_name
        chip_pkg.Path = os.path.join(self.depend_path, 'chips')
        self.__chip.attrib = chip_pkg.attrib()

    def __cdk_board(self):
        iteam = ET.Element('Package')
        board_pkg = CdkPackage()
        board_pkg.ID = self.chip_name + '_evb'
        board_pkg.Path = os.path.join(self.board_path, 'boards')
        board_pkg.IsBasic = 'false'
        self.__board.attrib = board_pkg.attrib()

    def __cdk_cpu(self):
        self.__cpu.text = self.cpu_name
        self.__cpu_number.text = self.cpu_number

    def __cdk_flash_tools(self):
        self.__flash_tools.text = self.flash_tools

    def __cdk_down_to_flash(self):
        self.__down_to_flash.text = self.down_to_flash

    def dump(self, filename):
        self.__cdk_project_name()
        self.__cdk_depends()
        self.__cdk_chip()
        self.__cdk_board()
        self.__cdk_cpu()
        self.__cdk_flash_tools()
        self.__cdk_down_to_flash()
        self.prj_xml.write(filename)
