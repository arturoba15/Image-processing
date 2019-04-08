#include <iostream>
#include "imagen.h"

using namespace std;

int main()
{
  // Imagen figs = Imagen("circulos.ppm");
  // int obj = figs.etiquetaObjetos();
  // printf("%d objetos\n", obj);
  Imagen etud = Imagen("bnetud.ppm");
  etud.mGeometrica();

}
