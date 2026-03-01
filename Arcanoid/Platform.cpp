#include "Platform.h"

// AsPlatform
//-----------------------------------------------------------------------------
AsPlatform::AsPlatform()
   : Width(Normal_Inner_Width + Circle_Diameter),
   X_Pos((AsConfig::Max_X_Pos + AsConfig::Level_X_Offset - Width) / 2),
   X_Step(AsConfig::Global_Scale * 2),
   Platform_State(EPS_Normal),
   Inner_Width(Normal_Inner_Width),
   Rolling_Step(0),
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
void AsPlatform::Act()
{
   switch (Platform_State)
   {
   case EPS_Meltdown:
   case EPS_Roll_In:
   case EPS_Expand_Roll_In:
      Redraw();
   }
}
//-----------------------------------------------------------------------------
void AsPlatform::Set_State(EPlatform_State new_state)
{
   int i, len;

   if (Platform_State == new_state)
      return;

   switch (new_state)
   {
   case EPS_Roll_In:
      X_Pos = AsConfig::Max_X_Pos - Circle_Diameter;
      Rolling_Step = Max_Rolling_Step - 2;
      break;
   case EPS_Meltdown:
      len = sizeof(Meltdown_Y_Poses) / sizeof(Meltdown_Y_Poses[0]);
      for (i = 0; i < len; i++)
         Meltdown_Y_Poses[i] = Platform_Rect.bottom;
      break;
   }

   Platform_State = new_state;
}
//-----------------------------------------------------------------------------
void AsPlatform::Redraw()
{
   int platform_width;

   Prev_Platform_Rect = Platform_Rect;

   if (Platform_State == EPS_Roll_In)
      platform_width = Circle_Diameter;
   else
      platform_width = Width;

   Platform_Rect.left = X_Pos * AsConfig::Global_Scale;
   Platform_Rect.top = AsConfig::Platform_Y_Pos * AsConfig::Global_Scale;
   Platform_Rect.right = (X_Pos + platform_width) * AsConfig::Global_Scale;
   Platform_Rect.bottom =
         (AsConfig::Platform_Y_Pos + Height) * AsConfig::Global_Scale;

   if (Platform_State == EPS_Meltdown)
      Prev_Platform_Rect.bottom =
                  (AsConfig::Max_Y_Pos + 1) * AsConfig::Global_Scale;

   InvalidateRect(AsConfig::Hwnd, &Prev_Platform_Rect, FALSE);
   InvalidateRect(AsConfig::Hwnd, &Platform_Rect, FALSE);
}
//-----------------------------------------------------------------------------
void AsPlatform::Draw(HDC hdc, RECT &paint_area)
{
   RECT intersection_rect;
   if (! IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))
      return;

   switch (Platform_State)
   {
   case EPS_Normal:
      Draw_Normal_State(hdc, paint_area);
      break;
   case EPS_Meltdown:
      Draw_Meltdown_State(hdc, paint_area);
      break;
   case EPS_Roll_In:
      Draw_Roll_In_State(hdc, paint_area);
      break;
   case EPS_Expand_Roll_In:
      Draw_Expanding_Roll_In_State(hdc, paint_area);
      break;
   }
}
//-----------------------------------------------------------------------------
void AsPlatform::Clear_BG(HDC hdc)
{// Redraw previous platform position with GB color
   SelectObject(hdc, AsConfig::BG_Pen);
   SelectObject(hdc, AsConfig::BG_Brush);
   Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top,
                  Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);
}
//-----------------------------------------------------------------------------
void AsPlatform::Draw_Circle_Highlight(HDC hdc, int x, int y)
{
   SelectObject(hdc, Highlight_Pen);
   Arc(hdc, x + 1 * AsConfig::Global_Scale, y + 1 * AsConfig::Global_Scale,
      x + (Circle_Diameter - 1) * AsConfig::Global_Scale,
      y + (Circle_Diameter - 1) * AsConfig::Global_Scale,
      x + (1 + 1) * AsConfig::Global_Scale, y + 1 * AsConfig::Global_Scale,
      x + 1 * AsConfig::Global_Scale, y + (1 + 2) * AsConfig::Global_Scale);
}
//-----------------------------------------------------------------------------
void AsPlatform::Draw_Normal_State(HDC hdc, RECT &paint_area)
{
   int x = X_Pos, y = AsConfig::Platform_Y_Pos;

   Clear_BG(hdc);

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
void AsPlatform::Draw_Roll_In_State(HDC hdc, RECT &paint_area)
{
   //int x = (AsConfig::Max_X_Pos - Circle_Diameter) * AsConfig::Global_Scale;
   int x = X_Pos * AsConfig::Global_Scale;
   int y = AsConfig::Platform_Y_Pos * AsConfig::Global_Scale;
   int roller_size = Circle_Diameter * AsConfig::Global_Scale;
   int line_width = 1 * AsConfig::Global_Scale;
   double alpha;
   XFORM old_xform, xform;

   Clear_BG(hdc);

   // 1. Ball
   SelectObject(hdc, Platform_Circle_Pen);
   SelectObject(hdc, Platform_Circle_Brush);
   Ellipse(hdc, x, y, x + roller_size, y + roller_size);
   // 2. Dividing line
   SetGraphicsMode(hdc, GM_ADVANCED);
   alpha = -2.0 * M_PI / (double)Max_Rolling_Step * (double)Rolling_Step;
   xform.eM11 = (float)cos(alpha );
   xform.eM12 = (float)sin(alpha );
   xform.eM21 = (float)-sin(alpha);
   xform.eM22 = (float)cos(alpha );
   xform.eDx = (float)(x + (roller_size / 2));
   xform.eDy  = (float)(y + (roller_size / 2));
   GetWorldTransform(hdc, &old_xform);
   SetWorldTransform(hdc, &xform);

   SelectObject(hdc, AsConfig::BG_Pen);
   SelectObject(hdc, AsConfig::BG_Brush);
   Rectangle(hdc, -AsConfig::Global_Scale / 2, -roller_size / 2,
                  AsConfig::Global_Scale / 2, roller_size / 2 + 1);
   SetWorldTransform(hdc, &old_xform);
   // 3. Highlight
   Draw_Circle_Highlight(hdc, x, y);

   ++Rolling_Step;

   if (Rolling_Step >= Max_Rolling_Step)
      Rolling_Step -= Max_Rolling_Step;

   X_Pos -= Rolling_Speed;

   if (X_Pos <= Roll_End_X_Pos)
   {
      Platform_State = EPS_Expand_Roll_In;
      Inner_Width = 1;
   }
}
//-----------------------------------------------------------------------------
void AsPlatform::Draw_Expanding_Roll_In_State(HDC hdc, RECT &paint_area)
{
   Draw_Normal_State(hdc, paint_area);

   --X_Pos;
   Inner_Width += 2;

   if (Inner_Width >= Normal_Inner_Width)
   {
      Inner_Width = Normal_Inner_Width;
      Platform_State = EPS_Normal;
      Redraw();
   }
}
//-----------------------------------------------------------------------------
