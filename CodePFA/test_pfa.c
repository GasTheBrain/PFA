/******************************************************/
/* test_pfa.c                                         */
/* Tests des fonctions finance et assurance           */
/* Creation date: 31 July, 2025                       */
/*                                                    */
/* Les valeurs "exactes" affichées en comparaison     */
/* sont soit des valeurs analytiques fermées, soit    */
/* des valeurs calculées par intégration numérique    */
/* de haute précision (scipy, limit=200).             */
/******************************************************/

#include "pfa.h"
#include "integration.h"

/* ====================================================
   Utilitaire d'affichage
   ==================================================== */
static void ligne(void)
{
  printf("  %-10s  %-18s  %-18s  %-12s\n",
         "x", "valeur calculée", "valeur exacte", "erreur abs.");
  printf("  %s\n", "-------------------------------------------------------------------");
}

/* ====================================================
   TEST 1 : loi normale N(0,1) — phi et PHI
   ==================================================== */
void test_loi_normale(void)
{
  printf("╔══════════════════════════════════════════════════════════════╗\n");
  printf("║  TEST 1 : loi normale N(0,1) — phi et PHI                    ║\n");
  printf("╚══════════════════════════════════════════════════════════════╝\n\n");

  /* --- phi(x) = (1/sqrt(2*pi)) * exp(-x²/2) ---
     Valeurs exactes calculées analytiquement.    */
  printf("  phi(x) = (1/sqrt(2*pi)) * exp(-x²/2)\n");
  printf("  %-8s  %-18s  %-18s  %-12s\n", "x", "valeur calculée", "valeur exacte", "erreur");
  printf("  %s\n", "-------------------------------------------------------------------");

  struct { double x; double exact; } cas_phi[] = {
    {  0.0,  0.3989422804014327 },
    {  1.0,  0.2419707245191434 },
    { -1.0,  0.2419707245191434 },
    {  2.0,  0.0539909665131881 },
    { -2.0,  0.0539909665131881 },
  };
  for (int i = 0; i < 5; i++)
  {
    double x      = cas_phi[i].x;
    double exact  = cas_phi[i].exact;
    double calcul = phi(x);
    printf("  %-8.2f  %-18.10f  %-18.10f  %.2e\n", x, calcul, exact, fabs(calcul - exact));
  }

  /* --- PHI(x) = P(Z <= x), Z ~ N(0,1) ---
     Valeurs exactes issues de la table N(0,1).   */
  printf("\n  PHI(x) = 0.5 + intégrale de phi entre 0 et x\n");
  printf("  %-8s  %-18s  %-18s  %-12s\n", "x", "valeur calculée", "valeur exacte", "erreur");
  printf("  %s\n", "-------------------------------------------------------------------");

  struct { double x; double exact; } cas_PHI[] = {
    {  0.0,  0.5000000000000000 },
    {  1.0,  0.8413447460685429 },
    { -1.0,  0.1586552539314571 },
    {  1.96, 0.9750021048517795 },
    { -1.96, 0.0249978951482205 },
    {  2.0,  0.9772498680518208 },
    { -2.0,  0.0227501319481792 },
  };
  for (int i = 0; i < 7; i++)
  {
    double x      = cas_PHI[i].x;
    double exact  = cas_PHI[i].exact;
    double calcul = PHI(x);
    printf("  %-8.4f  %-18.10f  %-18.10f  %.2e\n", x, calcul, exact, fabs(calcul - exact));
  }

  /* --- symétrie : PHI(x) + PHI(-x) = 1 --- */
  printf("\n  Symétrie : PHI(x) + PHI(-x) doit valoir exactement 1\n");
  double xs[] = {0.5, 1.0, 1.5, 2.0};
  for (int i = 0; i < 4; i++)
  {
    double x   = xs[i];
    double som = PHI(x) + PHI(-x);
    printf("  PHI(%.1f) + PHI(-%.1f) = %.10f  (exact : 1.0000000000)\n", x, x, som);
  }
}

/* ====================================================
   TEST 2 : prix d'options call et put
   Valeurs exactes calculées analytiquement avec
   les formules fermées du document mathématique,
   en utilisant les vraies valeurs de PHI (table N(0,1)).
   ==================================================== */
