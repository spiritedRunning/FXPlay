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
    virtual void Close();

protected:
    void *view = 0;
    XTexture *txt = 0;
    std::mutex mux;
};


#endif //FXPLAY_GLVIDEOVIEW_H
