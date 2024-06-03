# -*- coding:utf-8 -*-
#
# Copyright (C) 2015-2020 Alibaba Group Holding Limited


from __future__ import print_function
import json
import shutil
import glob

from .tools import *
from .log import logger
#from tools import *

class DmacPort(object):
    def __init__(self, dic):
        self.name = ''
        self.idx = ''
        self.channel_num = ''
        for k,v in dic.items():
            self.__dict__[k] = v

def dmac_ports(dmac_port):
    ports = []
    for v in dcit_sorted(dmac_port, 'idx'):
        ports.append(DmacPort(v))
    return ports

class DmacTarget(object):
    def __init__(self, dic):
        self.device = ''
        self.vendor = ''
        self.channel = []          #dma channel
        for k,v in dic.items():
            if k in ['dma']:
                self.channel = v
            else:
                self.__dict__[k] = v

def dma_targets(dma_target):
    targets = []
    for v in dma_target:
        targets.append(DmacTarget(v))
    return targets

class TriggerSource(object):
    def __init__(self, dic):
        self.name = list(dic.keys())[0]
        self.dmac_hs = []

        for v in dcit_sorted(dic[self.name], 'idx'):
            self.dmac_hs.append(v['num'])

def trigger_source(list):
        trigger = []
        for v in list:
            trigger.append(TriggerSource(v))
        return trigger

class DmacTcip:
    def __init__(self, devices, device_attr):
        self.target_info = ''
        self.device_attr = device_attr['dma']
    def dev_attr_parsing(self):
        self.target_info += self.__g_dma_chnum()
        self.target_info += self.__handshake()
        self.target_info += self.__dma_ch_list()
        self.target_info += self.__dma_spt_list()
        return self.target_info
    def soc_attr_parsing(self):
        return ''
    def __g_dma_chnum(self):
        dma_ch_num = ''
        ports = dmac_ports(self.device_attr['dma_port'])

        for port in ports:
            dma_ch_num += str(port.channel_num) + ', '
        dma_ch_num = dma_ch_num[0 : -2]
        return 'const uint8_t g_dma_chnum[] = {' + dma_ch_num + '};\n\n'

    def __handshake(self):
        hs_num = ''
        trigger = trigger_source(self.device_attr['trigger_source'])
        for v in trigger:
            hs_num += 'const uint16_t ' + v.name + '_hs_num[] = {'
            for hs in v.dmac_hs:
                hs_num += str(hs) + ', '
            hs_num = hs_num[0 : -2]
            hs_num += '};\n'
        return hs_num + '\n'

    def __dma_ch_list(self):
        ch_list = ''
        targets = dma_targets(self.device_attr['dma_target'])
        for v in targets:
            i = 0
            ch_list += 'const csi_dma_ch_desc_t ' + v.device + '_dma_ch_list[] = {\n' + ' ' * 4
            ch_list += self.__dma_ch(v.channel)
            ch_list += '{0xff, 0xff}\n};\n\n'
        return ch_list

    def __dma_ch(self, ch):
        c = ''
        i = 0
        for v in ch:
            idx = str(v['idx'])
            for vv in v['channel']:
                i += 1
                c += '{' + str(idx) + ', ' + str(vv) + '}, '
                if i % 10 == 0 :
                    c += '\n' + ' ' * 4
        return c

    def __dma_spt_list(self):
        spt_list = 'const csi_dma_ch_spt_list_t dma_spt_list[] = {\n'
        targets = dma_targets(self.device_attr['dma_target'])
        for v in targets:
            dev_name = ''
            idx = 0
            if v.device[-1].isdigit():
                if v.device[-2].isdigit():
                    idx = v.device[-2: -1]
                    dev_name = v.device[0:-2]
                else:
                    idx = v.device[-1]
                    dev_name = v.device[0:-1]

            spt_list += 4 * ' ' + '{DEV_' + v.vendor.upper() + '_' + dev_name.upper() + '_TAG, ' \
                        + idx  + ', '+ v.device + '_dma_ch_list},\n'
        spt_list += '};\n'
        return spt_list

def test():
    chip_info = ChipInfo('../soc.yaml')
    dma = DmacTcip(chip_info.device_attr)
    dma.attr_parsing()
if __name__ == "__main__":
    test()

