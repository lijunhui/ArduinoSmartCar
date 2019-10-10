#include "Driver.h"

CDriver::CDriver()
{
    m_ulLastTimestamp = 0;
    m_ulNeedMoveMS = 0;
    m_LeftDefaultSpeed = 80;
    m_RightDefaultSpeed = 80;
    m_DriverStatus = Driver_Stop;
}

CDriver::~CDriver()
{
}

void CDriver::setup()
{
    //设置电机驱动引脚,为输出信号
    pinMode(DRIVER_LEFT0_PIN, OUTPUT);
    pinMode(DRIVER_LEFT1_PIN, OUTPUT);
    pinMode(DRIVER_LEFTSPEED_PIN, OUTPUT);

    pinMode(DRIVER_RIGHT0_PIN, OUTPUT);
    pinMode(DRIVER_RIGHT1_PIN, OUTPUT);
    pinMode(DRIVER_RIGHTSPEED_PIN, OUTPUT);

    //设置速度
    setSpeed(DRIVER_LEFTSPEED_PIN, m_LeftDefaultSpeed);
    setSpeed(DRIVER_RIGHTSPEED_PIN, m_RightDefaultSpeed);
}

//////////////////////////////////////////////////////////////////////////

void CDriver::startForward()
{
    forward(0);
}

void CDriver::tureLeftDelay(unsigned long ulDelayMS)
{
    m_ulNeedMoveMS = ulDelayMS;
    m_ulLastTimestamp = ::millis();
    turnLeft(0);
}

void CDriver::tureRightDelay(unsigned long ulDelayMS)
{
    m_ulNeedMoveMS = ulDelayMS;
    m_ulLastTimestamp = ::millis();
    turnRight(0);
}

void CDriver::forwardDelay(unsigned long ulDelayMS)
{
    m_ulNeedMoveMS = ulDelayMS;
    m_ulLastTimestamp = ::millis();
    forward(0);
}

void CDriver::backwardDelay(unsigned long ulDelayMS)
{
    m_ulNeedMoveMS = ulDelayMS;
    m_ulLastTimestamp = ::millis();
    backward(0);
}

//////////////////////////////////////////////////////////////////////////

void CDriver::setHold()
{
    m_ulLastTimestamp = ::millis(); 
}

