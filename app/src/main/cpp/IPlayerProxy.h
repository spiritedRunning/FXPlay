
#ifndef FXPLAY_IPLAYERPROXY_H
#define FXPLAY_IPLAYERPROXY_H


#include <mutex>
#include "IPlayer.h"
#

class IPlayerProxy : public IPlayer{
public:
    static IPlayerProxy *Get() {
        static IPlayerProxy px;
        return &px;
    }
    void Init(void *vm = 0);
    virtual bool Open(const char *path);
    virtual bool Start();
    virtual void InitView(void *win);

protected:
    IPlayerProxy(){}
    IPlayer *player = 0;
    std::mutex mux;
};


#endif //FXPLAY_IPLAYERPROXY_H
