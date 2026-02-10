/******************************************************/
/* Program to run and test the PFA functions.        */
/* Creation date: 31 July, 2025                       */
/******************************************************/

#include "pfa.h"

void test_normal_distribution()
{
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║          TEST 1: NORMAL DISTRIBUTION FUNCTIONS            ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
    // Test phi (density)
    printf("Testing phi (density of N(0,1)):\n");
    printf("  phi(0) = %.10f (expected ≈ 0.3989422804)\n", phi(0.0));
    printf("  phi(1) = %.10f (expected ≈ 0.2419707245)\n", phi(1.0));
    printf("  phi(-1) = %.10f (expected ≈ 0.2419707245)\n", phi(-1.0));
    
    // Test PHI (CDF)
    printf("\nTesting PHI (CDF of N(0,1)):\n");
    printf("  PHI(0) = %.10f (expected = 0.5)\n", PHI(0.0));
    printf("  PHI(1) = %.10f (expected ≈ 0.8413)\n", PHI(1.0));
    printf("  PHI(-1) = %.10f (expected ≈ 0.1587)\n", PHI(-1.0));
    printf("  PHI(2) = %.10f (expected ≈ 0.9772)\n", PHI(2.0));
    printf("  PHI(-2) = %.10f (expected ≈ 0.0228)\n", PHI(-2.0));
    
    // Verify symmetry: PHI(-x) = 1 - PHI(x)
    double x = 1.5;
    double phi_x = PHI(x);
    double phi_mx = PHI(-x);
    printf("\nSymmetry check: PHI(%.1f) + PHI(-%.1f) = %.10f (expected = 1.0)\n", 
           x, x, phi_x + phi_mx);
}

void test_option_pricing()
{
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║              TEST 2: OPTION PRICING (FINANCE)             ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
    // Example 1: Call option
    Option call;
    call.type = CALL;
    call.S0 = 100.0;    // Current price
    call.K = 100.0;     // Strike price (at the money)
    call.T = 1.0;       // 1 year expiry
    call.mu = 0.05;     // 5% expected return
    call.sig = 0.2;     // 20% volatility
    
    double call_price = optionPrice(&call);
    printf("CALL Option Pricing:\n");
    printf("  S0 = %.2f, K = %.2f, T = %.2f, μ = %.4f, σ = %.4f\n", 
           call.S0, call.K, call.T, call.mu, call.sig);
    printf("  Call Price = %.4f euros\n", call_price);
    
    // Example 2: Put option (same parameters)
    Option put = call;
    put.type = PUT;
    
    double put_price = optionPrice(&put);
    printf("\nPUT Option Pricing:\n");
    printf("  S0 = %.2f, K = %.2f, T = %.2f, μ = %.4f, σ = %.4f\n", 
           put.S0, put.K, put.T, put.mu, put.sig);
    printf("  Put Price = %.4f euros\n", put_price);
    
    // Example 3: In-the-money call
    Option call2;
    call2.type = CALL;
    call2.S0 = 110.0;   // Current price above strike
    call2.K = 100.0;    // Strike price
    call2.T = 0.5;      // 6 months
    call2.mu = 0.08;    // 8% expected return
    call2.sig = 0.25;   // 25% volatility
    
    double call2_price = optionPrice(&call2);
    printf("\nIn-the-money CALL Option:\n");
    printf("  S0 = %.2f, K = %.2f, T = %.2f, μ = %.4f, σ = %.4f\n", 
           call2.S0, call2.K, call2.T, call2.mu, call2.sig);
    printf("  Call Price = %.4f euros\n", call2_price);
    
    // Example 4: Out-of-the-money put
    Option put2;
    put2.type = PUT;
    put2.S0 = 110.0;    // Current price above strike
    put2.K = 100.0;     // Strike price
    put2.T = 0.5;       // 6 months
    put2.mu = 0.08;
    put2.sig = 0.25;
    
    double put2_price = optionPrice(&put2);
    printf("\nOut-of-the-money PUT Option:\n");
    printf("  S0 = %.2f, K = %.2f, T = %.2f, μ = %.4f, σ = %.4f\n", 
           put2.S0, put2.K, put2.T, put2.mu, put2.sig);
    printf("  Put Price = %.4f euros\n", put2_price);
}

