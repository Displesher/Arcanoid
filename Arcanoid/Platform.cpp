#include "Platform.h"

// AsPlatform
//-----------------------------------------------------------------------------
AsPlatform::AsPlatform()
   : Inner_Width(21), X_Pos(AsConfig::Max_X_Pos / 2),
   X_Step(AsConfig::Global_Scale * 2),
   Width(21 + Circle_Diameter),
   Platform_Circle_Pen(0), Platform_Inner_Pen(0), Highlight_Pen(0),
   Platform_Circle_Brush(0), Platform_Inner_Brush(0),
   Platform_Rect{}, Prev_Platform_Rect{}
{
}
//-----------------------------------------------------------------------------
void AsPlatform::Init()
{
   AsConfig::Create_Pen_Brush(Platform_Circle_Pen, Platform_Circle_Brush,
      170, 120, 80);
   AsConfig::Create_Pen_Brush(Platform_Inner_Pen, Platform_Inner_Brush,
      200, 190, 170);
   Highlight_Pen = CreatePen(PS_SOLID, 0, RGB(255, 245, 230));
}
//-----------------------------------------------------------------------------
void AsPlatform::Redraw(HWND hwnd)
{
   Prev_Platform_Rect = Platform_Rect;

   Platform_Rect.left = X_Pos * AsConfig::Global_Scale;
   Platform_Rect.top = AsConfig::Platform_Y_Pos * AsConfig::Global_Scale;
   Platform_Rect.right = (X_Pos + Width) * AsConfig::Global_Scale;
   Platform_Rect.bottom = (AsConfig::Platform_Y_Pos + Height) *
      AsConfig::Global_Scale;

   InvalidateRect(hwnd, &Prev_Platform_Rect, FALSE);
   InvalidateRect(hwnd, &Platform_Rect, FALSE);
}
//-----------------------------------------------------------------------------
void AsPlatform::Draw(HDC hdc, RECT &paint_area, HPEN bg_pen, HBRUSH bg_brush)
{
   int x = X_Pos, y = AsConfig::Platform_Y_Pos;
   RECT intersection_rect;
   if (! IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))
      return;
   // Redraw previous platform position with GB color
   SelectObject(hdc, bg_pen);
   SelectObject(hdc, bg_brush);
   Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top,
      Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);

   // Draw circles
   SelectObject(hdc, Platform_Circle_Pen);
   SelectObject(hdc, Platform_Circle_Brush);
   Ellipse(hdc,
      x * AsConfig::Global_Scale, y * AsConfig::Global_Scale,
      (x + Circle_Diameter) * AsConfig::Global_Scale,
      (y + Circle_Diameter) * AsConfig::Global_Scale);
   Ellipse(hdc,
      (x + Inner_Width) * AsConfig::Global_Scale, y * AsConfig::Global_Scale,
      (x + Inner_Width + Circle_Diameter) * AsConfig::Global_Scale,
      (y + Circle_Diameter) * AsConfig::Global_Scale);

   // Draw middle part
   SelectObject(hdc, Platform_Inner_Pen);
   SelectObject(hdc, Platform_Inner_Brush);
   RoundRect(hdc, (x + 4) * AsConfig::Global_Scale,
      (y + 1) * AsConfig::Global_Scale,
      (x + 4 + Inner_Width - 1) * AsConfig::Global_Scale,
      (y + 1 + 5) * AsConfig::Global_Scale,
      3 * AsConfig::Global_Scale, 3 * AsConfig::Global_Scale);

   // 3. Draw highlight
   SelectObject(hdc, Highlight_Pen);
   Arc(hdc, (x + 1) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale,
      (x + Circle_Diameter - 1) * AsConfig::Global_Scale,
      (y + Circle_Diameter - 1) * AsConfig::Global_Scale,
      (x + 1 + 1) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale,
      (x + 1) * AsConfig::Global_Scale, (y + 1 + 2) * AsConfig::Global_Scale);
}
//-----------------------------------------------------------------------------
