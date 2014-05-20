/**
 * @file gpg/android_platform_configuration.h
 * @copyright Copyright 2014 Google Inc. All Rights Reserved.
 * @brief Platform-specific builder configuration for Android.
 */

#ifndef GPG_ANDROID_PLATFORM_CONFIGURATION_H_
#define GPG_ANDROID_PLATFORM_CONFIGURATION_H_

#ifndef __cplusplus
#error Header file supports C++ only
#endif  // __cplusplus

#include <jni.h>
#include <memory>
#include "gpg/common.h"

namespace gpg {

class AndroidPlatformConfigurationImpl;

class GPG_EXPORT AndroidPlatformConfiguration {
 public:
  AndroidPlatformConfiguration();
  ~AndroidPlatformConfiguration();

  /**
   * Specifies the activity that will own the GameServices object.
   *
   * The provided activity must implement Activity.onActivityResult() in such a
   * way that it allows the superclass implementation to handle invocations with
   * a requestCode having the special value 0x475047. This special value should
   * not be used by the application as the request code for calls to
   * Activity.startActivityForResult(), Activity.startIntentSenderForResult(),
   * etc. Standard practice for overriding Activity.onActivityResult(), which is
   * to pass invocations with unrecognized requestCodes to the superclass
   * implementation, should be followed.
   */
  AndroidPlatformConfiguration &SetActivity(jobject android_app_activity);

  bool Valid() const;

 private:
  AndroidPlatformConfiguration(AndroidPlatformConfiguration const &) = delete;
  AndroidPlatformConfiguration &operator=(
      AndroidPlatformConfiguration const &) = delete;

  friend class GameServicesImpl;
  std::unique_ptr<AndroidPlatformConfigurationImpl> impl_;
};

}  // namespace gpg

#endif  // GPG_ANDROID_PLATFORM_CONFIGURATION_H_
