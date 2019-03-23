#ifndef IMAGEN_H
#define IMAGEN_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include "pixel.h"

using namespace std;

class Imagen {
  private:
    int ancho;
    int alto;
    int bpp;
    string dir;
    void cp_cabeceras(ifstream &imagen, ofstream &imagen2);
    void w_pixel(ofstream &os, Pixel p);
    bool r_pixel(ifstream &is, Pixel& p);
  public:
    Imagen(string dir);
    ~Imagen();
    Imagen bn();
    void histograma();
    Imagen binarizar();
    Imagen gamma(float L);
    Imagen umbral(int umbral);
    Imagen extension(int umbral1, int umbral2);
    Imagen umBinInvertido(int um1, int um2);
};

Imagen::Imagen(string dir) {
  string line;
  this->dir = dir;
  ifstream imagen(this->dir);
  // Aquí se obtiene la cabecera P3 y el comentario
  getline(imagen, line);
  getline(imagen, line);
  // Se leen los atributos de la imagen
  getline(imagen, line, ' ');
  ancho = stoi(line.c_str());
  getline(imagen, line);
  alto = stoi(line.c_str());
  getline(imagen, line);
  bpp = stoi(line.c_str());
  imagen.close();
};

Imagen::~Imagen(){}

// copia la cabecera a una nueva imagen
void Imagen::cp_cabeceras(ifstream &imagen, ofstream &imagen2) {
  string linea;
  getline(imagen, linea);
  imagen2 << "P3" << endl;
  imagen.get();
  getline(imagen, linea);
  imagen2 << "#" << linea << endl;
  getline(imagen, linea);
  imagen2 << linea << endl;
  getline(imagen, linea);
  imagen2 << linea << endl;
};

// Escribe un pixel a la imagen
void Imagen::w_pixel(ofstream &os, Pixel p) {
  os << p.getR() << endl;
  os << p.getG() << endl;
  os << p.getB() << endl;
};

// Lee y regresa un pixel de la imagen en la variable pasada como parametro
bool Imagen::r_pixel(ifstream &is, Pixel& p) {
  string linea;
  getline(is, linea);
  // checamos si se terminó la imagen
  if(!is) {
    return false;
  }
  p.setR(stoi(linea.c_str()));
  getline(is, linea);
  p.setG(stoi(linea.c_str()));
  getline(is, linea);
  p.setB(stoi(linea.c_str()));
  return true;
}

Imagen Imagen::bn() {
  string out = "bn" + this->dir;
  ifstream imagen(this->dir);
  ofstream imagen2(out);

  cp_cabeceras(imagen, imagen2);

  Pixel p;
  int avg;
  while(r_pixel(imagen, p)) {
    avg = (int)((p.getR() + p.getG() + p.getB())/3);
    p.set(avg, avg, avg);
    w_pixel(imagen2, p);
  }
  imagen.close();
  imagen2.close();

  return Imagen(out);
};

Imagen Imagen::binarizar() {
  string out = "binario" + this->dir;
  ifstream imagen(this->dir);
  ofstream imagen2(out);

  cp_cabeceras(imagen, imagen2);

  int x;
  Pixel p;
  while(r_pixel(imagen, p)) {
    x = p.getR();
    if(x > 128)
      p.set(255, 255, 255);
    else
      p.set(0, 0, 0);

    w_pixel(imagen2, p);
  }

  imagen.close();
  imagen2.close();

  return Imagen(out);
};

void Imagen::histograma() {
  string linea;
  string out = this->dir + "hist.data";
  int count[256] = {0};
  ifstream imagen(this->dir);
  ofstream histData(out);

  // Ignoramos las cabeceras
  for (int i = 0; i < 4; i++)
    getline(imagen, linea);

  // Llena el arreglo del histograma
  Pixel p;
  while(r_pixel(imagen, p))
    count[p.getR()]++;

  // Generar archivo para gnuplot
  for (int k = 0; k < 256; k++) {
    histData << k << " " << count[k] << endl;
  }

  imagen.close();
  histData.close();

  system(("gnuplot -p -e \"set boxwidth 1; set style fill solid; set xrange[0:255]; plot '" + out + "' using 1:2 with boxes;\"").c_str());
};

Imagen Imagen::gamma(float L) {
  int avg;
  Pixel p;
  string out = "gamma" + to_string(L) + this->dir;
  ifstream imagen(this->dir);
  ofstream imagen2(out);

  cp_cabeceras(imagen, imagen2);

  // comenzar a hacer la correción
  while(r_pixel(imagen, p)) {
    avg = (int)round(255 * pow(((double)p.getR()/(double)255), ((double)1/(double)L)));
    p.set(avg, avg, avg);
    w_pixel(imagen2, p);
  }

  imagen.close();
  imagen2.close();

  return Imagen(out);
}

Imagen Imagen::umbral(int umbral) {
  Pixel p;
  string out = "umbral" + to_string(umbral) + this->dir;
  ifstream imagen(this->dir);
  ofstream imagen2(out);

  cp_cabeceras(imagen, imagen2);

  while(r_pixel(imagen, p)) {
    if(p.getR() > umbral) {
      p.set(255, 255, 255);
    } else {
      p.set(0, 0, 0);
    }
    w_pixel(imagen2, p);
  }

  imagen.close();
  imagen2.close();

  return Imagen(out);
};

// Umbral binario invertido
Imagen Imagen::umBinInvertido(int um1, int um2) {
  Pixel p;
  string out = "umbininv_" + this->dir;
  ifstream imagen(this->dir);
  ofstream imagen2(out);

  cp_cabeceras(imagen, imagen2);

  while(r_pixel(imagen, p)) {
    if(um1 < p.getR() && p.getR() < um2)
      p.set(255, 255, 255);
    else
      p.set(0, 0, 0);

    w_pixel(imagen2, p);
  }

  imagen.close();
  imagen2.close();

  return Imagen(out);
};

Imagen Imagen::extension(int um1, int um2) {
  Pixel p;
  int x;
  string out = "extension" + to_string(um1) + "-" + to_string(um2) + this->dir;
  ifstream imagen(this->dir);
  ofstream imagen2(out);

  cp_cabeceras(imagen, imagen2);

  while(r_pixel(imagen, p)) {
    if(p.getR() <= um1 || p.getR() >= um2)
      p.set(255, 255, 255);
    else {
      x = (int)(p.getR()-um1)*255/(um2-um1);
      p.set(x, x, x);
    }

    w_pixel(imagen2, p);
  }

  imagen.close();
  imagen2.close();

  return Imagen(out);
};

#endif
