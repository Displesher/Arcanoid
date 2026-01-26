#include "Ball.h"

// ABall
//-----------------------------------------------------------------------------
ABall::ABall()
   : Ball_Pen(0), Ball_Brush(0),
   Ball_X_Pos(20), Ball_Y_Pos(170), Ball_Speed(3.0),
   Ball_X_Direction(M_PI_4 / 2), Ball_Y_Direction(M_PI_4 / 2),
   Ball_Rect{}, Prev_Ball_Rect{}
{
}
//-----------------------------------------------------------------------------
void ABall::Init()
{
   AsConfig::Create_Pen_Brush(Ball_Pen, Ball_Brush, 255, 255, 255);
}
//-----------------------------------------------------------------------------
void ABall::Redraw(HWND hwnd)
{
   Prev_Ball_Rect = Ball_Rect;

   Ball_Rect.left = Ball_X_Pos * AsConfig::Global_Scale;
   Ball_Rect.top = Ball_Y_Pos * AsConfig::Global_Scale;
   Ball_Rect.right = (Ball_Rect.left + AsConfig::Ball_Size *
                                                      AsConfig::Global_Scale);
   Ball_Rect.bottom = (Ball_Rect.top + AsConfig::Ball_Size *
                                                      AsConfig::Global_Scale);

   InvalidateRect(hwnd, &Prev_Ball_Rect, FALSE);
   InvalidateRect(hwnd, &Ball_Rect, FALSE);
}
//-----------------------------------------------------------------------------
void ABall::Draw(HDC hdc, RECT &paint_area, HPEN bg_pen, HBRUSH bg_brush)
{
   RECT intersection_rect;
   if (! IntersectRect(&intersection_rect, &paint_area, &Ball_Rect))
      return;

   // 1. Redraw previous ball position with GB color
   SelectObject(hdc, bg_pen);
   SelectObject(hdc, bg_brush);
   Rectangle(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top,
      Prev_Ball_Rect.right, Prev_Ball_Rect.bottom);

   // 2. Draw current ball position
   Ball_Rect.left = (Ball_X_Pos) * AsConfig::Global_Scale;
   Ball_Rect.top = (Ball_Y_Pos) * AsConfig::Global_Scale;
   Ball_Rect.right = (Ball_Rect.left + AsConfig::Ball_Size *
                                                     AsConfig::Global_Scale);
   Ball_Rect.bottom = (Ball_Rect.top + AsConfig::Ball_Size *
                                                     AsConfig::Global_Scale);

   SelectObject(hdc, Ball_Pen);
   SelectObject(hdc, Ball_Brush);

   Ellipse(hdc, Ball_Rect.left, Ball_Rect.top,
      Ball_Rect.right - 1, Ball_Rect.bottom - 1);
}
//-----------------------------------------------------------------------------
void ABall::Move(HWND hwnd, ALevel *level, int platform_x_pos,
                                                      double platform_width)
{
   int next_x_pos, next_y_pos;
   int max_x_pos = AsConfig::Max_X_Pos - AsConfig::Ball_Size;
   int max_y_pos = AsConfig::Max_Y_Pos - AsConfig::Ball_Size;
   int platform_y_pos = AsConfig::Platform_Y_Pos - AsConfig::Ball_Size;

   next_x_pos = Ball_X_Pos + (int)(Ball_Speed * cos(Ball_X_Direction));
   next_y_pos = Ball_Y_Pos - (int)(Ball_Speed * sin(Ball_Y_Direction));

   // Reflection from the horizontal border
   if (next_x_pos < AsConfig::Border_X_Offset || next_x_pos > max_x_pos)
      Ball_X_Direction += M_PI;
   // Reflection from the vertical border
   if (next_y_pos < AsConfig::Border_Y_Offset || next_y_pos > max_y_pos ||
      // Reflection from the platform
      (next_y_pos > platform_y_pos &&
         next_x_pos >= platform_x_pos &&
         next_x_pos <= (platform_x_pos + platform_width)))
      Ball_Y_Direction -= M_PI;
   // Reflection from the bricks
   level->Check_Level_Brick_Hit(next_y_pos, Ball_Y_Direction, Ball_Speed);

   Ball_X_Pos = next_x_pos;
   Ball_Y_Pos = next_y_pos;

   Redraw(hwnd);
}
//-----------------------------------------------------------------------------
