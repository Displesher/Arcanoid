#include "Engine.h"

char Level_01[AsEngine::Level_Height][AsEngine::Level_Width] =
{
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
//-----------------------------------------------------------------------------
ABall::ABall()
   : Ball_X_Pos(20), Ball_Y_Pos(170),
     Ball_Speed(3.0),
     Ball_X_Direction(M_PI_4 / 2), Ball_Y_Direction(M_PI_4 / 2)
{
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
void ABall::Move(AsEngine *engine)
{
   int next_x_pos, next_y_pos;
   int max_x_pos = AsEngine::Max_X_Pos - Ball_Size;
   int max_y_pos = AsEngine::Max_Y_Pos - Ball_Size;
   int platform_y_pos = AsEngine::Platform_Y_Pos - Ball_Size;

   next_x_pos = Ball_X_Pos + (int)(Ball_Speed * cos(Ball_X_Direction));
   next_y_pos = Ball_Y_Pos - (int)(Ball_Speed * sin(Ball_Y_Direction));

   // Reflection from the horizontal border
   if (next_x_pos < AsEngine::Border_X_Offset || next_x_pos > max_x_pos)
   {
      Ball_X_Direction += M_PI;
      next_x_pos = Ball_X_Pos + (int)(Ball_Speed * cos(Ball_X_Direction));
   }
   // Reflection from the vertical border
   if (next_y_pos < AsEngine::Border_Y_Offset || next_y_pos > max_y_pos ||
      // Reflection from the platform
      (next_y_pos > platform_y_pos &&
         next_x_pos >= engine->Platform_X_Pos &&
         next_x_pos <= (engine->Platform_X_Pos + engine->Platform_Width)))
   {
      Ball_Y_Direction -= M_PI;
      next_y_pos = Ball_Y_Pos - (int)(Ball_Speed * sin(Ball_Y_Direction));
   }
   // Reflection from the bricks
   engine->Check_Level_Brick_Hit(next_y_pos);

   Ball_X_Pos = next_x_pos;
   Ball_Y_Pos = next_y_pos;

   Redraw(engine);
}
//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------
AsEngine::AsEngine()
   : Inner_Width(21), Platform_X_Pos(Max_X_Pos / 2),
   Platform_X_Step(Global_Scale * 2),
   Platform_Width(21 + Circle_Diameter)
{
}
//-----------------------------------------------------------------------------
void AsEngine::Create_Pen_Brush(HPEN &pen, HBRUSH &brush,
                        unsigned char r, unsigned char g, unsigned char b)
{
   pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
   brush = CreateSolidBrush(RGB(r, g, b));
}
//-----------------------------------------------------------------------------
void AsEngine::Redraw_Platform()
{
   Prev_Platform_Rect = Platform_Rect;

   Platform_Rect.left = Platform_X_Pos * Global_Scale;
   Platform_Rect.top = Platform_Y_Pos * Global_Scale;
   Platform_Rect.right = (Platform_X_Pos + Platform_Width) * Global_Scale;
   Platform_Rect.bottom = (Platform_Y_Pos + Platform_Height) * Global_Scale;

   InvalidateRect(Hwnd, &Prev_Platform_Rect, FALSE);
   InvalidateRect(Hwnd, &Platform_Rect, FALSE);
}
//-----------------------------------------------------------------------------
void AsEngine::Init_Engine(HWND hwnd)
{// Setting up the game before start
   Hwnd = hwnd;

   Create_Pen_Brush(BG_Pen, BG_Brush, 15, 63, 31);
   Create_Pen_Brush(Brick_Red_Pen, Brick_Red_Brush, 185, 45, 50);
   Create_Pen_Brush(Brick_Blue_Pen, Brick_Blue_Brush, 45, 140, 180);
   Create_Pen_Brush(Platform_Circle_Pen, Platform_Circle_Brush, 170, 120, 80);
   Create_Pen_Brush(Ball.Ball_Pen, Ball.Ball_Brush, 255, 255, 255);
   Create_Pen_Brush(Border_Blue_Pen, Border_Blue_Brush, 45, 140, 180);
   Create_Pen_Brush(Border_White_Pen, Border_White_Brush, 255, 255, 255);

   Create_Pen_Brush(Platform_Inner_Pen, Platform_Inner_Brush, 200, 190, 170);

   Highlight_Pen = CreatePen(PS_SOLID, 0, RGB(255, 245, 230));
   Letter_Pen = CreatePen(PS_SOLID, 3, RGB(255, 245, 230));

   Level_Rect.left = Level_X_Offset * Global_Scale;
   Level_Rect.top = Level_Y_Offset * Global_Scale;
   Level_Rect.right = Level_Rect.left + Cell_Width * Level_Width * Global_Scale;
   Level_Rect.bottom = Level_Rect.top + Cell_Width * Level_Height * Global_Scale;

   Redraw_Platform();
   Ball.Redraw(this);

   SetTimer(hwnd, Timer_ID , 25, nullptr);
}
//-----------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{// Draw game frame

   RECT intersection_rect;

   if (IntersectRect(&intersection_rect, &paint_area, &Level_Rect))
      Draw_Level(hdc);

   if (IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))
      Draw_Platform(hdc, Platform_X_Pos, Platform_Y_Pos);

   //int i;
   //for (i = 0; i < 16; i++)
   //{
   //   Rotate_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale,
   //                                                100, EBT_Blue, ELT_O, i);
   //   Rotate_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale,
   //                                                130, EBT_Red, ELT_O, i);
   //}

   Ball.Draw(hdc, paint_area, this);

   Draw_Bounds(hdc, paint_area);
}
//-----------------------------------------------------------------------------
int AsEngine::On_Key_Down(EKey_Type key_type)
{
   switch (key_type)
   {
   case EKT_Left:
      Platform_X_Pos -= Platform_X_Step;
      if (Platform_X_Pos < Border_X_Offset)
         Platform_X_Pos = Border_X_Offset;
      Redraw_Platform();
      break;

   case EKT_Right:
      Platform_X_Pos += Platform_X_Step;
      if (Platform_X_Pos >= Max_X_Pos - Platform_Width + 1)
         Platform_X_Pos = Max_X_Pos - Platform_Width + 1;
      Redraw_Platform();
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
   Ball.Move(this);

   return 0;
}
//-----------------------------------------------------------------------------
void AsEngine::Check_Level_Brick_Hit(int &next_y_pos)
{// Reflection from the bricks
   int i, j;
   int brick_y_pos = Level_Y_Offset + Level_Height * Cell_Height;

   for (i = Level_Height - 1; i >= 0; i--)
   {
      for (j = 0; j < Level_Width; j++)
      {
         if(Level_01[i][j] == 0)
            continue;
         if (next_y_pos < brick_y_pos)
         {
            Ball.Ball_Y_Direction -= M_PI;
            next_y_pos = Ball.Ball_Y_Pos -
               (int)(Ball.Ball_Speed * sin(Ball.Ball_Y_Direction));
         }
      }
      brick_y_pos -= Cell_Height;
   }
}
//-----------------------------------------------------------------------------
void AsEngine::Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)
{

   HPEN pen;
   HBRUSH brush;

   switch (brick_type)
   {
   case EBT_None:
      return;
   case EBT_Red:
      pen = Brick_Red_Pen;
      brush = Brick_Red_Brush;
      break;
   case EBT_Blue:
      pen = Brick_Blue_Pen;
      brush = Brick_Blue_Brush;
      break;
   default:
      return;
   }
   //SelectObject(hdc, pen);
   SelectObject(hdc, brush);

   RoundRect(
      hdc, x * Global_Scale, y * Global_Scale,
      (x + Brick_Width) * Global_Scale, (y + Brick_Height) * Global_Scale,
      2 * Global_Scale, 2 * Global_Scale);
}
//-----------------------------------------------------------------------------
void AsEngine::Set_Brick_Letter_Color(bool is_switch_color,
   HPEN &front_pen, HPEN &back_pen, HBRUSH &front_brush, HBRUSH &back_brush)
{
   if (is_switch_color)
   {// Front is red, back is blue
      front_pen = Brick_Red_Pen;
      front_brush = Brick_Red_Brush;
      back_pen = Brick_Blue_Pen;
      back_brush = Brick_Blue_Brush;
   }
   else
   {// Front is blue, back is red
      front_pen = Brick_Blue_Pen;
      front_brush = Brick_Blue_Brush;
      back_pen = Brick_Red_Pen;
      back_brush = Brick_Red_Brush;
   }
}
//-----------------------------------------------------------------------------
void AsEngine::Rotate_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type,
                                    ELetter_Type letter_type, int rotation_step)
{// Draw fallen letter

   bool switch_color;
   double rotation_angle;
   double offset;
   // Converting a step to a rotation angle
   int brick_half_height = Brick_Height * Global_Scale / 2;
   // int brick_half_height = Brick_Height / 2 * Global_Scale;
   int back_part_offset;
   HPEN front_pen, back_pen;
   HBRUSH front_brush, back_brush;
   XFORM xform, old_xform;


   if (!(brick_type == EBT_Blue || brick_type == EBT_Red))
      return; // falling letter may be only from such of these bricks

   if (rotation_step > 4 && rotation_step <= 12)
      switch_color = brick_type == EBT_Blue;
   else
      switch_color = brick_type == EBT_Red;
   Set_Brick_Letter_Color(switch_color, front_pen, back_pen,
                                          front_brush, back_brush);

   // Correct rotation step and rotation angle
   rotation_step %= 16;
   if (rotation_step < 8)
      rotation_angle = 2.0 * M_PI / 16.0 * (double)rotation_step;
   else
      rotation_angle = 2.0 * M_PI / 16.0 * (double)(8 - rotation_step);
   
   if (rotation_step == 4 || rotation_step == 12)
   {
      // Draw background
      SelectObject(hdc, back_pen);
      SelectObject(hdc, back_brush);

      Rectangle(hdc, x, y + brick_half_height - Global_Scale,
            x + Brick_Width * Global_Scale, 
            y + brick_half_height);

      // Draw foreground
      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);

      Rectangle(hdc, x, y + brick_half_height,
            x + Brick_Width * Global_Scale,
            y + brick_half_height + Global_Scale - 1);
   }
   else
   {
      SetGraphicsMode(hdc, GM_ADVANCED);
      // Config rotate matrix of letter
      xform.eM11 = 1.0f;
      xform.eM12 = 0.0f;
      xform.eM21 = 0.0f;
      xform.eM22 = (float)cos(rotation_angle);
      xform.eDx  = (float)x;
      xform.eDy  = (float)y + (float)brick_half_height;
      GetWorldTransform(hdc, &old_xform);
      SetWorldTransform(hdc, &xform);

      // Draw background
      SelectObject(hdc, back_pen);
      SelectObject(hdc, back_brush);

      offset = 3.0 * (1.0 - fabs(xform.eM22)) * (double)Global_Scale;
      back_part_offset = (int)round(offset);
      Rectangle(hdc, 0, -brick_half_height - back_part_offset,
         Brick_Width * Global_Scale, brick_half_height - back_part_offset);
      
      // Draw foreground
      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);
      
      Rectangle(hdc, 0, -brick_half_height,
         Brick_Width * Global_Scale, brick_half_height);

      if (rotation_step > 4 && rotation_step <= 12)
      {
         if (letter_type == ELT_O)
         {
            SelectObject(hdc, Letter_Pen);
            Ellipse(hdc, (0 + 5) * Global_Scale, (-5 * Global_Scale) / 2,
               (0 + 10) * Global_Scale, 5 * Global_Scale / 2);
         }
      }
      
      SetWorldTransform(hdc, &old_xform); 
   }
}
//-----------------------------------------------------------------------------
void AsEngine::Draw_Level(HDC hdc)
{// Draw level's bricks
   int i, j;

   for (i = 0; i < Level_Height; i++)
      for (j = 0; j < Level_Width; j++)
         Draw_Brick(hdc, Level_X_Offset + j * Cell_Width,
            Level_Y_Offset + i * Cell_Height, (EBrick_Type)Level_01[i][j]);
}
//-----------------------------------------------------------------------------
void AsEngine::Draw_Platform(HDC hdc, int x, int y)
{
   // Redraw previous platform position with GB color
   SelectObject(hdc, BG_Pen);
   SelectObject(hdc, BG_Brush);
   Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top,
      Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);

   // Draw circles
   SelectObject(hdc, Platform_Circle_Pen);
   SelectObject(hdc, Platform_Circle_Brush);
   Ellipse(hdc, x * Global_Scale, y * Global_Scale,
      (x + Circle_Diameter) * Global_Scale,
      (y + Circle_Diameter) * Global_Scale);
   Ellipse(hdc, (x + Inner_Width) * Global_Scale, y * Global_Scale,
      (x + Inner_Width + Circle_Diameter) * Global_Scale,
      (y + Circle_Diameter) * Global_Scale);
   
   // Draw middle part
   SelectObject(hdc, Platform_Inner_Pen);
   SelectObject(hdc, Platform_Inner_Brush);
   RoundRect(hdc, (x + 4) * Global_Scale, (y + 1) * Global_Scale,
      (x + 4 + Inner_Width - 1) * Global_Scale, (y + 1 + 5) * Global_Scale,
      3 * Global_Scale, 3 * Global_Scale);

   // 3. Draw highlight
   SelectObject(hdc, Highlight_Pen );
   Arc(hdc, (x + 1) * Global_Scale, (y + 1) * Global_Scale,
      (x + Circle_Diameter - 1) * Global_Scale,
      (y + Circle_Diameter - 1) * Global_Scale,
      (x + 1 + 1) * Global_Scale, (y + 1) * Global_Scale,
      (x + 1) * Global_Scale, (y + 1 + 2) * Global_Scale);
}
//-----------------------------------------------------------------------------
void AsEngine::Draw_Border(HDC hdc, int x, int y, bool top_border)
{ // Draw level border element
   // 1. Draw main line
   SelectObject(hdc, Border_Blue_Pen);
   SelectObject(hdc, Border_Blue_Brush);
   if (top_border)
      Rectangle(hdc, x * Global_Scale, (y + 1) * Global_Scale,
                    (x + 4) * Global_Scale, (y + 4) * Global_Scale);
   else
      Rectangle(hdc, (x + 1) * Global_Scale, y * Global_Scale,
                     (x + 4) * Global_Scale, (y + 4) * Global_Scale);

   // 2. Draw border line
   SelectObject(hdc, Border_White_Pen);
   SelectObject(hdc, Border_White_Brush);
   if (top_border)
      Rectangle(hdc, x * Global_Scale, y * Global_Scale,
                    (x + 4) * Global_Scale, (y + 1) * Global_Scale);
   else
      Rectangle(hdc, x * Global_Scale, y * Global_Scale,
                    (x + 1) * Global_Scale, (y + 4) * Global_Scale);

   // 3. Draw perforation
   SelectObject(hdc, BG_Pen);
   SelectObject(hdc, BG_Brush);
   if (top_border)
      Rectangle(hdc, (x + 2) * Global_Scale, (y + 2) * Global_Scale,
                     (x +3) * Global_Scale, (y + 3) * Global_Scale);
   else
      Rectangle(hdc, (x + 2) * Global_Scale, (y + 1) * Global_Scale,
                     (x +3) * Global_Scale, (y + 2) * Global_Scale);
}
//-----------------------------------------------------------------------------
void AsEngine::Draw_Bounds(HDC hdc, RECT &paint_area)
{ // Draw level border

   int i;

   // 1. Left line
   for (i = 0; i < 50; i++)
      Draw_Border(hdc, 2, 1 + i * 4, false);
   // 2. Right line
   for (i = 0; i < 50; i++)
      Draw_Border(hdc, 201, 1 + i * 4, false);
   // 3. Top line
   for (i = 0; i < 50; i++)
      Draw_Border(hdc, 3 + i * 4, 0, true);
}
//-----------------------------------------------------------------------------
