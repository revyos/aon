#!/bin/bash

find boards/ \
     examples/core/ \
     examples/driver/ \
     examples/hello_world/ \
     examples/kernel/ \
     include/ \
     modules/dtest/ \
     modules/libc/ \
     tests/core/ \
     tests/driver/ \
     tests/kernel/ \
     csi/chips/ \
     csi/drivers/ \
     csi/csi_kernel/freertosv8.2.3/adapter/ \
     csi/csi_kernel/rhino/adapter/ \
     csi/csi_kernel/ucos3/adapter/ \
 -name "*.c" -o -name "*.h" | xargs astyle --style=linux -Sw -fpXHU -jk3 -n -z2
