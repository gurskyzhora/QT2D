#include <stdlib.h>
#include "22.h"

static int n;
static double a;
static double b;

static double *x = NULL;
static double *f_x = NULL;
static double *c = NULL;
static double *d = NULL;

static double (*f) (double);

void create_arrays_1(void){
    x = new double[n];
    f_x = new double[n];
    c = new double[4 * (n - 1)];
    d = new double[n];
}

int method_1(int n_, double a_, double b_, double (*f_) (double))
{
    int i;
    double k;

    n = n_;
    a = a_;
    b = b_;
    f = f_;
    create_arrays_1();

    if (!(x && f_x && c && d)) return 0;

    k = (b - a)/(n-1);
    for (i = 0; i < n; i++){
        x[i] = a + i * k;
        f_x[i] = f(x[i]);
    }

    return 1;
}

void free1(void){
    if (x){
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
    x = nullptr;
    f_x = nullptr;
    c = nullptr;
    d = nullptr;
}

void delta_method_1(int n, double delta){
    f_x[n] = delta + f_x[n];
}

void coefficients_method_1(void){
    int i, j;
    double f1, f2, f_1, f_2, tmp1, tmp2;

    for (i = 1; i < n - 1; i++) {
        tmp1 = (f_x[i] - f_x[i - 1]) / (x[i] - x[i - 1]);
        tmp2 = (f_x[i + 1] - f_x[i]) / (x[i + 1] - x[i]);
        d[i] = ((x[i + 1] - x[i]) * tmp1 + (x[i] - x[i - 1]) * tmp2) / (x[i + 1] - x[i - 1]);
    }
    tmp1 = x[0] - (x[1] - x[0]);
    f1 = f(tmp1);
    tmp2 = x[n - 1] + (x[n - 1] - x[n - 2]);
    f2 = f(tmp2);
    f_1 = (f_x[0] - f1) / (x[0] - tmp1);
    f_2 = (f_x[1] - f_x[0]) / (x[1] - x[0]);
    d[0] = ((x[0] - tmp1) * f_1 + (x[1] - x[0]) * f_2) / (x[1] - tmp1);
    f_1 = (f_x[n - 1] - f_x[n - 2]) / (x[n - 1] - x[n - 2]);
    f_2 = (f2 - f_x[n - 1]) / (tmp2 - x[n - 1]);
    d[n - 1] = ((tmp2 - x[n - 1]) * f_1 + (x[n - 1] - x[n - 2]) * f_2) / (tmp2 - x[n - 2]);
    j = 0;
    for (i = 0; i < n - 1; i++){
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

double f_1(double t){
    int i;
    for (i = 0; i < n - 2; i++) if (t <= x[i + 1]) break;
    return c[4 * i] + c[4 * i + 1] * (t - x[i]) +
        c[4 * i + 2] * (t - x[i]) * (t - x[i]) +
        c[4 * i + 3] * (t - x[i]) * (t - x[i]) * (t - x[i]);
}

