//
// Created by Zach on 28/6/18.
//

#ifndef FXPLAY_IOBSERVER_H
#define FXPLAY_IOBSERVER_H


#include <vector>
#include <mutex>
#include "XData.h"
#include "XThread.h"

class IObserver : public XThread {
public:
    // 观察者接收数据函数
    virtual void Update(XData data) {}

    // 主体函数 添加观察者(线程安全)
    void AddObs(IObserver *obs);
    // 通知所有观察者(线程安全)
    void Notify(XData data);

protected:
    std::vector<IObserver *>obss;
    std::mutex mux;  // c++11 支持

};


#endif //FXPLAY_IOBSERVER_H
