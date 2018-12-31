
#ifndef FXPLAY_XTEXTURE_H
#define FXPLAY_XTEXTURE_H


class XTexture {

public:
    static XTexture *Create();
    virtual bool Init(void *win) = 0;
    virtual void Draw(unsigned char *data[], int width, int height) = 0;

};


#endif //FXPLAY_XTEXTURE_H
