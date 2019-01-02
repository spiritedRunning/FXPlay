
#include "XThread.h"
#include "XLog.h"
#include <thread>
using namespace std;

void XSleep(int mis) {
    chrono::milliseconds du(mis);
    this_thread::sleep_for(du);
}

void XThread::ThreadMain() {
    isRunning = true;
    XLOGI("ThreadMain begin");
    Main();
    XLOGI("ThreadMain end");
    isRunning = false;
}

void XThread::SetPause(bool isP) {
    isPause = isP;
    for (int i = 0; i < 10; i++) {
        if (isPausing == isPause) {
            break;
        }
        XSleep(10);
    }
}



bool XThread::Start() {
    isExit = false;
    isPause = false;
    thread th(&XThread::ThreadMain, this);
    th.detach();
    return true;
}

void XThread::Stop() {
    isExit = true;
    for (int i = 0; i < 200; i++) {
        if (!isRunning) {
            XLOGI("Stop thread success!");
            return;
        }
        XSleep(1);
    }
    XLOGI("Stop thread timeout!");

}