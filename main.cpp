#include <iostream>
#include "pixel.h"
#include "imagen.h"

using namespace std;

int main()
{
  Imagen uno = Imagen("etud.ppm");
  Imagen bn = uno.convert_BN("etud_bn.ppm");
  bn.histograma("histData.dat");

  // Crear imágenes con diferentes valores
  // bn.gamma("gamma0.1.ppm", 0.1);
  // bn.gamma("gamma0.2.ppm", 0.2);
  // bn.gamma("gamma0.4.ppm", 0.4);
  // bn.gamma("gamma1.ppm", 1);
  // bn.gamma("gamma2.5.ppm", 2.5);
  // bn.gamma("gamma5.ppm", 5);
  // bn.gamma("gamma10.ppm", 10);

  return 0;
}
