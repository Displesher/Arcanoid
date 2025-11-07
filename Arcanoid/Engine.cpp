#include "Engine.h"

#define _USE_MATH_DEFINES
#include <math.h>

enum EBrick_Type
{
   EBT_None,
   EBT_Red,
   EBT_Blue
};

enum ELetter_Type
{
   ELT_None,
   ELT_O
};

HWND Hwnd;

HPEN Brick_Red_Pen, Brick_Blue_Pen;
HPEN Platform_Circle_Pen, Platform_Inner_Pen;
HPEN Highlight_Pen, Letter_Pen;
HPEN BG_Pen;

HBRUSH Brick_Red_Brush, Brick_Blue_Brush;
HBRUSH Platform_Circle_Brush, Platform_Inner_Brush;
HBRUSH BG_Brush;

double offset;
const int Global_Scale = 3;
const int Brick_Width = 15;
const int Brick_Height = 7;
const int Cell_Width = 16;
const int Cell_Height = 8;
const int Level_X_Offset = 8;
const int Level_Y_Offset = 6;
const int Level_Width = 14; // Width a level in cells
const int Level_Height = 12; // Height a level in cells
const int Circle_Diameter = 7;
const int Platform_Y_Pos = 185;
const int Platform_Height = Circle_Diameter;

int Inner_Width = 21;
int Platform_X_Pos = 130;
int Platform_X_Step = Global_Scale * 2;
int Platform_Width = Inner_Width + Circle_Diameter;

RECT Platform_Rect, Prev_Platform_Rect;
RECT Level_Rect;

