#include <jni.h>
#include <string>
#include <android/native_window_jni.h>

#include "FFDemux.h"
#include "XLog.h"
#include "IDecode.h"
#include "FFDecode.h"
#include "XEGL.h"
#include "XShader.h"
#include "IVideoView.h"
#include "GLVideoView.h"
#include "FFResample.h"
#include "IAudioPlay.h"
#include "SLAudioPlay.h"


class TestObs : public IObserver {
public:
    void Update(XData d) {
//        XLOGI("TestObs update data size: %d ", d.size);
    }
};

IVideoView *view = NULL;

extern "C" JNIEXPORT
jint JNI_OnLoad(JavaVM *vm, void *res) {
    FFDecode::InitHard(vm);


    // 测试代码
    TestObs *tobs = new TestObs();
    IDemux *de = new FFDemux();
//    de->AddObs(tobs);
    de->Open("/sdcard/iloveyou.mp4");

    XLOGI("start video decode");
    IDecode *vdecode = new FFDecode();
    vdecode->Open(de->GetVPara(), true);

    XLOGI("start audio decode");
    IDecode *adecode = new FFDecode();
    adecode->Open(de->GetAPara());
    de->AddObs(vdecode);
    de->AddObs(adecode);

    view = new GLVideoView();
    vdecode->AddObs(view);

    IResample *resample = new FFResample();
    XParameter outPara = de->GetAPara();
    resample->Open(de->GetAPara(), outPara);
    adecode->AddObs(resample);

    IAudioPlay *audioPlay = new SLAudioPlay();
    audioPlay->StartPlay(outPara);
    resample->AddObs(audioPlay);

    de->Start();
    vdecode->Start();
    adecode->Start();
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
    view->SetRender(win);
//    XEGL::Get()->Init(win);
//    XShader shader;
//    shader.Init();


}