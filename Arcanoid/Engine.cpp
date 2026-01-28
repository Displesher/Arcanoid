#include "Engine.h"

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

   Platform.Redraw(hwnd);
   Ball.Redraw(hwnd);

   SetTimer(hwnd, Timer_ID , 1000 / AsConfig::FPS, nullptr);
}
//-----------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{// Draw game frame

   Level.Draw(hdc, paint_area);

   Platform.Draw(hdc, paint_area, BG_Pen, BG_Brush);

   //int i;
   //for (i = 0; i < 16; i++)
   //{
   //   Rotate_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale,
   //                                                100, EBT_Blue, ELT_O, i);
   //   Rotate_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale,
   //                                                130, EBT_Red, ELT_O, i);
   //}

   Ball.Draw(hdc, paint_area, BG_Pen, BG_Brush);

   Border.Draw(hdc, paint_area, BG_Pen, BG_Brush);
}
//-----------------------------------------------------------------------------
int AsEngine::On_Key_Down(EKey_Type key_type)
{
   switch (key_type)
   {
   case EKT_Left:
      Platform.X_Pos -= Platform.X_Step;
      if (Platform.X_Pos < AsConfig::Border_X_Offset)
         Platform.X_Pos = AsConfig::Border_X_Offset;
      Platform.Redraw(Hwnd);
      break;

   case EKT_Right:
      Platform.X_Pos += Platform.X_Step;
      if (Platform.X_Pos >= AsConfig::Max_X_Pos - Platform.Width + 1)
         Platform.X_Pos = AsConfig::Max_X_Pos - Platform.Width + 1;
      Platform.Redraw(Hwnd);
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
   Ball.Move(Hwnd, &Level, Platform.X_Pos, Platform.Width);
   Level.Active_Brick.Fade_Out(Hwnd);

   return 0;
}
//-----------------------------------------------------------------------------