void test_options(void)
{
  printf("\n╔══════════════════════════════════════════════════════════════╗\n");
  printf("║  TEST 2 : prix d'options call et put                         ║\n");
  printf("╚══════════════════════════════════════════════════════════════╝\n\n");

  /* --- Cas 1 : S0=100, K=100, T=1, mu=0, sig=0.2 ---
     z0 = (ln(100/100) - 1*(0 - 0.04/2)) / (0.2*1) = 0.02/0.2 = 0.1000
     Call = S0*e^(mu*T)*PHI(sig*sqrt(T)-z0) - K*PHI(-z0)
          = 100*PHI(0.1) - 100*PHI(-0.1) = 7.965567
     Put  = K*PHI(z0) - S0*e^(mu*T)*PHI(z0-sig*sqrt(T))
          = 100*PHI(0.1) - 100*PHI(-0.1) = 7.965567  (égaux car mu=0, K=S0) */
  {
    Option opt = { CALL, 100.0, 100.0, 1.0, 0.0, 0.2 };
    double c = optionPrice(&opt);
    opt.type = PUT;
    double p = optionPrice(&opt);
    printf("  Cas 1 — S0=100, K=100, T=1, mu=0, sig=0.2\n");
    printf("  z0 = 0.100000  (exact analytique)\n");
    printf("  %-6s  %-14s  %-14s  %-12s\n", "", "valeur calculée", "valeur exacte", "erreur");
    printf("  %-6s  %-14.6f  %-14.6f  %.2e\n", "Call",  c, 7.965567, fabs(c  - 7.965567));
    printf("  %-6s  %-14.6f  %-14.6f  %.2e\n", "Put",   p, 7.965567, fabs(p  - 7.965567));
    printf("  |Call - Put| = %.8f  (exact : 0  — car mu=0 et K=S0)\n\n", fabs(c - p));
  }

  /* --- Cas 2 : S0=100, K=100, T=1, mu=0.10, sig=0.3 ---
     z0 = (0 - (0.10 - 0.09/2)) / 0.3 = -0.055/0.3 = -0.183333
     Call exacte = 18.494078
     Put  exacte =  7.976986  */
  {
    Option opt = { CALL, 100.0, 100.0, 1.0, 0.10, 0.3 };
    double c = optionPrice(&opt);
    opt.type = PUT;
    double p = optionPrice(&opt);
    printf("  Cas 2 — S0=100, K=100, T=1, mu=0.10, sig=0.3\n");
    printf("  z0 = -0.183333  (exact analytique)\n");
    printf("  %-6s  %-14s  %-14s  %-12s\n", "", "valeur calculée", "valeur exacte", "erreur");
    printf("  %-6s  %-14.6f  %-14.6f  %.2e\n", "Call",  c, 18.494078, fabs(c  - 18.494078));
    printf("  %-6s  %-14.6f  %-14.6f  %.2e\n", "Put",   p,  7.976986, fabs(p  -  7.976986));
    printf("  Call > Put : %s  (attendu OUI, mu > 0)\n\n", (c > p) ? "OUI" : "NON (erreur)");
  }

  /* --- Cas 3 : S0=120, K=100, T=1, mu=0.05, sig=0.2 ---
     z0 = (ln(100/120) - (0.05 - 0.02)) / 0.2 = (-0.18232 - 0.03) / 0.2 = -1.061608
     Call exacte = 27.510760
     Put  exacte =  1.358228  */
  {
    Option opt = { CALL, 120.0, 100.0, 1.0, 0.05, 0.2 };
    double c = optionPrice(&opt);
    opt.type = PUT;
    double p = optionPrice(&opt);
    printf("  Cas 3 — S0=120, K=100, T=1, mu=0.05, sig=0.2  (call dans la monnaie)\n");
    printf("  z0 = -1.061608  (exact analytique)\n");
    printf("  %-6s  %-14s  %-14s  %-12s\n", "", "valeur calculée", "valeur exacte", "erreur");
    printf("  %-6s  %-14.6f  %-14.6f  %.2e\n", "Call",  c, 27.510760, fabs(c  - 27.510760));
    printf("  %-6s  %-14.6f  %-14.6f  %.2e\n", "Put",   p,  1.358228, fabs(p  -  1.358228));
    printf("  Call > Put : %s  (attendu OUI, S0 >> K)\n\n", (c > p) ? "OUI" : "NON (erreur)");
  }

  /* --- Cas 4 : pointeur NULL --- */
  {
    double res = optionPrice(NULL);
    printf("  Cas 4 — optionPrice(NULL)\n");
    printf("  Résultat = %.4f  (exact : 0.0000)\n", res);
  }
}

