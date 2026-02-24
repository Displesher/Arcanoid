#pragma once

#include "Config.h"

//-----------------------------------------------------------------------------
enum EBrick_Type
{
   EBT_None,
   EBT_Red,
   EBT_Blue
};
//-----------------------------------------------------------------------------
class AActive_Brick
{
public:
   AActive_Brick(EBrick_Type brick_type);
   void Fade_Out(HWND hwnd);
   void Draw(HDC hdc);

   static void Setup_Colors();

private:
   EBrick_Type Brick_Type;
   int Fade_Step;
   RECT Brick_Rect;

   static unsigned char Get_Fading_Channel(unsigned char channel,
                                             unsigned char bg_channel,
                                                               int step);
   static COLORREF Get_Fading_Color(const AColor &color, int step);

   static const int Max_Fade_Step = 20;

   static HPEN Fadindg_Red_Brick_Pens[Max_Fade_Step];
   static HBRUSH Fadindg_Red_Brick_Brushes[Max_Fade_Step];

   static HPEN Fadindg_Blue_Brick_Pens[Max_Fade_Step];
   static HBRUSH Fadindg_Blue_Brick_Brushes[Max_Fade_Step];
};
//-----------------------------------------------------------------------------
