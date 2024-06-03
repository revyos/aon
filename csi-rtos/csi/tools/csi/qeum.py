# -*- coding:utf-8 -*-
#
# Copyright (C) 2015-2020 Alibaba Group Holding Limited
from __future__ import print_function
import os
import shutil
import pickle

from .log import logger
from .tools import *
import xml.etree.ElementTree as ET
from .template import *


class QeumIteamBase(object):
    def __init__(self):
        pass
    def attr(self):
        dic = {}
        for k, v in self.__dict__.items():
            dic[k] = v
        return dic
    def show(self):
        print(self.attr())

class QeumBoard(QeumIteamBase):
    def __init__(self, name = '', arch = '', cskysimv = ''):
        self.name = name
        self.arch = arch
        self.cskysimv = cskysimv

class QeumCpuInfo(QeumIteamBase):
    def __init__(self, cpu = '', endian = '', abi = ''):
        self.cpu = cpu
        self.endian = endian
        self.abi = abi

class QeumDev(QeumIteamBase):
    def __init__(self, name = '', filename = '', addr = '', type = '', extra = '', irq = ''):
        self.name = name
        self.filename = filename
        self.addr = addr
        self.type = type
        self.extra = extra
        self.irq = irq

class QeumMem(QeumIteamBase):
    def __init__(self, name = '', addr = '', size = '', attr = ''):
        self.name = name
        self.addr = addr
        self.size = size
        self.attr = attr

class QeumCpuProp(QeumIteamBase):
    def __init__(self, name, value):
        self.name = name
        self.value = value

class QeumFile:
    def __init__(self, qeum_cfg_file):
        self.qeum_cfg_file = qeum_cfg_file
        self.qeum_xml = XmlTree(qeum_cfg_file)
        self.Board = self.qeum_xml.find('Board')
        self.cpu_info = self.qeum_xml.find('cpu_info')
        self.__dyndev = self.qeum_xml.find('dyndev')
        self.__memory = self.qeum_xml.find('memory')
        self.__cpu_prop = self.qeum_xml.find('cpu_prop')

    def dyndev_add(self, dic):
        iteam = ET.Element('dev')
        iteam.attrib = dic
        self.__dyndev.append(iteam)

    def memory_add(self, dic):
        iteam = ET.Element('mem')
        iteam.attrib = dic
        self.__memory.append(iteam)

    def cpu_prop_add(self, dic):
        iteam = ET.Element('cpu')
        iteam.attrib = dic
        self.__cpu_prop.append(iteam)

    def cpu_info_set(self, dic):
        self.cpu_info.attrib = dic

    def board_set(self, dic):
        self.Board.attrib = dic
    def save(self):
        self.qeum_xml.write(self.qeum_cfg_file )

