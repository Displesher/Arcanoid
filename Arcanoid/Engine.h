#pragma once

#include <Windows.h>

#include "Border.h"
#include "Ball.h"
#include "Platform.h"

//-----------------------------------------------------------------------------
enum EKey_Type
{
   EKT_A, // = 0
   EKT_Left  = 0,
   EKT_D, // = 1
   EKT_Right = 1,
   EKT_Space
};
//-----------------------------------------------------------------------------
class AsEngine
{
public:
   AsEngine();

   void Init_Engine(HWND hwnd);
   void Draw_Frame(HDC hdc, RECT &paint_area);
   int On_Key_Down(EKey_Type key_type);
   int On_Timer();

   HWND Hwnd;

   HPEN BG_Pen;
   HBRUSH BG_Brush;

   static const int Timer_ID = WM_USER + 1;

private:
   ABall Ball;
   ALevel Level;
   AsPlatform Platform;
   AsBorder Border;
};
//-----------------------------------------------------------------------------
