#include "FFDemux.h"
#include "XLog.h"

extern "C" {
#include <libavformat/avformat.h>
}

/**
 * 分数转为浮点数, 方便计算
 */
static double r2d(AVRational r) {
    return r.num == 0 || r.den == 0 ? 0. : (double)r.num/(double)r.den;
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

// seek位置 0.0~1.0
bool FFDemux::Seek(double pos) {
    if (pos < 0 || pos > 1) {
        XLOGE("seek value must 0.0~1.0");
        return false;
    }

    bool re = false;
    mux.lock();
    if (!ic) {
        mux.unlock();
        return false;
    }
    // 清空读取的缓冲
    avformat_flush(ic);
    long long seekPts = 0;
    seekPts = ic->streams[videoStream]->duration * pos;
    // 向后跳转到关键帧， 按时间顺序，实际是进度条左边
    re = av_seek_frame(ic, videoStream, seekPts, AVSEEK_FLAG_FRAME | AVSEEK_FLAG_BACKWARD);

    mux.unlock();
    return re;
}

// 打开文件，或流媒体  rtmp http rtsp
bool FFDemux::Open(const char *url) {
    XLOGI("Open file %s begin", url);
    Close();
    mux.lock();
    int re = avformat_open_input(&ic, url, 0, 0);
    if (re != 0) {
        mux.unlock();
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf));
        XLOGE("FFDemux open %s failed", url);
        return false;
    }
    XLOGI("FFDemux open succ");

    // 读取文件信息, 非MP4格式需要
    re = avformat_find_stream_info(ic, 0);
    if (re != 0) {
        mux.unlock();
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf));
        XLOGE("avformat_find_stream_info open %s failed", url);
        return false;
    }
    this->totalMs = ic->duration/(AV_TIME_BASE/1000);  // 此值不一定有
    mux.unlock();

    XLOGI("total ms = %d", totalMs);
    GetVPara();
    GetAPara();

    return true;
}

void FFDemux::Close() {
    mux.lock();
    if (ic) {
        avformat_close_input(&ic);
    }
    mux.unlock();
}

// 获取视频参数
XParameter FFDemux::GetVPara() {
    mux.lock();
    if (!ic) {
        mux.unlock();
        XLOGE("GetVPara failed, is is null");
        return XParameter();
    }
    // 获取了视频流的索引
    int re = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, -1, -1, 0, 0);
    if (re < 0) {
        mux.unlock();
        XLOGE("av_find_best_stream failed");
        return XParameter();
    }
    videoStream = re;
    XParameter para;
    para.para = ic->streams[re]->codecpar;
    mux.unlock();
    return para;
}


// 获取音频参数
XParameter FFDemux::GetAPara() {
    mux.lock();
    if (!ic) {
        mux.unlock();
        XLOGE("GetVPara failed, is is null");
        return XParameter();
    }
    // 获取了音频流的索引
    int re = av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, -1, -1, 0, 0);
    if (re < 0) {
        mux.unlock();
        XLOGE("av_find_best_stream failed");
        return XParameter();
    }
    audioStream = re;
    XParameter para;
    para.para = ic->streams[re]->codecpar;
    para.channels = ic->streams[re]->codecpar->channels;
    para.sample_rate = ic->streams[re]->codecpar->sample_rate;
    mux.unlock();
    return para;
}


// 读取一帧数据， 数据由调用者清理
XData FFDemux::Read() {
    mux.lock();
    if (!ic) {
        mux.unlock();
        return XData();
    }
    XData d;
    AVPacket *pkt = av_packet_alloc();
    int re = av_read_frame(ic, pkt);
    if (re != 0) { // 失败
        mux.unlock();
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
        mux.unlock();
        av_packet_free(&pkt);
        return XData();
    }

    // 转换pts, 毫秒
    pkt->pts = pkt->pts * (1000 * r2d(ic->streams[pkt->stream_index]->time_base));
    pkt->dts = pkt->dts * (1000 * r2d(ic->streams[pkt->stream_index]->time_base));
    d.pts = (int) pkt->pts;
//    XLOGI("demux pts %d", d.pts);

    mux.unlock();
    return d;
}

