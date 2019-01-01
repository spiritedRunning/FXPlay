#include <jni.h>
#include <string>
#include <android/native_window_jni.h>

#include "XLog.h"
#include "IPlayerProxy.h"


extern "C" JNIEXPORT
jint JNI_OnLoad(JavaVM *vm, void *res) {
    IPlayerProxy::Get()->Init(vm);

    IPlayerProxy::Get()->Open("/sdcard/SUZHOU.mp4");
    IPlayerProxy::Get()->Start();
    return JNI_VERSION_1_4;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_quanshi_uc_fxplay_XPlay_InitView(JNIEnv *env, jobject instance, jobject surface) {
    ANativeWindow *win =  ANativeWindow_fromSurface(env, surface);
    IPlayerProxy::Get()->InitView(win);

}


extern "C" JNIEXPORT jstring JNICALL
Java_com_quanshi_uc_fxplay_MainActivity_stringFromJNI( JNIEnv *env, jobject /* this */) {
    std::string hello = "Hello from";

    return env->NewStringUTF(hello.c_str());
}


