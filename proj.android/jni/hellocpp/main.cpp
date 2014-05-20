#include "AppDelegate.h"
#include "cocos2d.h"
#include "CCEventType.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include <android/log.h>

#include "StateManager.h"

#define  LOG_TAG    "main"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

using namespace cocos2d;

void cocos_android_app_init (JNIEnv* env, jobject thiz) {
    LOGD("cocos_android_app_init");
    AppDelegate *pAppDelegate = new AppDelegate();
}

extern "C"
{

void Java_org_cocos2dx_cpp_AppActivity_nativeInitGPGS(JNIEnv*  env, jobject thiz, jobject activity)
{
    gpg::AndroidPlatformConfiguration platform_configuration;
    platform_configuration.SetActivity(activity);
    StateManager::InitServices(
                                platform_configuration,
                                [](gpg::AuthOperation op) {
                                    LOGD("Start oauth action ... by Jacky_Android");
                                   //  if (op == gpg::AuthOperation::SIGN_IN) {
                                   //      LOGD("Signing In...");
                                   //   } else {
                                   //      LOGD("Signing Out...");
                                   // }
                                },
                                [](gpg::AuthOperation op, gpg::AuthStatus status) {
                                    LOGD("Finished oauth action ... by Jacky_Android");
                                    // if (op == gpg::AuthOperation::SIGN_IN) {
                                    //     LOGD("Signing In.");
                                    // } else {
                                    //     LOGD("Signing Out.");
                                    // }
                                    switch( status )
                                    {
                                        case gpg::AuthStatus::VALID:
                                            LOGD("Signed in!!");
                                            break;
                                        case gpg::AuthStatus::ERROR_INTERNAL:
                                        case gpg::AuthStatus::ERROR_NOT_AUTHORIZED:
                                        case gpg::AuthStatus::ERROR_VERSION_UPDATE_REQUIRED:
                                        case gpg::AuthStatus::ERROR_TIMEOUT:
                                        default:
                                            LOGD("Sign-in failure");
                                            break;
                                    }
                               });

}

}
