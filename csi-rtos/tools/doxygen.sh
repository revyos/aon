#!/bin/bash
ROOT_DIR=$PWD
DOXYGEN="doxygen"
DIR_CONFIG_DOX=$PWD/tools
FILE_CONFIG_DOX=$PWD/tools/Doxyfile
OUT_PUT_DIR=$PWD/Out/docs
INPUT=$PWD/csi-api

INPUT_KERNEL=csi_kernel
INPUT_DRIVER=csi_driver
INPUT_CORE=csi_core

# search doxygen in host
which "$DOXYGEN"  > /dev/null 2>&1
if [ $? == 0 ]; then
        echo "start to create doc"
else
        echo "can not find $DOXYGEN"
	echo "we will install it"
	sudo apt-get install $DOXYGEN
fi
cd $DIR_CONFIG_DOX
echo $OUTTA |  grep "Doxyfile" > /dev/null 2>&1
if [ $? -eq 0 ]; then
        echo "HOST HAVE INSTALL Doxyfile config"
        exit 0
fi

cd $ROOT_DIR

if [ ! -d $OUT_PUT_DIR ] ;then
mkdir -p $OUT_PUT_DIR
fi

if [ ! -d $INPUT ] ;then
mkdir -p $INPUT
fi

if [ -d $INPUT_KERNEL ] ;then
mkdir $INPUT/$INPUT_KERNEL
cp -rf $INPUT_KERNEL/include/* $INPUT/$INPUT_KERNEL
fi

if [ -d $INPUT_DRIVER ] ;then
mkdir $INPUT/$INPUT_DRIVER
cp -rf $INPUT_DRIVER/include/* $INPUT/$INPUT_DRIVER 
fi

if [ -d $INPUT_CORE ] ;then
mkdir $INPUT/$INPUT_CORE
cp -rf $INPUT_CORE/include/*  $INPUT/$INPUT_CORE
fi

$DOXYGEN $FILE_CONFIG_DOX

if [ -d $INPUT ] ;then
rm  -rf $INPUT
fi

if [ -f "Doxyfile" ] ;then
rm -f Doxyfile
fi
