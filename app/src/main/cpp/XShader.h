//
// Created by Zach on 30/8/18.
//

#ifndef FXPLAY_XSHADER_H
#define FXPLAY_XSHADER_H

enum XShaderType {
    XSHADER_YUV420P = 0, // 软解码和虚拟机
    XSHADER_NV12 = 25, // 手机
    XSHADER_NV21 = 26
};

class XShader {
public:
    virtual bool Init(XShaderType type = XSHADER_YUV420P);

    // 获取材质并映射到内存
    virtual void GetTexture(unsigned int index, int width, int height, unsigned char *buf, bool isalpha=false);

    virtual void Draw();

protected:
    unsigned int vsh = 0;
    unsigned int fsh = 0;
    unsigned int program = 0;
    unsigned int texts[100] = {0};
};


#endif //FXPLAY_XSHADER_H
