# -*- coding:utf-8 -*-
#
# Copyright (C) 2015-2020 Alibaba Group Holding Limited

from __future__ import print_function
import json
import shutil
import glob

from .tools import *
from .log import logger

class PinPullMode(object):
    def __init__(self, dic):
        self.pull_none = False
        self.pull_up = False
        self.pull_down = False
        self.open_drain = False
        self.push_pull = False
        for k, v in dic.items():
            if v == None:
                continue
            else:
                self.__dict__[k] = v

class PinFunc(object):
    def __init__(self):
        pass

    def func(self, dic):
        try:
            for k, v in dic.items():
                if v == None:
                    continue
                else:
                    if '_' not in k:
                        logger.error('The func name format error, Please write in the format of xxx_yyy(the xxx is device name, yyy is pin function. for example IIC1_SCL).')
                        return False
                    self.__dict__[k] = v
        except:
            logger.error('ioc pin func < %s > error' %(dic))
            return False

        return True


class Pin(object):
    def __init__(self, dic = None):
        if dic == None:
            return
        self.name = list(dic.keys())[0]
        self.chip_pin = ''
        self.pin_func = []
        self.def_status = ''
        self.pull_mode = PinPullMode({})
        self.drive = []
        self.speed = []
        for k, v in dic[self.name].items():
            if v == None:
                continue
            if k in ['pull_mode']:
                self.__dict__[k] = PinPullMode(v)
            elif k in ['func']:
                for vv in v:
                    pinfunc = PinFunc()
                    if not pinfunc.func(vv):
                        logger.error('The %s pin function definition error' %(self.name))
                        exit(-1)
                    self.pin_func.append(pinfunc)
            else:
                self.__dict__[k] = v

class DevPinMap:
    def __init__(self):
        self.pin_name = ''
        self.idx = ''
        self.channel = ''
        self.pin_func = ''
        self.dev_name = ''
        self.func = ''

class IoctrlTcip:
    def_max_len = 40
    def __init__(self, devices, device_attr):
        self.target_info = ''
        self.devices = devices
        self.device_attr = device_attr['ioc']
        self.pins = []
        for v  in self.device_attr:
            self.pins.append(Pin(v))

    def dev_attr_parsing(self):
        txt = ''
        devices = []
        for v in self.devices:
            if v.name in devices:
                continue
            dev_map = self.pinmap(v.name)
            if len(dev_map) == 0:
                continue
            txt += self.__g_dev_map(dev_map) + '\n'
            devices.append(v.name)
        return txt

    def soc_attr_parsing(self):
        txt = ''
        txt += self.__pin_name_t()
        txt += self.__pin_func_t()
        return txt

    def __pin_name_t(self):
        txt = 'typedef enum {\n'
        for v  in self.pins:
            txt += ' ' * 4 + v.name + ',\n'
        txt += '} pin_name_t;\n\n'
        return txt

    def __pin_func_t(self):
        txt = 'typedef enum {\n'
        for v in self.pins:
            for func in v.pin_func:
                for k, vv in func.__dict__.items():
                    name  =  v.name + '_' + k
                    txt += ' ' * 4 + name + (IoctrlTcip.def_max_len - len(name)) * ' ' \
                           + '= ' + str(vv) + ',\n'
        name = 'PIN_FUNC_GPIO'
        txt += ' ' * 4 + name + (IoctrlTcip.def_max_len - len(name)) * ' ' + '= -1,\n'
        txt += '} pin_func_t;\n\n'
        return txt

    def __g_dev_map(self, map_list):
        txt = 'const csi_pinmap_t ' + map_list[0].dev_name.lower() + '_pinmap[] = {\n'
        val = '{0:20}{1:20}{2:20}{3:20}'
        iteam = ' ' * 4 + '{%s},\n'
        for v in map_list:
            txt += iteam %(val.format(v.pin_name + ',', v.idx + ',', v.channel + ',', v.pin_func))
        txt += iteam %(val.format('-1,', '-1,', '-1,', '-1'))
        txt +='};\n'
        return txt

    def pinmap(self, devname):
        txt = ''
        pin_map = []
        dev_pins = self.__get_dev_pin(devname)
        for v in dev_pins:
            map = DevPinMap()
            map.dev_name = devname
            map.pin_name = v.name
            map.pin_func = v.name + '_' + v.pin_func
            split = v.pin_func.split('_')
            map.idx = self.__get_tag_num(split[0])
            if devname in ['UART', 'IIC', 'SPI', 'I2S', 'ADC', 'PWM', 'USI', 'QSPI']:
                map.func = split[1]
            if devname in ['UART', 'IIC', 'SPI', 'I2S', 'USI', 'QSPI']:
                map.channel = 'PIN_' + devname + '_' + split[1]
            elif devname in ['GPIO']:
                map.pin_func = 'PIN_FUNC_GPIO'
                map.idx = '0'
                map.channel = self.__get_tag_num(v.name)
            elif devname in ['ADC', 'PWM']:
                map.channel = self.__get_tag_num(v.pin_func)
            else:
                print("%s pinmap generate failed!!" %(devname))
            pin_map.append(map)
        return pin_map

    def __get_tag_num(self, v):
        idx = -1
        while v[idx:].isdigit():
            idx -= 1
        if not v[idx + 1:].isdigit():
            return '0'
        return v[idx + 1:]

    def __get_dev_pin(self, devname):
        devpins = []
        for pin in self.pins:
            for v in pin.pin_func:
                if devname in list(v.__dict__.keys())[0]:
                    p = Pin()
                    p.name = pin.name
                    p.pin_func = list(v.__dict__.keys())[0]
                    devpins.append(p)
                elif devname == 'GPIO':
                    p = Pin()
                    p.name = pin.name
                    p.pin_func = ''
                    devpins.append(p)
        return devpins

    def check(self):
        pass

