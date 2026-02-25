

// #define GRAPHIC

#define PFA_C

#include "pfa.h"

/* Initialize the integration variables.
   Arguments :
   - quadrature : name of the quadrature formula that will be used. It can be "left", "right", 
     "middle", "trapezes", "simpson", "gauss2" or "gauss3".
   - dt : a positive value, which will be used to decide the number of subdivisions of an 
          interval [a,b], when computing the integration.
          The number of subdivisions will be N such that (b-a)/N ~ dt
*/
bool init_integration(char* quadrature, double dt)
{
  pfa_dt=dt;
  return setQuadFormula(&pfaQF,quadrature);
}



/* Density of the normal distribution */
double phi(double x)
{
  return 0.398942280401433 * exp( -x*x/2 );
}

/* Cumulative distribution function of the normal distribution */
double PHI(double x)
{
  char methode[]="gauss3";
  if (init_integration(methode, 0.1)==false)
  {
    return 0.0;
  }
  
  return 1.0/2.0+integrate_dx(phi, 0, x, pfa_dt, &pfaQF);
}

/* =====================================
   Finance function: price of an option 
*/

double CALL(double z0,double S0, double K,double mu,double sig, double T)
{
  return S0*exp(mu*T)*PHI(sig*sqrt(T)-z0)-K*PHI(-1*z0);
}

double PUT(double z0,double S0, double K,double mu,double sig, double T)
{
  return K*PHI(z0)-S0*exp(mu*T)*PHI(z0-sig*sqrt(T));
}
double optionPrice(Option* option)
{
  if (option==NULL)
  {
    return 0.0;
  }
  
  double z0=log(option->K/option->S0)-option->T*(option->mu-((option->sig)*(option->sig)/2.0));
  if (option->type==0)
  {
    return CALL(z0, option->S0, option->K, option->mu, option->sig, option->T);
  }
  else
  {
    return PUT(z0, option->S0, option->K, option->mu, option->sig, option->T);
  }
}



/* ===============================================*/
/* Insurance functions */

/* Probability density function (PDF) of variable X.
   X is the reimbursement in case of a claim from the client.
*/
double f(InsuredClient* client, double t)
{
  return (1 /(client->s*t))*phi((log(t)-client->m)/client->s);
}
double F(InsuredClient* client, double t)
{
  return PHI((log(x)-client->m)/client->s);
}
double clientPDF_X(InsuredClient* client, double x)
{
  if (x <= 0)
  {
    return 0.0;
  }
  return f(client,x);

  
}


/* Cumulative distribution function (CDF) of variable X.
   X is the reimbursement in case of a claim from the client.
*/
double clientCDF_X(InsuredClient* client, double x)
{
  if (client==NULL)
  {
    return 0.0;
  }
  

  return F(client,x);
}


/* Probability density function (PDF) of variable X1+X2.
   X1 and X2 are the reimbursements of the two claims from the client (assuming there are 
   two claims).
*/
double clientPDF_X1X2(InsuredClient* client, double x)
{
  if (client==NULL)
  {
    return 0.0;
  }
  integrate_dx(&fX1X2,0, x,pfa_dt, &(pfaQF));
  
  
}


/* Cumulative distribution function (CDF) of variable X1+X2.
   X1 and X2 are the reimbursements of the two claims from the client (assuming there are 
   two claims).
*/
double clientCDF_X1X2(InsuredClient* client, double x)
{
  if (client==NULL)
  {
    /* code */
  }
  
  
  return 0.0;
}



/* Cumulative distribution function (CDF) of variable S.
   Variable S is the sum of the reimbursements that the insurance company will pay to client.
*/
double integral_sa_mère(double x, )
double clientCDF_S(InsuredClient* client, double x)
{

  return 0.0;
}






