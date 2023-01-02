#include <iostream>
#include<Windows.h>
#include <stdio.h>
#include<math.h>
#include "Hack.h"
#include"D3D.h"
#include"WorldToScreen.h"

#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)

using namespace std;

class D3D坐标 {
public:
    float x;
    float y;
    float z;
};

class AdressBass {
public:
    DWORD Adr_Client;
    DWORD Adr_ClientMe;
    DWORD Adr_Enemy;
    DWORD Adr_Server;
    DWORD Adr_Engine;
    DWORD Adr_Rect;  
};

//------------------全局变量------------------
bool state=true;
Hack hack;
AdressBass Adr_Data;
RECT GameRect;
DWORD PID;
HWND hwnd;
HANDLE hprocess;
int Game_Width;
int Game_Height;
DWORD real_size;
float ViewWorld[4][4];
int View_Width;
int View_Height;
D3D坐标 My_xyz;
D3D坐标 Enemy_xyz;
float Camera_Z;
float Camera_X;
float zoom;
float Camera_Y;
float Camera_Y2;

      //--------------功能---------------
bool King_ESP;
bool King_Display_HP;
bool King_Display_距离;
bool King_Display_Ray;
bool King_Aim;

//-------------------窗口消息---------------------
void Msg_Event()
{
    HackDrawStart();
    WorldToScreen();
    //DrawMenu();
    HackDrawEnd();
}

void main(){
    //---------------获取游戏句柄等-----------------
    PID = hack.GetPid("Counter-Strike: Global Offensive - Direct3D 9");//获取进程PID
    hwnd = FindWindowA(nullptr, "Counter-Strike: Global Offensive - Direct3D 9");//获取进程句柄
    hprocess = hack.HackOpenProcess(PID);//读和写的权限打开进程
    //--------------获取游戏窗口宽/高---------------
    GetWindowRect(hwnd,&GameRect);//获取窗口大小
    Game_Width = GameRect.right - GameRect.left;//窗口宽
    Game_Height = GameRect.bottom - GameRect.top;//窗口宽
    //-------------------基地址----------------------
    Adr_Data.Adr_Client= hack.HackGetModuleHandle(PID, "client.dll");//客户端模块基址
    ReadProcessMemory(hprocess,(LPVOID)(Adr_Data.Adr_Client + 0x4DFFF14),&Adr_Data.Adr_ClientMe,4,&real_size);//本人基址
    Adr_Data.Adr_Server= hack.HackGetModuleHandle(PID, "server.dll");//服务端模块基址
    Adr_Data.Adr_Engine= hack.HackGetModuleHandle(PID, "engine.dll");//引擎类模块基址
    Adr_Data.Adr_Rect= hack.HackGetModuleHandle(PID, "client.dll") + 0x4DF0E54;//矩阵基址
    //-------------------默认功能---------------------
    King_ESP = true;                  //方框透视  默认开启

    King_Display_HP = true;           //显示血量  默认关闭

    King_Display_Ray = true;          //显示射线  默认关闭

    King_Display_距离 = true;         //显示距离  默认关闭

    King_Aim = true;                  //内存自瞄  默认关闭

    //--------------------消息处理--------------------
    cout<<"游戏内按 INS" << endl;
    while (1)
    {     
        
        if (KEY_DOWN(VK_END)) {
            Sleep(500);
            exit(1);
        }

        if (KEY_DOWN(VK_INSERT) && state) {
            if (state)
            {
                CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)start, NULL, 0, NULL);
                state = false;
            }
            Sleep(500);
        }
        
        Sleep(1);
    }
    
}

