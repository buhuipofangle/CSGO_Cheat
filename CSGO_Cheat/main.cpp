#include <iostream>
#include<Windows.h>
#include <stdio.h>
#include<math.h>
#include "Hack.h"
#include"D3D.h"
#include"WorldToScreen.h"

#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)

using namespace std;

class D3D���� {
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

//------------------ȫ�ֱ���------------------
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
D3D���� My_xyz;
D3D���� Enemy_xyz;
float Camera_Z;
float Camera_X;
float zoom;
float Camera_Y;
float Camera_Y2;

      //--------------����---------------
bool King_ESP;
bool King_Display_HP;
bool King_Display_����;
bool King_Display_Ray;
bool King_Aim;

//-------------------������Ϣ---------------------
void Msg_Event()
{
    HackDrawStart();
    WorldToScreen();
    //DrawMenu();
    HackDrawEnd();
}

void main(){
    //---------------��ȡ��Ϸ�����-----------------
    PID = hack.GetPid("Counter-Strike: Global Offensive - Direct3D 9");//��ȡ����PID
    hwnd = FindWindowA(nullptr, "Counter-Strike: Global Offensive - Direct3D 9");//��ȡ���̾��
    hprocess = hack.HackOpenProcess(PID);//����д��Ȩ�޴򿪽���
    //--------------��ȡ��Ϸ���ڿ�/��---------------
    GetWindowRect(hwnd,&GameRect);//��ȡ���ڴ�С
    Game_Width = GameRect.right - GameRect.left;//���ڿ�
    Game_Height = GameRect.bottom - GameRect.top;//���ڿ�
    //-------------------����ַ----------------------
    Adr_Data.Adr_Client= hack.HackGetModuleHandle(PID, "client.dll");//�ͻ���ģ���ַ
    ReadProcessMemory(hprocess,(LPVOID)(Adr_Data.Adr_Client + 0x4DFFF14),&Adr_Data.Adr_ClientMe,4,&real_size);//���˻�ַ
    Adr_Data.Adr_Server= hack.HackGetModuleHandle(PID, "server.dll");//�����ģ���ַ
    Adr_Data.Adr_Engine= hack.HackGetModuleHandle(PID, "engine.dll");//������ģ���ַ
    Adr_Data.Adr_Rect= hack.HackGetModuleHandle(PID, "client.dll") + 0x4DF0E54;//�����ַ
    //-------------------Ĭ�Ϲ���---------------------
    King_ESP = true;                  //����͸��  Ĭ�Ͽ���

    King_Display_HP = true;           //��ʾѪ��  Ĭ�Ϲر�

    King_Display_Ray = true;          //��ʾ����  Ĭ�Ϲر�

    King_Display_���� = true;         //��ʾ����  Ĭ�Ϲر�

    King_Aim = true;                  //�ڴ�����  Ĭ�Ϲر�

    //--------------------��Ϣ����--------------------
    cout<<"��Ϸ�ڰ� INS" << endl;
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

//------------��������ת��Ļ����--------------
void WorldToScreen() {

    //������������WorldToScreen.h��

      ReadProcessMemory(//��ȡ����
          hprocess,
          (LPVOID)Adr_Data.Adr_Rect,
          ViewWorld,
          sizeof(float)*4*4,
          &real_size
      );

      View_Width = GetGameWidth()/2;//�ӽǿ�
      View_Height = GetGameHeight()/2;//�ӽǸ�
      
    //----------------------��������------------------------
      ReadProcessMemory(//������Ӫ
          hprocess,
          (LPVOID)(Adr_Data.Adr_ClientMe + 0x100-0xC),
          &My_Team,
          4,
          &real_size
      );

      ReadProcessMemory(//��ȡ����X
          hprocess,
          (LPVOID)(Adr_Data.Adr_ClientMe+0x100+0x38),
          &My_xyz.x, 
          sizeof(float),
          &real_size
      );

      ReadProcessMemory(//��ȡ����Y
          hprocess,
          (LPVOID)(Adr_Data.Adr_ClientMe + 0x100 + 0x3C),
          &My_xyz.y,
          sizeof(float),
          &real_size
      );

      ReadProcessMemory(//��ȡ����Z
          hprocess,
          (LPVOID)(Adr_Data.Adr_ClientMe + 0x100 + 0x40),
          &My_xyz.z,
          sizeof(float),
          &real_size
      );
    //-------------------------------------------------------
      for (int i=0;i <19;i++)
      {
          ReadProcessMemory(//��ȡ���˻�ַ
              hprocess,
              (LPVOID)(Adr_Data.Adr_Client + 0x4DFFF14 + i * 0x10),
              &Adr_Data.Adr_Enemy,
              4,
              &real_size
          );
          if (Adr_Data.Adr_Client == Adr_Data.Adr_ClientMe) { continue; }
          ReadProcessMemory(//������Ӫ
              hprocess,
              (LPVOID)(Adr_Data.Adr_Enemy +0x100 -0xC),
              &Enemy_Team,
              4,
              &real_size
          );

          if (My_Team == Enemy_Team) { continue; }

          ReadProcessMemory(//����Ѫ��
              hprocess,
              (LPVOID)(Adr_Data.Adr_Enemy + 0x100),
              &Enemy_HP,
              4,
              &real_size
          );

         if (Enemy_HP < 1) { continue; }
          
          
          //����͸��
          if (King_ESP)
          {
            //----------------------��������------------------------
              ReadProcessMemory(//��ȡ����X
                  hprocess,
                  (LPVOID)(Adr_Data.Adr_Enemy + 0x100 + 0x38),
                  &Enemy_xyz.x,
                  sizeof(float),
                  &real_size
              );
              ReadProcessMemory(//��ȡ����Y
                  hprocess,
                  (LPVOID)(Adr_Data.Adr_Enemy + 0x100 + 0x3C),
                  &Enemy_xyz.y,
                  sizeof(float),
                  &real_size
              );
              ReadProcessMemory(//��ȡ����Z
                  hprocess,
                  (LPVOID)(Adr_Data.Adr_Enemy + 0x100 + 0x40),
                  &Enemy_xyz.z,
                  sizeof(float),
                  &real_size
              );
            //----------------------�������㷨------------------------
              Camera_Z = ViewWorld[2][0] * Enemy_xyz.x + ViewWorld[2][1] * Enemy_xyz.y + ViewWorld[2][2] * Enemy_xyz.z + ViewWorld[2][3];
              
              zoom = 1 / Camera_Z;

              if (Camera_Z < 0) { continue; }

              Camera_X = View_Width + (ViewWorld[0][0] * Enemy_xyz.x + ViewWorld[0][1] * Enemy_xyz.y + ViewWorld[0][2] * Enemy_xyz.z + ViewWorld[0][3]) * zoom * View_Width;
              Camera_Y = View_Height - (ViewWorld[1][0] * Enemy_xyz.x + ViewWorld[1][1] * Enemy_xyz.y + ViewWorld[1][2] * (Enemy_xyz.z - 8) + ViewWorld[1][3]) * zoom * View_Height;
              Camera_Y2 = View_Height - (ViewWorld[1][0] * Enemy_xyz.x + ViewWorld[1][1] * Enemy_xyz.y + ViewWorld[1][2] * (Enemy_xyz.z + 78) + ViewWorld[1][3]) * zoom * View_Height;
              
              ESP_Height = Camera_Y - Camera_Y2;
              ESP_Width = ESP_Height * 0.526515151552;

              //���Ʒ���
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

//-------------------�˵�--------------------
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

//-------------------��ȡ���---------------------
float GetGameWidth() {
    return Game_Width = GameRect.right - GameRect.left;
}

float GetGameHeight() {
    return Game_Height = GameRect.bottom - GameRect.top;
}
//------------------------------------------------
float ȡ_׼�Ǿ���(float ׼��X, float ׼��Y, float ����X, float ����Y) {
    float ����X = ׼��X - ����X;;
    float ����Y = ׼��Y - ����Y;;
    float ���� = sqrt(����X * ����X + ����Y * ����Y);//��ƽ����;
    return ����;
}

float ȡ_���˾���(D3D���� ��������, D3D���� ��������) {
    float ����X = ��������.x - ��������.x;
    float ����Y = ��������.y - ��������.y;;
    float ���� = sqrt(����X * ����X + ����Y * ����Y);//��ƽ����;
    return ����;
}