/* ====================================================
   TEST 3 : loi de X (log-normale, m=7, s=1.5)
   Valeurs exactes :
     fX(x) = phi((ln(x)-m)/s) / (s*x)   — formule fermée
     FX(x) = PHI((ln(x)-m)/s)            — formule fermée
   Calculées avec les vraies valeurs de PHI.

   Pour m=7, s=1.5 :
     x=1000 : z=(ln(1000)-7)/1.5 = (6.9078-7)/1.5 = -0.06148
              fX(1000) = phi(-0.06148)/1500 = 0.00026546
              FX(1000) = PHI(-0.06148)      = 0.47548191
     x=5000 : z=(ln(5000)-7)/1.5 = (8.5172-7)/1.5 =  1.01160
              fX(5000) = phi(1.01160)/7500  = 0.00003189
              FX(5000) = PHI(1.01160)       = 0.84410235
   ==================================================== */
void test_loi_X(void)
{
  printf("\n╔══════════════════════════════════════════════════════════════╗\n");
  printf("║  TEST 3 : assurance — densité et répartition de X            ║\n");
  printf("║  X ~ Log-Normale(m=7, s=1.5)                                 ║\n");
  printf("╚══════════════════════════════════════════════════════════════╝\n\n");

  InsuredClient client;
  client.m = 7.0;
  client.s = 1.5;
  double probs[3] = {0.7, 0.25, 0.05};
  client.p = probs;

  /* médiane = e^m = e^7 ~ 1096.63 */
  printf("  Médiane de X = e^m = e^7 = %.4f\n\n", exp(client.m));

  /* --- clientPDF_X ---
     x=1000 : z=(ln(1000)-7)/1.5=-0.06148  => phi(-0.06148)/1500 = 0.00026546
     x=5000 : z=(ln(5000)-7)/1.5= 1.01160  => phi(1.01160) /7500 = 0.00003189  */
  printf("  clientPDF_X — fX(x) = phi((ln(x)-7)/1.5) / (1.5*x)\n");
  ligne();
  struct { double x; double exact; } cas_pdf[] = {
    {    0.0, 0.00000000 },   /* x<=0 => 0 par définition */
    { 1000.0, 0.00026546 },   /* phi(-0.06148)/1500       */
    { 5000.0, 0.00003189 },   /* phi( 1.01160)/7500       */
  };
  for (int i = 0; i < 3; i++)
  {
    double x = cas_pdf[i].x, ex = cas_pdf[i].exact;
    double calc = clientPDF_X(&client, x);
    printf("  %-10.1f  %-18.8f  %-18.8f  %.2e\n", x, calc, ex, fabs(calc - ex));
  }

  /* --- clientCDF_X ---
     x=1000 : PHI(-0.06148) = 0.47548191
     x=5000 : PHI( 1.01160) = 0.84410235  */
  printf("\n  clientCDF_X — FX(x) = PHI((ln(x)-7)/1.5)\n");
  ligne();
  struct { double x; double exact; } cas_cdf[] = {
    {    0.0, 0.00000000 },
    { 1000.0, 0.47548191 },   /* PHI(-0.06148) */
    { 5000.0, 0.84410235 },   /* PHI( 1.01160) */
  };
  for (int i = 0; i < 3; i++)
  {
    double x = cas_cdf[i].x, ex = cas_cdf[i].exact;
    double calc = clientCDF_X(&client, x);
    printf("  %-10.1f  %-18.8f  %-18.8f  %.2e\n", x, calc, ex, fabs(calc - ex));
  }
}

