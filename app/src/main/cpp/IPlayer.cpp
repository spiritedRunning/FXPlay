
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

void IPlayer::Close() {
    mux.lock();
    // 1 关闭主体线程
    XThread::Stop();
    if (demux) {
        demux->Stop();
    }
    if (vdecode) {
        vdecode->Stop();
    }
    if (adecode) {
        adecode->Stop();
    }

    // 2 清理缓冲队列
    if (vdecode) {
        vdecode->Clear();
    }
    if (adecode) {
        adecode->Clear();
    }
    if (audioPlay) {
        audioPlay->Clear();
    }

    // 3 清理资源
    if (audioPlay) {
        audioPlay->Close();
    }
    if (videoView) {
        videoView->Close();
    }
    if (vdecode) {
        vdecode->Close();
    }
    if (adecode) {
        adecode->Close();
    }
    if (demux) {
        demux->Close();
    }

    mux.unlock();

}

double IPlayer::PlayPos() {
    double pos = 0.0;
    mux.lock();

    int total = 0;
    if (demux) {
        total = demux->totalMs;
    }
    if (total > 0) {
        if (vdecode) {
            pos = (double)vdecode->pts / (double)total;
        }
    }
    mux.unlock();
    return pos;
}

bool IPlayer::Open(const char *path) {
    Close();
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
//    if (outPara.sample_rate <= 0) { // 如用户未配置，取默认参数
        outPara = demux->GetAPara();
//    }
    if (!resample || !resample->Open(demux->GetAPara(), outPara)) {
        XLOGE("resample open %s failed", path);
    }

    mux.unlock();
    return true;
}

bool IPlayer::Start() {
    mux.lock();
    // 这里注意线程的启动顺序!!
    if (vdecode) {
        vdecode->Start();
    }

    if (!demux || !demux->Start()) {
        mux.unlock();
        XLOGE("demux start failed");
        return false;
    }

    // 可以先启动音频，让音频先缓冲
    if (adecode) {
        adecode->Start();
    }
    if (audioPlay) {
        audioPlay->StartPlay(outPara);
    }

    // 启动同步线程
    XThread::Start();

    mux.unlock();
    return true;
}

void IPlayer::InitView(void *win) {
    if (videoView) {
        videoView->Close();
        videoView->SetRender(win);
    }
}