#ifndef IMAGEN_H
#define IMAGEN_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include "pixel.h"
#include "unionfind.h"

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
  void ignora_cabeceras(ifstream &imagen);
  int get_index(int fila, int col, int n_cols);
public:
  Imagen(string dir);
  ~Imagen();
  Imagen bn();
  void histograma();
  Imagen binarizar();
  Imagen gamma(float L);
  Imagen umbral(int umbral);
  Imagen suma(Imagen sum);
  Imagen resta(Imagen res);
  Imagen umBinInvertido(int um1, int um2);
  Imagen extension(int umbral1, int umbral2);
  int etiquetaObjetos();
  Imagen mAritmetica();
  Imagen mGaussiana(double variance);
  Imagen mGeometrica();
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

// ignora cabeceras
void Imagen::ignora_cabeceras(ifstream &imagen) {
  string linea;
  getline(imagen, linea);
  getline(imagen, linea);
  getline(imagen, linea);
  getline(imagen, linea);
}

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

// Obtiene el index de una matriz a partir de la fila y columna
int Imagen::get_index(int fila, int col, int n_cols) {
  return (fila * n_cols) + col;
}

// Convierte la imagen a blanco y negro
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
};

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

Imagen Imagen::suma(Imagen sum) {
  string out = "sum.ppm" + this->dir;
  ifstream imagen(this->dir);
  ifstream imagen2(sum.dir);
  ofstream imagen_out(out);

  cp_cabeceras(imagen, imagen_out);
  ignora_cabeceras(imagen2);

  Pixel p0, p1, pw;
  int r, g, b;
  while(r_pixel(imagen, p0)) {
    r_pixel(imagen2, p1);
    r = p0.getR() + p1.getR();
    g = p0.getG() + p1.getG();
    b = p0.getB() + p1.getB();
    if(r > 255)
      r = 255;
    if(g > 255)
      g = 255;
    if(b > 255)
      b = 255;
    pw.set(r, g, b);
    w_pixel(imagen_out, pw);
  }

  imagen.close();
  imagen2.close();
  imagen_out.close();

  return Imagen(out);
};

Imagen Imagen::resta(Imagen res) {
  string out = "resta" + this->dir;
  ifstream imagen(this->dir);
  ifstream imagen2(res.dir);
  ofstream imagen_out(out);

  cp_cabeceras(imagen, imagen_out);
  ignora_cabeceras(imagen2);

  Pixel p0, p1, pw;
  int r, g, b;
  while(r_pixel(imagen, p0)) {
    r_pixel(imagen2, p1);
    r = p0.getR() - p1.getR();
    g = p0.getG() - p1.getG();
    b = p0.getB() - p1.getB();
    if(r < 0)
      r = 0;
    if(g < 0)
      g = 0;
    if(b < 0)
      b = 0;
    pw.set(r, g, b);
    w_pixel(imagen_out, pw);
  }

  imagen.close();
  imagen2.close();
  imagen_out.close();

  return Imagen(out);
};

