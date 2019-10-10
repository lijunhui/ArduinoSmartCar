#ifndef SMARTCAR_DEFINE_H
#define SMARTCAR_DEFINE_H

#include <Arduino.h>

/*
//  引脚使用定义
*/

//驱动者：(L298N) 左轮
#define DRIVER_LEFT0_PIN  7      //方向控制引脚1,数字输出
#define DRIVER_LEFT1_PIN  8      //方向控制引脚2,数字输出
#define DRIVER_LEFTSPEED_PIN  5  //使能引脚,模拟输出,可以控制电机速度,注必须是PWM引脚~

//驱动者：(L298N) 右轮
#define DRIVER_RIGHT0_PIN  12     //方向控制引脚1,数字输出
#define DRIVER_RIGHT1_PIN  13     //方向控制引脚2,数字输出
#define DRIVER_RIGHTSPEED_PIN  6  //使能引脚,模拟输出,可以控制电机速度,注必须是PWM引脚~


//////////////////////////////////////////////////////////////////////////

//探测者：伺服舵机信号引脚
#define DETECTOR_SERVO_PIN  3    //必须是PWM引脚~

//探测者：超声波引脚
#define DETECTOR_TRIG_PIN  4     //发送信号引脚,数字输出
#define DETECTOR_ECHO_PIN  2     //返回信号引脚,数字输入

//////////////////////////////////////////////////////////////////////////

//巡线者：巡线传感器
#define TRACER_N1_PIN  11        //左侧巡线传感器
#define TRACER_N2_PIN  12        //右侧巡线传感器

//////////////////////////////////////////////////////////////////////////

//红外接收者：红外接收器
#define IRREMOTE_PIN   A5        //模拟输入引脚

//////////////////////////////////////////////////////////////////////////

//无线接收者：PS2X手柄接收端
#define PS2_DAT        13
#define PS2_CMD        11
#define PS2_SEL        10
#define PS2_CLK        12

//////////////////////////////////////////////////////////////////////////

//蓝牙接收者：HC-08蓝牙从模块
#define HC08_RX      11
#define HC08_TX      10

//////////////////////////////////////////////////////////////////////////


//驱动命令枚举类型
typedef enum tag_SmartCar_Command
{
    Cmd_Stop = 0,          //停止
    Cmd_Left = 1,          //左转
    Cmd_Right = 2,         //右转
    Cmd_Forward = 3,       //前进
    Cmd_Backward = 4,      //后退
    Cmd_Hold = 5,          //长按保持
    Cmd_Start_Manual = 6,  //手动模式
    Cmd_Start_Auto = 7,    //自动模式
    Cmd_Start_Trace = 8,   //巡线模式
    
} SmartCar_Command;

//驱动模式
typedef enum tag_DriveMode
{
    DM_Manual = 0,   //手动模式
    DM_Auto = 1,     //自动模式
    DM_Tracing = 2,  //巡线模式
} DriveMode; 

#endif //SMARTCAR_DEFINE_H
