
#ifndef FXPLAY_IPLAYER_H
#define FXPLAY_IPLAYER_H


#include <mutex>
#include "XThread.h"
#include "XParameter.h"

class IDemux;
class IDecode;
class IResample;
class IVideoView;
class IAudioPlay;


class IPlayer : public XThread{
public:
    static IPlayer *Get(unsigned char index = 0);
    virtual bool Open(const char *path);
    virtual void Close();

    virtual bool Start();
    virtual void InitView(void *win);

    // 获取当前播放进度 0.0~1.0
    virtual double PlayPos();
    virtual bool Seek(double pos);
    virtual void SetPause(bool isP);

    // 是否硬解码
    bool isHardDecode = true;
    // 音频输出参数配置
    XParameter outPara;

    IDemux *demux = 0;
    IDecode *vdecode = 0;
    IDecode *adecode = 0;
    IResample *resample = 0;
    IVideoView *videoView = 0;
    IAudioPlay *audioPlay = 0;

protected:
    // 用作音视频同步
    void Main();
    std::mutex mux;
    IPlayer(){};

};


#endif //FXPLAY_IPLAYER_H
