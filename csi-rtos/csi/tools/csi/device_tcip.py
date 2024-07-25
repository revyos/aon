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
from .soc_tcip import *
from .ip_attr import *

device_brief = ' * @brief    source file for the devices'

class DevicesTcip:
    def_max_len = 30
    def __init__(self, chip_info):
        self.txt = ''
        gen = CreateCFile()
        license = gen.generate_license()
        desc = gen.generate_file_description('devices.c', device_brief, 'v1.0')
        self.txt += license + '\n' + desc + '\n' + self.__dev_head(chip_info.devices) + '\n'
        self.txt += self.__g_soc_info(chip_info.devices)
        self.txt += self.__dev_attr(chip_info.devices, chip_info.device_attr)
    def __dev_head(self, devices):
        inc = '#include '
        h = inc + '<stdio.h>\n' + inc + '<csi_config.h>\n' + inc + '<soc.h>\n' \
            + inc + '<drv/pin.h>\n'
        for v in devices:
            if v.is_driver == 0:
                continue
            name = v.name.lower()
            if name in ['pinmux', 'pmu']:
                continue
            h_name = inc + '<drv/' + name + '.h>'
            if h_name in h:
                continue
            h += h_name + '\n'
        return h

    def __g_soc_info(self, devices):
        g_soc_info = 'const csi_perip_info_t g_soc_info[] = {\n'
        for v in devices:
            if v.is_driver == 0 or v.base == '':
                continue
            dev = DevSocInfo(v)
            if dev.irq_n_name == None:
                irq_n_name = '-1'
            else:
                irq_n_name = dev.irq_n_name

            g_soc_info += 4 * ' ' + '{' + dev.base_name + ',' + \
                          + (DevicesTcip.def_max_len - len(dev.base_name))  * ' ' \
                          + irq_n_name + ',' + (DevicesTcip.def_max_len - len(irq_n_name)) * ' ' \
                          + dev.dev_idx + ',' + 4 * ' ' + dev.tag + '},\n'
        g_soc_info += 4 * ' ' + '{0, 0, 0, 0}};\n\n'
        return g_soc_info

    def __dev_attr(self, devices, attr):
        ip_attr = ''
        for v in devices:
            if v.name.lower() in list(ip_attr_list.keys()):
                dev = ip_attr_list[v.name.lower()](devices, attr)
                if dev != None:
                    ip_attr += dev.dev_attr_parsing()
        return ip_attr

def test():
    chip_info = ChipInfo('./soc.yaml')
    devices = DevicesTcip(chip_info)

if __name__ == "__main__":
    test()

