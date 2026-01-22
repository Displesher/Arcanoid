#include "Config.h"

// AsConfig
//-----------------------------------------------------------------------------
void AsConfig::Create_Pen_Brush(HPEN &pen, HBRUSH &brush,
   unsigned char r, unsigned char g, unsigned char b)
{
   pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
   brush = CreateSolidBrush(RGB(r, g, b));
}
//-----------------------------------------------------------------------------
