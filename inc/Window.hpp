#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include "Bitmap.hpp"

class Window
{
private:
  int width;
  int height;
  const int backgroundColor;
public:
  static bool doQuit;
  Window(int, int, const int);
  ~Window();
  void SetTitle(const char[]);
  Bitmap* GetBitmap();
  static void CloseButtonHandler();
  static int KeyboardCallback(int);
};

#endif