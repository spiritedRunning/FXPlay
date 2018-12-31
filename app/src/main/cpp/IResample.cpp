
#include "IResample.h"
#include "XLog.h"

void IResample::Update(XData data) {
    XData d = this->Resample(data);
//    XLOGD("Resample data: %d", d.size);
    if (d.size > 0) {
        this->Notify(d);
    }
}