#ifndef WIRELESS_RECV_H
#define WIRELESS_RECV_H

#include "Define.h"
#include <PS2X_lib.h>

class CWirelessRecv
{
public:
    CWirelessRecv();
    virtual~CWirelessRecv();
    void setup(int nDelayTime);
    bool recvCmd(SmartCar_Command& rCmd);
    
private:
    void protocolTransform(SmartCar_Command& rCmd);

    void leftFrontButton();
    void leftDirectionButton();
    void leftRocker();
    
    void middleButton();
    
    void rightFrontButton();
    void rightFunctionButton();
    void rightRocker();

private:
    PS2X m_PS2X;
    byte m_nError; 
    byte m_Type;
    byte m_Vibrate;
    byte m_nXSpeed;
    byte m_nYSpeed;
    bool m_bPressures;
    bool m_Rumble;
    int  m_psCmd;
    unsigned long  m_ulTimestamp;
};

#endif //WIRELESS_RECV_H
