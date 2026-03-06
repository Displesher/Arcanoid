#include "Ball.h"

// ABall
const double ABall::Start_Ball_Y_Pos = AsConfig::Platform_Y_Pos - Radius;
const double ABall::Radius = AsConfig::Ball_Size / 2.0;
//-----------------------------------------------------------------------------
ABall::ABall()
   : Ball_X_Direction(0.0), Ball_Y_Direction(0.0),
   Ball_State(EBS_Normal), Ball_Pen(0), Ball_Brush(0),
   Center_X_Pos(0.0), Center_Y_Pos(Start_Ball_Y_Pos),
   Ball_Speed(0.0), Rest_Distance(0.0),
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
      Ball_Rect.left = (int)((Center_X_Pos - Radius) * AsConfig::Global_Scale);
      Ball_Rect.top = (int)((Center_Y_Pos - Radius) * AsConfig::Global_Scale);
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
void ABall::Move(int platform_x_pos, double platform_width,
                                    ALevel *level, AHit_Cheker *hit_cheker)
{
   bool got_hit;
   double next_x_pos, next_y_pos;
   int platform_y_pos = AsConfig::Platform_Y_Pos - AsConfig::Ball_Size;
   double step_size = 1.0 / AsConfig::Global_Scale;

   switch (Ball_State)
   {
   case EBS_On_Platform:
      Center_X_Pos = platform_x_pos + (platform_width) / 2.0;
      Center_Y_Pos = AsConfig::Platform_Y_Pos - Radius;
      break;


   case EBS_Normal:
      Rest_Distance += Ball_Speed;
      while (Rest_Distance >= step_size)
      {
         next_x_pos = Center_X_Pos + step_size * cos(Ball_X_Direction);
         next_y_pos = Center_Y_Pos - step_size * sin(Ball_Y_Direction);

         // Reflection from borders
         got_hit = hit_cheker->Check_Hit(next_x_pos, next_y_pos, this);
         // Reflection from the platform
         //if (next_y_pos - Radius > platform_y_pos &&
         //   next_x_pos + Radius >= platform_x_pos &&
         //   next_x_pos - Radius <= (platform_x_pos + platform_width))
         //{
         //      got_hit = true;
         //      Ball_Y_Direction -= M_PI;
         //}
         // Reflection from the bricks
         level->Check_Level_Brick_Hit(next_y_pos, Ball_Y_Direction, step_size);
         Rest_Distance -= step_size;

         // ball continue moving if not collision
         if (!got_hit)
         {
            Center_X_Pos = next_x_pos;
            Center_Y_Pos = next_y_pos;
         }
      }
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
void ABall::Set_State(EBall_State new_state, double x_pos)
{
   switch (new_state)
   {
   case EBS_On_Platform:
      Center_X_Pos = x_pos - Radius;
      Center_Y_Pos = Start_Ball_Y_Pos;
      Ball_Speed = 0.0;
      Rest_Distance = 0.0;
      Ball_X_Direction = M_PI_4 / 2;
      Ball_Y_Direction = M_PI_4 / 2;
      break;


   case EBS_Normal:
      Center_X_Pos = x_pos - Radius;
      Center_Y_Pos = Start_Ball_Y_Pos;
      Ball_Speed = 3.0;
      Rest_Distance = 0.0;
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
void ABall::Redraw()
{
   Prev_Ball_Rect = Ball_Rect;

   Ball_Rect.left = (int)((Center_X_Pos - Radius) * AsConfig::Global_Scale);
   Ball_Rect.top = (int)((Center_Y_Pos - Radius) * AsConfig::Global_Scale);
   Ball_Rect.right = (Ball_Rect.left + AsConfig::Ball_Size *
                                                      AsConfig::Global_Scale);
   Ball_Rect.bottom = (Ball_Rect.top + AsConfig::Ball_Size *
                                                      AsConfig::Global_Scale);

   InvalidateRect(AsConfig::Hwnd, &Prev_Ball_Rect, FALSE);
   InvalidateRect(AsConfig::Hwnd, &Ball_Rect, FALSE);
}
//-----------------------------------------------------------------------------
