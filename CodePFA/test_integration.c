/******************************************************/
/* test_integration.c                                 */
/* Tests des fonctions d'intégration numérique        */
/* Creation date: 31 July, 2025                       */
/******************************************************/

#include "integration.h"
#include <math.h>

/* ====================================================
   Fonctions de test avec valeurs exactes connues
   ==================================================== */

/* f1(x) = x²   => intégrale sur [0,1] = 1/3 ~ 0.333333333 */
double f1(double x) { return x * x; }

/* f2(x) = sin(x) => intégrale sur [0, pi] = 2 */
double f2(double x) { return sin(x); }

/* f3(x) = e^x   => intégrale sur [0,1] = e-1 ~ 1.718281828 */
double f3(double x) { return exp(x); }

/* f4(x) = 1/(1+x²) => intégrale sur [0,1] = pi/4 ~ 0.785398163 */
double f4(double x) { return 1.0 / (1.0 + x * x); }

/* f5(x) = sin(x²) => intégrale sur [-1,4] ~ 1.057402146  (pas de primitive simple) */
double f5(double x) { return sin(x * x); }

/* ====================================================
   Utilitaires d'affichage
   ==================================================== */

static void ligne_tirets()
{
  printf("  %-10s  %-18s  %-18s  %-12s\n",
         "Formule", "Valeur calculée", "Valeur exacte", "Erreur abs.");
  printf("  %s\n", "-------------------------------------------------------------------");
}

static void test_une_formule(char* nom, double (*f)(double),
                              double a, double b, int N, double exact)
{
  QuadFormula qf;
  if (!setQuadFormula(&qf, nom))
  {
    printf("  %-10s  ERREUR setQuadFormula\n", nom);
    return;
  }
  double res   = integrate(f, a, b, N, &qf);
  double erreur = fabs(res - exact);
  printf("  %-10s  %-18.10f  %-18.10f  %.2e\n", nom, res, exact, erreur);
}

static void tester_toutes_formules(char* titre, double (*f)(double),
                                    double a, double b, int N, double exact)
{
  printf("\n=== %s  [%.2f, %.2f]  N=%d  exacte=%.10f ===\n", titre, a, b, N, exact);
  ligne_tirets();
  test_une_formule("left",      f, a, b, N, exact);
  test_une_formule("right",     f, a, b, N, exact);
  test_une_formule("middle",    f, a, b, N, exact);
  test_une_formule("trapezes",  f, a, b, N, exact);
  test_une_formule("simpson",   f, a, b, N, exact);
  test_une_formule("gauss2",    f, a, b, N, exact);
  test_une_formule("gauss3",    f, a, b, N, exact);
}

/* ====================================================
   Test 1 : résultats sur plusieurs intégrales (N=100)
   ==================================================== */
void test_formules_N100()
{
  printf("\n╔══════════════════════════════════════════════════════════════╗\n");
  printf("║  TEST 1 : comparaison des 7 formules sur 4 intégrales (N=100) ║\n");
  printf("╚══════════════════════════════════════════════════════════════╝\n");

  tester_toutes_formules("x² sur [0,1]",        f1, 0.0,  1.0,  100, 1.0 / 3.0);
  tester_toutes_formules("sin(x) sur [0,pi]",   f2, 0.0,  M_PI, 100, 2.0);
  tester_toutes_formules("e^x sur [0,1]",        f3, 0.0,  1.0,  100, exp(1.0) - 1.0);
  tester_toutes_formules("1/(1+x²) sur [0,1]",  f4, 0.0,  1.0,  100, M_PI / 4.0);
}

/* ====================================================
   Test 2 : convergence en fonction de N (sin(x²))
   ==================================================== */
void test_convergence()
{
  printf("\n╔══════════════════════════════════════════════════════════════╗\n");
  printf("║  TEST 2 : convergence en N  — sin(x²) sur [-1,4]            ║\n");
  printf("║  Valeur exacte : 1.0574021460                                ║\n");
  printf("╚══════════════════════════════════════════════════════════════╝\n");

  double exact = 1.057402146;
  char* formules[] = {"left", "middle", "trapezes", "simpson", "gauss3"};
  int   nb         = 5;

  printf("\n  N\t\t");
  for (int j = 0; j < nb; j++) printf("%-14s", formules[j]);
  printf("\n  (%*s)", 14 * nb - 2, "erreurs absolues");
  printf("\n  %s\n", "--------------------------------------------------------------------------------");

  int valeurs_N[] = {10, 50, 100, 500, 1000};
  for (int i = 0; i < 5; i++)
  {
    int N = valeurs_N[i];
    printf("  %d\t\t", N);
    for (int j = 0; j < nb; j++)
    {
      QuadFormula qf;
      setQuadFormula(&qf, formules[j]);
      double res = integrate(f5, -1.0, 4.0, N, &qf);
      printf("%-14.2e", fabs(res - exact));
    }
    printf("\n");
  }
  printf("\n  Observation : plus N est grand, plus l'erreur diminue.\n");
  printf("  Simpson et gauss3 convergent beaucoup plus vite que left/right.\n");
}

