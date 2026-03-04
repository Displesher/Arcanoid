#include "Ball.h"

// ABall
const double ABall::Start_Ball_Y_Pos = 181.0;
//-----------------------------------------------------------------------------
ABall::ABall()
   : Ball_State(EBS_Normal), Ball_Pen(0), Ball_Brush(0),
   Ball_X_Pos(0.0), Ball_Y_Pos(Start_Ball_Y_Pos), Ball_Speed(0.0),
   Ball_X_Direction(0.0), Ball_Y_Direction(0.0),
   Ball_Rect{}, Prev_Ball_Rect{}
{
   Set_State(EBS_Normal, 0);
}
//-----------------------------------------------------------------------------
void ABall::Init()
{
   AsConfig::Create_Pen_Brush(Ball_Pen, Ball_Brush, 255, 255, 255);
}
//-----------------------------------------------------------------------------
void ABall::Redraw()
{
   Prev_Ball_Rect = Ball_Rect;

   Ball_Rect.left = (int)Ball_X_Pos * AsConfig::Global_Scale;
   Ball_Rect.top = (int)Ball_Y_Pos * AsConfig::Global_Scale;
   Ball_Rect.right = (Ball_Rect.left + AsConfig::Ball_Size *
                                                      AsConfig::Global_Scale);
   Ball_Rect.bottom = (Ball_Rect.top + AsConfig::Ball_Size *
                                                      AsConfig::Global_Scale);

   InvalidateRect(AsConfig::Hwnd, &Prev_Ball_Rect, FALSE);
   InvalidateRect(AsConfig::Hwnd, &Ball_Rect, FALSE);
}
//-----------------------------------------------------------------------------
void ABall::Draw(HDC hdc, RECT &paint_area)
{
   RECT intersection_rect;

   if (IntersectRect(&intersection_rect, &paint_area, &Prev_Ball_Rect))
   {
      // 1. Redraw previous ball position with GB color
      SelectObject(hdc, AsConfig::BG_Pen);
      SelectObject(hdc, AsConfig::BG_Brush);
      Rectangle(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top,
         Prev_Ball_Rect.right, Prev_Ball_Rect.bottom);
   }
   if (IntersectRect(&intersection_rect, &paint_area, &Ball_Rect))
   {
      // 2. Draw current ball position
      Ball_Rect.left = (int)(Ball_X_Pos * AsConfig::Global_Scale);
      Ball_Rect.top = (int)(Ball_Y_Pos * AsConfig::Global_Scale);
      Ball_Rect.right =
         (Ball_Rect.left + AsConfig::Ball_Size * AsConfig::Global_Scale);
      Ball_Rect.bottom =
         (Ball_Rect.top + AsConfig::Ball_Size * AsConfig::Global_Scale);

   SelectObject(hdc, Ball_Pen);
   SelectObject(hdc, Ball_Brush);

   Ellipse(hdc, Ball_Rect.left, Ball_Rect.top,
      Ball_Rect.right - 1, Ball_Rect.bottom - 1);
   }
}
//-----------------------------------------------------------------------------
void ABall::Move(ALevel *level, int platform_x_pos, double platform_width)
{
   double next_x_pos, next_y_pos;
   int max_x_pos = AsConfig::Max_X_Pos - AsConfig::Ball_Size;
   int max_y_pos = AsConfig::Max_Y_Pos - AsConfig::Ball_Size;
   int platform_y_pos = AsConfig::Platform_Y_Pos - AsConfig::Ball_Size;

   switch (Ball_State)
   {
   case EBS_On_Platform:
      Ball_X_Pos =
         platform_x_pos + (platform_width - AsConfig::Ball_Size) / 2.0;
      Ball_Y_Pos = AsConfig::Platform_Y_Pos - AsConfig::Ball_Size;
      break;


   case EBS_Normal:
      next_x_pos = Ball_X_Pos + Ball_Speed * cos(Ball_X_Direction);
      next_y_pos = Ball_Y_Pos - Ball_Speed * sin(Ball_Y_Direction);

      // Reflection from the horizontal border
      if (next_x_pos < AsConfig::Border_X_Offset || next_x_pos > max_x_pos)
         Ball_X_Direction += M_PI;
      // Reflection from the vertical border
      if (next_y_pos < AsConfig::Border_Y_Offset ||
         // Reflection from the floor if exists
         (next_y_pos > max_y_pos && level->Has_Floor) ||
         // Reflection from the platform
         (next_y_pos > platform_y_pos &&
            next_x_pos >= platform_x_pos &&
            next_x_pos <= (platform_x_pos + platform_width)))
         Ball_Y_Direction -= M_PI;
      // if level does not have the floor
      if (next_y_pos > (double)(max_y_pos + AsConfig::Ball_Size * 2.0))
         Ball_State = EBS_Lost;
      // Reflection from the bricks
      level->Check_Level_Brick_Hit(next_y_pos, Ball_Y_Direction, Ball_Speed);

      Ball_X_Pos = next_x_pos;
      Ball_Y_Pos = next_y_pos;
      break;


   case EBS_Lost:
      return;
      break;
   }

   Redraw();
}
//-----------------------------------------------------------------------------
EBall_State ABall::Get_State()
{
   return Ball_State;
}
//-----------------------------------------------------------------------------
void ABall::Set_State(EBall_State new_state, int x_pos)
{
   switch (new_state)
   {
   case EBS_On_Platform:
      Ball_X_Pos = x_pos - AsConfig::Ball_Size / 2;
      Ball_Y_Pos = Start_Ball_Y_Pos;
      Ball_Speed = 0.0;
      Ball_X_Direction = M_PI_4 / 2;
      Ball_Y_Direction = M_PI_4 / 2;
      break;


   case EBS_Normal:
      Ball_X_Pos = x_pos - AsConfig::Ball_Size / 2;
      Ball_Y_Pos = Start_Ball_Y_Pos;
      Ball_Speed = 3.0;
      Ball_X_Direction = M_PI_4 / 2;
      Ball_Y_Direction = M_PI_4 / 2;
      break;


   case EBS_Lost:
      Ball_Speed = 0.0;
      break;
   }
   Ball_State = new_state;
}
//-----------------------------------------------------------------------------
