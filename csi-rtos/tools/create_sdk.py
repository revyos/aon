#!/usr/bin/python3

import sys
import os
import subprocess

tool_path = os.path.split(os.path.realpath(__file__))[0]
os.chdir(tool_path )
sys.path.append('../csi/tools/')
from csi import *

def show_help():
    print('-' * 50)
    print('help:')
    print('show [component], used to display the components in warehouse')
    print('example: show chips')
    print('compponent:')
    print('\t[chips]: display chip components')
    print('\t[drivers]: display driver components')
    print('\t[boards]: display board components')
    print('\t[examples]: display example components')
    print('sdk [chip_name], used to create chip sdk')
    print('example: sdk danica')
    print('\t[chip_name]: chip component name')

def cmd_error():
    print('cmd error')
    show_help()

show_compents_path = {'chips' : '../csi/chips/', 'drivers' : '../csi/drivers', \
        'boards' : '../boards', 'examples' : '../examples'}

if len(sys.argv) < 2:
    show_help()
    exit(0)

if sys.argv[1] in ['show']:
    if len(sys.argv) < 3:
        cmd_error()
        exit(-1)
    if sys.argv[2] not in show_compents_path:
        cmd_args = list(show_compents_path.keys())
        print("Don't have %s, please select : %s" %(sys.argv[2], cmd_args))
        exit(-1)
    scan_path = show_compents_path[sys.argv[2]]
    cmd = 'python3 ../csi/tools/test.py list ' + scan_path
    os.system(cmd)

elif sys.argv[1] in ['sdk']:
    if len(sys.argv) < 3:
        cmd_error()
        exit(-1)

    csi = CSI('../csi/chips')
    csi_ips = csi.ips
    csi_chip = csi_ips.get(sys.argv[2], 'thead')
    used_flash = True
    if len(sys.argv) > 3:
        if sys.argv[3] == "RAM":
            used_flash = False

    soc_cfg = ''
    prj_cfg = ''
    if csi_chip != None:
        for v in os.listdir(csi_chip.path):
            if 'soc.yaml' in v:
                soc_cfg = os.path.join(csi_chip.path, v)
            elif 'flash_project.yaml' in v and used_flash:
                prj_cfg = os.path.join(csi_chip.path, v)
            elif 'project.yaml' in v and 'flash' not in v and used_flash == False:
                prj_cfg = os.path.join(csi_chip.path, v)
    else:
        print('error, can\'t find chip ', sys.argv[2])
        exit(-1)

    cmd = 'python3 ../csi/tools/test.py chip ../csi ../ %s %s %s'
    cmd = cmd %(soc_cfg, prj_cfg, '../release')
    print(cmd)
    os.system(cmd)

elif sys.argv[1] in ['h', 'help', '-h', '-help']:
    show_help()
else:
    print("error, invalid cmd!!")
    show_help()


