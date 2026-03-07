#pragma once

#include "Config.h"

enum EBall_State
{
   EBS_On_Platform,
   EBS_Normal,
   EBS_Lost
};
//-----------------------------------------------------------------------------
class ABall;
class AHit_Cheker
{
public:
   virtual bool Check_Hit(double next_x_pos, double next_y_pos,
                                                            ABall *ball) = 0;
};
//-----------------------------------------------------------------------------
class ABall
{
public:
   ABall();

   void Init();
   void Draw(HDC hdc, RECT &paint_area);
   void Move(int platform_x_pos, double platform_width);
   EBall_State Get_State();
   void Set_State(EBall_State new_state, double x_pos);

   double Ball_X_Direction, Ball_Y_Direction;

   static void Add_Hit_Cheker(AHit_Cheker *hit_cheker);

   static const double Radius;

private:
   void Redraw();

   EBall_State Ball_State;
   HPEN Ball_Pen;
   HBRUSH Ball_Brush;

   double Center_X_Pos, Center_Y_Pos;
   double Ball_Speed, Rest_Distance;

   RECT Ball_Rect, Prev_Ball_Rect;

   static const double Start_Ball_Y_Pos;
   static int Hit_Chekers_Count;
   static AHit_Cheker *Hit_Chekers[3];
};
//-----------------------------------------------------------------------------
