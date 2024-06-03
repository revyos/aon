# Copyright (C) 2015-2020 Alibaba Group Holding Limited
from __future__ import print_function
import os
import shutil
import pickle

from .ip import *
from .log import logger
from .tools import *
from .dma_attr import *
from .ioctrl_attr import *

ip_attr_list = {'dma' : DmacTcip, 'ioc' : IoctrlTcip}

