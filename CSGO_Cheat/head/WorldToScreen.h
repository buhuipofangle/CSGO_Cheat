#pragma once
//WorldToScreen:
class EspRectData {
public:
    float x;
    float y;
};
EspRectData EspRect;
static float ESP_Width;
static float ESP_Height;
static int My_Team;
static int Enemy_Team;
static int Enemy_Number;
static int Enemy_HP;



void start();

float GetGameWidth();

float GetGameHeight();

void WorldToScreen();

void DrawMenu();

float 取_准星距离();

float 取_敌人距离();