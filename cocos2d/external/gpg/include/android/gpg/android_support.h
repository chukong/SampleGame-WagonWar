/**
 * @file gpg/android_support.h
 * @copyright Copyright 2014 Google Inc. All Rights Reserved.
 * @brief Support functions used when running on pre- Android 4.0 devices.
 */

#ifndef GPG_ANDROID_SUPPORT_H_
#define GPG_ANDROID_SUPPORT_H_

#ifndef __cplusplus
#error Header file supports C++ only
#endif  // __cplusplus

#include "gpg/common.h"

namespace gpg {

/**
 * Functions which enable pre- Android 4.0 support.
 *
 * <h1>Android Lifecycle Callbacks</h1>
 *
 * For apps which target Android 2.3 or 3.x devices (API Version prior to 14),
 * Play Game Services has no way to automatically receive Activity lifecycle
 * callbacks. In these cases, Play Game Services relies on the owning Activity
 * to notify it of lifecycle events. Any Activity which owns a GameServices
 * object should call the AndroidSupport::* functions from within their own
 * lifecycle callback functions. The arguments in these functions match those
 * provided by Android, so no additional processing is necessary.
 *
 * <h1>Example code for a pre- Android 4.0 game using a Java Activity</h1>
 * In your Java Activity, please add the following. You may need to merge this
 * code with your existing lifecycle functions.
 * \code
 * public class YourActivity extends Activity {
 *   protected void onCreate(Bundle savedInstanceState){
 *     super.onCreate(savedInstanceState);
 *     nativeOnActivityCreated(this, savedInstanceState);
 *   }
 *
 *   protected void onDestroy(){
 *     super.onDestroy();
 *     nativeOnActivityDestroyed(this);
 *   }
 *
 *   protected void onPause(){
 *     super.onPause();
 *     nativeOnActivityPaused(this);
 *   }
 *
 *   protected void onResume(){
 *     super.onResume();
 *     nativeOnActivityResumed(this);
 *   }
 *
 *   protected void onStart(){
 *     super.onStart();
 *     nativeOnActivityStarted(this);
 *   }
 *
 *   protected void onStop(){
 *     super.onStop();
 *     nativeOnActivityStopped(this);
 *   }
 *
 *   protected void onSaveInstanceState(Bundle outState) {
 *     super.onSaveInstanceState(outState);
 *     nativeOnActivitySaveInstanceState(this, outState);
 *   }
 *
 *   // Implemented in C++.
 *   private static native void nativeOnActivityCreated(
 *       Activity activity, Bundle savedInstanceState);
 *   private static native void nativeOnActivityDestroyed(Activity activity);
 *   private static native void nativeOnActivityPaused(Activity activity);
 *   private static native void nativeOnActivityResumed(Activity activity);
 *   private static native void nativeOnActivitySaveInstanceState(
 *       Activity activity,
 *       Bundle outState);
 *   private static native void nativeOnActivityStarted(Activity activity);
 *   private static native void nativeOnActivityStopped(Activity activity);
 * }
 * \endcode
 * Then, in your native library, add the following forwarding functions.
 * \code
 * void Java_com_example_yourapp_YourActivity_nativeOnActivityCreated(
 *     JNIEnv *env,
 *     jobject thiz,
 *     jobject activity,
 *     jobject saved_instance_state) {
 *   gpg::AndroidSupport::OnActivityCreated(env,
 *                                          activity,
 *                                          saved_instance_state);
 * }
 *
 * void Java_com_example_yourapp_YourActivity_nativeOnActivityDestroyed(
 *     JNIEnv *env, jobject thiz, jobject activity) {
 *   gpg::AndroidSupport::OnActivityDestroyed(env, activity);
 * }
 *
 * void Java_com_example_yourapp_YourActivity_nativeOnActivityPaused(
 *     JNIEnv *env, jobject thiz, jobject activity) {
 *   gpg::AndroidSupport::OnActivityPaused(env, activity);
 * }
 *
 * void Java_com_example_yourapp_YourActivity_nativeOnActivityResumed(
 *     JNIEnv *env, jobject thiz, jobject activity) {
 *   gpg::AndroidSupport::OnActivityResumed(env, activity);
 * }
 *
 * void Java_com_example_yourapp_YourActivity_nativeOnActivitySaveInstanceState(
 *     JNIEnv *env, jobject thiz, jobject activity, jobject out_state) {
 *   gpg::AndroidSupport::OnActivitySaveInstanceState(env, activity, out_state);
 * }
 *
 * void Java_com_example_yourapp_YourActivity_nativeOnActivityStarted(
 *     JNIEnv *env, jobject thiz, jobject activity) {
 *   gpg::AndroidSupport::OnActivityStarted(env, activity);
 * }
 *
 * void Java_com_example_yourapp_YourActivity_nativeOnActivityStopped(
 *     JNIEnv *env, jobject thiz, jobject activity) {
 *   gpg::AndroidSupport::OnActivityStopped(env, activity);
 * }
 * \endcode
 */
struct GPG_EXPORT AndroidSupport {
  static void OnActivityCreated(JNIEnv *env,
                                jobject activity,
                                jobject saved_instance_state);
  static void OnActivityDestroyed(JNIEnv *env, jobject activity);
  static void OnActivityPaused(JNIEnv *env, jobject activity);
  static void OnActivityResumed(JNIEnv *env, jobject activity);
  static void OnActivitySaveInstanceState(JNIEnv *env,
                                          jobject activity,
                                          jobject out_state);
  static void OnActivityStarted(JNIEnv *env, jobject activity);
  static void OnActivityStopped(JNIEnv *env, jobject activity);
};

}  // namespace gpg

#endif  // GPG_ANDROID_SUPPORT_H_
