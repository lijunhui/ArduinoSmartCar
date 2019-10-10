#ifndef IR_REMOTE_RECV_H
#define IR_REMOTE_RECV_H

/*
红外接收者模块
by 李京锐（2019-07-10）
接收来自红外遥控板的指令
*/

#include "Define.h"
#include <IRremote.h>

class CIRRemoteRecv
{
public:
    CIRRemoteRecv();
    virtual~CIRRemoteRecv();
    void setup();
    bool recvCmd(SmartCar_Command& rCmd);

private:
    bool parserCmd(decode_results& rResults, SmartCar_Command& rCmd);

private:
    IRrecv* m_pRrecv;
};

#endif //IR_REMOTE_RECV_H
