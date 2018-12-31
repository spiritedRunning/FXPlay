//
// Created by Zach on 30/8/18.
//

#ifndef FXPLAY_GLVIDEOVIEW_H
#define FXPLAY_GLVIDEOVIEW_H


#include "XData.h"
#include "IVideoView.h"

class XTexture;

class GLVideoView : public IVideoView{
public:
    virtual void SetRender(void *win);
    virtual void Render(XData data);

protected:
    void *view = 0;
    XTexture *txt = 0;
};


#endif //FXPLAY_GLVIDEOVIEW_H