int Imagen::etiquetaObjetos() {
  string linea;
  string fout = "etiqueta-" + this->dir;
  ifstream imagen(this->dir);
  ofstream imagen2(fout);

  int pixeles[2 * this->ancho]; // Se tendrán 2 filas de la imagen en memoria
  int wrpixel; // Pixel a escribir en el archivo al terminar las comparaciones
  int tag = 1;
  int colors[3];
  Pixel px;
  UnionFind ufStruct = UnionFind();

  printf("Contando objetos\n");

  cp_cabeceras(imagen, imagen2);

  // Primeras 2 lineas del archivo
  // En la primera linea de la matriz se guardan 0's
  // En la segunda linea guardamos la primera fila de la imagen
  // Dejamos 0 como negro, 1 como blanco
  for(int j = 0; j < this->ancho; j++) {
    r_pixel(imagen, px);
    if(px.getR() == 255)
      pixeles[get_index(1, j, this->ancho)] = 1;
    else
      pixeles[get_index(1, j, this->ancho)] = 0;
    pixeles[get_index(0, j, this->ancho)] = 0;
  }

  // Primera pasada
  // > 1 significa que no es blanco y negro y tampoco es del fondo
  for(int j = 0; j < this->alto; j++) {
    for(int i = 0; i < this->ancho; i++) {
      if(pixeles[get_index(1, i, this->ancho)] == 1) { // Si el actual es blanco, checamos vecinos
        if(i-1 > 0) { // Solo si no esta hasta la izquierda, checamos los 2
          if(pixeles[get_index(1, i-1, this->ancho)] > 1 && pixeles[get_index(0, i, this->ancho)] > 1) {
            if(pixeles[get_index(1, i-1, this->ancho)] < pixeles[get_index(0, i, this->ancho)])
              pixeles[get_index(1, i, this->ancho)] = pixeles[get_index(1, i-1, this->ancho)];
            else
              pixeles[get_index(1, i, this->ancho)] = pixeles[get_index(0, i, this->ancho)];
            wrpixel = pixeles[get_index(1, i, this->ancho)];
            // Si sucede esto, guardamos y decimos que los 2 pertenecen a lo mismo
            ufStruct.merge(pixeles[get_index(1, i-1, this->ancho)], pixeles[get_index(0, i, this->ancho)]);
          } else if(pixeles[get_index(0, i, this->ancho)] > 1) { // arriba
            pixeles[get_index(1, i, this->ancho)] = pixeles[get_index(0, i, this->ancho)];
            wrpixel = pixeles[get_index(1, i, this->ancho)];
          } else if(pixeles[get_index(1, i-1, this->ancho)] > 1) { // izquierdo
            pixeles[get_index(1, i, this->ancho)] = pixeles[get_index(1, i-1, this->ancho)];
            wrpixel = pixeles[get_index(1, i, this->ancho)];
          } else {
            // Creamos nueva etiqueta
            pixeles[get_index(1, i, this->ancho)] = ++tag;
            wrpixel = tag;
            ufStruct.addRoot();
          }
        } else { // Si está hasta la izquierda, checamos arriba
          if(pixeles[get_index(0, i, this->ancho)] > 1) { // arriba
            pixeles[get_index(1, i, this->ancho)] = pixeles[get_index(0, i, this->ancho)];
            wrpixel = pixeles[get_index(1, i, this->ancho)];
          } else {
            // Creamos nueva etiqueta
            pixeles[get_index(1, i, this->ancho)] = ++tag;
            wrpixel = tag;
            ufStruct.addRoot();
          }
        }
      } else { // Si es del fondo, lo dejamos como está
        wrpixel = 0;
      }
      // Escribimos el pixel en el archivo
      if(wrpixel != 0) {
        // for(int i = 0; i < 3; i++) {
          // srand(wrpixel + i);
          // colors[i] = rand() %  256;
        // }
        // w_pixel(imagen2, Pixel(colors));
        w_pixel(imagen2, Pixel(wrpixel, wrpixel, wrpixel));
      } else {
        w_pixel(imagen2, Pixel(0, 0, 0));
      }
    }

    // Movemos la fila inferior de la matriz hacia arriba
    // Leemos la nueva linea en la linea inferior
    for(int p = 0; p < this->ancho; p++) {
      pixeles[get_index(0, p, this->ancho)] = pixeles[get_index(1, p, this->ancho)];
      r_pixel(imagen, px);
      if(px.getR() == 255)
        pixeles[get_index(1, p, this->ancho)] = 1;
      else
        pixeles[get_index(1, p, this->ancho)] = 0;
    }
  }

  imagen.close();
  imagen2.close();

  // Abrimos la imagen que cerramos
  imagen.open(fout);
  imagen2.open("2" + fout);
  cp_cabeceras(imagen, imagen2);

  // Segunda pasada para corregir errores
  int root;
  for(int j = 0; j < this->ancho; j++) { // Llenar matriz de nuevo
    r_pixel(imagen, px);
    pixeles[get_index(1, j, this->ancho)] = px.getR();
    pixeles[get_index(0, j, this->ancho)] = 1;
  }

  for(int j = 0; j < this->alto; j++) {
    for(int i = 0; i < this->ancho; i++) {
      if(pixeles[get_index(1, i, this->ancho)] != 0) { // Checar solo los que no son negros
        // Checar si la etiqueta es raiz
        root = ufStruct.find(pixeles[get_index(1, i, this->ancho)]);
        if(root == pixeles[get_index(1, i, this->ancho)]-2) { // Lo dejamos como está
          wrpixel = pixeles[get_index(1, i, this->ancho)];
        } else { // Le asignamos su raíz
          wrpixel = root + 2;
          pixeles[get_index(1, i, this->ancho)] = wrpixel;
        }
      } else {
        wrpixel = 0;
      }

      // Escribimos el pixel en el archivo
      if(wrpixel != 0) {
        for(int i = 0; i < 3; i++) {
          srand(wrpixel + i);
          colors[i] = rand() %  256;
        }
        w_pixel(imagen2, Pixel(colors));
      } else {
        w_pixel(imagen2, Pixel(0, 0, 0));
      }
    }

    // Movemos la fila inferior de la matriz hacia arriba
    // Leemos la nueva linea en la linea inferior
    for(int p = 0; p < this->ancho; p++) {
      pixeles[get_index(0, p, this->ancho)] = pixeles[get_index(1, p, this->ancho)];
      r_pixel(imagen, px);
      pixeles[get_index(1, p, this->ancho)] = px.getR();
    }
  }

  imagen.close();
  imagen2.close();

  return ufStruct.length;
};

