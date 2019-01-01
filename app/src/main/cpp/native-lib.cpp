#include <jni.h>
#include <string>
#include <android/native_window_jni.h>

#include "XLog.h"
#include "FFPlayerBuilder.h"


static IPlayer *player = NULL;

extern "C" JNIEXPORT
jint JNI_OnLoad(JavaVM *vm, void *res) {
    FFPlayerBuilder::InitHard(vm);

    player = FFPlayerBuilder::Get()->BuilderPlayer();

    player->Open("/sdcard/SUZHOU.mp4");
    player->Start();

//    de->Start();
//    vdecode->Start();
//    adecode->Start();
    return JNI_VERSION_1_4;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_quanshi_uc_fxplay_MainActivity_stringFromJNI( JNIEnv *env, jobject /* this */) {
    std::string hello = "Hello from";

//    XSleep(3000);
//    de->Stop();
//    for (;;) {
//        XData d = de->Read();
//        XLOGI("Read data size = %d", d.size);
//    }

    return env->NewStringUTF(hello.c_str());
}


extern "C"
JNIEXPORT void JNICALL
Java_com_quanshi_uc_fxplay_XPlay_InitView(JNIEnv *env, jobject instance, jobject surface) {
    ANativeWindow *win =  ANativeWindow_fromSurface(env, surface);
    if (player) {
        player->InitView(win);
    }

//    view->SetRender(win);
//    XEGL::Get()->Init(win);
//    XShader shader;
//    shader.Init();


}