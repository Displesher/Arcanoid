#pragma once

#include "Config.h"
#include "Level.h"

//-----------------------------------------------------------------------------
class ABall
{
public:
   ABall();

   void Init();
   void Redraw();
   void Draw(HDC hdc, RECT &paint_area);
   void Move(ALevel *level, int platform_x_pos, double platform_width);

private:
   HPEN Ball_Pen;
   HBRUSH Ball_Brush;

   int Ball_X_Pos, Ball_Y_Pos;
   double Ball_Speed, Ball_X_Direction, Ball_Y_Direction;

   RECT Ball_Rect, Prev_Ball_Rect;
};
//-----------------------------------------------------------------------------
