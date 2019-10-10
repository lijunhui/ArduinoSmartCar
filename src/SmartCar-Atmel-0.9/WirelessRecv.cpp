#include "WirelessRecv.h"

CWirelessRecv::CWirelessRecv()
{
    m_bPressures = true;
    m_Rumble = true;
    m_nError = 1;
    m_Type = 0;
    m_Vibrate = 0;
    m_psCmd = 0;
    m_nXSpeed = -1;
    m_nYSpeed = -1;
    
    m_ulTimestamp = ::millis();
}

CWirelessRecv::~CWirelessRecv()
{
}

void CWirelessRecv::setup(int nDelayTime)
{
    //延时一会，确保接收终端已和手柄无线连接上
    if (::millis() - m_ulTimestamp < nDelayTime)
    {
        return;
    }
    //设置四个引脚，及支持震动
    m_nError = m_PS2X.config_gamepad(PS2_DAT, PS2_CMD, PS2_SEL, PS2_CLK, m_bPressures, m_Rumble);
    if(m_nError == 0)
    {
        Serial.println("configured ps2 successful!");
        m_Type = m_PS2X.readType();
        switch(m_Type)
        {
            case 0:
            Serial.println("Unknown ps2 type");
            break;
            case 1:
            Serial.println("DualShock ps2 Found");
            break;
            case 2:
            Serial.println("GuitarHero ps2 Found");
            break;
        }
    }
    else if(m_nError == 1)
    {
        Serial.println("No found ps2!");
    }
    else if(m_nError == 2) 
    {
        Serial.println("Ps2 found but not accepting commands.");
    }
    else if(m_nError == 3)
    {
        Serial.println("Ps2 refusing to enter Pressures mode, may not support it. ");
    }
    
    m_ulTimestamp = ::millis();
}

bool CWirelessRecv::recvCmd(SmartCar_Command& rCmd)
{
    bool bRet = false;
    if(m_nError == 1) 
    {
        setup(1000);
    }

    if(m_Type == 1)
    { //DualShock 模式

        m_psCmd = 0;
        m_nXSpeed = -1;
        m_nYSpeed = -1;
    
        //读取控制器并设置电机以“振动”速度旋转
        m_PS2X.read_gamepad(false, m_Vibrate);
        
        //左右摇杆
        leftRocker();
        rightRocker();
        
        //左右方向按键
        leftDirectionButton();
        rightFunctionButton();
        
        //左右前方按键
        leftFrontButton();
        rightFrontButton();
        
        //中间功能按键
        middleButton();
        
        m_Vibrate = m_PS2X.Analog(PSAB_BLUE);

        if (m_psCmd != 0)
        { //收到指令，进行命令转换
            protocolTransform(rCmd);
            bRet = true;
        }
        delay(10);
    }

    return bRet;
}

void CWirelessRecv::protocolTransform(SmartCar_Command& rCmd)
{
    switch (m_psCmd)
    {
    case PSB_PINK:     //方形--粉色 --> 手动模式
        rCmd = Cmd_Start_Manual;
        break;
    case PSB_RED:      //圆形--红色 --> 自动模式
        rCmd = Cmd_Start_Auto;
        break;
    case PSB_GREEN:    //三角 绿色  --> 巡线模式
        rCmd = Cmd_Start_Trace;
        break;
    case PSB_PAD_UP:   //UP        --> 前进
        rCmd = Cmd_Forward;
        break;
    case PSB_PAD_DOWN: //DOWN      --> 后退
        rCmd = Cmd_Backward;
        break;
    case PSB_PAD_LEFT: //LEFT      --> 左转
        rCmd = Cmd_Left;
        break;
    case PSB_PAD_RIGHT://RIGHT     --> 右转
        rCmd = Cmd_Right;
        break;
    case PSB_SELECT:   //SELECT    --> 停止
        rCmd = Cmd_Stop;
        break;
    }
}

//左前两个按键
void CWirelessRecv::leftFrontButton()
{
    //左前L1
    if(m_PS2X.Button(PSB_L1) )
    {
        Serial.print("L1 Stick Values:");
        Serial.print(m_PS2X.Analog(PSS_LY), DEC);
        Serial.print(",");
        Serial.println(m_PS2X.Analog(PSS_LX), DEC);
        m_psCmd = PSB_L1;
    }
    
    //左前L2
    if (m_PS2X.NewButtonState() && m_PS2X.Button(PSB_L2))
    {
        Serial.println("L2 pressed");
        m_psCmd = PSB_L2;
    }
}