Imagen Imagen::mAritmetica() {
  string out = "arit" + this->dir;
  ifstream imagen(this->dir);
  ofstream imagen2(out);
  // Las 2 columnas, serán 0's para también procesar los bordes la imagen
  const int cols = this->ancho + 2;
  int imagMat[3][cols];
  int mean = 0; int inc = 0;
  Pixel pix;

  cp_cabeceras(imagen, imagen2);

  // Llenar las ultimas 2 filas de la matriz
  // Los 0's de los bordes serán estáticos
  imagMat[0][0] = 0; imagMat[0][this->ancho + 1] = 0;
  imagMat[1][0] = 0; imagMat[1][this->ancho + 1] = 0;
  imagMat[2][0] = 0; imagMat[2][this->ancho + 1] = 0;
  for(int j = 1; j < this->ancho + 1; j++)
    imagMat[1][j] = 0;
  for(int j = 1; j < this->ancho + 1; j++) {
    r_pixel(imagen, pix);
    imagMat[2][j] = pix.getR();
  }

  for(int m = 0; m < this->alto -1 ; m++) {
    // Llenar matriz:
    // Lee 1 linea, recorre las 2 que ya estaban
    for(int p = 1; p < this->ancho + 1; p++) {
      imagMat[0][p] = imagMat[1][p];
      imagMat[1][p] = imagMat[2][p];
      r_pixel(imagen, pix);
      imagMat[2][p] = pix.getR();
    }

    // Procesar cada pixel (columna de la fila[m] actual)
    for(int n = 0; n < this->ancho; n++) {
      // Obtener la media
      // Con inc vamos moviendo la mascara a través de la matriz (a la derecha)
      for(int maskm = 0; maskm < 3; maskm++)
        for(int maskn = inc; maskn < inc + 3; maskn++)
          mean += imagMat[maskm][maskn];
      mean /= 9;
      inc++;

      w_pixel(imagen2, Pixel(mean, mean, mean));
      mean = 0;
    }
    inc = 0;
  }

  // La última fila, la vamos a procesar añadiendo otra linea negra
  for(int p = 1; p < this->ancho + 1; p++) {
    imagMat[0][p] = imagMat[1][p];
    imagMat[1][p] = imagMat[2][p];
    imagMat[2][p] = 0;
  }
  // Sacamos media de la última fila
  inc = 0;
  for(int i = 0; i < this->ancho; i++) {
    for(int maskm = 0; maskm < 3; maskm++)
      for(int maskn = inc; maskn < inc + 3; maskn++)
        mean += imagMat[maskm][maskn];
    mean /= 9;
    inc++;

    w_pixel(imagen2, Pixel(mean, mean, mean));
    mean = 0;
  }

  imagen.close();
  imagen2.close();

  return Imagen(out);
};