/* ====================================================
   Test 3 : integrate_dx — vérification du calcul de N
   ==================================================== */
void test_integrate_dx()
{
  printf("\n╔══════════════════════════════════════════════════════════════╗\n");
  printf("║  TEST 3 : integrate_dx  — x² sur [0,1]  (exacte = 0.333...)  ║\n");
  printf("╚══════════════════════════════════════════════════════════════╝\n");

  double exact = 1.0 / 3.0;
  QuadFormula qf;
  setQuadFormula(&qf, "simpson");

  printf("\n  dx\t\tN attendu\tValeur calculée\t\tErreur\n");
  printf("  %s\n", "------------------------------------------------------------------");

  double dxs[] = {0.5, 0.2, 0.1, 0.05, 0.01};
  for (int i = 0; i < 5; i++)
  {
    double dx = dxs[i];
    int N_attend = (int) round(1.0 / dx);
    if (N_attend == 0) N_attend = 1;
    double res = integrate_dx(f1, 0.0, 1.0, dx, &qf);
    printf("  %.3f\t\t%d\t\t%.10f\t\t%.2e\n", dx, N_attend, res, fabs(res - exact));
  }

  /* Cas particulier : intervalle très petit => N doit valoir au moins 1 */
  double res_petit = integrate_dx(f1, 0.0, 0.001, 0.5, &qf);
  printf("\n  Intervalle [0, 0.001] avec dx=0.5 => N=1 imposé\n");
  printf("  Valeur calculée = %.10f  (attendu ~ 0.0000000003)\n", res_petit);
}

/* ====================================================
   Test 4 : exemple tiré des spécifications
   ==================================================== */
void test_exemple_specifications()
{
  printf("\n╔══════════════════════════════════════════════════════════════╗\n");
  printf("║  TEST 4 : exemple des spécifications — sin(t²) sur [-1, 4]   ║\n");
  printf("║  Valeur exacte : ~ 1.0574021460                               ║\n");
  printf("╚══════════════════════════════════════════════════════════════╝\n\n");

  QuadFormula qf;
  double I1, I2, I3;

  setQuadFormula(&qf, "trapezes");
  I1 = integrate(f5, -1.0, 4.0, 10, &qf);
  printf("  trapezes   N=10           I1 = %.10f\n", I1);

  setQuadFormula(&qf, "middle");
  I2 = integrate(f5, -1.0, 4.0, 10, &qf);
  printf("  middle     N=10           I2 = %.10f\n", I2);

  setQuadFormula(&qf, "simpson");
  I3 = integrate_dx(f5, -1.0, 4.0, 0.1, &qf);
  printf("  simpson    dx=0.1  N=50   I3 = %.10f\n", I3);
  printf("\n  Valeur exacte :            1.0574021460\n");
}

/* ====================================================
   Test 5 : setQuadFormula — noms invalides
   ==================================================== */
void test_noms_invalides()
{
  printf("\n╔══════════════════════════════════════════════════════════════╗\n");
  printf("║  TEST 5 : setQuadFormula avec noms invalides                  ║\n");
  printf("╚══════════════════════════════════════════════════════════════╝\n\n");

  QuadFormula qf;
  bool ok;

  ok = setQuadFormula(&qf, "Simpson"); /* majuscule => invalide */
  printf("  setQuadFormula(\"Simpson\") => %s  (attendu : false)\n", ok ? "true" : "false");

  ok = setQuadFormula(&qf, "");
  printf("  setQuadFormula(\"\")        => %s  (attendu : false)\n", ok ? "true" : "false");

  ok = setQuadFormula(&qf, "gauss3");
  printf("  setQuadFormula(\"gauss3\")  => %s  (attendu : true)\n",  ok ? "true" : "false");
}

/* ====================================================
   main
   ==================================================== */
int main()
{
  printf("╔══════════════════════════════════════════════════════════════╗\n");
  printf("║          TESTS — INTÉGRATION NUMÉRIQUE                       ║\n");
  printf("╚══════════════════════════════════════════════════════════════╝\n");

  test_formules_N100();
  test_convergence();
  test_integrate_dx();
  test_exemple_specifications();
  test_noms_invalides();

  printf("\n╔══════════════════════════════════════════════════════════════╗\n");
  printf("║  TOUS LES TESTS TERMINÉS                                      ║\n");
  printf("╚══════════════════════════════════════════════════════════════╝\n");

  return 0;
}