//左边方向按键
void CWirelessRecv::leftDirectionButton()
{
    //上
    if(m_PS2X.Button(PSB_PAD_UP))
    {
        Serial.print("Up held this hard: ");
        Serial.println(m_PS2X.Analog(PSAB_PAD_UP), DEC);
        m_psCmd = PSB_PAD_UP;
    }
    //下
    if(m_PS2X.Button(PSB_PAD_DOWN))
    {
        Serial.print("Down held this hard: ");
        Serial.println(m_PS2X.Analog(PSAB_PAD_DOWN), DEC);
        m_psCmd = PSB_PAD_DOWN;
    }
    
    //左
    if(m_PS2X.Button(PSB_PAD_LEFT))
    {
        Serial.print("Left held this hard: ");
        Serial.println(m_PS2X.Analog(PSAB_PAD_LEFT), DEC);
        m_psCmd = PSB_PAD_LEFT;
    }
    //右
    if(m_PS2X.Button(PSB_PAD_RIGHT))
    {
        Serial.print("Right held this hard: ");
        Serial.println(m_PS2X.Analog(PSAB_PAD_RIGHT), DEC);
        m_psCmd = PSB_PAD_RIGHT;
    }
}

//左边摇杆
void CWirelessRecv::leftRocker()
{
    //二维坐标系
    //水平方向 X
    int LX = m_PS2X.Analog(PSS_LX);
    if (LX<128)
    { //左转
        m_nXSpeed = 2*(128-LX);
        Serial.print("turnLeft: ");
        Serial.println(m_nXSpeed);
    }

    if (LX>128)
    { //右转
        m_nXSpeed = 2*(LX-128);
        Serial.print("turnRight: ");
        Serial.println(m_nXSpeed);
    }
        
    //垂直方向 Y
    int LY = m_PS2X.Analog(PSS_LY);
    if (LY<128)
    { //前进
        m_nYSpeed = 2*(128-LY);
        Serial.print("forward: ");
        Serial.println(m_nYSpeed);
    }

    if (LY>128)
    { //后退
        m_nYSpeed = 2*(LY-128);
        Serial.print("back: ");
        Serial.println(m_nYSpeed);
    }

    if (LY==128 && LX==128)
    { //摇杆居中
        ;
    }
}

//中间两个按钮
void CWirelessRecv::middleButton()
{
    if(m_PS2X.Button(PSB_START))
    {
        Serial.println("Start is being held");
        m_psCmd = PSB_START;
    }
    
    if(m_PS2X.Button(PSB_SELECT))
    {
        Serial.println("Select is being held");
        m_psCmd = PSB_SELECT;
    }
    
    //注意: MODE键，是PS2手柄的模式键，它在PS2X_lib库中已经设定
    //      所以一旦手柄连接上接收终端，模式会被自动设置好（模式 m_Type=1），模式灯会变量
}

//右前两个按键
void CWirelessRecv::rightFrontButton()
{
    //右前R1
    if(m_PS2X.Button(PSB_R1))
    {
        Serial.print("R1 Stick Values:");
        Serial.print(m_PS2X.Analog(PSS_RY), DEC);
        Serial.print(",");
        Serial.println(m_PS2X.Analog(PSS_RX), DEC);
        m_psCmd = PSB_R1;
    }
    
    //右前R2  
    if (m_PS2X.NewButtonState() && m_PS2X.Button(PSB_R2))
    {
        Serial.println("R2 pressed");
        m_psCmd = PSB_R2;
    }
}

//右边功能按键(四个图形)
void CWirelessRecv::rightFunctionButton()
{
    //绿色--三角形
    if(m_PS2X.ButtonPressed(PSB_GREEN))
    {
        Serial.println("Triangle just pressed");
        m_psCmd = PSB_GREEN;
    }
    //红色--圆形
    if(m_PS2X.ButtonPressed(PSB_RED))
    {
        Serial.println("Circle just pressed");
        m_psCmd = PSB_RED;
    }
    //粉色--正方形
    if(m_PS2X.ButtonReleased(PSB_PINK))
    {
        Serial.println("Square just released");
        m_psCmd = PSB_PINK;
    }
    //蓝色--X键
    if(m_PS2X.NewButtonState(PSB_BLUE))
    {
        Serial.println("X just changed");
        m_psCmd = PSB_BLUE;
    }
}

//右边摇杆
void CWirelessRecv::rightRocker()
{
    //二维坐标系
    //水平方向 X
    int RX = m_PS2X.Analog(PSS_RX);
    if (RX<128)
    { //左转
        m_nXSpeed = 2*(128-RX);
        Serial.print("turnLeft: ");
        Serial.println(m_nXSpeed);
    }

    if (RX>128)
    { //右转
        m_nXSpeed = 2*(RX-128);
        Serial.print("turnRight: ");
        Serial.println(m_nXSpeed);
    }
        
    //垂直方向 Y
    int RY = m_PS2X.Analog(PSS_RY);
    if (RY<128)
    { //前进
        m_nYSpeed = 2*(128-RY);
        Serial.print("forward: ");
        Serial.println(m_nYSpeed);
    }

    if (RY>128)
    { //后退
        m_nYSpeed = 2*(RY-128);
        Serial.print("back: ");
        Serial.println(m_nYSpeed);
    }

    if (RY==128 && RX==128)
    { //摇杆居中
        ;
    }
}
