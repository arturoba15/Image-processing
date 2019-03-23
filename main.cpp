#include <iostream>
#include "imagen.h"

using namespace std;

int main()
{
  Imagen etud = Imagen("etud.ppm");
  Imagen etud_bn = etud.bn();
  etud_bn.extension(10, 220);

  return 0;
}
