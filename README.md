# Slice

1. Export Directory path:

```bash
export WORKDIR=~/docker-yocto-build/luxomed/am62x-var-som-u-boot
```

2. Export variables:

```bash
export PATH=$HOME/arm-gnu-toolchain-11.3.rel1-x86_64-arm-none-linux-gnueabihf/bin:$PATH &&
  export PATH=$HOME/arm-gnu-toolchain-11.3.rel1-x86_64-aarch64-none-linux-gnu/bin:$PATH &&  
  export TI_LINUX_FW_DIR=$WORKDIR/ti-linux-firmware &&
  export UBOOT_DIR=$WORKDIR/ti-u-boot && 
  export TFA_DIR=$WORKDIR/trusted-firmware-a && 
  export OPTEE_DIR=$WORKDIR/optee_os
```
3. Build Images:
    - ATF:

    ```bash
    cd $TFA_DIR
    make -j$(nproc) ARCH=aarch64 CROSS_COMPILE=aarch64-none-linux-gnu- PLAT=k3 TARGET_BOARD=lite SPD=opteed
    ```

    - OP-TEE:

    ```bash
    cd $OPTEE_DIR
    make -j$(nproc) CROSS_COMPILE64=aarch64-none-linux-gnu- CROSS_COMPILE32=arm-none-linux-gnueabihf- PLATFORM=k3-am62x CFG_ARM64_core=y
    ```

    - U-Boot A53 Images:

    Optional - Modify am62x_var_som_a53_slice_defconfig: 

    ```bash
    cd $WORKDIR/ti-u-boot
    make -j$(nproc) ARCH=arm CROSS_COMPILE=aarch64-none-linux-gnu- am62x_var_som_a53_slice_defconfig O=$UBOOT_DIR/out/a53
    make -j$(nproc) ARCH=arm CROSS_COMPILE=aarch64-none-linux-gnu- O=$UBOOT_DIR/out/a53 menuconfig
    ... make and save some changes ... 
    make -j$(nproc) ARCH=arm CROSS_COMPILE=aarch64-none-linux-gnu- O=$UBOOT_DIR/out/a53 savedefconfig
    cp out/a53/defconfig configs/am62x_var_som_a53_slice_defconfig
    ```

    Build tispl.bin and u-boot.img. Saved in $UBOOT_DIR/out/a53. Requires bl31.bin, tee-pager_v2.bin, and ipc_echo_testb_mcu1_0_release_strip.xer5f. 

    ```bash
    cd $WORKDIR/ti-u-boot
    make -j$(nproc) ARCH=arm CROSS_COMPILE=aarch64-none-linux-gnu- am62x_var_som_a53_slice_defconfig O=$UBOOT_DIR/out/a53
    make -j$(nproc) ARCH=arm CROSS_COMPILE=aarch64-none-linux-gnu- \
        BL31=$TFA_DIR/build/k3/lite/release/bl31.bin \
        TEE=$OPTEE_DIR/out/arm-plat-k3/core/tee-pager_v2.bin \
        O=$UBOOT_DIR/out/a53 BINMAN_INDIRS=$TI_LINUX_FW_DIR
    ```

4. Update Uboot:

To update U-Boot on an existing system, the following SPL and U-Boot images need to be installed to the boot partition of the boot media (eMMC or SD):

- ***UBOOT_DIR/out/r5/tiboot3.bin***
- ***UBOOT_DIR/out/a53/tispl.bin***
- ***UBOOT_DIR/out/a53/u-boot.img***


