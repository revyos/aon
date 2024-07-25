# -*- coding:utf-8 -*-
#
# Copyright (C) 2015-2020 Alibaba Group Holding Limited


from __future__ import print_function

from csi import *


class List(Command):
    common = True
    helpSummary = "woshi list"
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
        if len(args) > 0:
            csi = CSI(args[0])
            csi.list()
        else:
            self.Usage()