char Level_01[Level_Width][Level_Height] =
{
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

//-----------------------------------------------------------------------------
void Create_Pen_Brush(HPEN &pen, HBRUSH &brush, 
                        unsigned char r, unsigned char g, unsigned char b)
{
   pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
   brush = CreateSolidBrush(RGB(r, g, b));
}
//-----------------------------------------------------------------------------
void Redraw_Platform()
{
   Prev_Platform_Rect = Platform_Rect;

   Platform_Rect.left = Platform_X_Pos * Global_Scale;
   Platform_Rect.top = Platform_Y_Pos * Global_Scale;
   Platform_Rect.right = (Platform_X_Pos + Platform_Width) * Global_Scale;
   Platform_Rect.bottom = (Platform_Y_Pos + Platform_Height) * Global_Scale;

   InvalidateRect(Hwnd, &Prev_Platform_Rect, FALSE);
   InvalidateRect(Hwnd, &Platform_Rect, FALSE);
}
//-----------------------------------------------------------------------------
void Init_Engine(HWND hwnd)
{// Setting up the game before start
   Hwnd = hwnd;

   Create_Pen_Brush(Brick_Red_Pen, Brick_Red_Brush, 185, 45, 50);
   Create_Pen_Brush(Brick_Blue_Pen, Brick_Blue_Brush, 45, 140, 180);
   Create_Pen_Brush(Platform_Circle_Pen, Platform_Circle_Brush, 170, 120, 80);
   Create_Pen_Brush(Platform_Inner_Pen, Platform_Inner_Brush, 200, 190, 170);
   Create_Pen_Brush(BG_Pen, BG_Brush, 15, 63, 31);

   Highlight_Pen = CreatePen(PS_SOLID, 0, RGB(255, 245, 230));
   Letter_Pen = CreatePen(PS_SOLID, 3, RGB(255, 245, 230));

   Level_Rect.left = Level_X_Offset * Global_Scale;
   Level_Rect.top = Level_Y_Offset * Global_Scale;
   Level_Rect.right = Level_Rect.left + Cell_Width * Level_Width * Global_Scale;
   Level_Rect.bottom = Level_Rect.top + Cell_Width * Level_Height * Global_Scale;

   Redraw_Platform();
}
//-----------------------------------------------------------------------------
void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)
{// Draw brick

   HPEN pen;
   HBRUSH brush;

   switch (brick_type)
   {
   case EBT_None:
      return;
   case EBT_Red:
      pen = Brick_Red_Pen;
      brush = Brick_Red_Brush;
      break;
   case EBT_Blue:
      pen = Brick_Blue_Pen;
      brush = Brick_Blue_Brush;
      break;
   default:
      return;
   }
   //SelectObject(hdc, pen);
   SelectObject(hdc, brush);

   RoundRect(
      hdc, x * Global_Scale, y * Global_Scale, 
      (x + Brick_Width) * Global_Scale, (y + Brick_Height) * Global_Scale, 
      2 * Global_Scale, 2 * Global_Scale);
}
//-----------------------------------------------------------------------------
void Set_Brick_Letter_Color(bool is_switch_color, 
   HPEN &front_pen, HPEN &back_pen, HBRUSH &front_brush, HBRUSH &back_brush)
{
   if (is_switch_color)
   {// Front is red, back is blue
      front_pen = Brick_Red_Pen;
      front_brush = Brick_Red_Brush;
      back_pen = Brick_Blue_Pen;
      back_brush = Brick_Blue_Brush;
   }
   else
   {// Front is blue, back is red
      front_pen = Brick_Blue_Pen;
      front_brush = Brick_Blue_Brush;
      back_pen = Brick_Red_Pen;
      back_brush = Brick_Red_Brush;
   }
}
//-----------------------------------------------------------------------------
void Rotate_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, 
                                 ELetter_Type letter_type, int rotation_step)
{// Draw fallen letter

   bool switch_color;
   double rotation_angle;
   // Converting a step to a rotation angle
   int brick_half_height = Brick_Height * Global_Scale / 2;
   // int brick_half_height = Brick_Height / 2 * Global_Scale;
   int back_part_offset;
   HPEN front_pen, back_pen;
   HBRUSH front_brush, back_brush;
   XFORM xform, old_xform;

   if (!(brick_type == EBT_Blue || brick_type == EBT_Red))
      return; // falling letter may be only from such of these bricks

   if (rotation_step > 4 && rotation_step <= 12)
      switch_color = brick_type == EBT_Blue;
   else
      switch_color = brick_type == EBT_Red;
   Set_Brick_Letter_Color(switch_color, front_pen, back_pen, 
                                          front_brush, back_brush);

   // Correct rotation step and rotation angle
   rotation_step %= 16;
   if (rotation_step < 8)
      rotation_angle = 2.0 * M_PI / 16.0 * (double)rotation_step;
   else
      rotation_angle = 2.0 * M_PI / 16.0 * (double)(8 - rotation_step);
   
   if (rotation_step == 4 || rotation_step == 12)
   {
      // Draw background
      SelectObject(hdc, back_pen);
      SelectObject(hdc, back_brush);

      Rectangle(hdc, x, y + brick_half_height - Global_Scale, 
            x + Brick_Width * Global_Scale, 
            y + brick_half_height);

      // Draw foreground
      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);

      Rectangle(hdc, x, y + brick_half_height, 
            x + Brick_Width * Global_Scale,
            y + brick_half_height + Global_Scale - 1);
   }
   else
   {
      SetGraphicsMode(hdc, GM_ADVANCED);
      // Config rotate matrix of letter
      xform.eM11 = 1.0f;
      xform.eM12 = 0.0f;
      xform.eM21 = 0.0f;
      xform.eM22 = (float)cos(rotation_angle);
      xform.eDx  = (float)x;
      xform.eDy  = (float)y + (float)brick_half_height;
      GetWorldTransform(hdc, &old_xform);
      SetWorldTransform(hdc, &xform);

      // Draw background
      SelectObject(hdc, back_pen);
      SelectObject(hdc, back_brush);

      offset = 3.0 * (1.0 - fabs(xform.eM22)) * (double)Global_Scale;
      back_part_offset = (int)round(offset);
      Rectangle(hdc, 0, -brick_half_height - back_part_offset, 
         Brick_Width * Global_Scale, brick_half_height - back_part_offset);
      
      // Draw foreground
      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);
      
      Rectangle(hdc, 0, -brick_half_height, 
         Brick_Width * Global_Scale, brick_half_height);

      if (rotation_step > 4 && rotation_step <= 12)
      {
         if (letter_type == ELT_O)
         {
            SelectObject(hdc, Letter_Pen);
            Ellipse(hdc, (0 + 5) * Global_Scale, (-5 * Global_Scale) / 2, 
               (0 + 10) * Global_Scale, 5 * Global_Scale / 2);
         }
      }
      
      SetWorldTransform(hdc, &old_xform); 
   }
}
//-----------------------------------------------------------------------------
void Draw_Level(HDC hdc)
{// Draw level's bricks
   int i, j;

   for (i = 0; i < 14; i++)
      for (j = 0; j < 12; j++)
         Draw_Brick(hdc, Level_X_Offset + j * Cell_Width, 
            Level_Y_Offset + i * Cell_Height, (EBrick_Type)Level_01[i][j]);
}
//-----------------------------------------------------------------------------
void Draw_Platform(HDC hdc, int x, int y)
{
   // Redraw previous platform position with GB color
   SelectObject(hdc, BG_Pen);
   SelectObject(hdc, BG_Brush);
   Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, 
      Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);

   // Draw circles
   SelectObject(hdc, Platform_Circle_Pen);
   SelectObject(hdc, Platform_Circle_Brush);
   Ellipse(hdc, x * Global_Scale, y * Global_Scale, 
      (x + Circle_Diameter) * Global_Scale,
      (y + Circle_Diameter) * Global_Scale);
   Ellipse(hdc, (x + Inner_Width) * Global_Scale, y * Global_Scale,
      (x + Inner_Width + Circle_Diameter) * Global_Scale,
      (y + Circle_Diameter) * Global_Scale);
   
   // Draw middle part
   SelectObject(hdc, Platform_Inner_Pen);
   SelectObject(hdc, Platform_Inner_Brush);
   RoundRect(hdc, (x + 4) * Global_Scale, (y + 1) * Global_Scale, 
      (x + 4 + Inner_Width - 1) * Global_Scale, (y + 1 + 5) * Global_Scale,
      3 * Global_Scale, 3 * Global_Scale);

   // 3. Draw highlight
   SelectObject(hdc, Highlight_Pen );
   Arc(hdc, (x + 1) * Global_Scale, (y + 1) * Global_Scale, 
      (x + Circle_Diameter - 1) * Global_Scale, 
      (y + Circle_Diameter - 1) * Global_Scale, 
      (x + 1 + 1) * Global_Scale, (y + 1) * Global_Scale, 
      (x + 1) * Global_Scale, (y + 1 + 2) * Global_Scale);
}
//-----------------------------------------------------------------------------
void Draw_Platform_Alt(HDC hdc, int x, int y, int inner_width)
{
   int inner_circles_radius = 5;

   // 1. Draw circles
   SelectObject(hdc, Platform_Circle_Pen);
   SelectObject(hdc, Platform_Circle_Brush);

   Ellipse(hdc, x * Global_Scale, y * Global_Scale,
            (x + Circle_Diameter) * Global_Scale, 
            (y + Circle_Diameter) * Global_Scale);
   Ellipse(hdc, (x + inner_width) * Global_Scale, y * Global_Scale,
            (x + inner_width + Circle_Diameter) * Global_Scale, 
            (y + Circle_Diameter) * Global_Scale);

   // 2. Draw middle part
   SelectObject(hdc, Platform_Inner_Pen);
   SelectObject(hdc, Platform_Inner_Brush);

   Ellipse(hdc, (x + 4) * Global_Scale, (y + 1) * Global_Scale,
            (x + 4 + inner_circles_radius) * Global_Scale, 
            (y + 1 + inner_circles_radius) * Global_Scale);
   Ellipse(hdc, (x - 2 + inner_width) * Global_Scale, (y + 1) * Global_Scale,
            (x - 2 + inner_width + inner_circles_radius) * Global_Scale, 
            (y + 1 + inner_circles_radius) * Global_Scale);
   Rectangle
   (
      hdc, 
      (x + 3 + Circle_Diameter/2) * Global_Scale, 
      (y + 1) * Global_Scale,
      (x + 3 + inner_width - Circle_Diameter/2 + 1) * Global_Scale, 
      (y + 1 + inner_circles_radius) * Global_Scale
   );

   // 3. Draw highlight
   SelectObject(hdc, Highlight_Pen );
   Arc(hdc, (x + 1) * Global_Scale, (y + 1) * Global_Scale, 
      (x + Circle_Diameter - 1) * Global_Scale, 
      (y + Circle_Diameter - 1) * Global_Scale, 
      (x + 1 + 2) * Global_Scale, (y + 1) * Global_Scale, 
      (x + 1) * Global_Scale, (y + 1 + 3) * Global_Scale);
}
//-----------------------------------------------------------------------------
void Draw_Frame(HDC hdc, RECT &paint_area)
{// Draw game frame

   RECT intersection_rect;

   if (IntersectRect(&intersection_rect, &paint_area, &Level_Rect))
      Draw_Level(hdc);

   if (IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))
      Draw_Platform(hdc, Platform_X_Pos, Platform_Y_Pos);
   //
   //Draw_Platform_Alt(hdc, 90, 175, 42);
   //Draw_Platform_Alt(hdc, 60, 185, 12);
   //Draw_Platform_Alt(hdc, 130, 185, 21);

   //int i;

   //for (i = 0; i < 16; i++)
   //{
   //   Rotate_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 
   //                                                100, EBT_Blue, ELT_O, i);
   //   Rotate_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 
   //                                                130, EBT_Red, ELT_O, i);
   //}
}
//-----------------------------------------------------------------------------
int On_Key_Down(EKey_Type key_type)
{
   switch (key_type)
   {
   case EKT_Left:
      Platform_X_Pos -= Platform_X_Step;
      Redraw_Platform();
      break;

   case EKT_Right:
      Platform_X_Pos += Platform_X_Step;
      Redraw_Platform();
      break;

   case EKT_Space:
      break;
   default:
      break;
   }
   return 0;
}
//-----------------------------------------------------------------------------
