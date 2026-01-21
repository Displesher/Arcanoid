#pragma once

#include <Windows.h>

#define _USE_MATH_DEFINES
#include <math.h>

class AsEngine;
class ALevel;
class AsPlatform;

//-----------------------------------------------------------------------------
enum EBrick_Type
{
   EBT_None,
   EBT_Red,
   EBT_Blue
};
//-----------------------------------------------------------------------------
enum ELetter_Type
{
   ELT_None,
   ELT_O
};
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
class ALevel
{
public:
   void Init();
   void Draw_Level(HDC hdc, RECT &paint_area);
   void Check_Level_Brick_Hit(int &next_y_pos, ABall *ball);

   static const int Level_Width = 12; // Width a level in cells
   static const int Level_Height = 14; // Height a level in cells
   static const int Level_X_Offset = 8;
   static const int Level_Y_Offset = 6;
   static const int Cell_Width = 16;
   static const int Cell_Height = 8;

private:
   void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type);
   void Set_Brick_Letter_Color(bool is_switch_color,
                                 HPEN &front_pen, HPEN &back_pen,
                                 HBRUSH &front_brush, HBRUSH &back_brush);
   void Rotate_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type,
                              ELetter_Type letter_type, int rotation_step);

   HPEN Brick_Red_Pen, Brick_Blue_Pen, Letter_Pen;
   HBRUSH Brick_Red_Brush, Brick_Blue_Brush;
   RECT Level_Rect;

   static const int Brick_Width = 15;
   static const int Brick_Height = 7;
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

   static void Create_Pen_Brush(HPEN &pen, HBRUSH &brush,
      unsigned char r, unsigned char g, unsigned char b);

   HWND Hwnd;

   HPEN BG_Pen;
   HBRUSH BG_Brush;

   static const int Global_Scale = 3;
   static const int Timer_ID = WM_USER + 1;

   static const int Max_X_Pos = ALevel::Level_X_Offset +
                                 ALevel::Cell_Width * ALevel::Level_Width;
   static const int Max_Y_Pos = 199 - ABall::Ball_Size; // 199
   static const int Border_X_Offset = 6, Border_Y_Offset = 4;

private:
   void Draw_Border(HDC hdc, int x, int y, bool top_border);
   void Draw_Bounds(HDC hdc, RECT &paint_area);

   HPEN Border_Blue_Pen, Border_White_Pen;
   HBRUSH Border_Blue_Brush, Border_White_Brush;

   ABall Ball;
   ALevel Level;
   AsPlatform Platform;
};
//-----------------------------------------------------------------------------
