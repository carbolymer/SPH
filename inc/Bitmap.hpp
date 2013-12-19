#ifndef _BITMAP_HPP_
#define _BITMAP_HPP_
#include <allegro.h>

class Bitmap 
{
private:
  BITMAP *bitmap;
  int width;
  int height;
public:
  Bitmap(int, int);
  ~Bitmap();
  void RectFill(const int color, int x, int y, int width, int height);
};

#endif