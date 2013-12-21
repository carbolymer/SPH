#ifndef _LATTICE_HPP_
#define _LATTICE_HPP_

class Lattice {
public:
  Lattice(int width, int height);
  Bitmap* Draw(int zoom);
};

#endif