#ifndef SMART_CAR_H
#define SMART_CAR_H

#include "Define.h"       //引脚定义文件
#include "IRRemoteRecv.h" //红外遥控接收者----处理红外遥控器发过来的指令
#include "WirelessRecv.h" //无线接收者----处理PS2X无线手柄发送过来的指令
#include "Bluetooth.h"    //蓝牙接收者----处理通过手机APP发送过来的指令
#include "Detector.h"     //探测者----超声波和伺服舵机组合而成
#include "Tracer.h"       //巡线者----由两个TCRT5000红外反射光电开关组成
#include "Driver.h"       //驱动者----由L298N电机驱动模块和两个直流电机组成

class CSmartCar
{
public:
    CSmartCar();
    virtual~CSmartCar();
    void setup();
    void run();

private:
    void manualCmd();
    void autoDriving();
    void autoTracing();

private:
    CIRRemoteRecv m_IRRecv;
    CWirelessRecv m_wirelessRecv;
    CBTRecv       m_btRecv;
    CTracer       m_Tracer;
    CDetector     m_Detector;
    CDriver       m_Driver;
    
    SmartCar_Command m_Cmd;
    DriveMode     m_DriveMode;
};

#endif //SMART_CAR_H
