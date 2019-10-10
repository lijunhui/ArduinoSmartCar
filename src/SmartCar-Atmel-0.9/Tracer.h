#ifndef TRACER_H
#define TRACER_H

#include "Define.h"

class CTracer
{
public:
    CTracer();
    virtual~CTracer();
    void setup();
    void doTracing(SmartCar_Command& rCmd);
};

#endif //TRACER_H
