dtb-$(CONFIG_ARCH_VEXPRESS) += rtsm_ve-aemv8a.dtb foundation-v8.dtb \
				fvp-base-gicv2-psci.dtb
dtb-$(CONFIG_ARCH_VEXPRESS) += juno.dtb
dtb-$(CONFIG_ARCH_XGENE) += apm-mustang.dtb
dtb-$(CONFIG_MACH_ESPRESSO7420) += exynos7420-espresso7420.dtb
ifeq ($(CONFIG_SUPPORT_CODEGEN),y)
dtb-$(CONFIG_MACH_MV7420) += exynos7420-mv7420-codegen.dtb
else
dtb-$(CONFIG_MACH_MV7420) += exynos7420-mv7420.dtb
endif
dtb-$(CONFIG_MACH_MV7420_HD) += exynos7420-mv7420-hd.dtb
dtb-$(CONFIG_MACH_MV7420_FHD) += exynos7420-mv7420-fhd.dtb
dtb-$(CONFIG_MACH_SMDK7580) += exynos7580-smdk7580.dtb

targets += dtbs

DTB_NAMES := $(subst $\",,$(CONFIG_BUILD_ARM64_APPENDED_DTB_IMAGE_NAMES))
ifneq ($(DTB_NAMES),)
DTB_LIST := $(addsuffix .dtb,$(DTB_NAMES))
else
DTB_LIST := $(dtb-y)
endif
targets += $(DTB_LIST)

dtbs: $(addprefix $(obj)/, $(DTB_LIST))

clean-files := dts/*.dtb *.dtb ../*.dtb
