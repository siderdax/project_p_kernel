#!/bin/sh
echo "Start cleaning useless contents..."

# USB
rm -f drivers/usb/gadget/usbdiag.h

# POWER
rm -f drivers/misc/exynos_manual_power_mode.c

# PCIE/WIFI
find ./drivers/net/wireless/bcmdhd/ -name '*.[ch]' -delete

# BT
rm -f ./drivers/bluetooth/bcm43241.c

# MODEM
#find ./drivers/misc/modem_v1/ -name '*.[ch]' -delete
rm -rf ./drivers/misc/modem_v1/
rm -f arch/arm64/boot/dts/exynos7420-modem-ss333.dtsi
rm -f arch/arm64/configs/mv7420_s333_defconfig
#find ./drivers/misc/mipi-lli/ -name '*.[ch]' -delete

# FPC
find ./drivers/misc/FPC1020/ -name '*.[ch]' -delete

# CONFIGS
rm -f ./arch/arm64/configs/espresso7420_defconfig
rm -f ./arch/arm64/configs/mv7420_cmdlcd_defconfig
rm -f ./arch/arm64/configs/mv7420_cmdlcd_eng_defconfig
rm -f ./arch/arm64/configs/mv7420_defconfig
rm -f ./arch/arm64/configs/mv7420_720p_defconfig
rm -f ./arch/arm64/configs/mv7420_720p_eng_defconfig
rm -f ./arch/arm64/configs/mv7420_720p_eng_pcie_defconfig
rm -f ./arch/arm64/configs/mv7420_eng_pcie_defconfig
rm -f ./arch/arm64/configs/mv7420_eng_defconfig
rm -f ./arch/arm64/configs/mv7420_cmdlcd_ref_defconfig
rm -f ./arch/arm64/configs/mv7420_ref_defconfig
rm -f ./arch/arm64/configs/espresso7420_evt0_defconfig
rm -f ./arch/arm64/configs/mv7420_720p_ref_defconfig

# dts

rm -f arch/arm64/boot/dts/exynos7420-mv7420.dts
rm -f arch/arm64/boot/dts/exynos7420-mv7420-fhd.dts
rm -f arch/arm64/boot/dts/exynos7420-mv7420-hd.dts
rm -f arch/arm64/boot/dts/exynos7420-mv7420-pd_gpio.dtsi
rm -f arch/arm64/boot/dts/exynos7420.dtsi
rm -f arch/arm64/boot/dts/exynos7420-espresso7420.dts
rm -f arch/arm64/boot/dts/exynos7420-espresso7420_evt0.dts
rm -f arch/arm64/boot/dts/exynos7420_evt0.dtsi
rm -f arch/arm64/boot/dts/exynos7420-pinctrl.dtsi
rm -f arch/arm64/boot/dts/exynos7420-pinctrl_evt0.dtsi

make clean distclean
echo "Start packaging..."
mv .git/ ../
tar -jcf ../mv7420_kernel.tar.bz2 *
mv ../.git/ .

echo "Finished!"
