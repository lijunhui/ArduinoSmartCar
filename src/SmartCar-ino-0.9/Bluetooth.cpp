#include "Bluetooth.h"

CBTRecv::CBTRecv()
 : m_pBTSerial(nullptr)
{

}

CBTRecv::~CBTRecv()
{
    if (m_pBTSerial != nullptr)
    {
        delete m_pBTSerial;
        m_pBTSerial = nullptr;
    }
}

void CBTRecv::setup()
{
    if (m_pBTSerial == nullptr)
    {
        m_pBTSerial = new SoftwareSerial(HC08_RX, HC08_TX);
    }        
    m_pBTSerial->begin(9600);
    m_pBTSerial->print("AT");
}

bool CBTRecv::recvCmd(SmartCar_Command& rCmd)
{
    bool bRet = false;
    if (m_pBTSerial->available() > 0 )
    {
        char BTserialData =  m_pBTSerial->read();
        if (BTserialData == 'U')
        {
           rCmd = Cmd_Forward;
           bRet = true;
        }
        else if (BTserialData == 'D')
        {
            rCmd = Cmd_Backward;
            bRet = true;
        }
        else if (BTserialData == 'L')
        {
            rCmd = Cmd_Left;
            bRet = true;
        }
        else if (BTserialData == 'R')
        {
            rCmd = Cmd_Right;
            bRet = true;
        }
        else if (BTserialData == 'S')
        {
            rCmd = Cmd_Stop;
            bRet = true;  
        }
        else if (BTserialData == 'M')
        {
            rCmd = Cmd_Start_Manual;
            bRet = true;
        }
        else if (BTserialData == 'A')
        {
            rCmd = Cmd_Start_Auto;
            bRet = true;
        }
        else if (BTserialData == 'T')
        {
            rCmd = Cmd_Start_Trace;
            bRet = true;
        }
    }
    return bRet;
}