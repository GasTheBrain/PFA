
#include <math.h>
#include "integration.h"
#include "integration.c"
double f(double t)
{
    return sin(t*t);
}
double erreur(double x, double valeur_exacte)
{
    return fabs(x-valeur_exacte);
}

int main()
{
    double valeur_exacte = 1.057402146;
    QuadFormula qf;
    double I1, I2;
    int N=100000000;
    printf("integrate\n");
    setQuadFormula(&qf, "left");
    I1= integrate(f, -1, 4, N, &qf);
    printf("left    %.10f erreur %.10f\n", I1, erreur(I1, valeur_exacte));
    setQuadFormula(&qf, "right");
    I1= integrate(f, -1, 4, N, &qf);
    printf("right   %.10f erreur %.10f\n", I1, erreur(I1, valeur_exacte));
    setQuadFormula(&qf, "middle");
    I1= integrate(f, -1, 4, N, &qf);
    printf("middle  %.10f erreur %.10f\n", I1, erreur(I1, valeur_exacte));
    setQuadFormula(&qf, "simpson");
    I1= integrate(f, -1, 4, N, &qf);
    printf("simpson %.10f erreur %.10f\n", I1, erreur(I1, valeur_exacte));
    setQuadFormula(&qf, "gauss2");
    I1= integrate(f, -1, 4, N, &qf);
    printf("gauss2  %.10f erreur %.10f\n", I1, erreur(I1, valeur_exacte));
    setQuadFormula(&qf, "gauss3");
    I1= integrate(f, -1, 4, N, &qf);
    printf("gauss3  %.10f erreur %.10f\n\n", I1, erreur(I1, valeur_exacte));

    double dx=0.000001;
    printf("integrate_dx\n");
    setQuadFormula(&qf, "left");
    I2 = integrate_dx(f, -1, 4, dx, &qf);
    printf("left    %.10f erreur %.10f\n", I2, erreur(I2, valeur_exacte));
    setQuadFormula(&qf, "right");
    I2 = integrate_dx(f, -1, 4, dx, &qf);
    printf("right   %.10f erreur %.10f\n", I2, erreur(I2, valeur_exacte));
    setQuadFormula(&qf, "middle");
    I2 = integrate_dx(f, -1, 4, dx, &qf);
    printf("middle  %.10f erreur %.10f\n", I2, erreur(I2, valeur_exacte));
    setQuadFormula(&qf, "simpson");
    I2 = integrate_dx(f, -1, 4, dx, &qf);
    printf("simpson %.10f erreur %.10f\n", I2, erreur(I2, valeur_exacte));
    setQuadFormula(&qf, "gauss2");
    I2 = integrate_dx(f, -1, 4, dx, &qf);
    printf("gauss2  %.10f erreur %.10f\n", I2, erreur(I2, valeur_exacte));
    setQuadFormula(&qf, "gauss3");
    I2 = integrate_dx(f, -1, 4, dx, &qf);
    printf("gauss3  %.10f erreur %.10f\n", I2, erreur(I2, valeur_exacte));
    return 0;
}