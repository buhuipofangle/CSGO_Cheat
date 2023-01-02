#pragma once
#include"Precompile.h"

static MARGINS Margin;
static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};
static ID3DXLine* pLine = 0;
static ID3DXFont* Font;

static HWND 辅助窗口句柄, GameHwnd;
static RECT 窗口矩形;
static int 窗口宽, 窗口高;
static int GameWidth, GameHeight;
//窗口类
static WNDCLASSEX wClass;


typedef void(*Draw)();
static Draw Render;


//窗口消息处理函数
LRESULT WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);

bool InsD3D();

void HackCreatWindow(HWND 游戏窗口句柄, Draw 绘制函数);

void MsgWhile();

void HackDrawLine(D3DCOLOR Color, float X1, float Y1, float X2, float Y2, float Width);

void HackDrawText(float X, float Y, const char* Str, D3DCOLOR Color);

void HackDrawBox(float X, float Y, float W, float H, float Width, D3DCOLOR Color);

void HackDrawStart();

void HackDrawEnd();



