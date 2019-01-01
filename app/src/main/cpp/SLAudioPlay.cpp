#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include "SLAudioPlay.h"
#include "XLog.h"

static SLObjectItf g_objectItf = NULL;
static SLEngineItf eng = NULL;
static SLObjectItf mix = NULL;
static SLObjectItf player = NULL;
static SLPlayItf iplayer = NULL;
static SLAndroidSimpleBufferQueueItf pcmQueue = NULL;

SLAudioPlay::SLAudioPlay() {
    buf = new unsigned char[1024*1024];
}

SLAudioPlay::~SLAudioPlay() {
    delete buf;
    buf = 0;
}

static SLEngineItf CreateSL() {
    SLresult re;
    SLEngineItf engineItf;
    re = slCreateEngine(&g_objectItf, 0, 0, 0, 0, 0);
    if (re != SL_RESULT_SUCCESS) {
        return NULL;
    }
    re = (*g_objectItf)->Realize(g_objectItf, SL_BOOLEAN_FALSE);  // 等待创建
    if (re != SL_RESULT_SUCCESS) {
        return NULL;
    }
    re = (*g_objectItf)->GetInterface(g_objectItf, SL_IID_ENGINE, &engineItf);
    if (re != SL_RESULT_SUCCESS) {
        return NULL;
    }

    return engineItf;
}

void SLAudioPlay::PlayCall(void *bufq) {
    if (!bufq) {
        return;
    }
    SLAndroidSimpleBufferQueueItf bf = (SLAndroidSimpleBufferQueueItf) bufq;
//    XLOGI("SLAudioPlay PlayCall succ");
    // 阻塞
    XData d = GetData();
    if (d.size <= 0) {
        XLOGE("GetData size is 0");
        return;
    }
    if (!buf) {
        return;
    }
    memcpy(buf, d.data, d.size);
    mux.lock();
    if (pcmQueue && (*pcmQueue)) {
        (*pcmQueue)->Enqueue(pcmQueue, buf, d.size);
    }

    mux.unlock();
    d.Drop();

}

 /**
 * 回调函数队列
 */
static void PcmCall(SLAndroidSimpleBufferQueueItf bf, void *context) {
    SLAudioPlay *ap = (SLAudioPlay *) context;
    if (!ap) {
        XLOGE("PcmCall failed, context is null");
        return;
    }
    ap->PlayCall((void *)bf);

}

void SLAudioPlay::Close() {
    IAudioPlay::Clear();
    mux.lock();
    // 停止播放
    if (iplayer && (*iplayer)) {
        (*iplayer)->SetPlayState(iplayer, SL_PLAYSTATE_STOPPED);
    }
    // 清理播放队列
    if (pcmQueue && (*pcmQueue)) {
        (*pcmQueue)->Clear(pcmQueue);
    }
    // 销毁player对象
    if (player && (*player)) {
        (*player)->Destroy(player);
    }
    // 销毁混音器
    if (mix && (*mix)) {
        (*mix)->Destroy(mix);
    }
    // 销毁播放引擎
    if (g_objectItf && (*g_objectItf)) {
        (*g_objectItf)->Destroy(g_objectItf);
    }


    g_objectItf = NULL;
    eng = NULL;
    mix = NULL;
    player = NULL;
    iplayer = NULL;
    pcmQueue = NULL;

    mux.unlock();
}


bool SLAudioPlay::StartPlay(XParameter out) {
    Close();

    mux.lock();
    // 1 创建引擎
    eng = CreateSL();
    if (eng) {
        XLOGD("CreateSL succ");
    } else {
        mux.unlock();
        XLOGE("CreateSL failed!");
        return false;
    }

    // 2 创建混音器
    SLresult re = 0;
    re = (*eng)->CreateOutputMix(eng, &mix, 0, 0, 0);
    if (re != SL_RESULT_SUCCESS) {
        mux.unlock();
        XLOGE("CreateOutputMix failed");
        return false;
    }

    re = (*mix)->Realize(mix, SL_BOOLEAN_FALSE);  // 阻塞式等待创建完成
    if (re != SL_RESULT_SUCCESS) {
        mux.unlock();
        XLOGE("mix realize failed");
        return false;
    }
    SLDataLocator_OutputMix outMix = {SL_DATALOCATOR_OUTPUTMIX, mix};
    SLDataSink audioSink = {&outMix, 0};

    // 3 配置音频信息
    // 缓冲队列
    SLDataLocator_AndroidSimpleBufferQueue que = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 10};
    SLDataFormat_PCM pcm = {
            SL_DATAFORMAT_PCM,
            (SLuint32)out.channels, // 声道数
            (SLuint32)out.sample_rate * 1000, // 注意，不一定可靠 ， SL_SAMPLINGRATE_44_1
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,
            SL_BYTEORDER_LITTLEENDIAN   // 字节序
    };
    SLDataSource ds = {&que, &pcm};

    // 4 创建播放器
    const SLInterfaceID ids[] = {SL_IID_BUFFERQUEUE};
    const SLboolean req[] = {SL_BOOLEAN_TRUE};

    re = (*eng)->CreateAudioPlayer(eng, &player, &ds, &audioSink, sizeof(ids) / sizeof(SLInterfaceID), ids, req);
    if (re != SL_RESULT_SUCCESS) {
        mux.unlock();
        XLOGE("CreateAudioPlayer failed");
        return false;
    }
    (*player)->Realize(player, SL_BOOLEAN_FALSE);
    re = (*player)->GetInterface(player, SL_IID_PLAY, &iplayer);
    if (re != SL_RESULT_SUCCESS) {
        mux.unlock();
        XLOGE("player GetInterface SL_IID_PLAY failed");
        return false;
    }
    re = (*player)->GetInterface(player, SL_IID_BUFFERQUEUE, &pcmQueue);
    if (re != SL_RESULT_SUCCESS) {
        mux.unlock();
        XLOGE("player GetInterface SL_IID_BUFFERQUEUE failed");
        return false;
    }

    // 设置回调函数，播放队列空调用
    (*pcmQueue)->RegisterCallback(pcmQueue, PcmCall, this);
    (*iplayer)->SetPlayState(iplayer, SL_PLAYSTATE_PLAYING);

    // 启动队列回调
    (*pcmQueue)->Enqueue(pcmQueue, "", 1);

    isExit = false;
    XLOGI("SLAudioPlay StartPlay succ");
    mux.unlock();
    return true;
}