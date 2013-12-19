#include <allegro.h>
#include "Bitmap.hpp"

Bitmap::Bitmap(int _width, int _height) : width(_width), height(_height) {
  scare_mouse();
  bitmap = create_bitmap (width, height);
  clear(bitmap);
}

Bitmap::~Bitmap() {
  blit(bitmap, screen, 0, 0, 0, 0, width, height);
  destroy_bitmap(bitmap);
  unscare_mouse();
}

void Bitmap::RectFill(const int color, int x, int y, int width, int height) {
  rectfill (bitmap, x, y, width, height, color);
}