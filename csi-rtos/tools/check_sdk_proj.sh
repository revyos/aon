#!/bin/bash

set -e

if [ "$1" != "" ];then
    check_dir=$1
else
    check_dir=$1
fi

basepath=$(cd `dirname $0`; pwd)

all_makefile=`find $check_dir -name Makefile`
for makefile in $all_makefile
do
    proj_dir=`echo $makefile|awk -F 'Makefile' '{print $1}'`
    echo $proj_dir
    cd $proj_dir

    make clean
    set +e
    make 
#    set -e

    if [ $? -eq 0 ]; then
    	echo "OK"
    else
	echo "error"
	echo $proj_dir 
	echo $proj_dir >> $basepath/error_log    
    fi

    cd -
done

