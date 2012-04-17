#
# Copyright (C) 2011 The Android Open-Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# These two variables are set first, so they can be overridden
# by BoardConfigVendor.mk

# Audio
# BOARD_USES_AUDIO_LEGACY := true
BOARD_USES_GENERIC_AUDIO := true
TARGET_PROVIDES_LIBAUDIO := false
BOARD_USES_ALSA_AUDIO := false
BOARD_WITH_ALSA_UTILS := false

#Platform
TARGET_BOARD_PLATFORM := s3c6410
TARGET_CPU_ABI := armeabi-v6
TARGET_CPU_ABI2 := armeabi
TARGET_ARCH_VARIANT := armv6-vfp
TARGET_ARCH_VARIANT_CPU := arm1176jzf-s

BOARD_HAVE_BLUETOOTH := false
TARGET_NO_BOOTLOADER := true
TARGET_NO_RADIOIMAGE := true

DISABLE_DEXPREOPT := true

BOARD_EGL_CFG := device/pandigital/s3c6410/egl.cfg

#CM patch for legacy touchscreen drivers
BOARD_USE_LEGACY_TOUCHSCREEN := true

TARGET_USERIMAGES_USE_EXT4 := true
BOARD_RECOVERYIMAGE_PARTITION_SIZE := 0x500000
BOARD_SYSTEMIMAGE_PARTITION_SIZE := 268435456
BOARD_USERDATAIMAGE_PARTITION_SIZE := 536870912
BOARD_FLASH_BLOCK_SIZE := 4096
BOARD_HAS_SDCARD_INTERNAL   := true
BOARD_SDCARD_DEVICE_PRIMARY := /dev/block/mmcblk0p1


TARGET_PROVIDES_INIT_RC := true

# Misc
BOARD_HAS_NO_SELECT_BUTTON := true
WITH_JIT := true
ENABLE_JSC_JIT := true
JS_ENGINE := v8
BUILD_WITH_FULL_STAGEFRIGHT := true
TARGET_LIBAGL_USE_GRALLOC_COPYBITS := true
TARGET_WEBKIT_USE_MORE_MEMORY := true

# Vold
TARGET_USE_CUSTOM_LUN_FILE_PATH := "/sys/devices/platform/s3c6410-usbgadget/gadget/lun0/file"

#Recovery
BOARD_UMS_LUNFILE := "/sys/devices/platform/s3c6410-usbgadget/gadget/lun0/file"
BOARD_HAS_NO_MISC_PARTITION := true

# Connectivity - Wi-Fi

WPA_SUPPLICANT_VERSION := VER_0_5_X
BOARD_WPA_SUPPLICANT_DRIVER := AWEXT
BOARD_WLAN_DEVICE := ra0
#WIFI_DRIVER_MODULE_ARG := "firmware_path=/system/etc/firmware/rt2870.bin"


# 3D
BOARD_EGL_CFG := device/pandigital/s3c6410/egl.cfg
USE_OPENGL_RENDERER := false

BOARD_USES_OVERLAY := true
USE_OPENGL_RENDERER := false

#Kernel
BOARD_USES_UBOOT := true
TARGET_PREBUILT_KERNEL := device/pandigital/s3c6410/kernel

