
#include <math.h>
#include "integration.h"
#include "integration.c"
double f(double x)
{
    return sin(x*x);
}
double erreur(double x, double valeur_exacte)
{
    return fabs(x-valeur_exacte);
}

int main()
{
    double valeur_exacte_exp = 1149402.09724167548;
    double valeur_exacte_sin = 1.057402146;
    double valeur_exacte=valeur_exacte_sin;
    QuadFormula qf;
    double I1;
    int N=10;
    printf("___Test pour la fontion sin(x*x) sur [-1, 4]___\n");
    printf("Valeur exacte : %.9f / valeur de N : %d \n", valeur_exacte, N);
    printf("integrate\n");
    setQuadFormula(&qf, "left");
    I1= integrate(f, -1, 4, N, &qf);
    printf("left     %.10f erreur %.10f\n", I1, erreur(I1, valeur_exacte));
    setQuadFormula(&qf, "right");
    I1= integrate(f, -1, 4, N, &qf);
    printf("right    %.10f erreur %.10f\n", I1, erreur(I1, valeur_exacte));
    setQuadFormula(&qf, "middle");
    I1= integrate(f, -1, 4, N, &qf);
    printf("middle   %.10f erreur %.10f\n", I1, erreur(I1, valeur_exacte));
    setQuadFormula(&qf, "trapezes");
    I1= integrate(f, -1, 4, N, &qf);
    printf("trapezes %.10f erreur %.10f\n", I1, erreur(I1, valeur_exacte));
    setQuadFormula(&qf, "simpson");
    I1= integrate(f, -1, 4, N, &qf);
    printf("simpson  %.10f erreur %.10f\n", I1, erreur(I1, valeur_exacte));
    setQuadFormula(&qf, "gauss2");
    I1= integrate(f, -1, 4, N, &qf);
    printf("gauss2   %.10f erreur %.10f\n", I1, erreur(I1, valeur_exacte));
    setQuadFormula(&qf, "gauss3");
    I1= integrate(f, -1, 4, N, &qf);
    printf("gauss3   %.10f erreur %.10f\n\n", I1, erreur(I1, valeur_exacte));

    // double I1;
    // double dx=0.000001;
    // printf("integrate_dx\n");
    // setQuadFormula(&qf, "left");
    // I2 = integrate_dx(f, -1, 4, dx, &qf);
    // printf("left    %.10f erreur %.10f\n", I2, erreur(I2, valeur_exacte));
    // setQuadFormula(&qf, "right");
    // I2 = integrate_dx(f, -1, 4, dx, &qf);
    // printf("right   %.10f erreur %.10f\n", I2, erreur(I2, valeur_exacte));
    // setQuadFormula(&qf, "middle");
    // I2 = integrate_dx(f, -1, 4, dx, &qf);
    // printf("middle  %.10f erreur %.10f\n", I2, erreur(I2, valeur_exacte));
    // setQuadFormula(&qf, "simpson");
    // I2 = integrate_dx(f, -1, 4, dx, &qf);
    // printf("simpson %.10f erreur %.10f\n", I2, erreur(I2, valeur_exacte));
    // setQuadFormula(&qf, "gauss2");
    // I2 = integrate_dx(f, -1, 4, dx, &qf);
    // printf("gauss2  %.10f erreur %.10f\n", I2, erreur(I2, valeur_exacte));
    // setQuadFormula(&qf, "gauss3");
    // I2 = integrate_dx(f, -1, 4, dx, &qf);
    // printf("gauss3  %.10f erreur %.10f\n", I2, erreur(I2, valeur_exacte));
    return 0;
}