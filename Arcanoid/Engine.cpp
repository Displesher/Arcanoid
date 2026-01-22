#include "Engine.h"

// ABall
//-----------------------------------------------------------------------------
ABall::ABall()
   : Ball_Pen(0), Ball_Brush(0),
     Ball_X_Pos(20), Ball_Y_Pos(170), Ball_Speed(3.0),
     Ball_X_Direction(M_PI_4 / 2), Ball_Y_Direction(M_PI_4 / 2),
     Ball_Rect{}, Prev_Ball_Rect{}
{
}
//-----------------------------------------------------------------------------
void ABall::Init()
{
   AsConfig::Create_Pen_Brush(Ball_Pen, Ball_Brush, 255, 255, 255);
}
//-----------------------------------------------------------------------------
void ABall::Redraw(AsEngine *engine)
{
   Prev_Ball_Rect = Ball_Rect;

   Ball_Rect.left = Ball_X_Pos * AsEngine::Global_Scale;
   Ball_Rect.top = Ball_Y_Pos * AsEngine::Global_Scale;
   Ball_Rect.right = (Ball_Rect.left + Ball_Size * AsEngine::Global_Scale);
   Ball_Rect.bottom = (Ball_Rect.top + Ball_Size * AsEngine::Global_Scale);

   InvalidateRect(engine->Hwnd, &Prev_Ball_Rect, FALSE);
   InvalidateRect(engine->Hwnd, &Ball_Rect, FALSE);
}
//-----------------------------------------------------------------------------
void ABall::Draw(HDC hdc, RECT &paint_area, AsEngine *engine)
{
   RECT intersection_rect;
   if (! IntersectRect(&intersection_rect, &paint_area, &Ball_Rect))
      return;

   // 1. Redraw previous ball position with GB color
   SelectObject(hdc, engine->BG_Pen);
   SelectObject(hdc, engine->BG_Brush);
   Rectangle(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top,
      Prev_Ball_Rect.right, Prev_Ball_Rect.bottom);

   // 2. Draw current ball position
   Ball_Rect.left = (Ball_X_Pos) * AsEngine::Global_Scale;
   Ball_Rect.top = (Ball_Y_Pos) * AsEngine::Global_Scale;
   Ball_Rect.right = (Ball_Rect.left + Ball_Size * AsEngine::Global_Scale);
   Ball_Rect.bottom = (Ball_Rect.top + Ball_Size * AsEngine::Global_Scale);

   SelectObject(hdc, Ball_Pen);
   SelectObject(hdc, Ball_Brush);

   Ellipse(hdc, Ball_Rect.left, Ball_Rect.top,
      Ball_Rect.right - 1, Ball_Rect.bottom - 1);
}
//-----------------------------------------------------------------------------
void ABall::Move(AsEngine *engine, ALevel *level, AsPlatform *platform)
{
   int next_x_pos, next_y_pos;
   int max_x_pos = AsEngine::Max_X_Pos - Ball_Size;
   int max_y_pos = AsEngine::Max_Y_Pos - Ball_Size;
   int platform_y_pos = AsPlatform::Y_Pos - Ball_Size;

   next_x_pos = Ball_X_Pos + (int)(Ball_Speed * cos(Ball_X_Direction));
   next_y_pos = Ball_Y_Pos - (int)(Ball_Speed * sin(Ball_Y_Direction));

   // Reflection from the horizontal border
   if (next_x_pos < AsBorder::Border_X_Offset || next_x_pos > max_x_pos)
      Ball_X_Direction += M_PI;
   // Reflection from the vertical border
   if (next_y_pos < AsBorder::Border_Y_Offset || next_y_pos > max_y_pos ||
      // Reflection from the platform
      (next_y_pos > platform_y_pos &&
         next_x_pos >= platform->X_Pos &&
         next_x_pos <= (platform->X_Pos + platform->Width)))
      Ball_Y_Direction -= M_PI;
   // Reflection from the bricks
   level->Check_Level_Brick_Hit(next_y_pos, this);

   Ball_X_Pos = next_x_pos;
   Ball_Y_Pos = next_y_pos;

   Redraw(engine);
}
//-----------------------------------------------------------------------------



