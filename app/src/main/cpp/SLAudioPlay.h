
#ifndef FXPLAY_SLAUDIOPLAY_H
#define FXPLAY_SLAUDIOPLAY_H


#include "IAudioPlay.h"

class SLAudioPlay: public IAudioPlay {
public:
    virtual bool StartPlay(XParameter out);
    void PlayCall(void *bufq);
};


#endif //FXPLAY_SLAUDIOPLAY_H
