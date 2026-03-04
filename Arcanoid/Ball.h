#pragma once

#include "Config.h"
#include "Level.h"

enum EBall_State
{
   EBS_On_Platform,
   EBS_Normal,
   EBS_Lost
};
//-----------------------------------------------------------------------------
class ABall
{
public:
   ABall();

   void Init();
   void Redraw();
   void Draw(HDC hdc, RECT &paint_area);
   void Move(ALevel *level, int platform_x_pos, double platform_width);
   EBall_State Get_State();
   void Set_State(EBall_State new_state, int x_pos);

private:
   EBall_State Ball_State;
   HPEN Ball_Pen;
   HBRUSH Ball_Brush;

   double Ball_X_Pos, Ball_Y_Pos;
   double Ball_Speed, Ball_X_Direction, Ball_Y_Direction;

   RECT Ball_Rect, Prev_Ball_Rect;

   static const double Start_Ball_Y_Pos;
};
//-----------------------------------------------------------------------------
