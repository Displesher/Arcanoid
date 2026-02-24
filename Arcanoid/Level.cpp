#include "Level.h"

char ALevel::Level_01[AsConfig::Level_Height][AsConfig::Level_Width] =
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

// ALevel
//-----------------------------------------------------------------------------
ALevel::ALevel()
   : Active_Brick(EBT_Red), Brick_Red_Pen(0), Brick_Blue_Pen(0), Letter_Pen(0),
   Brick_Red_Brush(0), Brick_Blue_Brush(0),
   Level_Rect{}
{
}
//-----------------------------------------------------------------------------
void ALevel::Init()
{
   Letter_Pen = CreatePen(PS_SOLID, 3, RGB(255, 245, 230));

   AsConfig::Create_Pen_Brush(Brick_Red_Pen, Brick_Red_Brush,
                                          AsConfig::Red_Brick_Color);
   AsConfig::Create_Pen_Brush(Brick_Blue_Pen, Brick_Blue_Brush,
                                          AsConfig::Blue_Brick_Color);

   Level_Rect.left = AsConfig::Level_X_Offset * AsConfig::Global_Scale;
   Level_Rect.top = AsConfig::Level_Y_Offset * AsConfig::Global_Scale;
   Level_Rect.right = Level_Rect.left + AsConfig::Cell_Width *
      AsConfig::Level_Width * AsConfig::Global_Scale;
   Level_Rect.bottom = Level_Rect.top + AsConfig::Cell_Width *
      AsConfig::Level_Height * AsConfig::Global_Scale;
}
//-----------------------------------------------------------------------------
void ALevel::Draw(HDC hdc, RECT &paint_area)
{// Draw level's bricks
   int i, j;
   RECT intersection_rect;

   if (! IntersectRect(&intersection_rect, &paint_area, &Level_Rect))
      return;

   for (i = 0; i < AsConfig::Level_Height; i++)
      for (j = 0; j < AsConfig::Level_Width; j++)
         Draw_Brick(hdc, AsConfig::Level_X_Offset + j * AsConfig::Cell_Width,
                         AsConfig::Level_Y_Offset + i * AsConfig::Cell_Height,
                                                 (EBrick_Type)Level_01[i][j]);

   Active_Brick.Draw(hdc);
}
//-----------------------------------------------------------------------------
void ALevel::Check_Level_Brick_Hit(int &next_y_pos, double &ball_y_direction,
                                                            double ball_speed)
{// Reflection from the bricks
   int i, j;
   int brick_y_pos = AsConfig::Level_Y_Offset +
      AsConfig::Level_Height * AsConfig::Cell_Height;

   for (i = AsConfig::Level_Height - 1; i >= 0; i--)
   {
      for (j = 0; j < AsConfig::Level_Width; j++)
      {
         if(Level_01[i][j] == 0)
            continue;
         if (next_y_pos < brick_y_pos)
         {
            ball_y_direction -= M_PI;
            next_y_pos -= (int)(ball_speed * sin(ball_y_direction));
         }
      }
      brick_y_pos -= AsConfig::Cell_Height;
   }
}
//-----------------------------------------------------------------------------
void ALevel::Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)
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
      hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale,
      (x + AsConfig::Brick_Width) * AsConfig::Global_Scale,
      (y + AsConfig::Brick_Height) * AsConfig::Global_Scale,
      2 * AsConfig::Global_Scale, 2 * AsConfig::Global_Scale);
}
//-----------------------------------------------------------------------------
void ALevel::Set_Brick_Letter_Color(bool is_switch_color,
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
void ALevel::Rotate_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type,
   ELetter_Type letter_type, int rotation_step)
{// Draw fallen letter

   bool switch_color;
   double rotation_angle;
   double offset;
   // Converting a step to a rotation angle
   int brick_half_height = AsConfig::Brick_Height * AsConfig::Global_Scale / 2;
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

      Rectangle(hdc, x, y + brick_half_height - AsConfig::Global_Scale,
         x + AsConfig::Brick_Width * AsConfig::Global_Scale,
         y + brick_half_height);

      // Draw foreground
      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);

      Rectangle(hdc, x, y + brick_half_height,
         x + AsConfig::Brick_Width * AsConfig::Global_Scale,
         y + brick_half_height + AsConfig::Global_Scale - 1);
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

      offset = 3.0 * (1.0 - fabs(xform.eM22)) * (double)AsConfig::Global_Scale;
      back_part_offset = (int)round(offset);
      Rectangle(hdc, 0, -brick_half_height - back_part_offset,
         AsConfig::Brick_Width * AsConfig::Global_Scale,
         brick_half_height - back_part_offset);

      // Draw foreground
      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);

      Rectangle(hdc, 0, -brick_half_height,
         AsConfig::Brick_Width * AsConfig::Global_Scale, brick_half_height);

      if (rotation_step > 4 && rotation_step <= 12)
      {
         if (letter_type == ELT_O)
         {
            SelectObject(hdc, Letter_Pen);
            Ellipse(hdc, (0 + 5) * AsConfig::Global_Scale,
               (-5 * AsConfig::Global_Scale) / 2,
               (0 + 10) * AsConfig::Global_Scale,
               5 * AsConfig::Global_Scale / 2);
         }
      }

      SetWorldTransform(hdc, &old_xform); 
   }
}
//-----------------------------------------------------------------------------
