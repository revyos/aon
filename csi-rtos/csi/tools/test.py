from __future__ import print_function

from csi import *
import pdb

def test_package():
    p = Package('../solutions/helloworld/package.yaml')
    # p = Package('package.yaml')
    p.show()

def test_process():
    import time
    pm = Progress('Checkout %s' % "nb", 10)
    for i in range(10):
        pm.update()
        time.sleep(1)
    pm.end()


def test_cmd():
    #pdb.set_trace()
    cmd = YocCommand()
    cmd.Execute(sys.argv[1:])
    # cmd._Run('help', None, None)


def test_yoc():
    csi = CSI("..")
    csi.list()


if __name__ == "__main__":
    test_cmd()
