



#define INTEGRATION_C

#include "integration.h"

bool setQuadFormula(QuadFormula* qf, char* name)
{
  if (strcmp(name, "left") == 0)
  {
    qf->n=1;
    qf->w[0] = 1.0;
    qf->x[0] = 0.0;
  }
  else if (strcmp(name, "right") == 0)
  {
    qf->n=1;
    qf->w[0] = 1.0;
    qf->x[0] = 1.0;
  }
  else if (strcmp(name, "middle") == 0)
  {
    qf->n=1;
    qf->w[0] = 1.0;
    qf->x[0] = 1.0/2.0;
  }
  else if (strcmp(name, "trapezes") == 0)
  {
    qf->n=2;
    qf->w[0] = 1.0/2.0;
    qf->w[1] = 1.0/2.0;
    qf->x[0] = 0.0;
    qf->x[1] = 1.0;
  }
  else if (strcmp(name, "simpson") == 0)
  {
    qf->n=3;
    qf->w[0] = 1.0/6.0;
    qf->w[1] = 2.0/3.0;
    qf->w[2] = 1.0/6.0;
    qf->x[0] = 0.0;
    qf->x[1] = 1.0/2.0;
    qf->x[2] = 1.0;
  }
  else if (strcmp(name, "gauss2") == 0)
  {
    qf->n=2;
    qf->w[0] = 1.0/2.0;
    qf->w[1] = 1.0/2.0;
    qf->x[0] = 1.0/2.0-1.0/(2*sqrt(3));
    qf->x[1] = 1.0/2.0+1.0/(2*sqrt(3));
  }
  else if (strcmp(name, "gauss3") == 0)
  {
    qf->n=3;
    qf->w[0] = 5.0/18.0;
    qf->w[1] = 4.0/9.0;
    qf->w[2] = 5.0/18.0;
    qf->x[0] = 1.0/2.0*(1-sqrt(3.0/5.0));
    qf->x[1] = 1.0/2.0;
    qf->x[2] = 1.0/2.0*(1+sqrt(3.0/5.0));;
  }
  return true;
}

/* This function is not required ,but it may useful to debug */
void printQuadFormula(QuadFormula* qf)
{
  printf("Quadratic formula: %s\n", qf->name);
  /* Print everything else that may be useful */
}

double sum(double (*f)(double), double ai, double bi, QuadFormula* qf)
{
  double summ=0;
  for (int i = 0; i < qf->n; i++)
  {
    summ+=(qf->w[i])*(f(ai+(qf->x[i]*(bi-ai))));
  }
  // printf("summ : %.2f\n", summ);
  return summ;
}
double base(double (*f)(double), double ai, double bi, QuadFormula* qf)
{
  return (bi-ai)*sum(f,ai,bi, qf);
}
/* Approximate the integral of function f from a to b.
   - f is a pointer to a function pointer
   - a and b are the integration bounds
   - Interval [a,b] is split in N subdivisions [ai,bi]
   - Integral of f on each subdivision [ai,bi] is approximated by the quadrature formula qf.
*/
// double abss(double a)
// {
//   if (a<0)
//   {
//     return -a;
//   }
//   return a;
  
// }
double integrate(double (*f)(double), double a, double b, int N, QuadFormula* qf)
{
  double total=0;
  double sub=(b-a)/N;
  for (int i = 0; i < N; i++)
  {
    double ai=a+i*sub;
    double bi =a+(i+1)*sub;
    total+=base(f,ai,bi,qf);
    // printf("total : %.2f\n", total);
    // printf("ai : %.2f /bi : %.2f\n", ai,bi);
  }
  
  return total;
}

double integrate_dx(double (*f)(double), double a, double b, double dx, QuadFormula* qf)
{

  return integrate(f, a, b,(int) round( sqrt((b-a)*(b-a))/dx ), qf);
}


