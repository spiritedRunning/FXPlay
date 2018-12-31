#include "FFDemux.h"
#include "XLog.h"

extern "C" {
#include <libavformat/avformat.h>
}


FFDemux::FFDemux() {
    static bool isFirst = true;
    if (isFirst) {
        isFirst = false;
        // 注册封装器
        av_register_all();
        // 注册解码器
        avcodec_register_all();
        // 初始化网络
        avformat_network_init();
        XLOGI("register ffmpeg");
    }
}

// 打开文件，或流媒体  rtmp http rtsp
bool FFDemux::Open(const char *url) {
    XLOGI("Open file %s begin", url);
    int re = avformat_open_input(&ic, url, 0, 0);
    if (re != 0) {
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf));
        XLOGE("FFDemux open %s failed", url);
        return false;
    }
    XLOGI("FFDemux open succ");

    // 读取文件信息, 非MP4格式需要
    re = avformat_find_stream_info(ic, 0);
    if (re != 0) {
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf));
        XLOGE("avformat_find_stream_info open %s failed", url);
        return false;
    }
    this->totalMs = ic->duration/(AV_TIME_BASE/1000);  // 此值不一定有
    XLOGI("total ms = %d", totalMs);

    GetVPara();
    GetAPara();

    return true;
}

// 获取视频参数
XParameter FFDemux::GetVPara() {
    if (!ic) {
        XLOGE("GetVPara failed, is is null");
        return XParameter();
    }
    // 获取了视频流的索引
    int re = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, -1, -1, 0, 0);
    if (re < 0) {
        XLOGE("av_find_best_stream failed");
        return XParameter();
    }
    videoStream = re;
    XParameter para;
    para.para = ic->streams[re]->codecpar;
    return para;
}


// 获取音频参数
XParameter FFDemux::GetAPara() {
    if (!ic) {
        XLOGE("GetVPara failed, is is null");
        return XParameter();
    }
    // 获取了音频流的索引
    int re = av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, -1, -1, 0, 0);
    if (re < 0) {
        XLOGE("av_find_best_stream failed");
        return XParameter();
    }
    audioStream = re;
    XParameter para;
    para.para = ic->streams[re]->codecpar;
    para.channels = ic->streams[re]->codecpar->channels;
    para.sample_rate = ic->streams[re]->codecpar->sample_rate;
    return para;
}


// 读取一帧数据， 数据由调用者清理
XData FFDemux::Read() {
    if (!ic) {
        return XData();
    }
    XData d;
    AVPacket *pkt = av_packet_alloc();
    int re = av_read_frame(ic, pkt);
    if (re != 0) { // 失败
        av_packet_free(&pkt);
        return XData();
    }
//    XLOGI("pack size is %d, pts = %lld", pkt->size, pkt->pts);
    d.data = (unsigned char*)pkt;
    d.size = pkt->size;

    if (pkt->stream_index == audioStream) {
        d.isAudio = true;
    } else if (pkt->stream_index == videoStream) {
        d.isAudio = false;
    } else {
        av_packet_free(&pkt);
        return XData();
    }
    return d;
}

