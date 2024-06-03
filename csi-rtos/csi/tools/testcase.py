from __future__ import print_function

import pytest
import time
from csi import *

def test_yoc():
    yoc = YoC()
    # yoc.occ_update()
    assert yoc.ips.size() == 108
    ip = yoc.ips.get('helloworld')
    yoc.check_depend(ip)

    assert ip.name == 'helloworld'
    print(ip.depends, ip.depends_on)

    ip = yoc.ips.get('csi')

    assert ip.name == 'csi'
    print(ip.depends)
    print("-----------------")
    for c in ip.depends_on:
        print(c.name)
    print("=================")

if __name__ == "__main__":
    pytest.main(['-s', 'testcase.py'])
