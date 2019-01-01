
#ifndef FXPLAY_XEGL_H
#define FXPLAY_XEGL_H


class XEGL {
public:
    virtual bool Init(void *win) = 0;
    virtual void Close() = 0;
    virtual void Draw() = 0;
    static XEGL *Get(); //单件模式

protected:
    XEGL() {}
};


#endif //FXPLAY_XEGL_H
