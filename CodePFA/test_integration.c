/******************************************************/
/* Program to run and test the integration functions. */
/* Creation date: 31 July, 2025                       */
/******************************************************/

#include "integration.h"
#include "math.h"
#include "integration.c"

/* Test functions with known integrals */

/* f1(x) = x^2, integral from 0 to 1 = 1/3 */
double f1(double x) {
    return x * x;
}

/* f2(x) = sin(x), integral from 0 to π = 2 */
double f2(double x) {
    return sin(x);
}

/* f3(x) = e^x, integral from 0 to 1 = e - 1 */
double f3(double x) {
    return exp(x);
}

/* f4(x) = 1/(1+x^2), integral from 0 to 1 = π/4 */
double f4(double x) {
    return 1.0 / (1.0 + x * x);
}

/* f5(x) = sin(x^2), integral from -1 to 4 (test case from specifications) */
double f5(double x) {
    return sin(x * x);
}

void test_quadrature_formula(char* formula_name, double (*f)(double), 
                              double a, double b, double exact_value, int N)
{
    QuadFormula qf;
    
    if (!setQuadFormula(&qf, formula_name)) {
        printf("Error: Failed to set quadrature formula '%s'\n", formula_name);
        return;
    }
    
    double result = integrate(f, a, b, N, &qf);
    double error = fabs(result - exact_value);
    
    printf("%-10s: Result = %.10f, Exact = %.10f, Error = %.2e\n", 
           formula_name, result, exact_value, error);
}

void test_function(char* func_name, double (*f)(double), double a, double b, double exact_value)
{
    printf("\n=== Testing %s, integral from %.2f to %.2f (exact = %.10f) ===\n", 
           func_name, a, b, exact_value);
    
    int N = 100; // Number of subdivisions
    
    test_quadrature_formula("left", f, a, b, exact_value, N);
    test_quadrature_formula("right", f, a, b, exact_value, N);
    test_quadrature_formula("middle", f, a, b, exact_value, N);
    test_quadrature_formula("trapezes", f, a, b, exact_value, N);
    test_quadrature_formula("simpson", f, a, b, exact_value, N);
    test_quadrature_formula("gauss2", f, a, b, exact_value, N);
    test_quadrature_formula("gauss3", f, a, b, exact_value, N);
}

void test_convergence()
{
    printf("\n=== Testing convergence with increasing N ===\n");
    printf("Function: x^2 from 0 to 1 (exact = 0.333333333)\n\n");
    
    QuadFormula qf_trap, qf_simpson, qf_gauss3;
    setQuadFormula(&qf_trap, "trapezes");
    setQuadFormula(&qf_simpson, "simpson");
    setQuadFormula(&qf_gauss3, "gauss3");
    
    double exact = 1.0 / 3.0;
    
    printf("N\t\tTrapeze Error\tSimpson Error\tGauss3 Error\n");
    printf("--------------------------------------------------------\n");
    
    for (int N = 5; N <= 160; N *= 2) {
        double res_trap = integrate(f1, 0.0, 1.0, N, &qf_trap);
        double res_simp = integrate(f1, 0.0, 1.0, N, &qf_simpson);
        double res_gau3 = integrate(f1, 0.0, 1.0, N, &qf_gauss3);
        
        double err_trap = fabs(res_trap - exact);
        double err_simp = fabs(res_simp - exact);
        double err_gau3 = fabs(res_gau3 - exact);
        
        printf("%d\t\t%.2e\t%.2e\t%.2e\n", N, err_trap, err_simp, err_gau3);
    }
}

void test_integrate_dx()
{
    printf("\n=== Testing integrate_dx function ===\n");
    
    QuadFormula qf;
    setQuadFormula(&qf, "simpson");
    
    double exact = 1.0 / 3.0;
    
    printf("dx\t\tN computed\tResult\t\tError\n");
    printf("--------------------------------------------------------\n");
    
    for (double dx = 0.5; dx >= 0.0125; dx /= 2) {
        int N_computed = (int) round(fabs(1.0 - 0.0) / dx);
        if (N_computed == 0) N_computed = 1;
        
        double result = integrate_dx(f1, 0.0, 1.0, dx, &qf);
        double error = fabs(result - exact);
        
        printf("%.4f\t\t%d\t\t%.10f\t%.2e\n", dx, N_computed, result, error);
    }
}

int main()
{

    printf("TEST PROGRAM FOR NUMERICAL INTEGRATION METHODS\n");

    
    // Test 1: x^2 from 0 to 1
    test_function("f(x) = x^2", f1, 0.0, 1.0, 1.0/3.0);
    
    // Test 2: sin(x) from 0 to π
    test_function("f(x) = sin(x)", f2, 0.0, M_PI, 2.0);
    
    // Test 3: e^x from 0 to 1
    test_function("f(x) = e^x", f3, 0.0, 1.0, exp(1.0) - 1.0);
    
    // Test 4: 1/(1+x^2) from 0 to 1
    test_function("f(x) = 1/(1+x^2)", f4, 0.0, 1.0, M_PI / 4.0);
    
    // Test convergence
    test_convergence();
    
    // Test integrate_dx
    test_integrate_dx();
    
    printf("\n=== Test from specifications example ===\n");
    printf("Function: sin(t^2) from -1 to 4\n\n");
    
    QuadFormula qf;
    double I1, I2, I3;
    
    setQuadFormula(&qf, "trapezes");
    I1 = integrate(f5, -1.0, 4.0, 10, &qf);
    printf("Trapezes (N=10): I1 = %.10f\n", I1);
    
    setQuadFormula(&qf, "middle");
    I2 = integrate(f5, -1.0, 4.0, 10, &qf);
    printf("Middle (N=10):   I2 = %.10f\n", I2);
    
    setQuadFormula(&qf, "simpson");
    I3 = integrate_dx(f5, -1.0, 4.0, 0.1, &qf);
    printf("Simpson (dx=0.1, N=50): I3 = %.10f\n", I3);
    
    printf("\n=== Recommendations ===\n");
    printf("Based on the tests above:\n");
    printf("- For best accuracy: Use Simpson or Gauss3 with dx = 0.01\n");
    printf("- For good balance: Use Simpson with dx = 0.05\n");
    printf("- For speed: Use Gauss2 with dx = 0.1\n");
    
    return 0;
}
