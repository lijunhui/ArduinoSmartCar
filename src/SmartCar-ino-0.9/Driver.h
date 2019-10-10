#ifndef DRIVER_H
#define DRIVER_H

/*
驱动者模块
by 李京锐（2019-07-10）
它负责控制小车的两个轮子到底是：前进，后退，左转，右转，停止，及行驶的速度。
*/

#include "Define.h"

typedef enum tag_Driver_Status
{
    Driver_Stop = 0,
    Driver_TurnLeft = 1,
    Driver_TurnRight = 2,
    Driver_DoubleMoving = 3,
    
} Driver_Status;

class CDriver
{
public:
    CDriver();
    virtual ~CDriver();

    void setup();

    void startForward();
    void tureLeftDelay(unsigned long ulDelayMS);
    void tureRightDelay(unsigned long ulDelayMS);
    void forwardDelay(unsigned long ulDelayMS);
    void backwardDelay(unsigned long ulDelayMS);
    void setHold();

    void slowLeft(unsigned long ulDelayMS);
    void slowRight(unsigned long ulDelayMS);
    void slowForward(unsigned long ulDelayMS);
    void slowBackward(unsigned long ulDelayMS = 600);

    void turnLeft(unsigned long ulDelayMS);
    void turnRight(unsigned long ulDelayMS);
    void forward(unsigned long ulDelayMS);
    void backward(unsigned long ulDelayMS);
    
    void checkDelayStop();
    void slowStop();
    
private:
    void nowStop();
    void setSpeed(int pinEnable, unsigned short pwmVal);

private:
    byte m_LeftDefaultSpeed;
    byte m_RightDefaultSpeed;
    unsigned long  m_ulLastTimestamp;  //最后一次启动运动时间戳
    unsigned long  m_ulNeedMoveMS;     //需要运行多长时间
    Driver_Status  m_DriverStatus;     //驱动者状态 
};

#endif //DRIVER_H
