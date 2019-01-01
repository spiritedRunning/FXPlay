//
// Created by Zach on 28/8/18.
//

#include "IDecode.h"
#include "XLog.h"


void IDecode::Update(XData pkt) {
    if (pkt.isAudio != isAudio) {
        return;
    }
    while (!isExit) {

        packsMutex.lock();
        if (packs.size() < maxList) {
            // 生产者
            packs.push_back(pkt);
            packsMutex.unlock();
            break;
        }
        packsMutex.unlock();
        XSleep(1);
    }
}


void IDecode::Main() {
    while (!isExit) {
        packsMutex.lock();


        // 判断音视频同步
        if (!isAudio && synPts > 0) {
            if (synPts < pts) { // 音频比视频慢
                packsMutex.unlock();
                XSleep(1);
                continue;
            }
        }

        if (packs.empty()) {
            packsMutex.unlock();
            XSleep(1);
            continue;
        }
        // 取出packet 消费者
        XData pack = packs.front();
        packs.pop_front();

        // 发送解码数据到解码线程. 一个数据包，可能解码多个结果
        if (this->SendPacket(pack)) {
            while (!isExit) {
                // 获取解码数据
                XData frame = RecvFrame();
                if (!frame.data) {
                    break;
                }
                pts = frame.pts;
//                XLOGE("RecvFrame %d", frame.size);
                this->Notify(frame);
            }

        }
        pack.Drop();

        packsMutex.unlock();
    }
}