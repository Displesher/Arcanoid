#pragma once

#include <Windows.h>

//-----------------------------------------------------------------------------
class AsConfig
{
public:
   static void Create_Pen_Brush(HPEN &pen, HBRUSH &brush,
      unsigned char r, unsigned char g, unsigned char b);
   static const int Global_Scale = 3;
};
//-----------------------------------------------------------------------------
