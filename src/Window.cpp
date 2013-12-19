#include <allegro.h>
#include <iostream>
#include "Window.hpp"
#include "Bitmap.hpp"

Window::Window(int _width, int _height) : width(_width), height(_height) {
  allegro_init ();
  // install_timer ();
  install_keyboard ();
  install_mouse ();

  set_color_depth (16);
  if (set_gfx_mode (GFX_AUTODETECT_WINDOWED, width, height, 0, 0) != 0)
  {
    std::cerr << "Blad inicjowania 11 graficznego!";
    return;
  }
  show_mouse(screen);
}

Window::~Window() {
  allegro_exit();
}

void Window::DrawBackground(const int color) {
  Bitmap *bitmap = GetBitmap();
  bitmap->RectFill(color, 0, 0, width, height);
  delete bitmap;
}

Bitmap* Window::GetBitmap() {
  return new Bitmap(width, height);
}

void Window::SetTitle(const char title[]) {
  set_window_title(title);
}