# -*- coding:utf-8 -*-
#
# Copyright (C) 2015-2020 Alibaba Group Holding Limited
from __future__ import print_function
import os
import shutil
import pickle

from .tools import *
from .ip import *
from .log import logger
from .chip_info import *
from .ip_attr import *

soc_head = '''#ifndef _SOC_H_
#define _SOC_H_

#include "csi_core.h"
#include "stdint.h"
#include "sys_clk.h"

#ifdef __cplusplus
extern "C" {
#endif\n'''

soc_brief =  '''* @brief    CSI Core Peripheral Access Layer Header File for
*           CSKYSOC Device Series'''

soc_end = '''#ifdef __cplusplus
}
#endif

#endif  /* _SOC_H_ */'''

class DevSocInfo(object):
    def __init__(self, dic):
        self.dev_idx = str(dic.idx)
        if self.dev_idx == '':
            self.dev_idx = '0'
        self.tag = 'DEV_' + dic.vendor.upper() + '_' + dic.name.upper() + '_' + 'TAG'
        self.base_name = dic.vendor.upper() + '_' + dic.name.upper() + str(dic.idx) + '_BASE'
        self.base_val = None
        if dic.base:
            self.base_val = hex(dic.base)
        self.base_size_name = dic.vendor.upper() + '_' + dic.name.upper() + str(dic.idx) + '_SZIE'
        self.base_size_val = hex(dic.size)

        self.irq_n_name = None
        self.irq_n_val = None

        if dic.irq_n != '' and dic.irq_n != None:
            vendor = ''
            if dic.vendor.upper() != '':
                vendor = dic.vendor.upper() + '_'
            self.irq_n_name = vendor + dic.name.upper() + str(dic.idx) + '_IRQn'
            self.irq_n_val = str(dic.irq_n)


class SocDefine:
    def_max_len = 40
    def __init__(self, chip_info):
        self.txt = ''
        self.txt += self.__clock(chip_info.clock)
        self.txt += self.__irqn_type_t(chip_info.devices)
        self.txt += self.__dev_attr(chip_info.devices, chip_info.device_attr)
        self.txt += self.__cpu(chip_info.cpu)
        self.txt += self.__dev_base(chip_info.devices)
        gen = CreateCFile()
        license = gen.generate_license()

        desc = gen.generate_file_description('soc.h', soc_brief, 'v1.0')
        self.txt =  license + '\n' + desc + '\n' + soc_head + '\n' + self.txt + '\n' + soc_end

    def __clock(slef, clock):
        clk_def = ''
        for v in clock:
            name = v.name.upper() + '_VALUE'
            clk_def += '#ifndef ' + name + '\n'
            clk_def += '#define ' + name + (SocDefine.def_max_len - len(name)) * ' '
            clk_def += str(v.value) + '\n#endif\n\n'
        return clk_def

    def __irqn_type_t(self, device):
        irqn ='typedef enum {\n'
        for v in device:
            dev = DevSocInfo(v)
            if dev.irq_n_name == None:
                continue
            irqn += ' ' * 4 + dev.irq_n_name + (SocDefine.def_max_len - len(dev.irq_n_name)) *  ' '
            irqn += '= ' +  dev.irq_n_val + ',\n'
        irqn +='} irqn_type_t;\n\n'
        return irqn

    def __cpu(self, cpu):
        def_name = 'CONFIG_IRQ_NUM '
        txt = '#define ' + def_name + (SocDefine.def_max_len - len(def_name)) *  ' ' + str(cpu[0].irq_num) + '\n'
        return txt

    def __dev_attr(self, devices, attr):
        ip_attr = ''
        if attr == None:
            return ''

        for v in devices:
            try:
                if v.name.lower() in list(ip_attr_list.keys()):
                    dev = ip_attr_list[v.name.lower()](devices, attr)
                    if dev != None:
                        ip_attr += dev.soc_attr_parsing()
            except:
                logger.error('The %s attribute parsing error, please check soc.yaml' %(v.name.lower()))
                exit(-1)
        return ip_attr

    def __dev_base(self, device):
        dev_base = ''
        for v in device:
            if v.base == '':
                continue
            dev = DevSocInfo(v)
            dev_base += '#define ' + dev.base_name + (SocDefine.def_max_len \
                         - len(dev.base_name)) *  ' ' + dev.base_val + 'UL\n'

            dev_base += '#define '  + dev.base_size_name + (SocDefine.def_max_len \
                        - len(dev.base_size_name)) *  ' ' + dev.base_size_val + 'UL\n'

        return dev_base

def test():
    chip_info = ChipInfo('./soc.yaml')
    soc_h = SocDefine(chip_info)
    print(soc_h.txt)

if __name__ == "__main__":
    test()



