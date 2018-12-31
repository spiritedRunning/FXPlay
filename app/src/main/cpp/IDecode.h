//
// Created by Zach on 28/8/18.
//

#include <list>
#include "XParameter.h"
#include "IObserver.h"

#ifndef FXPLAY_IDECODE_H
#define FXPLAY_IDECODE_H


// 支持硬解码
class IDecode : public IObserver {
public:
    // 打开解码器
    virtual bool Open(XParameter para, bool isHard=false) = 0;

    // future模型 发送数据到线程解码
    virtual bool SendPacket(XData pkt) = 0;
    // 从线程中获取解码结果。 再次调用会复用上次空间， 线程不安全
    virtual XData RecvFrame() = 0;
    // 由主体notify的数据 阻塞
    virtual void Update(XData pkt);

    bool isAudio = false;
    int maxList = 100; // 假如每秒缓冲25帧， 大概缓冲时间4s

protected:
    virtual void Main();
    // 读取缓冲
    std::list<XData> packs;
    std::mutex packsMutex;
};

#endif //FXPLAY_IDECODE_H