void test_insurance()
{
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║              TEST 3: INSURANCE CALCULATIONS               ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
    // Create an insured client
    InsuredClient client;
    client.m = 7.0;     // μ parameter of log-normal distribution
    client.s = 1.5;     // σ parameter of log-normal distribution
    
    // Probabilities: p[0] = P(N=0), p[1] = P(N=1), p[2] = P(N=2)
    double probs[3] = {0.7, 0.25, 0.05}; // 70% no claims, 25% one claim, 5% two claims
    client.p = probs;
    
    printf("Client parameters:\n");
    printf("  Log-normal parameters: μ = %.2f, σ = %.2f\n", client.m, client.s);
    printf("  Claim probabilities: P(N=0) = %.2f, P(N=1) = %.2f, P(N=2) = %.2f\n", 
           probs[0], probs[1], probs[2]);
    
    // Test PDF and CDF of X (single claim)
    printf("\nSingle claim (X) - Log-Normal distribution:\n");
    double x_values[] = {1000.0, 5000.0};
    
    printf("  x\t\tPDF(X)\t\tCDF(X)\n");
    printf("  ------------------------------------------------\n");
    for (int i = 0; i < 2; i++) {
        double x = x_values[i];
        double pdf = clientPDF_X(&client, x);
        double cdf = clientCDF_X(&client, x);
        printf("  %.0f\t\t%.6f\t%.6f\n", x, pdf, cdf);
    }
    
    // Test PDF and CDF of X1+X2 (two claims) - SLOW!
    printf("\nTwo claims (X1+X2) - Testing one value only (slow):\n");
    printf("  x\t\tPDF(X1+X2)\tCDF(X1+X2)\n");
    printf("  ------------------------------------------------\n");
    double x = 2000.0;
    printf("  Computing PDF for x=%.0f...\n", x);
    double pdf = clientPDF_X1X2(&client, x);
    printf("  Computing CDF for x=%.0f...\n", x);
    double cdf = clientCDF_X1X2(&client, x);
    printf("  %.0f\t\t%.6f\t%.6f\n", x, pdf, cdf);
    
    // Test CDF of S (total reimbursement)
    printf("\nTotal reimbursement (S) - Cumulative Distribution:\n");
    printf("  x\t\tCDF(S)\t\tP(S ≤ x)\n");
    printf("  ------------------------------------------------\n");
    double s_values[] = {0.0, 1000.0, 5000.0};
    
    for (int i = 0; i < 3; i++) {
        double x = s_values[i];
        printf("  Computing CDF_S for x=%.0f...\n", x);
        double cdf_s = clientCDF_S(&client, x);
        printf("  %.0f\t\t%.6f\t%.2f%%\n", x, cdf_s, cdf_s * 100);
    }
    
    // Calculate some useful probabilities
    printf("\nUseful probabilities:\n");
    double p_leq_1000 = clientCDF_S(&client, 1000.0);
    double p_gt_1000 = 1.0 - p_leq_1000;
    
    printf("  P(S ≤ 1000) = %.4f (%.2f%%)\n", p_leq_1000, p_leq_1000 * 100);
    printf("  P(S > 1000) = %.4f (%.2f%%)\n", p_gt_1000, p_gt_1000 * 100);
}

void test_different_insurance_profiles()
{
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║          TEST 4: DIFFERENT INSURANCE PROFILES             ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
    // Profile 1: Low risk client
    InsuredClient low_risk;
    low_risk.m = 6.0;
    low_risk.s = 1.0;
    double probs_low[3] = {0.9, 0.08, 0.02}; // 90% no claims
    low_risk.p = probs_low;
    
    // Profile 2: Medium risk client
    InsuredClient medium_risk;
    medium_risk.m = 7.0;
    medium_risk.s = 1.5;
    double probs_med[3] = {0.7, 0.25, 0.05}; // 70% no claims
    medium_risk.p = probs_med;
    
    // Profile 3: High risk client
    InsuredClient high_risk;
    high_risk.m = 8.0;
    high_risk.s = 2.0;
    double probs_high[3] = {0.5, 0.35, 0.15}; // 50% no claims
    high_risk.p = probs_high;
    
    double thresholds[] = {1000.0, 3000.0, 5000.0, 10000.0};
    
    printf("Comparison of CDF_S for different risk profiles:\n");
    printf("x\t\tLow Risk\tMedium Risk\tHigh Risk\n");
    printf("----------------------------------------------------------------\n");
    
    for (int i = 0; i < 4; i++) {
        double x = thresholds[i];
        double cdf_low = clientCDF_S(&low_risk, x);
        double cdf_med = clientCDF_S(&medium_risk, x);
        double cdf_high = clientCDF_S(&high_risk, x);
        
        printf("%.0f\t\t%.4f\t\t%.4f\t\t%.4f\n", x, cdf_low, cdf_med, cdf_high);
    }
    
    printf("\nProbability of exceeding 5000 euros:\n");
    printf("  Low Risk:    %.2f%%\n", (1 - clientCDF_S(&low_risk, 5000.0)) * 100);
    printf("  Medium Risk: %.2f%%\n", (1 - clientCDF_S(&medium_risk, 5000.0)) * 100);
    printf("  High Risk:   %.2f%%\n", (1 - clientCDF_S(&high_risk, 5000.0)) * 100);
}

int main()
{
    // Initialize integration parameters
    // Use large dt (0.5) for faster computation
    // Note: For production use, smaller dt (0.01-0.1) would give better accuracy
    if (!init_integration("simpson", 0.5)) {
        printf("Error: Failed to initialize integration\n");
        return 1;
    }
    
    printf("Integration initialized: Simpson method with dt = 0.5\n");
    printf("NOTE: Using large dt for fast demonstration. Use smaller dt for accuracy.\n\n");
    
    // Run all tests
    test_normal_distribution();
    test_option_pricing();
    
    printf("\n[WARNING] Insurance tests with nested integrations may take some time...\n");
    test_insurance();
    
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                  ALL TESTS COMPLETED                      ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    return 0;
}