#include "Platform.h"

// AsPlatform
//-----------------------------------------------------------------------------
AsPlatform::AsPlatform()
   : Width(Normal_Inner_Width + Circle_Diameter),
   X_Pos((AsConfig::Max_X_Pos + AsConfig::Level_X_Offset - Width) / 2),
   X_Step(AsConfig::Global_Scale * 2),
   Platform_State(EPS_Normal),
   Inner_Width(Normal_Inner_Width),
   Meltdown_Y_Poses{},
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
void AsPlatform::Act(HWND hwnd)
{
   int i, len;
   if (Platform_State != EPS_Meltdown)
   {
      Platform_State = EPS_Meltdown;

      len = sizeof(Meltdown_Y_Poses) / sizeof(Meltdown_Y_Poses[0]);
      for (i = 0; i < len; i++)
      {
         Meltdown_Y_Poses[i] = Platform_Rect.bottom;
      }
   }

   if (Platform_State == EPS_Meltdown)
      Redraw(hwnd);
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

   if (Platform_State == EPS_Meltdown)
      Prev_Platform_Rect.bottom =
                  (AsConfig::Max_Y_Pos + 1) * AsConfig::Global_Scale;

   InvalidateRect(hwnd, &Prev_Platform_Rect, FALSE);
   InvalidateRect(hwnd, &Platform_Rect, FALSE);
}
//-----------------------------------------------------------------------------
void AsPlatform::Draw(HDC hdc, RECT &paint_area)
{
   switch (Platform_State)
   {
   case EPS_Normal:
      Draw_Normal_State(hdc, paint_area);
      break;
   case EPS_Meltdown:
      Draw_Meltdown_State(hdc, paint_area);
      break;
   }
}
//-----------------------------------------------------------------------------
void AsPlatform::Draw_Normal_State(HDC hdc, RECT &paint_area)
{
   int x = X_Pos, y = AsConfig::Platform_Y_Pos;
   RECT intersection_rect;
   if (! IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))
      return;
   // Redraw previous platform position with GB color
   SelectObject(hdc, AsConfig::BG_Pen);
   SelectObject(hdc, AsConfig::BG_Brush);
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
void AsPlatform::Draw_Meltdown_State(HDC hdc, RECT &paint_area)
{
   int i, j;
   int x, y, y_offset;
   int area_width, area_height;
   COLORREF pixel;
   COLORREF bg_pixel =
      RGB(AsConfig::BG_Color.R, AsConfig::BG_Color.G, AsConfig::BG_Color.B);
   RECT intersection_rect;

   if (! IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))
      return;

   area_width = Width * AsConfig::Global_Scale;
   area_height = Height * AsConfig::Global_Scale + 1;

   for (i = 0; i < area_width; i++)
   {
      y_offset = AsConfig::Rand(Meltdown_Speed) + 1;
      x = Platform_Rect.left + i;

      for (j = 0; j < area_height; j++)
      {
         y = Meltdown_Y_Poses[i] - j;
         pixel = GetPixel(hdc, x, y);
         SetPixel(hdc, x, y + y_offset, pixel);
      }
      for (j = 0; j < y_offset; j++)
      {
         y = Meltdown_Y_Poses[i] - area_height + 1 + j;
         SetPixel(hdc, x, y, bg_pixel);
      }
      Meltdown_Y_Poses[i] += y_offset;
   }
}
//-----------------------------------------------------------------------------
