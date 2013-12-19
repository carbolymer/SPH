#include <allegro.h>
#include <iostream>
#include "Window.hpp"

int main() {
  int key = 0;
  Window* window = new Window(500,500);

  window->DrawBackground(makecol(255, 255, 255));
  window->SetTitle("SPH");

  while ((key >> 8) != KEY_ESC)
    key = readkey ();
  
  return 0;
}