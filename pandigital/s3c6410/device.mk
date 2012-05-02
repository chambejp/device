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

ifeq ($(TARGET_PREBUILT_KERNEL),)
LOCAL_KERNEL := device/pandigital/s3c6410/kernel
else
LOCAL_KERNEL := $(TARGET_PREBUILT_KERNEL)
endif

PRODUCT_COPY_FILES := \
	$(LOCAL_KERNEL):kernel \
	device/pandigital/s3c6410/init.s3c6410.rc:root/init.s3c6410.rc \
	device/pandigital/s3c6410/init.s3c6410.usb.rc:root/init.s3c6410.usb.rc \
	device/pandigital/s3c6410/ueventd.s3c6410.rc:root/ueventd.s3c6410.rc \
	frameworks/base/data/etc/android.hardware.usb.host.xml:system/etc/permissions/android.hardware.usb.host.xml \
	frameworks/base/data/etc/android.hardware.usb.accessory.xml:system/etc/permissions/android.hardware.usb.accessory.xml \
	device/pandigital/s3c6410/egl.cfg:system/lib/egl/egl.cfg \
	device/pandigital/s3c6410/vold.fstab:system/etc/vold.fstab \
	device/pandigital/s3c6410/prebuilt/lights.s3c6410.so:system/lib/hw/lights.s3c6410.so \
        device/pandigital/s3c6410/prebuilt/libaudio.so:obj/lib/libaudio.so \
        device/pandigital/s3c6410/prebuilt/libaudiopolicy.so:obj/lib/libaudiopolicy.so \
        device/pandigital/s3c6410/prebuilt/libaudio.so:system/lib/libaudio.so \
        device/pandigital/s3c6410/prebuilt/libaudiopolicy.so:system/lib/libaudiopolicy.so \
	device/pandigital/s3c6410/Wireless/RT2870STA/RT2870STA.dat:system/etc/Wireless/RT2870STA/RT2870STA.dat
	
# Prebuilt kl keymaps
PRODUCT_COPY_FILES += \
	device/pandigital/s3c6410/AVRCP.kl:system/usr/keylayout/AVRCP.kl \
	device/pandigital/s3c6410/Generic.kl:system/usr/keylayout/Generic.kl \
	device/pandigital/s3c6410/qwerty.kl:system/usr/keylayout/qwerty.kl

# Generated kcm keymaps
#PRODUCT_PACKAGES := \
     
PRODUCT_PACKAGES := \
        make_ext4fs \
	com.android.future.usb.accessory \
	hwcomposer.default \
	gralloc.s3c6410 \
	copybit.s3c6410 \
        sensors.s3c6410.so \
	libGLES_fimg


#PRODUCT_PROPERTY_OVERRIDES := \
#	hwui.render_dirty_regions=true

PRODUCT_CHARACTERISTICS := tablet,sdcard

# Sensors
PRODUCT_COPY_FILES += \
    device/pandigital/s3c6410/prebuilt/akmd/akmd:system/bin/akmd

#Wifi
PRODUCT_COPY_FILES += \
	device/pandigital/s3c6410/wpa_supplicant.conf:system/etc/wifi/wpa_supplicant.conf
//	device/pandigital/s3c6410/dhcpcd.conf:system/etc/dhcpcd/dhcpcd.conf

#Modules
PRODUCT_COPY_FILES += \
	device/pandigital/s3c6410/modules/rt3070sta.ko:system/lib/modules/rt3070sta.ko \
	device/pandigital/s3c6410/modules/rtnet3070sta.ko:system/lib/modules/rtnet3070sta.ko \
	device/pandigital/s3c6410/modules/rtutil3070sta.ko:system/lib/modules/rtutil3070sta.ko

#overlay
DEVICE_PACKAGE_OVERLAYS := \
    device/pandigital/s3c6410/overlay

PRODUCT_TAGS += dalvik.gc.type-precise

PRODUCT_PACKAGES += \
	librs_jni \
	com.android.future.usb.accessory

# Libs
PRODUCT_PACKAGES += \
    audio.primary.s3c6410 \
    libs3cjpeg \
    gralloc.s3c6410 \
    libGLES_fimg \
    hwcomposer.default \
    audio_policy.default \
    librs_jni \
    sensors.s3c6410 \


#These are the hardware-specific features
PRODUCT_COPY_FILES += \
    frameworks/base/data/etc/handheld_core_hardware.xml:system/etc/permissions/handheld_core_hardware.xml \
    frameworks/base/data/etc/android.hardware.camera.autofocus.xml:system/etc/permissions/android.hardware.camera.autofocus.xml \
    frameworks/base/data/etc/android.hardware.telephony.gsm.xml:system/etc/permissions/android.hardware.telephony.gsm.xml \
    frameworks/base/data/etc/android.hardware.location.xml:system/etc/permissions/android.hardware.location.xml \
    frameworks/base/data/etc/android.hardware.location.gps.xml:system/etc/permissions/android.hardware.location.gps.xml \
    frameworks/base/data/etc/android.hardware.wifi.xml:system/etc/permissions/android.hardware.wifi.xml \
    frameworks/base/data/etc/android.hardware.touchscreen.multitouch.distinct.xml:system/etc/permissions/android.hardware.touchscreen.multitouch.distinct.xml \
    frameworks/base/data/etc/android.software.sip.voip.xml:system/etc/permissions/android.software.sip.voip.xml \
    packages/wallpapers/LivePicker/android.software.live_wallpaper.xml:system/etc/permissions/android.software.live_wallpaper.xml



# Used by BusyBox
KERNEL_MODULES_DIR:=/lib/modules

# Enable Windows Media if supported by the board
WITH_WINDOWS_MEDIA:=true

# The OpenGL ES API level that is natively supported by this device.
# This is a 16.16 fixed point number
PRODUCT_PROPERTY_OVERRIDES := \
    ro.opengles.version=65537
    ro.sf.lcd_density=120
# These are the hardware-specific settings that are stored in system properties.
# Note that the only such settings should be the ones that are too low-level to
# be reachable from resources or other mechanisms.
PRODUCT_PROPERTY_OVERRIDES += \
       wifi.interface=ra0 \
       wifi.supplicant_scan_interval=20 \
       mobiledata.interfaces=ra0 \
       dalvik.vm.heapsize=32m \
       dalvik.vm.dexopt-data-only=1 \

# enable Google-specific location features,
# like NetworkLocationProvider and LocationCollector
PRODUCT_PROPERTY_OVERRIDES += \
        ro.com.google.locationfeatures=1 \
        ro.com.google.networklocation=1

# Extended JNI checks
# The extended JNI checks will cause the system to run more slowly, but they can spot a variety of nasty bugs
# before they have a chance to cause problems.
# Default=true for development builds, set by android buildsystem.
PRODUCT_PROPERTY_OVERRIDES += \
    ro.kernel.android.checkjni=0 \
    dalvik.vm.checkjni=false

# Screen density is actually considered a locale (since it is taken into account
# the the build-time selection of resources). The product definitions including
# this file must pay attention to the fact that the first entry in the final
# PRODUCT_LOCALES expansion must not be a density.
PRODUCT_LOCALES := mdpi

# Filesystem management tools
PRODUCT_PACKAGES += \
	make_ext4fs

$(call inherit-product-if-exists, vendor/pandigital/s3c6410/device-vendor.mk)
$(call inherit-product, frameworks/base/build/tablet-dalvik-heap.mk)

WIFI_BAND             := 802_11_BGN
