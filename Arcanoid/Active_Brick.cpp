#include "Active_Brick.h"

// AActive_Brick

HPEN AActive_Brick::Fadindg_Red_Brick_Pens[Max_Fade_Step];
HBRUSH AActive_Brick::Fadindg_Red_Brick_Brushes[Max_Fade_Step];

HPEN AActive_Brick::Fadindg_Blue_Brick_Pens[Max_Fade_Step];
HBRUSH AActive_Brick::Fadindg_Blue_Brick_Brushes[Max_Fade_Step];
//-----------------------------------------------------------------------------
AActive_Brick::AActive_Brick(EBrick_Type brick_type)
   : Brick_Type(brick_type), Fade_Step(0), Brick_Rect{}
{
}
//-----------------------------------------------------------------------------
void AActive_Brick::Fade_Out(HWND hwnd)
{
   //// infinitely fading for debug
   //if (Fade_Step >= Max_Fade_Step - 1)
   //   Fade_Step = 0;

   if (Fade_Step < Max_Fade_Step - 1)
      ++Fade_Step;
   else
      return;

   InvalidateRect(hwnd, &Brick_Rect, FALSE);
}
//-----------------------------------------------------------------------------
void AActive_Brick::Draw(HDC hdc)
{// Draw a fading brick

   HPEN pen = 0;
   HBRUSH brush = 0;

   switch (Brick_Type)
   {
   case EBT_Red:
      pen = Fadindg_Red_Brick_Pens[Fade_Step];
      brush = Fadindg_Red_Brick_Brushes[Fade_Step];
      break;
   case EBT_Blue:
      pen = Fadindg_Blue_Brick_Pens[Fade_Step];
      brush = Fadindg_Blue_Brick_Brushes[Fade_Step];
      break;
   }

   if (pen)
      SelectObject(hdc, pen);
   if (brush)
      SelectObject(hdc, brush);

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
void AActive_Brick::Setup_Colors()
{
   COLORREF color;
   int i;

   for (i = 0; i < Max_Fade_Step; i++)
   {
      color = Get_Fading_Color(AsConfig::Brick_Border_Color, i);
      Fadindg_Red_Brick_Pens[i] = CreatePen(PS_SOLID, 1, color);
      Fadindg_Blue_Brick_Pens[i] = CreatePen(PS_SOLID, 1, color);

      color = Get_Fading_Color(AsConfig::Red_Brick_Color, i);
      Fadindg_Red_Brick_Brushes[i] = CreateSolidBrush(color);

      color = Get_Fading_Color(AsConfig::Blue_Brick_Color, i);
      Fadindg_Blue_Brick_Brushes[i] = CreateSolidBrush(color);
   }
}
//-----------------------------------------------------------------------------
unsigned char AActive_Brick::Get_Fading_Channel(unsigned char channel,
   unsigned char bg_channel,
   int step)
{
   return (channel -  step * (channel - bg_channel) / (Max_Fade_Step - 1));
}
//-----------------------------------------------------------------------------
COLORREF AActive_Brick::Get_Fading_Color(const AColor &color, int step)
{
   unsigned char r, g, b;

   r = Get_Fading_Channel(color.R, AsConfig::BG_Color.R, step);
   g = Get_Fading_Channel(color.G, AsConfig::BG_Color.G, step);
   b = Get_Fading_Channel(color.B, AsConfig::BG_Color.B, step);

   return RGB(r, g, b);
}
//-----------------------------------------------------------------------------
