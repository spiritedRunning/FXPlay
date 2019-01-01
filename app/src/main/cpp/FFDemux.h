
#ifndef FXPLAY_FFDEMUX_H
#define FXPLAY_FFDEMUX_H


#include "IDemux.h"
struct AVFormatContext;

class FFDemux : public IDemux{
public:
    FFDemux();

    // 打开文件，或流媒体  rtmp http rtsp
    virtual bool Open(const char *url);
    virtual void Close();
    // 获取视频参数
    virtual XParameter GetVPara();
    // 获取音频参数
    virtual XParameter GetAPara();
    // 读取一帧数据， 数据由调用者清理
    virtual XData Read();

private:
    AVFormatContext *ic = 0;    // 只有在无参数构造参数时，赋值才生效
    std::mutex mux;
    int audioStream = 1;
    int videoStream = 0;
};


#endif //FXPLAY_FFDEMUX_H
