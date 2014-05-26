LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := cocos_gpg_static
LOCAL_MODULE_FILENAME := gpg
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libgpg.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../include/android/gpg
include $(PREBUILT_STATIC_LIBRARY)
