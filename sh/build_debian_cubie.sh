#!/bin/bash

CUBIE_BOARD=Cubieboard1 #Cubieboard2 Cubietruck
WORK_DIR=/mnt/data
ROOTFS_DIR=${WORK_DIR}/chroot-armhf


build_process() {
    local sunxi=linux-sunxi
    #Install building tools.
    apt-get install build-essential libncurses5-dev u-boot-tools \
        qemu-user-static debootstrap git binfmt-support libusb-1.0-0-dev pkg-config
    apt-get install gcc-arm-linux-gnueabihf

    #Clone/update repositories.
    cd ${WORK_DIR}
    if [ -d u-boot-sunxi ]; then
        (cd u-boot-sunxi; git pull)
    else
        git clone https://github.com/cubieboard/u-boot-sunxi.git -b cubie/sunxi
    fi
    if [ -d sunxi-tools ]; then
        (cd sunxi-tools; git pull)
    else
        git clone https://github.com/cubieboard/sunxi-tools.git
    fi
    if [ -d sunxi-boards ]; then
        (cd sunxi-boards; git pull)
    else
        git clone https://github.com/cubieboard/sunxi-boards.git
    fi
    if [ -d linux-sunxi ]; then
        (cd linux-sunxi; git pull)
    else
        git clone https://github.com/cubieboard/linux-sunxi.git -b cubie/sunxi-3.4
    fi

    #Prepare debian rootfs.
    mkdir ${ROOTFS_DIR}
    cd ${ROOTFS_DIR}
    debootstrap --foreign --arch armhf wheezy .
    cp /usr/bin/qemu-arm-static usr/bin/
    LC_ALL=C LANGUAGE=C LANG=C chroot . /debootstrap/debootstrap --second-stage
    LC_ALL=C LANGUAGE=C LANG=C chroot . dpkg --configure -a
    tar jcpvf ../debian-rootfs-armhf-clean.tar.bz2 *

    #Build u-boot.
    cd ${WORK_DIR}/u-boot-sunxi
    make distclean CROSS_COMPILE=arm-linux-gnueabihf-
    make ${CUBIE_BOARD} CROSS_COMPILE=arm-linux-gnueabihf-

    #Build sunxi-tools.
    cd ${WORK_DIR}/sunxi-tools
    make

    #Build kernel.
    cd ${WORK_DIR}/linux-sunxi
    if [ ${CUBIE_BOARD} = "Cubieboard1" ]; then
        cp arch/arm/configs/sun4i_defconfig .config
    elif [ ${CUBIE_BOARD} = "Cubieboard2" -o ${CUBIE_BOARD} = "Cubietruck" ]; then
        cp arch/arm/configs/sun7i_defconfig .config
    fi
    make ARCH=arm menuconfig
    make -j5 ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- uImage modules

    #Customize rootfs.
    chroot ${ROOTFS_DIR} passwd
    echo "Cubieboard" > ${ROOTFS_DIR}/etc/hostname
    echo "127.0.0.1 Cubieboard" >> ${ROOTFS_DIR}/etc/hosts
    cp ${WORK_DIR}/linux-sunxi/arch/arm/boot/uImage ${ROOTFS_DIR}/boot/
    make -C ${WORK_DIR}/linux-sunxi INSTALL_MOD_PATH=${ROOTFS_DIR} ARCH=arm \
        CROSS_COMPILE=arm-linux-gnueabihf- modules_install
    echo "T0:2345:respawn:/sbin/getty -L ttyS0 115200 vt100" >> ${ROOTFS_DIR}/etc/inittab
    echo "deb http://http.debian.net/debian/ wheezy main contrib non-free" > ${ROOTFS_DIR}/etc/apt/sources.list
    echo "deb http://security.debian.org/ wheezy/updates main contrib non-free" >> ${ROOTFS_DIR}/etc/apt/sources.list
    chroot ${ROOTFS_DIR} apt-get update
    chroot ${ROOTFS_DIR} apt-get upgrade
    chroot ${ROOTFS_DIR} apt-get install openssh-server
    #Optional, install locales.
    chroot ${ROOTFS_DIR} apt-get install locales
    echo "en_US.UTF-8 UTF-8" > ${ROOTFS_DIR}/etc/locale.gen
    echo "zh_CN.UTF-8 UTF-8" >> ${ROOTFS_DIR}/etc/locale.gen
    chroot ${ROOTFS_DIR} locale-gen
    #Optional, install wifi tools.
    chroot ${ROOTFS_DIR} apt-get install wireless-tools wpasupplicant firmware-ralink

    #Prepare kernal boot parameters.
    cat <<EOF >${ROOTFS_DIR}/boot/uEnv.txt
mmcboot=fatload mmc 0 0x43000000 script.bin || fatload mmc 0 0x43000000 evb.bin; \
fatload mmc 0 0x48000000 uImage; if fatload mmc 0 0x43100000 uInitrd; \
then bootm 0x48000000 0x43100000; else bootm 0x48000000; fi
uenvcmd=run mmcboot
bootargs=console=ttyS0,115200 console=tty0 disp.screen0_output_mode=EDID:1280x1024p50 \
hdmi.audio=EDID:0 root=/dev/mmcblk0p1
EOF

    #Prepare script.bin.
    if [ ${CUBIE_BOARD} = "Cubieboard1" ]; then
        cp ${WORK_DIR}/sunxi-boards/sys_config/a10/cubieboard.fex ${ROOTFS_DIR}/boot/script.fex
    elif [ ${CUBIE_BOARD} = "Cubieboard2" ]; then
        cp ${WORK_DIR}/sunxi-boards/sys_config/a20/cubieboard2.fex ${ROOTFS_DIR}/boot/script.fex
    elif [ ${CUBIE_BOARD} = "Cubietruck" ]; then
        cp ${WORK_DIR}/sunxi-boards/sys_config/a20/cubietruck.fex ${ROOTFS_DIR}/boot/script.fex
    fi
    cat <<EOF >>${ROOTFS_DIR}/boot/script.fex
[gpio_para]
gpio_used = 1
gpio_num = 30
gpio_pin_1 = port:PD01<1><default><default><default>
gpio_pin_2 = port:PD02<1><default><default><default>
gpio_pin_3 = port:PD03<1><default><default><default>
gpio_pin_4 = port:PD04<1><default><default><default>
gpio_pin_5 = port:PD05<1><default><default><default>
gpio_pin_6 = port:PD06<1><default><default><default>
gpio_pin_7 = port:PD07<1><default><default><default>
gpio_pin_8 = port:PD08<1><default><default><default>
gpio_pin_9 = port:PD09<1><default><default><default>
gpio_pin_10 = port:PD10<1><default><default><default>
EOF


}

