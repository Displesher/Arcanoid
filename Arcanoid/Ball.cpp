#include "Ball.h"

// ABall
const double ABall::Start_Ball_Y_Pos = AsConfig::Platform_Y_Pos - Radius;
const double ABall::Radius = 2.0;
int ABall::Hit_Chekers_Count = 0;
AHit_Cheker *ABall::Hit_Chekers[] = {};
//-----------------------------------------------------------------------------
ABall::ABall()
   : Ball_Direction(0.0),
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
         (Ball_Rect.left + Radius * 2.0 * AsConfig::Global_Scale);
      Ball_Rect.bottom =
         (Ball_Rect.top + Radius * 2.0 *AsConfig::Global_Scale);

   SelectObject(hdc, Ball_Pen);
   SelectObject(hdc, Ball_Brush);

   Ellipse(hdc, Ball_Rect.left, Ball_Rect.top,
      Ball_Rect.right - 1, Ball_Rect.bottom - 1);
   }
}
//-----------------------------------------------------------------------------
void ABall::Move(int platform_x_pos, double platform_width)
{
   int i;
   bool got_hit;
   double next_x_pos, next_y_pos;
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
         got_hit = false;
         next_x_pos = Center_X_Pos + step_size * cos(Ball_Direction);
         next_y_pos = Center_Y_Pos - step_size * sin(Ball_Direction);

         // Reflection from borders, platform and bricks
         for (i = 0; i < Hit_Chekers_Count; i++)
            got_hit |= Hit_Chekers[i]->Check_Hit(next_x_pos, next_y_pos, this);

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
      Ball_Direction = M_PI_4 / 2.0;
      break;


   case EBS_Normal:
      Center_X_Pos = x_pos - Radius;
      Center_Y_Pos = Start_Ball_Y_Pos;
      Ball_Speed = 3.0;
      Rest_Distance = 0.0;
      Ball_Direction = M_PI_4 / 2.0;
      break;


   case EBS_Lost:
      Ball_Speed = 0.0;
      break;
   }
   Ball_State = new_state;
}
//-----------------------------------------------------------------------------
double ABall::Get_Direction()
{
   return Ball_Direction;
}
//-----------------------------------------------------------------------------
void ABall::Set_Direction(double new_direction)
{
   const double pi_2 = 2.0 * M_PI;

   while (new_direction < 0.0)
      new_direction += pi_2;

   while (new_direction > pi_2)
      new_direction -= pi_2;

   Ball_Direction = new_direction;
}
//-----------------------------------------------------------------------------
void ABall::Reflect(bool from_horizontal)
{
   if (from_horizontal)
      Set_Direction(-Ball_Direction);
   else
      Set_Direction(M_PI - Ball_Direction);
}
//-----------------------------------------------------------------------------
void ABall::Add_Hit_Cheker(AHit_Cheker *hit_cheker)
{
   if (Hit_Chekers_Count >= sizeof(Hit_Chekers) / sizeof(Hit_Chekers[0]))
      return;

   ABall::Hit_Chekers[Hit_Chekers_Count++] = hit_cheker;
}
//-----------------------------------------------------------------------------
void ABall::Redraw()
{
   Prev_Ball_Rect = Ball_Rect;

   Ball_Rect.left = (int)((Center_X_Pos - Radius) * AsConfig::Global_Scale);
   Ball_Rect.top = (int)((Center_Y_Pos - Radius) * AsConfig::Global_Scale);
   Ball_Rect.right = (Ball_Rect.left + Radius * 2.0 * AsConfig::Global_Scale);
   Ball_Rect.bottom = (Ball_Rect.top + Radius * 2.0 * AsConfig::Global_Scale);

   InvalidateRect(AsConfig::Hwnd, &Prev_Ball_Rect, FALSE);
   InvalidateRect(AsConfig::Hwnd, &Ball_Rect, FALSE);
}
//-----------------------------------------------------------------------------
