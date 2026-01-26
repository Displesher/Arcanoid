#pragma once

#include <Windows.h>

#include "Border.h"
#include "Ball.h"

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
class AsEngine;
class AsPlatform
{
public:
   AsPlatform();

   void Init();
   void Redraw(AsEngine *engine);
   void Draw(HDC hdc, RECT &paint_area, AsEngine *engine);

   int X_Pos;
   int X_Step;
   int Width;

private:
   int Inner_Width;

   HPEN Platform_Circle_Pen, Platform_Inner_Pen, Highlight_Pen;
   HBRUSH Platform_Circle_Brush, Platform_Inner_Brush;

   RECT Platform_Rect, Prev_Platform_Rect;

   static const int Circle_Diameter = 7;
   static const int Height = Circle_Diameter;
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
