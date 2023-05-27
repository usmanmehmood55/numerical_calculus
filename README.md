# Numerical Calculus

This code shows how numerical integrals and derivatives are taken of a given 
dataset. It is used as an example and a testing place for a relatively more 
complicated code, the [PID Controller](github.com/usmanmehmood55/pid_controller). 

The code starts with initializing the number of samples in the dataset, and the
resolution factor. The resolution factor dictates the time delta, and therefore
the accuracy of these calculations.

It then calculates the size for a ring buffer of an appropriate size.
```c
const uint16_t sample_count      = 10;
const uint16_t resolution_factor = 3000; 
const double   time_const        = 1.0 / (double)resolution_factor;
const uint16_t buffer_size       = (uint16_t)((double)sample_count / time_const);
```
It should be noted that specifically using a ring buffer instead of a usual 
buffer has no purpose in these calculations, but due to the application, it 
has to be used int the PID Controller project so it is being used here as 
well. 

The ring buffer is then initialized, and filled with values starting from
0 to the `sample_count`.
```c
ring_buffer rb;
ring_buffer_init(&rb, buffer_size);

for (uint16_t i = 1; i <= rb.size; i++)
{
    double number = (double)i * time_const;
    ring_buffer_add(&rb, f(number));
}
```

A function $y=f(x)$ is implemented as `double f (double x)` is also 
declared, upon which derivation and integration would be performed. In this 
code example it is set to $y=x^3$

## Explanation of Calculating Functions

### Derivative
Derivatives are taken using the simplest method possible
$$ \frac{d}{dt}x = \frac{\Delta x}{\Delta t} = \frac{x_t - x_{t-1}}{\Delta t} $$

This is implemented as
```c
double calculate_derivative(const ring_buffer *rb, uint16_t index, double time_interval)
{
    double derivative = 0.0;

    double x_this = ring_buffer_get_item(rb, index);
    double x_last = ring_buffer_get_item(rb, index - 1);
    double delta =  x_this - x_last;
    derivative = delta / time_interval;

    return derivative;
}
```

### Integral
Integrals can be taken using the typical trapezoidal method.
$$ \int_{a}^{b}{f(x) \space dx} = \frac{\Delta t}{2} \left\{ f(a) + 2 \sum_{i=a+1}^{b-1} f(x_i) + f(b) \right\} $$

This is implemented as
```c
double trapezoidal(const ring_buffer *rb, double time_interval)
{
    double integral = 0.0;

    double x_first = ring_buffer_get_item(rb, 0);

    double x_sum_between = 0.0;
    for (uint16_t i = 1; i < rb->size; i++)
    {
        double x_this = ring_buffer_get_item(rb, i);
        x_sum_between += x_this;
    }

    double x_last = ring_buffer_get_item(rb, rb->size);

    integral = (time_interval / 2.0) * (x_first + (2.0 * x_sum_between) + x_last);

    return integral;
}
```

However a more accurate integral is also taken using this modified method
$$ \int_{a}^{b}{f(x) \space dx} = \frac{\Delta t}{2} \space \sum_{i=a}^{b} \left\{  f(x_i) + f(x_{i-1}) \right\} $$

This is implemented as
```c
double calculate_integral(const ring_buffer *rb, double time_interval)
{
    double integral = 0.0;
    
    for (uint16_t i = 0; i < rb->size; i++)
    {
        double x_this = ring_buffer_get_item(rb, i);
        double x_last = ring_buffer_get_item(rb, i - 1);
        
        integral += (x_this + x_last);
    }

    integral *= time_interval;
    integral /= 2.0;
    
    return integral;
}
```

## Results

In this code example, the input data set is 1 to 10
$$ \left\{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 \right\}$$

The function used is
$$ y = x^3 $$

Which makes the function output dataset used for calculations
$$ \left\{ 1, 8, 27, 64, 125, 216, 343, 512, 729, 1000 \right\}$$

The exact integral should be $2500$, and the exact derivative at each point 
should be:
$$ \left\{ 3, 12, 27, 48, 75, 108, 147, 192, 243, 300 \right\}$$

When the time delta is 1 unit, the numerical integral is $2524.500000$ and the 
numerical derivative at each point is:
$$ \left\{ 1, 7, 19, 37, 61, 91, 127, 169, 217, 271 \right\}$$

When the time delta is reduced from 1 unit to 0.001 unit, the numerical 
integral becomes 2500.000025 and the numerical derivative at each point 
becomes:
$$ \left\{ 3, 11.99, 26.99, 47.99, 74.99, 107.98, 146.98, 191.98, 242.97, 299.97 \right\}$$
