# Release name
PRODUCT_RELEASE_NAME := s3c6410

# Inherit some common CM stuff.
$(call inherit-product, vendor/cm/config/common_full_tablet_wifionly.mk)

# Inherit device configuration
$(call inherit-product, device/pandigital/s3c6410/device.mk)

## Device identifier. This must come after all inclusions
PRODUCT_DEVICE       := s3c6410
PRODUCT_NAME         := cm_s3c6410
PRODUCT_BRAND        := Pandigital
PRODUCT_MODEL        := s3c6410
PRODUCT_MANUFACTURER := Pandigital

#Set build fingerprint / ID / Product Name ect.
#PRODUCT_BUILD_PROP_OVERRIDES += \
#    PRODUCT_NAME=picasso \
#    BUILD_FINGERPRINT="acer/a500_ww_gen1/picasso:4.0.3/IML74K/1329824557:user/release-keys" \
#    PRIVATE_BUILD_DESC="a500_ww_gen1-user 4.0.3 IML74K 1329824557 release-keys"