Imagen Imagen::mGaussiana(double variance) {
  string out = "mgauss" + this->dir;
  ifstream imagen(this->dir);
  ofstream imagen2(out);
  // Las 2 columnas, serán 0's para también procesar los bordes la imagen
  const int cols = this->ancho + 2;
  int imagMat[3][cols];
  int inc = 0;
  double mean;
  Pixel pix;

  cp_cabeceras(imagen, imagen2);

  // Generar matriz de pesos
  double weights[3][3];
  double summation;
  for(int i = 0; i < 3; i++) {
    for(int j = 0; j < 3; j++) {
      weights[i][j] = (1/(2*M_PI*variance*variance)) * exp(-1*((i-1)*(i-1)+(j-1)*(j-1))/(2*variance*variance));
      summation += weights[i][j];
    }
  }
  // Normalizarla
  for(int i = 0; i < 3; i++) {
    for(int j = 0; j < 3; j++) {
      weights[i][j] /= summation;
      printf("%f ", weights[i][j]);
    }
    printf("\n");
  }

  // Llenar las ultimas 2 filas de la matriz
  // Los 0's de los bordes serán estáticos
  imagMat[0][0] = 0; imagMat[0][this->ancho + 1] = 0;
  imagMat[1][0] = 0; imagMat[1][this->ancho + 1] = 0;
  imagMat[2][0] = 0; imagMat[2][this->ancho + 1] = 0;
  for(int j = 1; j < this->ancho + 1; j++)
    imagMat[1][j] = 0;
  for(int j = 1; j < this->ancho + 1; j++) {
    r_pixel(imagen, pix);
    imagMat[2][j] = pix.getR();
  }

  for(int m = 0; m < this->alto -1 ; m++) {
    // Llenar matriz:
    // Lee 1 linea, recorre las 2 que ya estaban
    for(int p = 1; p < this->ancho + 1; p++) {
      imagMat[0][p] = imagMat[1][p];
      imagMat[1][p] = imagMat[2][p];
      r_pixel(imagen, pix);
      imagMat[2][p] = pix.getR();
    }

    // Procesar cada pixel (columna de la fila[m] actual)
    for(int n = 0; n < this->ancho; n++) {
      // Obtener la media
      // Con inc vamos moviendo la mascara a través de la matriz (a la derecha)
      int iGauss = 0, jGauss = 0;
      for(int maskm = 0; maskm < 3; maskm++) {
        for(int maskn = inc; maskn < inc + 3; maskn++) {
          mean += imagMat[maskm][maskn] * weights[iGauss][jGauss];
          jGauss++;
        }
        jGauss = 0;
        iGauss++;
      }
      iGauss = 0;
      // mean /= 9;
      inc++;

      w_pixel(imagen2, Pixel((int)mean, (int)mean, (int)mean));
      mean = 0;
    }
    inc = 0;
  }

  // La última fila, la vamos a procesar añadiendo otra linea negra
  for(int p = 1; p < this->ancho + 1; p++) {
    imagMat[0][p] = imagMat[1][p];
    imagMat[1][p] = imagMat[2][p];
    imagMat[2][p] = 0;
  }
  // Sacamos media de la última fila
  inc = 0;
  for(int i = 0; i < this->ancho; i++) {
    for(int maskm = 0; maskm < 3; maskm++)
      for(int maskn = inc; maskn < inc + 3; maskn++)
        mean += imagMat[maskm][maskn];
    mean /= 9;
    inc++;

    w_pixel(imagen2, Pixel(mean, mean, mean));
    mean = 0;
  }

  imagen.close();
  imagen2.close();

  return Imagen(out);
};

Imagen Imagen::mGeometrica() {
  string out = "geom" + this->dir;
  ifstream imagen(this->dir);
  ofstream imagen2(out);
  // Las 2 columnas, serán 0's para también procesar los bordes la imagen
  const int cols = this->ancho + 2;
  int imagMat[3][cols];
  double mean = 1; int inc = 0;
  Pixel pix;

  cp_cabeceras(imagen, imagen2);

  // Llenar las ultimas 2 filas de la matriz
  // Los 0's de los bordes serán estáticos
  imagMat[0][0] = 0; imagMat[0][this->ancho + 1] = 0;
  imagMat[1][0] = 0; imagMat[1][this->ancho + 1] = 0;
  imagMat[2][0] = 0; imagMat[2][this->ancho + 1] = 0;
  for(int j = 1; j < this->ancho + 1; j++)
    imagMat[1][j] = 0;
  for(int j = 1; j < this->ancho + 1; j++) {
    r_pixel(imagen, pix);
    imagMat[2][j] = pix.getR();
  }

  for(int m = 0; m < this->alto -1 ; m++) {
    // Llenar matriz:
    // Lee 1 linea, recorre las 2 que ya estaban
    for(int p = 1; p < this->ancho + 1; p++) {
      imagMat[0][p] = imagMat[1][p];
      imagMat[1][p] = imagMat[2][p];
      r_pixel(imagen, pix);
      imagMat[2][p] = pix.getR();
    }

    // Procesar cada pixel (columna de la fila[m] actual)
    for(int n = 0; n < this->ancho; n++) {
      // Obtener la media
      // Con inc vamos moviendo la mascara a través de la matriz (a la derecha)
      for(int maskm = 0; maskm < 3; maskm++)
        for(int maskn = inc; maskn < inc + 3; maskn++)
          mean *= imagMat[maskm][maskn];
      mean = pow(mean, 1.0/9);
      inc++;
      mean = round(mean);
      w_pixel(imagen2, Pixel((int)mean, (int)mean, (int)mean));
      mean = 1;
    }
    inc = 0;
  }

  // La última fila, la vamos a procesar añadiendo otra linea negra
  for(int p = 1; p < this->ancho + 1; p++) {
    imagMat[0][p] = imagMat[1][p];
    imagMat[1][p] = imagMat[2][p];
    imagMat[2][p] = 0;
  }
  // Sacamos media de la última fila
  inc = 0;
  for(int i = 0; i < this->ancho; i++) {
    for(int maskm = 0; maskm < 3; maskm++)
      for(int maskn = inc; maskn < inc + 3; maskn++)
        mean *= imagMat[maskm][maskn];
    mean = pow(mean, 1/9);
    inc++;
    mean = round(mean);
    w_pixel(imagen2, Pixel((int)mean, (int)mean, (int)mean));
    mean = 1;
  }

  imagen.close();
  imagen2.close();

  return Imagen(out);
};

#endif
