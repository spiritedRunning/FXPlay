
#include "IPlayer.h"
#include "IDemux.h"
#include "IDecode.h"
#include "IResample.h"
#include "IVideoView.h"

#include "IAudioPlay.h"
#include "XLog.h"

IPlayer *IPlayer::Get(unsigned char index) {
    static IPlayer p[256];
    return &p[index];
}

bool IPlayer::Open(const char *path) {
    if (!demux || !demux->Open(path)) {
        XLOGE("demux open %s failed", path);
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
    XParameter outPara = demux->GetAPara();
    if (!resample || !resample->Open(demux->GetAPara(), outPara)) {
        XLOGE("resample open %s failed", path);
    }

    return true;
}

bool IPlayer::Start() {
    return true;
}
