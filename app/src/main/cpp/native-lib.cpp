#include <jni.h>
#include <string>
#include <android/native_window_jni.h>

#include "XLog.h"
#include "IPlayerProxy.h"


extern "C" JNIEXPORT
jint JNI_OnLoad(JavaVM *vm, void *res) {
    IPlayerProxy::Get()->Init(vm);

//    IPlayerProxy::Get()->Open("/sdcard/iloveyou.mp4");
//    IPlayerProxy::Get()->Start();
    return JNI_VERSION_1_4;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_quanshi_uc_fxplay_XPlay_InitView(JNIEnv *env, jobject instance, jobject surface) {
    ANativeWindow *win =  ANativeWindow_fromSurface(env, surface);
    IPlayerProxy::Get()->InitView(win);

}


extern "C"
JNIEXPORT void JNICALL
Java_com_quanshi_uc_fxplay_OpenUrlActivity_Open(JNIEnv *env, jobject instance, jstring url_) {
    const char *url = env->GetStringUTFChars(url_, 0);

    XLOGI("start open file : %s", url);
    IPlayerProxy::Get()->Open(url);
    IPlayerProxy::Get()->Start();
    env->ReleaseStringUTFChars(url_, url);
}

extern "C"
JNIEXPORT jdouble JNICALL
Java_com_quanshi_uc_fxplay_MainActivity_PlayPos(JNIEnv *env, jobject instance) {
    return IPlayerProxy::Get()->PlayPos();

}