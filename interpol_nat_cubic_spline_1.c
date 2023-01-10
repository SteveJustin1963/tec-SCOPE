 
#include <algorithm>
#include <vector>
#include <cmath>

using namespace std;

float interpolate_angle(float adc_val)
{
    // Data points
    vector<float> x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18};
    vector<float> y = {80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98};
    int n = x.size();
    vector<float> h, b, d, alpha, c(n), l(n), u(n), z(n);
    h.resize(n);
    b.resize(n);
    d.resize(n);
    alpha.resize(n);
    for (int i = 0; i < n - 1; i++)
        h[i] = x[i + 1] - x[i];
    for (int i = 1; i < n - 1; i++)
        alpha[i] = 3 * (y[i + 1] - y[i]) / h[i] - 3 * (y[i] - y[i - 1]) / h[i - 1];
    l[0] = 1;
    u[0] = 0;
    z[0] = 0;
    for (int i = 1; i < n - 1; i++) {
        l[i] = 2 * (x[i + 1] - x[i - 1]) - h[i - 1] * u[i - 1];
        u[i] = h[i] / l[i];
        z[i] = (alpha[i] - h[i - 1] * z[i - 1]) / l[i];
    }
    l[n - 1] = 1;
    c[n - 1] = 0;
    for (int i = n - 2; i >= 0; i--) {
        c[i] = z[i] - u[i] * c[i + 1];
        b[i] = (y[i + 1] - y[i]) / h[i] - h[i] * (c[i + 1] + 2 * c[i]) / 3;
        d[i] = (c[i + 1] - c[i]) / (3 * h[i]);
    }
    int k = 0;
    for (int i = 0; i < n - 1; i++) {
        if (adc_val >= x[i] && adc_val <= x[i + 1]) {
            k = i;
            break;
        }
    }
    float result = y[k] + (adc_val - x[k]) * b[k] + (adc_val - x[k]) * (adc_val - x[k]) * c[k] + (adc_val - x[k]) * (adc_val - x[k]) * (adc_val - x[k]) * d[k];
    return result;
}
