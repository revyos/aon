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

class CpuAccessRegion(object):
    def __init__(self, dic):
        self.type = ''
        self.start = ''
        self.size = ''
        for k, v in dic.items():
            self.__dict__[k] = v

    def show(self):
        for k, v in self.__dict__.items():
            print(k, str(v))

class CpuSrc(object):
    def __init__(self, dic):
        self.idx = None
        self.name = None
        self.arch = None
        self.endian = None
        self.irq_num = None
        self.icache = None
        self.mpu_regions = None
        self.dcache = None
        self.access_region = []
        self.version = None
        self.vendor = None
        for k, v in dic.items():
            if k in ['acess_region']:
                for vv in v:
                    self.access_region.append(CpuAccessRegion(vv))
            else:
                self.__dict__[k] = v

        self.check()

    def check(self):
        for k, v in self.__dict__.items():
            if v == None:
                logger.error('chip soc cpu must set: %s' %(k))
                exit(-1)
        if not self.access_region:
            logger.warn('chip soc cpu lack of : %s' %(access_region))

    def show(self):
        for k, v in self.__dict__.items():
            if v in ['access_region']:
                for vv in v:
                    vv.show()
            print(k, v)

class ClockSrc(object):
    def __init__(self, val):
        self.name = None
        self.value = None
        for k, v in val.items():
            self.__dict__[k] = v
        self.check()
    def show(self):
        for k, v in self.__dict__.items():
            print(k, v)

    def check(self):
        for k, v in self.__dict__.items():
            if v == None:
                logger.error('chip soc clock must set: %s' %(k))
                exit(-1)

class DeviceSrc(object):
    def __init__(self, dic):
        self.name = ''
        self.idx = ''
        self.vendor = ''
        self.version = ''
        self.base = ''
        self.size = ''
        self.irq_n = None
        self.is_driver = ''
        self.description = ''
        for k, v in dic.items():
             if v == None:
                continue
             else:
                self.__dict__[k] = v

        self.check()

    def show(self):
        for k, v in self.__dict__.items():
            print(k, v)

    def check(self):
        for k, v in self.__dict__.items():
            if k in ['name', 'is_driver'] and v == None:
                logger.error('The deviecs iteam <%s> can\'t set none' %(k))
                exit(-1)

class Vendor(object):
    def __init__(self, dic):
        self.chip = None
        self.chip_vendor = None
        self.license = None
        self.file_description = None
        self.version = None
        for k, v in dic.items():
            if v:
                self.__dict__[k] = v
        self.check()
    def check(self):
        for k, v in self.__dict__.items():
            if  k in ['license', 'file_description']:
                if v == None:
                    logger.warn('chip soc vneodr lack of: %s' %(k))
                continue
            if v == None:
                logger.error('chip soc vneodr must set: %s' %(k))
                exit(-1)

    def show(self):
        for k, v in self.__dict__.items():
            if not v:
                continue
            print(k, str(v))

def devices(list):
    devs = []
    for v in list:
        devs.append(DeviceSrc(v))
    return devs

def cpu(list):
    cpus = []
    for v in list:
        cpus.append(CpuSrc(v))
    return cpus

def clock(list):
    clks = []
    for v in list:
        clks.append(ClockSrc(v))
    return clks

class ChipInfo(object):
    def __init__(self, filename):
        self.vendor = None
        self.cpu = None
        self.clock = None
        self.devices = None
        self.device_attr = None
        self.cflag = None
        self.define = []
        self.load(filename)
        self.check()

    def load(self, filename):
        conf = yaml_load(filename)

        for k, v in conf.items():
            if k in ['vendor']:
                self.__dict__[k] = Vendor(v)
            if k in ['cpu']:
                self.__dict__[k] = cpu(v)
            if k in ['clock']:
                self.__dict__[k] = clock(v)
            if k in ['devices']:
                self.__dict__[k] = devices(v)
            if k in ['device_attr']:
                self.device_attr = v
            if k in ['cflag']:
                self.cflag = v
            if k in ['define']:
                self.__dict__[k] = v

    def check(self):
        for k, v in self.__dict__.items():
            if k in ['device_attr', 'cflag', 'define']:
                continue
            if not v:
                logger.error('chip soc must set %s' %(k))
                exit(-1)

    def show(self):
        for k, v in self.__dict__.items():
            if not v:
                continue
            if k in ['vnedor']:
                v.show()
            elif k in ['device_attr']:
                print(k, str(v))
            elif k in ['cpu', 'clock', 'devices']:
                for vv in v:
                    vv.show()

    def save(self, filename=None):
        pass

def test():
    chip_info = ChipInfo('./soc.yaml')
    chip_info.show()
if __name__ == "__main__":
    test()

