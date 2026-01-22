#pragma once

#include "Config.h"

//-----------------------------------------------------------------------------
class AsBorder
{
public:
   AsBorder();

   void Init();
   void Draw(HDC hdc, RECT &paint_area, HPEN bg_pen, HBRUSH bg_brush);

   static const int Border_X_Offset = 6, Border_Y_Offset = 4;

private:
   void Draw_Element(HDC hdc, int x, int y, bool top_border,
                                    HPEN BG_Pen, HBRUSH BG_Brush);

   HPEN Border_Blue_Pen, Border_White_Pen;
   HBRUSH Border_Blue_Brush, Border_White_Brush;
};
//-----------------------------------------------------------------------------
