#ifndef BLUETOOTH_RECV_H
#define BLUETOOTH_RECV_H

/*
蓝牙接收者模块
by 李京锐（2019-07-10）
接收来自蓝牙APP的指令
*/

#include "Define.h"
#include <SoftwareSerial.h>

class CBTRecv
{
public:
    CBTRecv();
    virtual~CBTRecv();
    void setup();
    bool recvCmd(SmartCar_Command& rCmd);

private:
   SoftwareSerial *m_pBTSerial;
   
};

#endif //BLUETOOTH_RECV_H
 
