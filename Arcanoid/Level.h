#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include "Config.h"

class ABall;

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
class ALevel
{
public:
   ALevel();

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
