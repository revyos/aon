● 配置
    目前开发板改动aon主要的配置项如下：
配置项	说明
CONFIG_PMIC_DA9063_V1	D9063驱动
CONFIG_PMIC_DA9121_V1	DA9121驱动
CONFIG_PMIC_SLG51000_V1	SLG51000驱动
cp boards/thead/configs/light_aon_defconfig .config
● 编译
make clean;make -j
输出的bin和elf文件在如下目录：
csi-rtos/out/light_aon.bin
csi-rtos/out/light_aon.elf