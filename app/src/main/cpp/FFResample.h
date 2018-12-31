
#ifndef FXPLAY_FFRESAMPLE_H
#define FXPLAY_FFRESAMPLE_H


#include "IResample.h"

struct SwrContext;

class FFResample : public IResample{
public:
    virtual bool Open(XParameter in, XParameter out=XParameter());
    virtual XData Resample(XData indata);

protected:
    SwrContext *actx = 0;
};



#endif //FXPLAY_FFRESAMPLE_H
