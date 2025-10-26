#include "Engine.h"

enum EBrick_Type
{
   EBT_None,
   EBT_Red,
   EBT_Blue
};

HPEN Brick_Red_Pen, Brick_Blue_Pen;
HPEN Platform_Circle_Pen, Platform_Inner_Pen;
HPEN Highlight_Pen ;

HBRUSH Brick_Red_Brush, Brick_Blue_Brush;
HBRUSH Platform_Circle_Brush, Platform_Inner_Brush;


const int Global_Scale = 3;
const int Brick_Width = 15;
const int Brick_Height = 7;
const int Cell_Width = 16;
const int Cell_Height = 8;
const int Level_X_Offset = 8;
const int Level_Y_Offset = 6;
const int Circle_Diameter = 7;

//const int inner_width = 7;

char Level_01[14][12] =
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
void Create_Pen_Brush(HPEN &pen, HBRUSH &brush, 
                        unsigned char r, unsigned char g, unsigned char b)
{
   pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
   brush = CreateSolidBrush(RGB(r, g, b));
}
//-----------------------------------------------------------------------------
void Init()
{// Setting up the game before start

   Create_Pen_Brush(Brick_Red_Pen, Brick_Red_Brush, 255, 85, 85);
   Create_Pen_Brush(Brick_Blue_Pen, Brick_Blue_Brush, 85, 255, 255);
   Create_Pen_Brush(Platform_Circle_Pen, Platform_Circle_Brush, 151, 0, 0);
   Create_Pen_Brush(Platform_Inner_Pen, Platform_Inner_Brush, 0, 128, 192);
   Highlight_Pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
}
//-----------------------------------------------------------------------------
void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)
{// Draw brick

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
void Draw_Level(HDC hdc)
{// Draw level's bricks
   int i, j;

   for (i = 0; i < 14; i++)
      for (j = 0; j < 12; j++)
         Draw_Brick(hdc, Level_X_Offset + j * Cell_Width, 
            Level_Y_Offset + i * Cell_Height, (EBrick_Type)Level_01[i][j]);
}
//-----------------------------------------------------------------------------
void Draw_Platform(HDC hdc, int x, int y, int inner_width)
{
   // Draw circles
   SelectObject(hdc, Platform_Circle_Pen);
   SelectObject(hdc, Platform_Circle_Brush);
   Ellipse(hdc, x * Global_Scale, y * Global_Scale, 
      (x + Circle_Diameter) * Global_Scale,
      (y + Circle_Diameter) * Global_Scale);
   Ellipse(hdc, (x + inner_width) * Global_Scale, y * Global_Scale,
      (x + inner_width + Circle_Diameter) * Global_Scale, 
      (y + Circle_Diameter) * Global_Scale);
   
   // Draw middle part
   SelectObject(hdc, Platform_Inner_Pen);
   SelectObject(hdc, Platform_Inner_Brush);
   RoundRect(hdc, (x + 4) * Global_Scale, (y + 1) * Global_Scale, 
      (x + 4 + inner_width - 1) * Global_Scale, (y + 1 + 5) * Global_Scale,
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
void Draw_Platform_Alt(HDC hdc, int x, int y, int inner_width)
{
   int inner_circles_radius = 5;

   // 1. Draw circles
   SelectObject(hdc, Platform_Circle_Pen);
   SelectObject(hdc, Platform_Circle_Brush);

   Ellipse(hdc, x * Global_Scale, y * Global_Scale,
            (x + Circle_Diameter) * Global_Scale, 
            (y + Circle_Diameter) * Global_Scale);
   Ellipse(hdc, (x + inner_width) * Global_Scale, y * Global_Scale,
            (x + inner_width + Circle_Diameter) * Global_Scale, 
            (y + Circle_Diameter) * Global_Scale);

   // 2. Draw middle part
   SelectObject(hdc, Platform_Inner_Pen);
   SelectObject(hdc, Platform_Inner_Brush);

   Ellipse(hdc, (x + 4) * Global_Scale, (y + 1) * Global_Scale,
            (x + 4 + inner_circles_radius) * Global_Scale, 
            (y + 1 + inner_circles_radius) * Global_Scale);
   Ellipse(hdc, (x - 2 + inner_width) * Global_Scale, (y + 1) * Global_Scale,
            (x - 2 + inner_width + inner_circles_radius) * Global_Scale, 
            (y + 1 + inner_circles_radius) * Global_Scale);
   Rectangle
   (
      hdc, 
      (x + 3 + Circle_Diameter/2) * Global_Scale, 
      (y + 1) * Global_Scale,
      (x + 3 + inner_width - Circle_Diameter/2 + 1) * Global_Scale, 
      (y + 1 + inner_circles_radius) * Global_Scale
   );

   // 3. Draw highlight
   SelectObject(hdc, Highlight_Pen );
   Arc(hdc, (x + 1) * Global_Scale, (y + 1) * Global_Scale, 
      (x + Circle_Diameter - 1) * Global_Scale, 
      (y + Circle_Diameter - 1) * Global_Scale, 
      (x + 1 + 2) * Global_Scale, (y + 1) * Global_Scale, 
      (x + 1) * Global_Scale, (y + 1 + 3) * Global_Scale);
}
//-----------------------------------------------------------------------------
void Draw_Frame(HDC hdc)
{// Draw game frame

   Draw_Level(hdc);

   Draw_Platform(hdc, 100, 185, 21);
   
   Draw_Platform_Alt(hdc, 90, 175, 42);
   //Draw_Platform_Alt(hdc, 60, 185, 12);
   //Draw_Platform_Alt(hdc, 100, 185, 21);

}
//-----------------------------------------------------------------------------
