#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

// Hereda de pixel
class Imagen : pixel
{
  private:
    int ancho;
    int alto;
    int bpp;
    pixel *pix;
  public:
    Imagen();
    ~Imagen();
    void convert_BN(string in, string out);
    void binarizar(string in, string out);
    void histograma(string in);
};

Imagen::Imagen()
{
  ancho = 0;
  alto = 0;
  bpp = 0;
};

Imagen::~Imagen(){}

void Imagen::convert_BN(string in, string out)
{
  string linea;
  string cabecera;
  char coment;

  ifstream imagen(in);
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
};

void Imagen::binarizar(string in, string out)
{
  string linea;
  string cabecera;
  char coment;

  ifstream imagen(in);
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
};

void Imagen::histograma(string in)
{
  string linea;
  string cabecera;
  char coment;
  ifstream imagen(in);

  getline(imagen, cabecera);

  if(cabecera == "P3")
  {
    cout << "Imagen en formato de: 25 bits" << endl;
    coment = imagen.get();
  }
  if(coment == '#')
  {
    cout << "Comentario: ";
    getline(imagen, linea);
    cout << linea << endl;
    getline(imagen, linea);
    cout << "La imagen mide: " << linea << endl;
  }
  getline(imagen, linea);
  int m = stoi(linea.c_str());
  cout << "Profundidad de bits: " << m << endl;

  int count[256] = {0};
  int r;
  while(getline(imagen, linea)) {
    getline(imagen, linea);
    getline(imagen, linea);
    r = stoi(linea.c_str());
    count[r]++;
  }
  imagen.close();

  for(int i = 0; i < 256; i++) {
    printf("%d = %d\n", i, count[i]);
  };
};
