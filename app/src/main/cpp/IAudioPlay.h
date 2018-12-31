//
// Created by Zach on 31/8/18.
//

#ifndef FXPLAY_IAUDIOPLAY_H
#define FXPLAY_IAUDIOPLAY_H


#include "IObserver.h"
#include "XParameter.h"

class IAudioPlay : public IObserver{

public:
    virtual void Update(XData data);
    virtual bool StartPlay(XParameter out) = 0;
};


#endif //FXPLAY_IAUDIOPLAY_H
