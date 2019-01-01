
#include "IPlayer.h"
#include "IDemux.h"
#include "IDecode.h"
#include "IResample.h"
#include "IVideoView.h"

#include "IAudioPlay.h"
#include "XLog.h"

void IPlayer::Main() {
    while (!isExit) {
        mux.lock();

        if (!audioPlay || !vdecode) {
            mux.unlock();
            XSleep(2);
            continue;
        }
        // 获取音频pts, 告诉视频
        int apts = audioPlay->pts;
//        XLOGI("apts = %d", apts);
        vdecode->synPts = apts;

        mux.unlock();
        XSleep(2);
    }


}

IPlayer *IPlayer::Get(unsigned char index) {
    static IPlayer p[256];
    return &p[index];
}

bool IPlayer::Open(const char *path) {
    mux.lock();
    if (!demux || !demux->Open(path)) {
        XLOGE("demux open %s failed", path);
        mux.unlock();
        return false;
    }
    // 如果解封之后是原始数据，则不需要解码
    if (!vdecode || !vdecode->Open(demux->GetVPara(), isHardDecode)) {
        XLOGE("vdecode open %s failed", path);
    }
    if (!adecode || !adecode->Open(demux->GetAPara())) {
        XLOGE("adecode open %s failed", path);
    }
    // 解码后或解封后可能是直接能播放的数据，则不需要重采样
    if (outPara.sample_rate <= 0) { // 如用户未配置，取默认参数
        outPara = demux->GetAPara();
    }
    if (!resample || !resample->Open(demux->GetAPara(), outPara)) {
        XLOGE("resample open %s failed", path);
    }

    mux.unlock();
    return true;
}

bool IPlayer::Start() {
    mux.lock();
    if (!demux || !demux->Start()) {
        XLOGE("demux start failed");
        mux.unlock();
        return false;
    }
    // 先启动音频，让音频先缓冲
    if (adecode) {
        adecode->Start();
    }
    if (audioPlay) {
        audioPlay->StartPlay(outPara);
    }

    if (vdecode) {
        vdecode->Start();
    }
    XThread::Start();

    mux.unlock();
    return true;
}

void IPlayer::InitView(void *win) {
    if (videoView) {
        videoView->SetRender(win);
    }
}