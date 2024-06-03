#!/bin/sh

if [ ! -f "include/csi_config.h" ]; then
    echo "config -> csi_config.h"
    sh ./tools/gencsiconfig.sh ./ > include/csi_config.h
else
    sh ./tools/gencsiconfig.sh ./ > include/config_h
    grep -Fvf include/config_h include/csi_config.h
    ret1=$?
    grep -Fvf include/csi_config.h include/config_h
    ret2=$?

    if [ "$ret1$ret2" = "11" ]; then
        echo config unchanged
        rm include/config_h
    else
        echo have new config
        mv include/config_h include/csi_config.h
    fi
    #echo "$ret1$ret2"
fi
