#include <stdlib.h>
#include "42.h"
#include <math.h>

#define h 0.00000001

static int n;
static double a;
static double b;

static double *x = NULL;
static double *f_x = NULL;
static double *c = NULL;
static double *d = NULL;
static double *a_1 = NULL;
static double *c_1 = NULL;
static double *d_1 = NULL;

static double (*f) (double);

// Первая производная
static double derivative(double x){
    return 0.5*(f(x + h) - f(x - h))/h;
}

// Вторая производная
static double second_derivative(double x){
    return 0.5*(derivative(x + h) - derivative(x - h))/h;
}

void create_arrays_2(void){
    x = new double[n];
    f_x = new double[n];
    c = new double[4 * (n - 1)];
    d = new double[n];
    a_1 = new double[n];
    c_1 = new double[n-1];
    d_1 = new double[n-1];
}

int method_2(int n_, double a_, double b_, double (*f_) (double))
{
    int i;
    double k;
    n = n_;
    a = a_;
    b = b_;
    f = f_;
    create_arrays_2();
    if (!(x && f_x && c && d && a_1 && c_1 && d_1))
        return 0;
    k = (b - a)/(n - 1);
    for (i = 0; i < n; i++){
        x[i] = a + i * k;
        f_x[i] = f(x[i]);
    }
    return 1;
}

void free2(void){
    if (x) {
        free(x);
    }
    if (f_x){
        free(f_x);
    }
    if (c){
        free(c);
    }
    if (d){
        free(d);
    }
    if (a_1){
        free(a_1);
    }
    if (c_1){
        free(c_1);
    }
    if (d_1){
        free(d_1);
    }
    x = nullptr;
    f_x = nullptr;
    c = nullptr;
    d = nullptr;
    a_1 = nullptr;
    c_1 = nullptr;
    d_1 = nullptr;
}

void delta_method_2(int n, double delta){
    f_x[n] = delta + f_x[n];
}

static void solsys(int n, double *a, double *c, double *d, double *b, double *x)
{
    int i;
    c[0] = c[0]/a[0];
    for (i = 1; i < n - 1; i++){
        a[i] -= d[i - 1] * c[i - 1];
        c[i] = c[i]/a[i];
    }
    a[n - 1] -= d[n - 2] * c[n - 2];
    x[0] = b[0] / a[0];
    for (i = 1; i < n; i++)
        x[i] = (b[i] - d[i - 1] * x[i - 1]) / a[i];
    for (i = n - 2; i >= 0; i--)
        x[i] -= c[i] * x[i + 1];
}

void coefficients_method_2(void)
{
    int i, j;
    double tmp1, tmp2;
    for (i = 1; i < n - 1; i++){
        tmp1 = (f_x[i] - f_x[i - 1]) / (x[i] - x[i - 1]);
        tmp2 = (f_x[i + 1] - f_x[i]) / (x[i + 1] - x[i]);
        c[i] = tmp1 * (x[i + 1] - x[i]) * 3 + tmp2 * (x[i] - x[i - 1]) * 3;
        a_1[i] = (x[i + 1] - x[i - 1]) * 2.0;
        c_1[i] = x[i] - x[i - 1];
        d_1[i - 1] = x[i + 1] - x[i];
    }
    a_1[0] = 2.0;
    c_1[0] = 1.0;
    a_1[n - 1] = 2.0;
    d_1[n - 2] = 1.0;
    tmp1 = (f_x[1] - f_x[0]) / (x[1] - x[0]);
    tmp2 = (f_x[n - 1] - f_x[n - 2]) / (x[n - 1] - x[n - 2]);
    c[0] = tmp1 * 3.0 - second_derivative(x[0]) * (x[1] - x[0]) * 0.5;
    c[n - 1] = tmp2 * 3.0 + second_derivative(x[n - 1]) * (x[n - 1] - x[n - 2]) * 0.5;
    solsys(n, a_1, c_1, d_1, c, d);
    j = 0;
    for (i = 0; i < n - 1; i ++){
        c[j + 0] = f_x[i];
        c[j + 1] = d[i];
        tmp2 = x[i + 1] - x[i];
        tmp1 = (f_x[i + 1] - f_x[i]) / tmp2;
        c[j + 2] = (3 * tmp1 - 2 * d[i] - d[i + 1]) / tmp2;
        tmp2 *= tmp2;
        c[j + 3] = (d[i] + d[i + 1] - 2 * tmp1) / tmp2;
        j += 4;
    }
}

double f_2(double t)
{
    int i;
    for (i = 0; i < n - 2; i++)
        if (t <= x[i + 1])
            break;
    return c[4 * i] + c[4 * i + 1] * (t - x[i]) +
           c[4 * i + 2] * (t - x[i]) * (t - x[i]) +
           c[4 * i + 3] * (t - x[i]) * (t - x[i]) * (t - x[i]);
}

