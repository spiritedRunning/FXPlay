//
// Created by Zach on 30/8/18.
//

#ifndef FXPLAY_IVIDEOVIEW_H
#define FXPLAY_IVIDEOVIEW_H


#include "XData.h"
#include "IObserver.h"

class IVideoView: public IObserver {
public:
    virtual void SetRender(void *win) = 0;
    virtual void Render(XData data) = 0;
    virtual void Update(XData data);
};


#endif //FXPLAY_IVIDEOVIEW_H
