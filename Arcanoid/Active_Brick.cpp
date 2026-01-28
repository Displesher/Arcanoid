#include "Active_Brick.h"

// AActive_Brick
//-----------------------------------------------------------------------------
AActive_Brick::AActive_Brick()
   : Fade_Step(0), Brick_Rect{}
{
}
//-----------------------------------------------------------------------------
void AActive_Brick::Fade_Out(HWND hwnd)
{
   //if (Fade_Step > Max_Fade_Step)
   //   return;
   //++Fade_Step;
   //Redraw_Brick(hwnd);

   if (Fade_Step < Max_Fade_Step)
      ++Fade_Step;
   else
      return;

   InvalidateRect(hwnd, &Brick_Rect, FALSE);
}
//-----------------------------------------------------------------------------
void AActive_Brick::Draw(HDC hdc)
{// Draw a fading brick

   HPEN fade_brick_pen;
   HBRUSH fade_brick_brush;

   // blue_brick_R = 45, blue_brick_G = 140, blue_brick_B = 180;
   // red_brick_R = 185, red_brick_G = 45, red_brick_B = 50;

   // bg_r = 15, bg_g = 63, bg_b = 31;

   //int fade_brick_R = (45 - ((45 - 15) / Fade_Step));
   //int fade_brick_G = (140 - ((140 - 45) / Fade_Step));
   //int fade_brick_B = (180 - ((180 - 31) / Fade_Step));

   int fade_pen_R = (0 +  Fade_Step * ((0 + 15) / Max_Fade_Step));
   int fade_pen_G = (0 + Fade_Step * ((0 + 63) / Max_Fade_Step));
   int fade_pen_B = (0 + Fade_Step * ((0 + 31) / Max_Fade_Step));

   int fade_brush_R = (45 -  Fade_Step * ((45 - 15) / Max_Fade_Step));
   int fade_brush_G = (140 - Fade_Step * ((140 - 63) / Max_Fade_Step));
   int fade_brush_B = (180 - Fade_Step * ((180 - 31) / Max_Fade_Step));

   if (Fade_Step == Max_Fade_Step)
   {
      fade_pen_R = 15;
      fade_pen_G = 63;
      fade_pen_B = 31;

      fade_brush_R = 15;
      fade_brush_G = 63;
      fade_brush_B = 31;
   }

   fade_brick_pen =
      CreatePen(PS_SOLID, 1, RGB(fade_pen_R, fade_pen_G, fade_pen_B));
   fade_brick_brush =
      CreateSolidBrush(RGB(fade_brush_R, fade_brush_G, fade_brush_B));

   SelectObject(hdc, fade_brick_pen);
   SelectObject(hdc, fade_brick_brush);

   Brick_Rect.left = (AsConfig::Level_X_Offset + 1 * AsConfig::Cell_Width)
      * AsConfig::Global_Scale;
   Brick_Rect.top = (AsConfig::Level_Y_Offset + 1 * AsConfig::Cell_Height)
      * AsConfig::Global_Scale;
   Brick_Rect.right =
      ((AsConfig::Level_X_Offset + 1 * AsConfig::Cell_Width)
         + AsConfig::Brick_Width) * AsConfig::Global_Scale;
   Brick_Rect.bottom =
      ((AsConfig::Level_Y_Offset + 1 * AsConfig::Cell_Height)
         + AsConfig::Brick_Height) * AsConfig::Global_Scale;

   RoundRect(hdc,
      Brick_Rect.left, Brick_Rect.top, Brick_Rect.right, Brick_Rect.bottom,
      2 * AsConfig::Global_Scale, 2 * AsConfig::Global_Scale);
}
//-----------------------------------------------------------------------------
