 
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
\\\\\\\\\\\\\\\\\\\\\\\\\\\
\\ forth 83 \\
: interpolate-angle ( adc-val -- angle )
    ( x and y data points )
    s" 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18" to x-values
    s" 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98" to y-values
    variable h
    variable b
    variable d
    variable alpha
    variable c
    variable l
    variable u
    variable z
    x-values count to n

    ( calculate h array )
    0 n 1- do i i 1+ x-values i fetch - h i store loop

    ( calculate alpha array )
    1 n 1- do i h i fetch h i 1- fetch * y-values i fetch y-values i 1- fetch - 3 * h i fetch h i 1- fetch * y-values i 1- fetch y-values i fetch - 3 * / alpha i store loop

    ( tridiagonal matrix algorithm )
    l 0 1 store
    u 0 store
    z 0 store
    1 n 1- do i l i 1- fetch h i 1- fetch * u i 1- fetch - 2 * h i fetch alpha i fetch h i 1- fetch * z i 1- fetch - / z i store u i fetch l i fetch / u i store loop
    l n 1- 1 store
    c n 1- 0 store
    n 1- do i c i 1+ fetch u i fetch * - c i store b i h i fetch * c i fetch c i 1+ fetch + 2 * 3 / - ( y[i+1]-y[i])/h[i] - ( h[i]/3 ) * d[i] = (c[i+1]-c[i])/3h[i]  store loop

    ( estimate angle at adc-val )
    0 n 1- do i x-values i fetch adc-val > if x-values i 1+ fetch adc-val < if
    i to k exit
    then then loop
    k x-values k fetch y-values k fetch - b k fetch * + k x-values k fetch adc-val - * k x-values k fetch adc-val - * k x-values k fetch adc-val - * d k fetch * + ;

\\\\\\\\\\\\\\\\\\\\\\\\\\
; Matlab has built-in functions such as interp1 which you can use for a cubic spline interpolation as well.
; Also you will need to replace the x and y arrays with your own data points.
;;;;;;; 

function angle = interpolate_angle(adc_val)
    % Data points
    x = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18];
    y = [80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98];
    n = length(x);
    h = zeros(1, n-1);
    b = zeros(1, n-1);
    d = zeros(1, n-1);
    alpha = zeros(1, n-1);
    c = zeros(1, n);
    l = zeros(1, n);
    u = zeros(1, n);
    z = zeros(1, n);
    
    % Calculate h array
    for i = 1:n-1
        h(i) = x(i+1) - x(i);
    end
    
    % Calculate alpha array
    for i = 2:n-1
        alpha(i) = (3*(y(i+1) - y(i))/h(i) - 3*(y(i) - y(i-1))/h(i-1));
    end
    
    % Tridiagonal matrix algorithm
    l(1) = 1;
    u(1) = 0;
    z(1) = 0;
    for i = 2:n-1
        l(i) = 2*(x(i+1) - x(i-1)) - h(i-1)*u(i-1);
        u(i) = h(i)/l(i);
        z(i) = (alpha(i) - h(i-1)*z(i-1))/l(i);
    end
    l(n) = 1;
    c(n) = 0;
    for i = n-1:-1:1
        c(i) = z(i) - u(i)*c(i+1);
        b(i) = (y(i+1) - y(i))/h(i) - h(i)*(c(i+1) + 2*c(i))/3;
        d(i) = (c(i+1) - c(i))/(3*h(i));
    end

    % Estimate angle at adc_val
    k = 0;
    for i = 1:n-1
        if adc_val >= x(i) && adc_val <= x(i+1)
            k = i;
            break;
        end
    end
    angle = y(k) + (adc_val - x(k))*b(k) + (adc_val - x(k))^2*c(k) + (adc_val - x(k))^3*d(k);
end
;;;;;
