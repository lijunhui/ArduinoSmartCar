#include "IRRemoteRecv.h"

CIRRemoteRecv::CIRRemoteRecv()
{
    m_pRrecv = nullptr;
}

CIRRemoteRecv::~CIRRemoteRecv()
{
    if (m_pRrecv != nullptr)
    {
        delete m_pRrecv;
        m_pRrecv = nullptr;
    }
}

void CIRRemoteRecv::setup()
{
    //设置红外遥控引脚,输入信号
    pinMode(IRREMOTE_PIN, INPUT);

    m_pRrecv = new IRrecv(IRREMOTE_PIN);

    m_pRrecv->enableIRIn();
}

bool CIRRemoteRecv::recvCmd(SmartCar_Command& rCmd)
{
    bool bRet = false;
    decode_results results;
    if (m_pRrecv->decode(&results))
    {
        Serial.print("begin decode --> ");
        Serial.print(results.value, HEX);
        Serial.print(" ");
        m_pRrecv->resume(); // Receive the next value
        bRet = parserCmd(results, rCmd);
        Serial.println(" <-- end decode");
    }
    return bRet;
}

bool CIRRemoteRecv::parserCmd(decode_results& rResults, SmartCar_Command& rCmd)
{
    bool bFlag = false;
    if (rResults.value == 0xFF10EF || rResults.value == 0x8C22657B)
    { //向左 左转
        rCmd = Cmd_Left;
        bFlag = true;
        Serial.println("Cmd_Left");
    }
    else if (rResults.value == 0xFF5AA5 || rResults.value == 0x449E79F)
    { //向右 右转
        rCmd = Cmd_Right;
        bFlag = true;
        Serial.println("Cmd_Right");
    }
    else if (rResults.value == 0xFF18E7 || rResults.value == 0x3D9AE3F7)
    { //向上 前进
        rCmd = Cmd_Forward;
        bFlag = true;
        Serial.println("Cmd_Forward");
    }
    else if (rResults.value == 0xFF4AB5 || rResults.value == 0x1BC0157B)
    { //向下 后退
        rCmd = Cmd_Backward;
        bFlag = true;
        Serial.println("Cmd_Backward");
    }
    else if (rResults.value == 0xFF38C7) 
    { //OK, 停止指令
        rCmd = Cmd_Stop;
        bFlag = true;
        Serial.println("Cmd_Stop");
    }
    else if (rResults.value == 0x488F3CBB) 
    { //可能是唤醒指令
        rCmd = Cmd_Stop;
        bFlag = true;
        Serial.println("Cmd_Stop");
    }
    else if (rResults.value == 0xFFFFFFFF) 
    { //保持
        rCmd = Cmd_Hold;
        bFlag = true;
        Serial.println("Cmd_Hold");
    }
    else if (rResults.value == 0xF0C41643) 
    { //# 启动巡线模式
        rCmd = Cmd_Start_Trace;
        bFlag = true;
        Serial.println("Cmd_Start_Trace");
    }
    else if (rResults.value == 0xC101E57B) 
    { //* 启动自动驾驶模式
        rCmd = Cmd_Start_Auto;
        bFlag = true;
        Serial.println("Cmd_Start_Auto");
    }
    else if (rResults.value == 0xBBBBBBBB) 
    { //* 切换模式--手动
        rCmd = Cmd_Start_Manual;
        bFlag = true;
        Serial.println("Cmd_Start_Manual");
    }
    else
    {
        Serial.println("Undefined command");
    }
    return bFlag;
}
