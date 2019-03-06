#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

using namespace std;

class Imagen {
  private:
    int ancho;
    int alto;
    int bpp;
    string dir;
  public:
    Imagen(string dir);
    ~Imagen();
    Imagen convert_BN(string out);
    Imagen binarizar(string out);
    Imagen gamma(string out, float L);
    void histograma(string out);
};

Imagen::Imagen(string dir) {
  ancho = 0;
  alto = 0;
  bpp = 0;
  this->dir = dir;
};

Imagen::~Imagen(){}

Imagen Imagen::convert_BN(string out) {
  string linea;
  string cabecera;
  char coment;

  ifstream imagen(this->dir);
  ofstream imagen2;
  imagen2.open(out);

  getline(imagen, cabecera);

  if(cabecera == "P3")
  {
    cout << "Imagen en formato de: 25 bits" << endl;
    imagen2 << "P3" << endl;
    coment = imagen.get();
  }
  if(coment == '#')
  {
    cout << "Comentario: ";
    getline(imagen, linea);
    cout << linea << endl;
    imagen2 << "# " << linea << endl;
    getline(imagen, linea);
    imagen2 << linea << endl;
    cout << "La imagen mide: " << linea << endl;
  }
  int n = stoi(linea.c_str());
  getline(imagen, linea);
  int m = stoi(linea.c_str());
  imagen2 << 255 << endl;
  cout << "Profundidad de bits: " << m << endl;

  int r = 0;
  int g = 0;
  int b = 0;
  while(getline(imagen, linea)) {
    r = stoi(linea.c_str());
    getline(imagen, linea);
    g = stoi(linea.c_str());
    getline(imagen, linea);
    b = stoi(linea.c_str());
    int avg = (int)(r + g + b)/3;
    imagen2 << avg << endl;
    imagen2 << avg << endl;
    imagen2 << avg << endl;
  }
  imagen.close();
  imagen2.close();

  return Imagen(out);
};

Imagen Imagen::binarizar(string out) {
  string linea;
  string cabecera;
  char coment;

  ifstream imagen(this->dir);
  ofstream imagen2;
  imagen2.open(out);

  getline(imagen, cabecera);

  if(cabecera == "P3")
  {
    cout << "Imagen en formato de: 25 bits" << endl;
    imagen2 << "P3" << endl;
    coment = imagen.get();
  }
  if(coment == '#')
  {
    cout << "Comentario: ";
    getline(imagen, linea);
    cout << linea << endl;
    getline(imagen, linea);
    imagen2 << linea << endl;
    cout << "La imagen mide: " << linea << endl;
  }
  getline(imagen, linea);
  int m = stoi(linea.c_str());
  imagen2 << 255 << endl;
  cout << "Profundidad de bits: " << m << endl;

  while(getline(imagen, linea)) {
    stringstream m(linea);
    int x = 0;
    m >> x;
    if(x > 128) {
      x = 255;
    } else {
      x = 0;
    }
    imagen2 << x << endl;
  }

  imagen.close();
  imagen2.close();

  return Imagen(out);
};

void Imagen::histograma(string out) {
  string linea, cabecera, systemstr;
  char coment;
  ifstream imagen(this->dir);

  ofstream histData;
  histData.open(out);

  getline(imagen, cabecera);

  if(cabecera == "P3")
  {
    coment = imagen.get();
  }
  if(coment == '#')
  {
    getline(imagen, linea);
    getline(imagen, linea);
  }
  getline(imagen, linea);

  int count[256] = {0};
  int r;

  while(getline(imagen, linea)) {
    getline(imagen, linea);
    getline(imagen, linea);
    r = stoi(linea.c_str());
    count[r]++;
  }
  imagen.close();

  // Generate file to feed gnuplot
  for (int k = 0; k < 256; k++) {
    histData << k << " " << count[k] << endl;
  }

  imagen.close();
  histData.close();

  system(("gnuplot -p -e \"set boxwidth 1; set style fill solid; set xrange[0:255]; plot '" + out + "' using 1:2 with boxes;\"").c_str());
  // for(int i = 0; i < 256; i++) {
  //   printf("%d: %d veces\n", i, count[i]);
  // };

};

Imagen Imagen::gamma(string out, float L)
{
  string linea;
  string cabecera;
  char coment;

  ifstream imagen(this->dir);
  ofstream imagen2;
  imagen2.open(out);

  // Obtener cabeceras
  getline(imagen, cabecera);

  if(cabecera == "P3")
  {
    imagen2 << "P3" << endl;
    coment = imagen.get();
  }
  if(coment == '#')
  {
    getline(imagen, linea);
    imagen2 << "#" << linea << endl;
    getline(imagen, linea);
    imagen2 << linea << endl;
  }
  int n = stoi(linea.c_str());
  getline(imagen, linea);
  int m = stoi(linea.c_str());
  imagen2 << 255 << endl;

  int p;
  int avg;
  // float ff;
  // comenzar a hacer la correción
  while(getline(imagen, linea)) {
    getline(imagen, linea);
    getline(imagen, linea);
    p = stoi(linea.c_str());
    // ff = (double)p/(double)255;
    // printf("%f\n", ff);
    avg = (int)round(255 * pow(((double)p/(double)255), ((double)1/(double)L)));
    imagen2 << avg << endl;
    imagen2 << avg << endl;
    imagen2 << avg << endl;
  }

  imagen.close();
  imagen2.close();

  return Imagen(out);

}
