#pragma once
#include"Precompile.h"

static MARGINS Margin;
static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};
static ID3DXLine* pLine = 0;
static ID3DXFont* Font;

static HWND �������ھ��, GameHwnd;
static RECT ���ھ���;
static int ���ڿ�, ���ڸ�;
static int GameWidth, GameHeight;
//������
static WNDCLASSEX wClass;


typedef void(*Draw)();
static Draw Render;


//������Ϣ������
LRESULT WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);

bool InsD3D();

void HackCreatWindow(HWND ��Ϸ���ھ��, Draw ���ƺ���);

void MsgWhile();

void HackDrawLine(D3DCOLOR Color, float X1, float Y1, float X2, float Y2, float Width);

void HackDrawText(float X, float Y, const char* Str, D3DCOLOR Color);

void HackDrawBox(float X, float Y, float W, float H, float Width, D3DCOLOR Color);

void HackDrawStart();

void HackDrawEnd();



