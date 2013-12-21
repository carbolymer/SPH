#include <allegro.h>
#include <iostream>
#include "Window.hpp"
#include "Bitmap.hpp"
#include "WindowClosedException.hpp"

bool Window::doQuit = false;

Window::Window(int _width, int _height, const int _color) :
    width(_width),
    height(_height),
    backgroundColor(_color){

  allegro_init ();
  install_timer ();
  LOCK_FUNCTION(Window::CloseButtonHandler);
  LOCK_FUNCTION(Window::KeyboardCallback);
  install_mouse ();
  set_close_button_callback(Window::CloseButtonHandler);
  install_keyboard ();
  keyboard_callback = Window::KeyboardCallback;

  set_color_depth (32);
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

Bitmap* Window::GetBitmap() {
  Bitmap* bitmap = new Bitmap(width, height);
  bitmap->RectFill(backgroundColor, 0, 0, width, height);
  return bitmap;
}

void Window::SetTitle(const char title[]) {
  set_window_title(title);
}

int Window::KeyboardCallback(int key) {
  if((key >> 8) == KEY_ESC)
    doQuit = true;
  std::cout << (key >> 8) << " "  << KEY_ESC << std::endl;
  return key;
}
END_OF_FUNCTION(Window::KeyboardCallback)

void Window::CloseButtonHandler() {
  doQuit = true;
}
END_OF_FUNCTION(Window::CloseButtonHandler)