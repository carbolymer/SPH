#include <allegro.h>
#include <iostream>
#include <unistd.h>
#include "Window.hpp"
#include "WindowClosedException.hpp"

int main() {
  Window* window = new Window(500,500, makecol(255, 255, 255));

  window->SetTitle("SPH");
  Bitmap *bitmap = window->GetBitmap();
  bitmap->RectFill(makecol(100,200,0),5,5,10,10);
  bitmap->Draw();
  delete bitmap;

  while(true) {
    usleep(1e5);
    if(Window::doQuit)
      break;
  }

  return 0;
}
END_OF_MAIN ()