void CDriver::checkDelayStop()
{
    if (m_DriverStatus != Driver_Stop && m_ulLastTimestamp != 0)
    {
        if ((::millis() - m_ulLastTimestamp) > m_ulNeedMoveMS)
        {
            slowStop();
            m_ulNeedMoveMS = 0;
            m_ulLastTimestamp = 0;
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void CDriver::slowStop()
{
    if (m_DriverStatus == Driver_TurnLeft || m_DriverStatus == Driver_TurnRight)  
    {
        for (unsigned short theSpeed = m_RightDefaultSpeed; theSpeed > 0; theSpeed -= 10)
        {
            setSpeed(m_DriverStatus == Driver_TurnLeft ? DRIVER_LEFTSPEED_PIN : DRIVER_RIGHTSPEED_PIN, theSpeed);       
            delay(30);
        }
    }
    else if (m_DriverStatus == Driver_DoubleMoving)
    {
        for (unsigned short theSpeed = m_RightDefaultSpeed; theSpeed > 0; theSpeed -= 10)
        {
            setSpeed(DRIVER_LEFTSPEED_PIN, theSpeed);
            setSpeed(DRIVER_RIGHTSPEED_PIN, theSpeed);  
            delay(30);
        }
    }
    nowStop();
}

void CDriver::nowStop()
{
    m_DriverStatus = Driver_Stop;
    
    digitalWrite(DRIVER_LEFT0_PIN, LOW);
    digitalWrite(DRIVER_LEFT1_PIN, LOW);
    digitalWrite(DRIVER_RIGHT0_PIN, LOW);
    digitalWrite(DRIVER_RIGHT1_PIN, LOW);
    
    setSpeed(DRIVER_LEFTSPEED_PIN, m_LeftDefaultSpeed);
    setSpeed(DRIVER_RIGHTSPEED_PIN, m_RightDefaultSpeed);
    Serial.println("---nowStop---");
}

//////////////////////////////////////////////////////////////////////////

void CDriver::turnLeft(unsigned long ulDelayMS)
{
    m_DriverStatus = Driver_TurnLeft;
    digitalWrite(DRIVER_LEFT0_PIN, LOW);
    digitalWrite(DRIVER_LEFT1_PIN, LOW);
    digitalWrite(DRIVER_RIGHT0_PIN, LOW);
    digitalWrite(DRIVER_RIGHT1_PIN, HIGH);
    Serial.println("---turnLeft---");
    delay(ulDelayMS);
}

void CDriver::slowLeft(unsigned long ulDelayMS)
{
    turnLeft(0);
    int delaytime = ulDelayMS / (m_LeftDefaultSpeed/10);
    for (unsigned short theSpeed = m_LeftDefaultSpeed; theSpeed > 0; theSpeed -= 10)
    {
        setSpeed(DRIVER_LEFTSPEED_PIN, theSpeed);
        delay(delaytime);
    }
    nowStop();
}

//////////////////////////////////////////////////////////////////////////

void CDriver::turnRight(unsigned long ulDelayMS)
{
    m_DriverStatus = Driver_TurnRight;
    digitalWrite(DRIVER_LEFT0_PIN, LOW);
    digitalWrite(DRIVER_LEFT1_PIN, HIGH);
    digitalWrite(DRIVER_RIGHT0_PIN, LOW);
    digitalWrite(DRIVER_RIGHT1_PIN, LOW);
    Serial.println("---turnRight---");
    delay(ulDelayMS);
}

void CDriver::slowRight(unsigned long ulDelayMS)
{
    turnRight(0);
    int delaytime = ulDelayMS / (m_RightDefaultSpeed/10);
    for (unsigned short theSpeed = m_RightDefaultSpeed; theSpeed > 0; theSpeed -= 10)
    {
        setSpeed(DRIVER_RIGHTSPEED_PIN, theSpeed);
        delay(delaytime);
    }
    nowStop();
}

//////////////////////////////////////////////////////////////////////////

void CDriver::slowForward(unsigned long ulDelayMS)
{
    forward(0);
    int delaytime = ulDelayMS / (m_RightDefaultSpeed/10);
    for (unsigned short theSpeed = m_RightDefaultSpeed; theSpeed > 0; theSpeed -= 10)
    {
        setSpeed(DRIVER_LEFTSPEED_PIN, theSpeed);
        setSpeed(DRIVER_RIGHTSPEED_PIN, theSpeed);
        delay(delaytime);
    }
    nowStop();
}

void CDriver::forward(unsigned long ulDelayMS)
{
    m_DriverStatus = Driver_DoubleMoving;
    digitalWrite(DRIVER_LEFT0_PIN, LOW);
    digitalWrite(DRIVER_LEFT1_PIN, HIGH);
    digitalWrite(DRIVER_RIGHT0_PIN, LOW);
    digitalWrite(DRIVER_RIGHT1_PIN, HIGH);
    
    setSpeed(DRIVER_LEFTSPEED_PIN, m_LeftDefaultSpeed);
    setSpeed(DRIVER_RIGHTSPEED_PIN, m_RightDefaultSpeed);
    Serial.println("---forward---");
    delay(ulDelayMS);
}

//////////////////////////////////////////////////////////////////////////

void CDriver::backward(unsigned long ulDelayMS)
{
    m_DriverStatus = Driver_DoubleMoving;
    digitalWrite(DRIVER_LEFT0_PIN, HIGH);
    digitalWrite(DRIVER_LEFT1_PIN, LOW);
    digitalWrite(DRIVER_RIGHT0_PIN, HIGH);
    digitalWrite(DRIVER_RIGHT1_PIN, LOW);
    Serial.println("---backward---");
    delay(ulDelayMS);
}

void CDriver::slowBackward(unsigned long ulDelayMS /* = 600 */)
{
    backward(0);
    int delaytime = ulDelayMS / (m_RightDefaultSpeed/10);
    for (unsigned short theSpeed = m_RightDefaultSpeed; theSpeed > 0; theSpeed -= 10)
    {
        setSpeed(DRIVER_LEFTSPEED_PIN, theSpeed);
        setSpeed(DRIVER_RIGHTSPEED_PIN, theSpeed);
        delay(delaytime);
    }
    nowStop();
}

//////////////////////////////////////////////////////////////////////////

void CDriver::setSpeed(int pinEnable, unsigned short pwmVal)
{
    analogWrite(pinEnable, pwmVal);
}
