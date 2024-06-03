# -*- coding:utf-8 -*-
#
# Copyright (C) 2015-2020 Alibaba Group Holding Limited


from __future__ import print_function

from csi import *

class Chip(Command):
    common = True
    helpSummary = "List ip"
    helpUsage = """
%prog <csi path>
"""
    helpDescription = """
List all projects; pass '.' to list the project for the cwd.
"""
    def __init__(self):
        pass

    def _Options(self, p):
        pass
    #     p.add_option('-a', '--all',
    #                  dest='show_all', action='store_true',
    #                  help='show the complete list of commands')

    def Execute(self, opt, args):
        if len(args) > 2:
            sdk = Sdk(args[0], args[1], args[2], args[3])
            sdk_name = ''
            if len(args) > 5:
                sdk_name = args[5]
            if 'autotest' in args:
                sdk.socns_dump(args[4], sdk_name, 'autotest')
            else:
                sdk.socns_dump(args[4], sdk_name)
            sdk.cdk_dump(args[4], sdk_name)
        else:
            self.Usage()


