#pragma once

#include <Windows.h>

#include "Border.h"
#include "Level.h"

class AsEngine;
class AsPlatform;

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
class ABall
{
public:
   ABall();

   void Init();
   void Redraw(AsEngine *engine);
   void Draw(HDC hdc, RECT &paint_area, AsEngine *engine);
   void Move(AsEngine *engine, ALevel *level, AsPlatform *platform);

   HPEN Ball_Pen;
   HBRUSH Ball_Brush;

   int Ball_X_Pos, Ball_Y_Pos;
   double Ball_Speed, Ball_X_Direction, Ball_Y_Direction;

   static const int Ball_Size = 4;

private:
   RECT Ball_Rect, Prev_Ball_Rect;
};
//-----------------------------------------------------------------------------
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

   static const int Y_Pos = 185;

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

   static const int Global_Scale = 3;
   static const int Timer_ID = WM_USER + 1;

   static const int Max_X_Pos = ALevel::Level_X_Offset +
                                 ALevel::Cell_Width * ALevel::Level_Width;
   static const int Max_Y_Pos = 199 - ABall::Ball_Size; // 199

private:
   ABall Ball;
   ALevel Level;
   AsPlatform Platform;
   AsBorder Border;
};
//-----------------------------------------------------------------------------
