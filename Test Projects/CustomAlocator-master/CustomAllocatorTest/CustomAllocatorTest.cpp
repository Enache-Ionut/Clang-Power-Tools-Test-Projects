
#include "stdafx.h"

using namespace std;

struct A
{
	int  a;
  int  b;
  bool c;
  char r[48];

  A() = default;
  A(bool v)
    : a(5)
    , b(v ? 1 : 0)
    , c(v)
  {
  }
};

char * p[2'500'00];

int main()
{
  /*A * pA = new A();
  pA->b++;
  cout << pA->b << endl;
  delete pA;

  A * pA2 = new A(true);
  pA2->b++;
  cout << pA2->b << endl;
  delete pA2;

  A * parrA = new A[5];
  parrA[2].c = true;
  delete[] parrA;

  const int NO_STR = 5;
  if (auto p = (string*)malloc(NO_STR * sizeof(string)))
  {
          for (int i = 0; i != NO_STR; ++i) // populate the array
          {
          void * placement = p + i;
          new(placement) string(4, (char)('a' + i));
          }

          for (int j = 0; j != NO_STR; ++j)
          cout << "p[" << j << "] == " << p[j] << '\n';

          for (int i = NO_STR; i != 0; --i) // clean up
          p[i - 1].~string();

          free(p);
  }*/

  /*int *a =  new int[2];

  a[0] = 1;
  a[1] = 2;

  printf("%d\n", a[0]);
  printf("%d\n", a[1]);

  char *b = new char;

  *b = 'a';

  printf("%c\n", *b);

  double *c = new double(2.3);

  printf("%f\n", *c);

  long long *d = new long long(3);

  printf("%lld\n\n", *d);

  printf("%d\n", a[0]);
  printf("%d\n", a[1]);

  printf("%c\n", *b);

  printf("%f\n", *c);

  printf("%lld\n", *d);*

  /*for (int i = 1; i <= 10000000; ++i)
  {
          char *a = new char[1];


          char *b = new char[1];

          char *c = new char[1];

          for (int j = 0; j < ; ++j)
                  a[j] = 1, b[j] = 2, c[j] = 3;



          delete a;

          delete c;

          checkMem();
          printf("\n");
          Sleep(200);

  }*/

  memoryVisualise();
  /*

          char *a = new char[10000];
          a[0] = 'x';

          char *b = new char[10000];
          b[0] = 'x';

          char *c = new char[10000];
          c[0] = 'x';

          char *d = new char[10000];
          d[0] = 'x';

          char *e = new char[10000];
          e[0] = 'x';

          char *f = new char[10000];
          f[0] = 'x';

          int *g = new int[10000];
          g[0] = 7;



          int *h = new int[12345];
          h[0] = 8;



          delete a;
          Sleep(100);
          delete b;
          Sleep(100);
          delete c;
          Sleep(100);
          delete d;
          Sleep(100);
          delete e;
          Sleep(100);
          delete f;
          Sleep(100);
          delete g;
          Sleep(100);
          delete h;
          Sleep(100);
          long long *x = new long long[61000];
          x[0] = 10;
          Sleep(100);
          delete x;

          long long *l1 = new long long[10];
          l1[0] = 1;
          Sleep(100);
          delete l1;

          long long *l2 = new long long[200];
          l2[0] = 2;
          Sleep(100);
          delete l2;

          long long *l3 = new long long[30];
          l3[0] = 4;
          delete l3;

          Sleep(100);

  */
  srand(53545);

  int k = 0;

  /*for (int i = 0; i < 2'500'00; ++i)
          p[i] = new char[1];

  for (int i = 0; i < 2'500'00; i += 2)
  {
          delete[] p[i];

          printf("matrix fragmentation:%f\n            ",metricFragmentation());



          COORD coord;

          coord.X = 0;

          coord.Y = 0;

          SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

  }*/

  while (true)
  {
    if (rand() % 5 > 1)
    {
      if (rand() % 4 == 0)
      {
        p[k] = new char[rand() % 1000 + 1];
      }
      else
      {
        p[k] = new char[rand() % 10 + 1];
      }

      if (p[k] != nullptr)
      {
        ++k;
      }
      else
      {
        break;
      }
    }
    else if (k != 0)
    {
      int x = rand() % (k / 2) + k / 2;

      delete[] p[x];

      for (int j = x + 1; j < k; ++j)
      {
        p[j - 1] = p[j];
      }
      --k;
    }

    memoryUsage();
    cout << "max available space:" << maxAvailable() << " bytes          \n";

    cout << "matrix fragmentation:" << metricFragmentation(0) << "            ";

    COORD coord;

    coord.X = 0;

    coord.Y = 0;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
  }

  /*set<int> st;
  vector<int> vec1,vec2;
  for (int i = 1; i <= 30000; ++i)
  {
          if (i % 1000 == 0)
          {
                  Sleep(10);
          }
          st.insert(i);
          vec1.push_back(i);


  }*/

  memoryUsage();

  // cout << "Max memory block available: " << maxAvailable() << " Bytes \n";

  system("pause");

  return 0;
}
