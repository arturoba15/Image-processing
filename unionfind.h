#ifndef UNIONFIND_H
#define UNIONFIND_H

using namespace std;

class UnionFind {
private:
  int *up;
  int *size;
  int sizearr = 0;
public:
  int length = 0;
  UnionFind();
  ~UnionFind();
  void addRoot();
  int find(int x);
  void merge(int x, int y);
  void printarrays();
};

UnionFind::UnionFind() {};

UnionFind::~UnionFind(){};

// Incrementa el arreglo en 1
// el elemento 0 se refiere a la etiqueta 2
void UnionFind::addRoot() {
  if(length != 0) {
    int *tempup, *tempsize;
    // Incrementa el arreglo en 1
    tempup = new int[sizearr + 1];
    for(int i = 0; i < sizearr; i++)
      tempup[i] = up[i];
    tempup[sizearr] = sizearr;
    delete [] up;
    up = tempup;

    tempsize = new int[sizearr + 1];
    for(int i = 0; i < sizearr; i++)
      tempsize[i] = size[i];
    tempsize[sizearr] = 1;
    delete [] size;
    size = tempsize;

    length++;
  } else {
    up = new int[1];
    size = new int[1];
    length++;
    up[0] = 0;
    size[0] = 1;
  }
  sizearr++;
};

int UnionFind::find(int x) {
  x -= 2;
  int root = x;
  while(root != up[root])
    root = up[root];
  while(x != root) {
    int newp = up[x];
    up[x] = root;
    x = newp;
  }

  return root;
};

void UnionFind::merge(int x, int y) {
  int i = find(x);
  int j = find(y);
  if(i == j) return;
  if(size[i] < size[j]) {
    up[i] = j;
    size[j] += size[i];
  } else {
    up[j] = i;
    size[i] += size[j];
  }
  length--;
};

void UnionFind::printarrays() {
  printf("up:  ");
  for(int i = 0; i < sizearr; i++)
    printf(" %d", up[i]);
  printf("\n");
  printf("size:");
  for(int i = 0; i < sizearr; i++)
    printf(" %d", size[i]);

};

#endif
