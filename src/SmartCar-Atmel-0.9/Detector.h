#ifndef DETECTOR_H
#define DETECTOR_H

/*
探测者模块
by 李京锐（2019-07-10）
当切换到自动模式下时，它将启动，探测前方障碍物，
它由超声波传感器和伺服舵机组合而成，协同工作。
*/

#include "Define.h"
#include <Servo.h>

class CDetector
{
public:
    CDetector();
    virtual ~CDetector();

    void setup();
    int lookAround();
    bool haveObstacle();
    
private:
    int checkDistance();
    void lookRightMax();
    void lookRightMin();
    void lookForward();
    void lookLeftMin();
    void lookLeftMax();

private:
    Servo m_servo;  //伺服舵机对象
    unsigned long  m_ulCheckTimestamp;
    byte  m_nCheckMinDistance;
    byte  m_nLookDelayTime;
};

#endif //DETECTOR_H

