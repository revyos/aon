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


class CSI:
    def __init__(self, path="."):
        self.csi_path = path

        # scanning yoc all ips
        self.ips = IPBox()
        dev_info_yamls = self.search_dev_info_yaml(
            ['.'])
        for filename in dev_info_yamls:
            ip = IP(filename)

            if not self.ips.add(ip):
                logger.error('ip `%s` is multiple (ip path: %s)' %
                             (ip.name,  ip.path))
                exit(0)

    def list(self):
        for ip in self.ips:
            ip.show()

    def chip_dev(self, devices):
        chip_ips = []
        for v in devices:
            if v.is_driver == 0:
                continue
            ip = self.ips.get(v.name, v.vendor)
            if ip == None:
                print('Can\'t find the ip name: %s, vendor %s' %(v.name, v.vendor))
                continue
            if v.version not in ip.versions.keys():
                print('IP: %s don\'t has version %s' %(v.name, v.version))
                continue
            ip.data = ip.versions[v.version]
            if ip not in chip_ips:
                chip_ips.append(ip)
        return chip_ips

    def chip_cpu(self, cpu):
        chip_ips = []
        for v in cpu:
            ip = self.ips.get(v.name, v.vendor)
            if ip == None:
                print("error, can't find ip: %s, %s:", v.vendor, v.name)
                continue
            if v.version not in ip.versions.keys():
                print('IP: %s don\'t has version %s' %(v.name, v.version))
                continue
            ip.data = ip.versions[v.version]
            if ip not in chip_ips:
                chip_ips.append(ip)

        csky_coretim = ['801', '802', '803', '804', '805']
        riscv_mtime = []
        tick = self.ips.get('TICK', '')
        tick.version = None
        tick.data = None
        if cpu[0].arch in ['csky'] :
            for v in csky_coretim:
                if v in cpu[0].name:
                    tick.data = tick.versions['coretim']
        elif cpu[0].arch in ['riscv']:
            for v in riscv_mtime:
                if v in cpu[0].name:
                    tick.data = tick.versions['mtime']
        if tick.data == None:
            tick.data = tick.versions['csitimer']

        if tick not in chip_ips:
            chip_ips.append(tick)
        return chip_ips
    def search_dev_info_yaml(self, subpath=[]):
        paths = []
        if subpath:
            for sub in subpath:
                p = os.path.join(self.csi_path, sub)
                if os.path.exists(p):
                    paths.append(p)
        else:
            paths.append(self.csi_path)

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