#pragma once

#include "Config.h"

//-----------------------------------------------------------------------------
enum EPlatform_State
{
   EPS_Missing,
   EPS_Normal,
   EPS_Meltdown
};
//-----------------------------------------------------------------------------
class AsPlatform
{
public:
   AsPlatform();

   void Init();
   void Act(HWND hwnd);
   void Redraw(HWND hwnd);
   void Draw(HDC hdc, RECT &paint_area);

   int Width;
   int X_Pos;
   int X_Step;

private:
   void Draw_Normal_State(HDC hdc, RECT &paint_area);
   void Draw_Meltdown_State(HDC hdc, RECT &paint_area);

   EPlatform_State Platform_State;
   int Inner_Width;
   static const int Circle_Diameter = 7;
   static const int Normal_Inner_Width = 21;
   static const int Normal_Width = Normal_Inner_Width + Circle_Diameter;
   int Meltdown_Y_Poses[Normal_Width * AsConfig::Global_Scale];

   HPEN Platform_Circle_Pen, Platform_Inner_Pen, Highlight_Pen;
   HBRUSH Platform_Circle_Brush, Platform_Inner_Brush;

   RECT Platform_Rect, Prev_Platform_Rect;

   static const int Height = Circle_Diameter;
   static const int Meltdown_Speed = 4;
};
//-----------------------------------------------------------------------------
