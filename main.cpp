#include <iostream>
#include "pixel.h"
#include "imagen.h"

using namespace std;

int main()
{
  Imagen uno;

  uno.convert_BN("etud.ppm", "etud_bn.ppm");
  uno.histograma("etud_bn.ppm");

  return 0;
}