/* ====================================================
   TEST 4 : loi de X1+X2 (convolution, m=7, s=1.5)
   Valeurs exactes calculées par intégration numérique
   haute précision (scipy, limit=200, erreur < 1e-8).

   Pour m=7, s=1.5 :
     x=1000 : fX1+X2(1000) = 0.00020807  (scipy quad, limit=200)
              FX1+X2(1000) = 0.14962520  (scipy quad, limit=200)
   ==================================================== */
void test_loi_X1X2(void)
{
  printf("\n╔══════════════════════════════════════════════════════════════╗\n");
  printf("║  TEST 4 : assurance — densité et répartition de X1+X2        ║\n");
  printf("║  Valeurs exactes : intégration scipy (erreur < 1e-8)          ║\n");
  printf("╚══════════════════════════════════════════════════════════════╝\n\n");

  InsuredClient client;
  client.m = 7.0;
  client.s = 1.5;
  double probs[3] = {0.7, 0.25, 0.05};
  client.p = probs;

  /* --- clientPDF_X1X2(1000) ---
     fX1+X2(x) = intégrale_0^x fX(t)*fX(x-t) dt
     Exact (scipy quad, limit=200) : 0.00020807  */
  printf("  clientPDF_X1X2(x) — convolution : intégrale_0^x fX(t)*fX(x-t) dt\n");
  printf("  %-10s  %-18s  %-18s  %-12s\n", "x", "valeur calculée", "valeur exacte (scipy)", "erreur");
  printf("  %s\n", "-------------------------------------------------------------------");
  {
    double x = 1000.0;
    printf("  Calcul en cours pour x=%.0f...\n", x);
    double calc  = clientPDF_X1X2(&client, x);
    double exact = 0.00020807;   /* scipy quad, limit=200 */
    printf("  %-10.1f  %-18.8f  %-18.8f  %.2e\n\n", x, calc, exact, fabs(calc - exact));
  }

  /* --- clientCDF_X1X2(1000) ---
     FX1+X2(x) = intégrale_0^x fX1+X2(t) dt
     Exact (scipy quad, limit=200) : 0.14962520  */
  printf("  clientCDF_X1X2(x) — intégrale de la densité de convolution\n");
  printf("  %-10s  %-18s  %-18s  %-12s\n", "x", "valeur calculée", "valeur exacte (scipy)", "erreur");
  printf("  %s\n", "-------------------------------------------------------------------");
  {
    double x = 1000.0;
    printf("  Calcul en cours pour x=%.0f...\n", x);
    double calc  = clientCDF_X1X2(&client, x);
    double exact = 0.14962520;   /* scipy quad, limit=200 */
    printf("  %-10.1f  %-18.8f  %-18.8f  %.2e\n\n", x, calc, exact, fabs(calc - exact));
  }

  /* cohérence : FX(x) > FX1+X2(x) pour tout x > 0 */
  printf("  Cohérence : FX(x) > FX1+X2(x) pour tout x > 0\n");
  printf("  (X1+X2 est stochastiquement plus grand que X seul)\n");
  double x_test   = 1000.0;
  double cdf_X    = clientCDF_X(&client, x_test);
  double cdf_X1X2 = clientCDF_X1X2(&client, x_test);
  printf("  FX(%.0f)    = %.8f\n", x_test, cdf_X);
  printf("  FX1+X2(%.0f) = %.8f\n", x_test, cdf_X1X2);
  printf("  FX > FX1+X2 : %s\n", (cdf_X > cdf_X1X2) ? "OUI (correct)" : "NON (erreur)");
}

/* ====================================================
   TEST 5 : loi de S (total des remboursements)
   p0=0.7, p1=0.25, p2=0.05, m=7, s=1.5

   Valeurs exactes :
     FS(0)    = p0 = 0.70000000                 (formule directe)
     FS(1000) = p0 + p1*FX(1000) + p2*FX1X2(1000)
              = 0.7 + 0.25*0.47548191 + 0.05*0.14962520
              = 0.82635174  (scipy)
     FS(5000) = p0 + p1*FX(5000) + p2*FX1X2(5000)
              = 0.7 + 0.25*0.84410235 + 0.05*0.64592068
              = 0.94332162  (scipy)
   ==================================================== */
