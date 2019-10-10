#include "SmartCar.h"

CSmartCar::CSmartCar()
{
    m_Cmd = Cmd_Stop;
    m_DriveMode = DM_Manual;
}

CSmartCar::~CSmartCar()
{
}

void CSmartCar::setup()
{   
    Serial.begin(9600);
    delay(100);
    Serial.println("");
    Serial.println("SmartCar --> setup...");
    
    //驱动者
    m_Driver.setup();
    //探测者
    m_Detector.setup();
    //巡线者
    m_Tracer.setup();
    //接收红外指令者
    m_IRRecv.setup();
    //接收无线指令者
    m_wirelessRecv.setup(1000);
    //接收蓝牙指令者
    m_btRecv.setup();

    Serial.println("SmartCar --> has run...");
}

void CSmartCar::run()
{
    if (m_IRRecv.recvCmd(m_Cmd) || m_btRecv.recvCmd(m_Cmd))
    {
        if (m_Cmd == Cmd_Start_Manual)
        {  //启动手动模式
             m_DriveMode = DM_Manual;
             m_Driver.slowStop();
        }
        else if (m_Cmd == Cmd_Start_Auto)
        {  //启动自动模式
            m_DriveMode = DM_Auto;
            m_Driver.startForward();
        }
        else if (m_Cmd == Cmd_Start_Trace)
        {  //启动巡线
            m_DriveMode = DM_Tracing;
            m_Driver.startForward();
        }
        else
        {  //手动模式指令
            manualCmd();
        }
    }

    if (m_DriveMode == DM_Auto)
    {
        autoDriving();
    }
    
    if (m_DriveMode == DM_Tracing)
    {
        autoTracing();
    }
}

void CSmartCar::manualCmd()
{
    switch (m_Cmd)
    {
        case Cmd_Stop:
        {
            m_DriveMode = DM_Manual;
            m_Driver.slowStop();
        }
        break;
        case Cmd_Left:
        {
            m_DriveMode = DM_Manual;
            m_Driver.tureLeftDelay(500);
        }
        break;
        case Cmd_Right:
        {
            m_DriveMode = DM_Manual;
            m_Driver.tureRightDelay(500);
        }
        break;
        case Cmd_Forward:
        {
            m_DriveMode = DM_Manual;
            m_Driver.forwardDelay(500);
        }
        break;
        case Cmd_Backward:
        {
            m_DriveMode = DM_Manual;
            m_Driver.backwardDelay(500);
        }
        break;
        case Cmd_Hold:
        {
            if (m_DriveMode != DM_Tracing && m_DriveMode != DM_Auto)
            {
                m_Driver.setHold();
            }
        }
        break;
    }

    //检测是否到停止时间
    m_Driver.checkDelayStop();
}

//自动驾驶模式
void CSmartCar::autoDriving()
{
    //检测障碍物
    if (m_Detector.haveObstacle())
    {
        m_Driver.slowStop();
        m_Driver.slowBackward();
        int nRet = m_Detector.lookAround();
        Serial.print("lookAround = ");
        Serial.println(nRet);
        switch (nRet)
        {
            case 1:
                m_Driver.slowRight(500);
                break;
            case 2:
                m_Driver.slowRight(250);
                break;
            case 3:
            case 0:
                m_Driver.forwardDelay(100);
                break;
            case 4:
                m_Driver.slowLeft(250);
                break;
            case 5:
                m_Driver.slowLeft(500);
                break;
            default:
                break;
        }
    }
    delay(50);
    m_Driver.startForward();
}

//自动巡线模式
void CSmartCar::autoTracing()
{
    for(int i = 0; i < 20; i++)
    {
        m_Tracer.doTracing(m_Cmd);
        if (m_Cmd == Cmd_Left)
        {
            m_Driver.turnLeft(100);
            //m_Driver.stop();
        }
        else if (m_Cmd == Cmd_Right)
        {
            m_Driver.turnRight(100);
            //m_Driver.stop();
        }
        else
        {
            m_Driver.startForward();
            break;
        }
    }
    delay(20);    
}