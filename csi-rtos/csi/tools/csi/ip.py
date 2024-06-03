# -*- coding:utf-8 -*-
#
# Copyright (C) 2015-2020 Alibaba Group Holding Limited

from __future__ import print_function
import os
import sys
import json
import zipfile
import shutil
from .tools import *
from .log import logger

class IpInfo(object):
    def __init__(self, filename):
        self.name = ''
        self.description = ''
        self.type = ''
        self.vendor = ''
        self.license = ''
        self.depends = []
        self.versions = {}

        conf = yaml_load(filename)

        for k, v in conf.items():
            if v:
                self.__dict__[k] = v

        if not self.name:
            logger.error('%s `name` cannot be empty!' % filename)
            exit(-1)

        if self.type not in ['driver', 'devices', 'core', 'chip', 'board', 'project']:
            logger.error(
                '%s `type` must be "solution" or "common" or "board" or "chip".' % filename)
            exit(-1)

    def save(self, filename=None):
        pass

class Version(object):
    def __init__(self, root_path, dic):
        self.drv_path = ''
        self.drv_common = ''
        self.doc = ''
        self.pictures = ''
        for k, v in dic.items():
            if not v:
                continue
            if k == 'doc_path':
                for kk, vv in v.items():
                    self.__dict__[kk]= os.path.join(root_path, vv)
            else:
                self.__dict__[k] = os.path.join(root_path, v)
    def check(self):
        if not self.drv_path:
            return False
        return True

class IP:
    def __init__(self, filename=''):
        ip_info = IpInfo(filename)
        self.path = os.path.dirname(filename)
        self.name = ip_info.name
        self.description =ip_info.description
        self.type = ip_info.type
        self.vendor = ip_info.vendor
        self.license = ip_info.license
        self.depends = ip_info.depends
        self.versions = {}
        self.data = None
        for ver, info in ip_info.versions.items():
            self.versions[ver] = Version(self.path, info)
            if self.versions[ver].check() == False:
                logger.error('IP %s must be set "drv_path"',self.name)
                exit(-1)

    def show(self, indent=0):
        ver = ''
        for k, _ in self.versions.items():
            ver += k + ', '
        if ver:
            ver = '(' + ver[:-2] + ')'

        s1 = self.name + ver
        size = len(s1)

        text1, text2 = string_strip(self.description, 80)
        print("%s %s %s - %s" %
              (' '*indent, s1, ' ' * (40 - size), text1))
        while text2:
            text1, text2 = string_strip(text2, 80)
            print(' ' * 46 + text1)

    def Dump(self, version, path):
        if version in self.versions:
            for val in self.versions[version].obj:
                print(val.type, val.path)

class IPBox(list):
    def __init__(self):
        list.__init__([])
        self.ips = {}

    def add(self, ip):
        not_exists = True
        if ip.name in self.ips:
            if self.ips[ip.name].vendor == ip.vendor:
                not_exists = False

        if not_exists:
            self.append(ip)
            self.ips[ip.name] = ip
        return not_exists

    def get(self, name, vendor):
        for c in self:
            if c.name == name and c.vendor == vendor:
                return c
        return None

    def remove(self, name):
        for c in self:
            if c.name == name:
                del c
                break

    def show(self):
        for c in self:
            c.show()

def string_strip(text, size):
    L = 0
    R = ''
    i = 0
    for c in text:
        if c >= '\u4E00' and c <= '\u9FA5':
            # print(c)
            L += 2
        else:
            # print('  ', c)
            L += 1
        R += c
        i += 1
        if L >= size:
            break
    return R, text[i:]
