
#ifndef FXPLAY_XLOG_H
#define FXPLAY_XLOG_H


class XLog {

};

// 某些手机DEBUG日志会被屏蔽
#include <android/log.h>
#define XLOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "FXPlay", __VA_ARGS__)
#define XLOGI(...) __android_log_print(ANDROID_LOG_INFO, "FXPlay", __VA_ARGS__)
#define XLOGE(...) __android_log_print(ANDROID_LOG_ERROR, "FXPlay", __VA_ARGS__)



#endif //FXPLAY_XLOG_H
