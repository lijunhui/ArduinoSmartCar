#include "Detector.h"

CDetector::CDetector()
{
    m_ulCheckTimestamp = ::millis();
    m_nCheckMinDistance = 20;  //最小距离20cm
    m_nLookDelayTime = 200;    //超声波旋转后停留200ms，确保角度转到位了再继续执行
}

CDetector::~CDetector()
{
}

void CDetector::setup()
{
    //超声波引脚
    pinMode(DETECTOR_TRIG_PIN, OUTPUT);
    pinMode(DETECTOR_ECHO_PIN, INPUT);

    //初始化舵机到100度位置
    m_servo.attach(DETECTOR_SERVO_PIN);
    m_servo.write(100);
}

bool CDetector::haveObstacle()
{
    bool bRet = false;
    if (::millis() - m_ulCheckTimestamp > 200)
    {
        int nDistance = 100000;
        nDistance = checkDistance();
        m_ulCheckTimestamp = ::millis();
        if (nDistance < m_nCheckMinDistance)
        {
            bRet = true;
        }
    }
    return bRet;
}

//观察周围情况，并返回方向代码
int CDetector::lookAround()
{
    lookRightMax();
    int D1 = checkDistance();

    lookRightMin();
    int D2 = checkDistance();

    lookLeftMin();
    int D4 = checkDistance();

    lookLeftMax();
    int D5 = checkDistance();

    lookForward();
    int D3 = checkDistance();

    if (D1 > D2 && D1 > D3 && D1 > D4 && D1 > D5 && D1 > m_nCheckMinDistance)
        return 1;
    if (D2 > D1 && D2 > D3 && D2 > D4 && D2 > D5 && D2 > m_nCheckMinDistance)
        return 2;
    if (D3 > D2 && D3 > D1 && D3 > D4 && D3 > D5 && D3 > m_nCheckMinDistance)
        return 3;
    if (D4 > D2 && D4 > D3 && D4 > D1 && D4 > D5 && D4 > m_nCheckMinDistance)
        return 4;
    if (D5 > D2 && D5 > D3 && D5 > D4 && D5 > D1 && D5 > m_nCheckMinDistance)
        return 5;

    return 0;
}

//向右看80度
void CDetector::lookRightMax() 
{
    m_servo.write(20);
    delay(m_nLookDelayTime);
}

//向右看45度
void CDetector::lookRightMin() 
{
    m_servo.write(55);
    delay(m_nLookDelayTime);
}

//向前看, 前面默认在100度
void CDetector::lookForward() 
{
    m_servo.write(100);
    delay(m_nLookDelayTime);
}

//向左看45度
void CDetector::lookLeftMin() 
{
    m_servo.write(145);
    delay(m_nLookDelayTime);
}

//向左看80度
void CDetector::lookLeftMax() 
{
    m_servo.write(180);
    delay(m_nLookDelayTime);
}

//HC-SR04 超声波传感器
int CDetector::checkDistance() 
{
    int nDistance = 100000;
    digitalWrite(DETECTOR_TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(DETECTOR_TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(DETECTOR_TRIG_PIN, LOW);
    nDistance = pulseIn(DETECTOR_ECHO_PIN, HIGH) / 58.00;
    delay(10);
    Serial.print("distance=");
    Serial.println(nDistance);
    return nDistance;
}