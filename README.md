# FXPlay
基于FFmpeg实现的简易播放器框架，视频渲染使用OpenGL ES，音频渲染使用OpenSL ES. 支持H.264硬解码和软解码，支持播放暂停，seek操作。用到了观察者模式。


目前播放流媒体声音还有点问题。 

可用VLC Player测试流媒体地址有效性，以下为当前可用的地址： 

rtmp:   
香港财经: rtmp://202.69.69.180:443/webcast/bshdlive-pc   
韩国GoodTV: rtmp://mobliestream.c3tv.com:554/live/goodtv.sdp    
韩国朝鲜日报：rtmp://live.chosun.gscdn.com/live/tvchosun1.stream    
湖南卫视: rtmp://58.200.131.2:1935/livetv/hunantv   


http直播源：   
CCTV1高清：http://ivi.bupt.edu.cn/hls/cctv1hd.m3u8  
CCTV3高清：http://ivi.bupt.edu.cn/hls/cctv3hd.m3u8   
CCTV5高清：http://ivi.bupt.edu.cn/hls/cctv5phd.m3u8   
CCTV6高清：http://ivi.bupt.edu.cn/hls/cctv6hd.m3u8   
