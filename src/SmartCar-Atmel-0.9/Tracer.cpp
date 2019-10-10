#include "Tracer.h"

CTracer::CTracer()
{
}

CTracer::~CTracer()
{
}

void CTracer::setup()
{
    pinMode(TRACER_N1_PIN, INPUT);
    pinMode(TRACER_N2_PIN, INPUT);
}

void CTracer::doTracing(SmartCar_Command& rCmd)
{
    //巡线处理
    int nTrac1 = digitalRead(TRACER_N1_PIN);
    int nTrac2 = digitalRead(TRACER_N2_PIN);
    if (nTrac1 == 1 && nTrac2 == 0)
    {
        rCmd = Cmd_Left;
        Serial.println("doTracing---TurnLeft");
    }
    else if (nTrac1 == 0 && nTrac2 == 1)
    {
        rCmd = Cmd_Right;
        Serial.println("doTracing---TurnRight");
    }
    else
    {
        rCmd = Cmd_Forward;
        Serial.println("doTracing---Forward");
    }
    return;
}
