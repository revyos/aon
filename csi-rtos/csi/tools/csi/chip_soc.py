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
from .soc_tcip import *
from .device_tcip import *

class ChipSoc:
    def __init__(self, chip_info):
        self.soc_h = SocDefine(chip_info).txt
        self.device_c = DevicesTcip(chip_info).txt
        self.startup = ''
        self.sys_freq = ''
        self.system_c = ''
