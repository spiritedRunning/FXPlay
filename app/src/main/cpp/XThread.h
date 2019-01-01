
#ifndef FXPLAY_XTHREAD_H
#define FXPLAY_XTHREAD_H

void XSleep(int mis);

// c++ 11 线程库
class XThread {
public:
    virtual bool Start();
    // 通过控制isExit安全停止线程（不一定线程）
    virtual void Stop();

    virtual void Main() {}

protected:
    bool isExit = false;
    bool isRunning = false;

private:
    void ThreadMain();
};


#endif //FXPLAY_XTHREAD_H
