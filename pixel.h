
using namespace std;

class pixel
{
  private:
    int R;
    int G;
    int B;

  public:
    pixel();
    ~pixel();
    int retR();
    int retG();
    int retB();
    void Valores(int a, int b, int c);
};

pixel::pixel()
{
  R = 0;
  G = 0;
  B = 0;
};

pixel::~pixel(){}

int pixel::retR()
{
  return R;
};

int pixel::retG()
{
  return G;
};

int pixel::retB()
{
  return B;
};

void pixel::Valores(int a, int b, int c)
{
  R = a;
  G = b;
  B = c;
};