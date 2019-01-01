
#ifndef FXPLAY_IPLAYERPROXY_H
#define FXPLAY_IPLAYERPROXY_H


#include <mutex>
#include "IPlayer.h"


class IPlayerProxy : public IPlayer{
public:
    static IPlayerProxy *Get() {
        static IPlayerProxy px;
        return &px;
    }
    void Init(void *vm = 0);
    virtual bool Open(const char *path);
    virtual void Close();
    virtual bool Start();
    virtual void InitView(void *win);

    virtual bool Seek(double pos);
    // 获取当前播放进度 0.0~1.0
    virtual double PlayPos();

protected:
    IPlayerProxy(){}
    IPlayer *player = 0;
    std::mutex mux;
};


#endif //FXPLAY_IPLAYERPROXY_H
