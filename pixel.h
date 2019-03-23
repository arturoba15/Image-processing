#ifndef PIXEL_H
#define PIXEL_H

using namespace std;

class Pixel
{
  private:
    int r, g, b;
  public:
    Pixel();
    Pixel(int nr, int ng, int nb);
    ~Pixel();
    int getR(), getG(), getB();
    void set(int nr, int ng, int nb);
    void setR(int nr), setG(int ng), setB(int nb);
};

Pixel::Pixel()
{
  r = 0;
  g = 0;
  b = 0;
};

Pixel::Pixel(int nr, int ng, int nb)
{
  r = nr;
  g = ng;
  b = nb;
};

Pixel::~Pixel(){}

int Pixel::getR() {
  return r;
};

int Pixel::getG() {
  return g;
};

int Pixel::getB() {
  return b;
};

void Pixel::set(int nr, int ng, int nb) {
  r = nr;
  g = ng;
  b = nb;
};

void Pixel::setR(int nr) {
  r = nr;
};

void Pixel::setG(int ng) {
  g = ng;
};

void Pixel::setB(int nb) {
  b = nb;
};

#endif
