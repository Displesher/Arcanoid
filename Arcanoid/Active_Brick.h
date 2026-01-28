#pragma once

#include "Config.h"

//-----------------------------------------------------------------------------
class AActive_Brick
{
public:
   AActive_Brick();
   void Fade_Out(HWND hwnd);
   void Draw(HDC hdc);

private:
   int Fade_Step;

   RECT Brick_Rect;

   static const int Max_Fade_Step = 20;
};
//-----------------------------------------------------------------------------