#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include "Bitmap.hpp"

class Window
{
private:
  int width;
  int height;
public:
  Window(int, int);
  ~Window();
  void DrawBackground(const int color);
  void SetTitle(const char[]);
  Bitmap* GetBitmap();
};

#endif