class Qeum:
    def __init__(self, chip_info, filename):
        self.qeum_csky_cpu_coret = QeumDev('csky_coret',     'builtin',        '0xe000e000',  'TIMER',     '', '1')
        self.qeum_csky_tcip = QeumDev('csky_tcip_v1',     'builtin',        '0xe000e100',  'INTC',     '', '31')

        self.qeum_dev_map = {'DW_GPIO':   QeumDev('csky_gpio',      'csky_gpio',      '',  'GPIO',      '', ''),
                             'WJ_SDIO':   QeumDev('csky_sdio',      'csky_sdio',      '',  'SDIO',      '', ''),
                             'DW_IIC':    QeumDev('csky_i2c',       'csky_i2c',       '',  'I2C',       '', ''),
                             'DW_SPI':    QeumDev('csky_spi',       'csky_spi',       '',  'SPI',       '', ''),
                             'DW_TIMER':  QeumDev('csky_timer',     'csky_timer',     '',  'TIMER',     '', ''),
                             'WJ_PWM':    QeumDev('csky_pwm',       'csky_pwm',       '',  'PWM',       '', ''),
                             'DW_WDT':    QeumDev('csky_wdt',       'csky_wdt',       '',  'WDT',       '', ''),
                             'WJ_DMA':    QeumDev('csky_wj_dmac',   'csky_wj_dmac',   '',  'WJ_DMAC',   '', ''),
                             'WJ_AES':    QeumDev('csky_wj_aes',    'csky_wj_aes',    '',  'WJ_AES',    '', ''),
                             'WJ_SHA':    QeumDev('csky_wj_sha',    'csky_wj_sha',    '',  'WJ_SHA',    '', ''),
                             'DW_BBC':    QeumDev('csky_bbc',       'csky_bbc',       '',  'BBC',       '', ''),
                             'DW_UART':   QeumDev('csky_uart',      'csky_uart',      '',  'UART',      '', ''),
                             'WJ_ADC':    QeumDev('csky_adc',       'csky_adc',       '',  'ADC',       '', ''),
                             'WJ_ETB':    QeumDev('csky_wj_etb',    'csky_wj_etb',    '',  'ETB',       '', ''),
                             'CD_QSPI':   QeumDev('csky_qspi',      'csky_qspi',      '',  'QSPI',      '', ''),
                             'WJ_I2S':    QeumDev('csky_iis',       'csky_iis',       '',  'IIS',       '', ''),
                             'WJ_USI':    QeumDev('csky_usi',       'csky_usi',       '',  'WJ USI',    '', ''),
                             'WJ_EFLASH': QeumDev('csky_eflash',    'csky_eflash',    '',  'EFLASH',    '', ''),
                             'WJ_RNG':    QeumDev('csky_wj_trng',   'csky_wj_trng',   '',  'TRNG',      '', ''),
                             'WJ_RSA':    QeumDev('csky_wj_rsa',    'csky_wj_rsa',    '',  'RSA',       '', ''),
                             'WJ_CRC':    QeumDev('csky_wj_crc'     'csky_wj_crc',    '',  'CRC',       '', ''),
                             'WJ_TIPC':   QeumDev('csky_wj_tipc'    'csky_wj_tipc',   '',  'TIPC',      '', ''),
                             'WJ_SASC':   QeumDev('csky_wj_sasc'    'csky_wj_sasc',   '',  'SASC',      '', ''),
                             'WJ_CODEC':  QeumDev('csky_wj_codec'   'csky_wj_codec',  '',  'CODEC',     '', ''),
                             'DW_DMA':    QeumDev('csky_dmac'       'csky_dmac',      '',  'DMAC',      '', ''),
                             'WJ_MBOX':   QeumDev('csky_wj_mcc'     'csky_wj_mcc',    '',  'MCC',       '', ''),
                             #'':   QeumDev('csky_eflash',    'csky_eflash',    '',  'EFLASH',    '', ''),
                            }

        self.filename = filename
        self.chip_info = chip_info
        write_file(qeum_template, filename)
        self.qeumfile = QeumFile(filename)

        board_info = QeumBoard(chip_info.vendor.chip + '_evb', chip_info.cpu[0].arch, "2.0")
        self.qeumfile.board_set(board_info.attr().copy())
        cpu_info = QeumCpuInfo(chip_info.cpu[0].name, chip_info.cpu[0].endian, 'abiv2')
        self.qeumfile.cpu_info_set(cpu_info.attr().copy())

        devs = self.__devs()
        if chip_info.cpu[0].arch in ['csky', 'CSKY']:
            self.qeumfile.dyndev_add(self.qeum_csky_tcip.attr().copy())
            self.qeumfile.dyndev_add(self.qeum_csky_cpu_coret.attr().copy())

        for v in devs:
            self.qeumfile.dyndev_add(v)

    def __devs(self):
        devs = []
        for v in self.chip_info.devices:
            if v.name in ['CORET', 'coret']:
                continue
            dev_map_key = v.name
            if v.vendor:
                dev_map_key = v.vendor + '_' + v.name
            if dev_map_key not in self.qeum_dev_map.keys():
                if dev_map_key not in ['IRQ']:
                    logger.warn("qeum doesn't has %s" %(dev_map_key))
                continue

            qeum_dev = self.qeum_dev_map[dev_map_key]
            if v.base:
                qeum_dev.addr = hex(v.base)
            if v.irq_n != None:
                qeum_dev.irq = str(v.irq_n)
            qeum_dev = qeum_dev.attr().copy()
            if qeum_dev in devs:
                logger.error('soc yaml error, %s Repeat definition!', v.name)
                for v in devs:
                    v.show()
                exit(-1)
            devs.append(qeum_dev)
        return devs

    def save(self):
        self.qeumfile.save()