void test_loi_S(void)
{
  printf("\n╔══════════════════════════════════════════════════════════════╗\n");
  printf("║  TEST 5 : assurance — fonction de répartition de S           ║\n");
  printf("║  FS(x) = p0 + p1*FX(x) + p2*FX1+X2(x)                       ║\n");
  printf("╚══════════════════════════════════════════════════════════════╝\n\n");

  InsuredClient client;
  client.m = 7.0;
  client.s = 1.5;
  double probs[3] = {0.7, 0.25, 0.05};
  client.p = probs;

  printf("  Paramètres : m=7, s=1.5  |  p[0]=0.70  p[1]=0.25  p[2]=0.05\n\n");

  /* x < 0 : doit retourner 0 */
  double cdf_neg = clientCDF_S(&client, -1.0);
  printf("  FS(-1) = %.8f  (exact : 0.00000000)\n", cdf_neg);

  /* x = 0 : doit retourner p0 = 0.70 */
  double cdf0 = clientCDF_S(&client, 0.0);
  printf("  FS(0)  = %.8f  (exact : 0.70000000 = p[0])\n\n", cdf0);

  /* x > 0 */
  printf("  %-8s  %-18s  %-18s  %-12s\n", "x", "valeur calculée", "valeur exacte (scipy)", "erreur");
  printf("  %s\n", "-------------------------------------------------------------------");

  struct { double x; double exact; } cas_S[] = {
    { 1000.0, 0.82635174 },  /* 0.7 + 0.25*0.47548191 + 0.05*0.14962520 */
    { 5000.0, 0.94332162 },  /* 0.7 + 0.25*0.84410235 + 0.05*0.64592068 */
  };
  for (int i = 0; i < 2; i++)
  {
    double x  = cas_S[i].x;
    double ex = cas_S[i].exact;
    printf("  Calcul en cours pour x=%.0f...\n", x);
    double calc = clientCDF_S(&client, x);
    printf("  %-8.0f  %-18.8f  %-18.8f  %.2e\n", x, calc, ex, fabs(calc - ex));
  }

  /* FS doit être croissante */
  printf("\n  FS est croissante : ");
  double prev = 0.0;
  double pts[] = {100.0, 500.0, 1000.0, 5000.0};
  bool ok = true;
  for (int i = 0; i < 4; i++)
  {
    double v = clientCDF_S(&client, pts[i]);
    if (v < prev) { ok = false; break; }
    prev = v;
  }
  printf("%s\n", ok ? "OUI (correct)" : "NON (erreur)");
}

/* ====================================================
   main
   ==================================================== */
int main(void)
{
  printf("╔══════════════════════════════════════════════════════════════╗\n");
  printf("║          TESTS — FINANCE ET ASSURANCE (PFA)                  ║\n");
  printf("╚══════════════════════════════════════════════════════════════╝\n\n");

  /* gauss3 + dt=0.1 : bon compromis précision/vitesse pour les tests 1-3. */
  if (!init_integration("gauss3", 0.1))
  {
    printf("ERREUR : init_integration a échoué\n");
    return 1;
  }
  printf("  Intégration : gauss3, dt=0.1\n\n");

  test_loi_normale();
  test_options();
  test_loi_X();

  /* Pour les tests 4 et 5 (double intégration sur [0, 1000] ou [0, 5000]),
     on augmente dt pour rester dans un temps raisonnable.
     dt=5.0 => ~200 subdivisions sur [0,1000] : précision correcte. */
  if (!init_integration("gauss3", 5.0))
  {
    printf("ERREUR : init_integration a échoué\n");
    return 1;
  }
  printf("\n  [Tests 4 et 5 : double intégration, dt=5.0 — quelques secondes...]\n");
  test_loi_X1X2();
  test_loi_S();

  printf("\n╔══════════════════════════════════════════════════════════════╗\n");
  printf("║  TOUS LES TESTS TERMINÉS                                      ║\n");
  printf("╚══════════════════════════════════════════════════════════════╝\n");
  return 0;
}