//------------世界坐标转屏幕坐标--------------
void WorldToScreen() {

    //变量声明在了WorldToScreen.h中

      ReadProcessMemory(//读取矩阵
          hprocess,
          (LPVOID)Adr_Data.Adr_Rect,
          ViewWorld,
          sizeof(float)*4*4,
          &real_size
      );

      View_Width = GetGameWidth()/2;//视角宽
      View_Height = GetGameHeight()/2;//视角高
      
    //----------------------本人数据------------------------
      ReadProcessMemory(//本人阵营
          hprocess,
          (LPVOID)(Adr_Data.Adr_ClientMe + 0x100-0xC),
          &My_Team,
          4,
          &real_size
      );

      ReadProcessMemory(//读取本人X
          hprocess,
          (LPVOID)(Adr_Data.Adr_ClientMe+0x100+0x38),
          &My_xyz.x, 
          sizeof(float),
          &real_size
      );

      ReadProcessMemory(//读取本人Y
          hprocess,
          (LPVOID)(Adr_Data.Adr_ClientMe + 0x100 + 0x3C),
          &My_xyz.y,
          sizeof(float),
          &real_size
      );

      ReadProcessMemory(//读取本人Z
          hprocess,
          (LPVOID)(Adr_Data.Adr_ClientMe + 0x100 + 0x40),
          &My_xyz.z,
          sizeof(float),
          &real_size
      );
    //-------------------------------------------------------
      for (int i=0;i <19;i++)
      {
          ReadProcessMemory(//获取敌人基址
              hprocess,
              (LPVOID)(Adr_Data.Adr_Client + 0x4DFFF14 + i * 0x10),
              &Adr_Data.Adr_Enemy,
              4,
              &real_size
          );
          if (Adr_Data.Adr_Client == Adr_Data.Adr_ClientMe) { continue; }
          ReadProcessMemory(//敌人阵营
              hprocess,
              (LPVOID)(Adr_Data.Adr_Enemy +0x100 -0xC),
              &Enemy_Team,
              4,
              &real_size
          );

          if (My_Team == Enemy_Team) { continue; }

          ReadProcessMemory(//敌人血量
              hprocess,
              (LPVOID)(Adr_Data.Adr_Enemy + 0x100),
              &Enemy_HP,
              4,
              &real_size
          );

         if (Enemy_HP < 1) { continue; }
          
          
          //方框透视
          if (King_ESP)
          {
            //----------------------敌人数据------------------------
              ReadProcessMemory(//读取敌人X
                  hprocess,
                  (LPVOID)(Adr_Data.Adr_Enemy + 0x100 + 0x38),
                  &Enemy_xyz.x,
                  sizeof(float),
                  &real_size
              );
              ReadProcessMemory(//读取敌人Y
                  hprocess,
                  (LPVOID)(Adr_Data.Adr_Enemy + 0x100 + 0x3C),
                  &Enemy_xyz.y,
                  sizeof(float),
                  &real_size
              );
              ReadProcessMemory(//读取敌人Z
                  hprocess,
                  (LPVOID)(Adr_Data.Adr_Enemy + 0x100 + 0x40),
                  &Enemy_xyz.z,
                  sizeof(float),
                  &real_size
              );
            //----------------------竖矩阵算法------------------------
              Camera_Z = ViewWorld[2][0] * Enemy_xyz.x + ViewWorld[2][1] * Enemy_xyz.y + ViewWorld[2][2] * Enemy_xyz.z + ViewWorld[2][3];
              
              zoom = 1 / Camera_Z;

              if (Camera_Z < 0) { continue; }

              Camera_X = View_Width + (ViewWorld[0][0] * Enemy_xyz.x + ViewWorld[0][1] * Enemy_xyz.y + ViewWorld[0][2] * Enemy_xyz.z + ViewWorld[0][3]) * zoom * View_Width;
              Camera_Y = View_Height - (ViewWorld[1][0] * Enemy_xyz.x + ViewWorld[1][1] * Enemy_xyz.y + ViewWorld[1][2] * (Enemy_xyz.z - 8) + ViewWorld[1][3]) * zoom * View_Height;
              Camera_Y2 = View_Height - (ViewWorld[1][0] * Enemy_xyz.x + ViewWorld[1][1] * Enemy_xyz.y + ViewWorld[1][2] * (Enemy_xyz.z + 78) + ViewWorld[1][3]) * zoom * View_Height;
              
              ESP_Height = Camera_Y - Camera_Y2;
              ESP_Width = ESP_Height * 0.526515151552;

              //绘制方框
              HackDrawBox(
                  Camera_X - ESP_Width / 2,//X
                  Camera_Y2,//Y
                  ESP_Width,//W
                  ESP_Height,//H
                  2,//PX
                  D3DCOLOR_ARGB(255, 127, 255, 0)//Color
              );
                    





          }


      }
    
}

//-------------------菜单--------------------
void DrawMenu() {

    HackDrawBox(
        0,//X
        0,//Y
        100,//W
        100,//H
        2,//PX
        D3DCOLOR_ARGB(255, 0, 255, 0)//Color
    );

}



void start()
{
    HackCreatWindow(hwnd, Msg_Event);
    MsgWhile();
}

//-------------------获取宽高---------------------
float GetGameWidth() {
    return Game_Width = GameRect.right - GameRect.left;
}

float GetGameHeight() {
    return Game_Height = GameRect.bottom - GameRect.top;
}
//------------------------------------------------
float 取_准星距离(float 准星X, float 准星Y, float 对象X, float 对象Y) {
    float 距离X = 准星X - 对象X;;
    float 距离Y = 准星Y - 对象Y;;
    float 距离 = sqrt(距离X * 距离X + 距离Y * 距离Y);//求平方根;
    return 距离;
}

float 取_敌人距离(D3D坐标 个人坐标, D3D坐标 对象坐标) {
    float 距离X = 个人坐标.x - 对象坐标.x;
    float 距离Y = 个人坐标.y - 对象坐标.y;;
    float 距离 = sqrt(距离X * 距离X + 距离Y * 距离Y);//求平方根;
    return 距离;
}
