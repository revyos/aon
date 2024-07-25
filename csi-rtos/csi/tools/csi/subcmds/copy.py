# -*- coding:utf-8 -*-
#
# Copyright (C) 2015-2020 Alibaba Group Holding Limited


from __future__ import print_function

from csi import *

class Copy(Command):
    common = True
    helpSummary = "List ip"
    helpUsage = """
%prog <csi path>
"""
    helpDescription = """
List all projects; pass '.' to list the project for the cwd.
"""

    def _Options(self, p):
        pass
    #     p.add_option('-a', '--all',
    #                  dest='show_all', action='store_true',
    #                  help='show the complete list of commands')

    def Execute(self, opt, args):
        if len(args) > 1:
            csi = CSI(args[0])
            for ip in csi.ips:
                ip.Dump("v1.0", args[1])

        else:
            self.Usage()
