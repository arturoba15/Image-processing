#include <iostream>
#include "imagen.h"

using namespace std;

int main()
{
  Imagen circulos = Imagen("circulos.ppm");
  int obj = circulos.etiquetaObjetos();
  printf("%d objetos\n", obj);
}
