#include "Config.h"

// AColor
//-----------------------------------------------------------------------------
AColor::AColor(unsigned char r, unsigned char g, unsigned char b)
   : R(r), G(g), B(b)
{

}
//-----------------------------------------------------------------------------




// AsConfig
int AsConfig::Curent_Timer_Tick = 0;
const AColor AsConfig::BG_Color(15, 63, 31);
const AColor AsConfig::Red_Brick_Color(185, 45, 50);
const AColor AsConfig::Blue_Brick_Color(45, 140, 180);
const AColor AsConfig::Brick_Border_Color(0, 0, 0);
HPEN AsConfig::BG_Pen;
HBRUSH AsConfig::BG_Brush;
HWND AsConfig::Hwnd;
//-----------------------------------------------------------------------------
void AsConfig::Setup_Colors()
{
   AsConfig::Create_Pen_Brush(BG_Pen, BG_Brush, AsConfig::BG_Color);
}
//-----------------------------------------------------------------------------
void AsConfig::Create_Pen_Brush(HPEN &pen, HBRUSH &brush, const AColor &color)
{
   pen = CreatePen(PS_SOLID, 0, RGB(color.R, color.G, color.B));
   brush = CreateSolidBrush(RGB(color.R, color.G, color.B));
}
//-----------------------------------------------------------------------------
void AsConfig::Create_Pen_Brush(HPEN &pen, HBRUSH &brush,
   unsigned char r, unsigned char g, unsigned char b)
{
   pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
   brush = CreateSolidBrush(RGB(r, g, b));
}
//-----------------------------------------------------------------------------
int AsConfig::Rand(int range)
{// Calculate a pseudo-random number in the range [0, .., range - 1]
   return rand() * range / RAND_MAX;
}
//-----------------------------------------------------------------------------
