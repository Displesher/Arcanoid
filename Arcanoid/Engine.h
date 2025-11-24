#pragma once

#include <Windows.h>

extern const int Global_Scale;
extern const int Timer_ID;

enum EKey_Type
{
   EKT_A, // = 0
   EKT_Left  = 0,
   EKT_D, // = 1
   EKT_Right = 1,
   EKT_Space
};

void Init_Engine(HWND hwnd);
void Draw_Frame(HDC hdc, RECT &paint_area);
int On_Key_Down(EKey_Type key_type);
int On_Timer();
