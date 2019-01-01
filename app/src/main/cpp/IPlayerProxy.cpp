
#include "IPlayerProxy.h"
#include "FFPlayerBuilder.h"

void IPlayerProxy::Close() {
    mux.lock();
    if (player) {
        player->Close();
    }
    mux.unlock();
}

void IPlayerProxy::Init(void *vm) {
    mux.lock();
    if (vm) {
        FFPlayerBuilder::InitHard(vm);
    }
    if (!player) {
        player = FFPlayerBuilder::Get()->BuilderPlayer();
    }
    mux.unlock();
}

double IPlayerProxy::PlayPos() {
    double pos = 0.0;
    mux.lock();
    if (player) {
        pos = player->PlayPos();
    }
    mux.unlock();
    return pos;
}



bool IPlayerProxy::Open(const char *path) {
    bool re = false;
    mux.lock();
    if (player) {
        re = player->Open(path);
    }
    mux.unlock();
    return re;
}
bool IPlayerProxy::Start() {
    bool re = false;
    mux.lock();
    if (player) {
        re = player->Start();
    }
    mux.unlock();
    return re;
}
void IPlayerProxy::InitView(void *win) {
    mux.lock();
    if (player) {
        player->InitView(win);
    }
    mux.unlock();

}