// AsPlatform
//-----------------------------------------------------------------------------
AsPlatform::AsPlatform()
   : Inner_Width(21), X_Pos(AsEngine::Max_X_Pos / 2),
     X_Step(AsEngine::Global_Scale * 2),
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
void AsPlatform::Redraw(AsEngine *engine)
{
   Prev_Platform_Rect = Platform_Rect;

   Platform_Rect.left = X_Pos * AsEngine::Global_Scale;
   Platform_Rect.top = Y_Pos * AsEngine::Global_Scale;
   Platform_Rect.right = (X_Pos + Width) * AsEngine::Global_Scale;
   Platform_Rect.bottom = (Y_Pos + Height) * AsEngine::Global_Scale;

   InvalidateRect(engine->Hwnd, &Prev_Platform_Rect, FALSE);
   InvalidateRect(engine->Hwnd, &Platform_Rect, FALSE);
}
//-----------------------------------------------------------------------------
void AsPlatform::Draw(HDC hdc, RECT &paint_area, AsEngine *engine)
{
   int x = X_Pos, y = Y_Pos;
   RECT intersection_rect;
   if (! IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))
      return;
   // Redraw previous platform position with GB color
   SelectObject(hdc, engine->BG_Pen);
   SelectObject(hdc, engine->BG_Brush);
   Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top,
      Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);

   // Draw circles
   SelectObject(hdc, Platform_Circle_Pen);
   SelectObject(hdc, Platform_Circle_Brush);
   Ellipse(hdc,
      x * AsEngine::Global_Scale, y * AsEngine::Global_Scale,
      (x + Circle_Diameter) * AsEngine::Global_Scale,
      (y + Circle_Diameter) * AsEngine::Global_Scale);
   Ellipse(hdc,
      (x + Inner_Width) * AsEngine::Global_Scale, y * AsEngine::Global_Scale,
      (x + Inner_Width + Circle_Diameter) * AsEngine::Global_Scale,
      (y + Circle_Diameter) * AsEngine::Global_Scale);

   // Draw middle part
   SelectObject(hdc, Platform_Inner_Pen);
   SelectObject(hdc, Platform_Inner_Brush);
   RoundRect(hdc, (x + 4) * AsEngine::Global_Scale,
      (y + 1) * AsEngine::Global_Scale,
      (x + 4 + Inner_Width - 1) * AsEngine::Global_Scale,
      (y + 1 + 5) * AsEngine::Global_Scale,
      3 * AsEngine::Global_Scale, 3 * AsEngine::Global_Scale);

   // 3. Draw highlight
   SelectObject(hdc, Highlight_Pen);
   Arc(hdc, (x + 1) * AsEngine::Global_Scale, (y + 1) * AsEngine::Global_Scale,
      (x + Circle_Diameter - 1) * AsEngine::Global_Scale,
      (y + Circle_Diameter - 1) * AsEngine::Global_Scale,
      (x + 1 + 1) * AsEngine::Global_Scale, (y + 1) * AsEngine::Global_Scale,
      (x + 1) * AsEngine::Global_Scale, (y + 1 + 2) * AsEngine::Global_Scale);
}
//-----------------------------------------------------------------------------



// AsEngine
//-----------------------------------------------------------------------------
AsEngine::AsEngine()
   : Hwnd(0), BG_Pen(0), BG_Brush(0)
{
}
//-----------------------------------------------------------------------------
void AsEngine::Init_Engine(HWND hwnd)
{// Setting up the game before start
   Hwnd = hwnd;

   AsConfig::Create_Pen_Brush(BG_Pen, BG_Brush, 15, 63, 31);

   Level.Init();
   Ball.Init();
   Platform.Init();
   Border.Init();

   Platform.Redraw(this);
   Ball.Redraw(this);

   SetTimer(hwnd, Timer_ID , 25, nullptr);
}
//-----------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{// Draw game frame

   Level.Draw_Level(hdc, paint_area);

   Platform.Draw(hdc, paint_area, this);

   //int i;
   //for (i = 0; i < 16; i++)
   //{
   //   Rotate_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale,
   //                                                100, EBT_Blue, ELT_O, i);
   //   Rotate_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale,
   //                                                130, EBT_Red, ELT_O, i);
   //}

   Ball.Draw(hdc, paint_area, this);

   Border.Draw(hdc, paint_area, BG_Pen, BG_Brush);
}
//-----------------------------------------------------------------------------
int AsEngine::On_Key_Down(EKey_Type key_type)
{
   switch (key_type)
   {
   case EKT_Left:
      Platform.X_Pos -= Platform.X_Step;
      if (Platform.X_Pos < AsBorder::Border_X_Offset)
         Platform.X_Pos = AsBorder::Border_X_Offset;
      Platform.Redraw(this);
      break;

   case EKT_Right:
      Platform.X_Pos += Platform.X_Step;
      if (Platform.X_Pos >= Max_X_Pos - Platform.Width + 1)
         Platform.X_Pos = Max_X_Pos - Platform.Width + 1;
      Platform.Redraw(this);
      break;

   case EKT_Space:
      break;
   default:
      break;
   }
   return 0;
}
//-----------------------------------------------------------------------------
int AsEngine::On_Timer()
{
   Ball.Move(this, &Level, &Platform);

   return 0;
}
//-----------------------------------------------------------------------------
