/**
 * The input dataset is 1 to 10, and the function is y = x^3 which makes 
 * function output:
 * {1, 8, 27, 64, 125, 216, 343, 512, 729, 1000 }
 * 
 * The exact integral should be 2500 and the exact derivative at
 * each point should be:
 * { 3, 12, 27, 48, 75, 108, 147, 192, 243, 300 }
 * 
 * When the time delta is 1 unit, the numerical integral is 2524.500000 and
 * the numerical derivative at each point is:
 * { 1, 7, 19, 37, 61, 91, 127, 169, 217, 271 }
 * 
 * When the time delta is reduced from 1 unit to 0.001 unit, the
 * numerical integral becomes 2500.000025 and the numerical derivative
 * at each point becomes:
 * { 3, 11.99, 26.99, 47.99, 74.99, 107.98, 146.98, 191.98, 242.97, 299.97 }
 */

#include <stdio.h>
#include "ring_buffer.h"

/**
 * @brief This function calculates the integral of the provided dataset using
 * a modified trapezoidal method.
 * 
 * @param rb            Ring buffer containing the dataset
 * @param time_interval Time interval of the dataset
 * 
 * @return double       Integral of the dataset
 */
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

/**
 * @brief This function calculates the integral of the provided dataset using
 * the trapezoidal method.
 * 
 * @param rb            Ring buffer containing the dataset
 * @param time_interval Time interval of the dataset
 * 
 * @return double       Integral of the dataset
 */
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

/**
 * @brief This function calculates the derivative of a given dataset on a give
 * index point.
 * 
 * @param  rb            Ring buffer containing the dataset
 * @param  index         Index to take derivative for
 * @param  time_interval Time interval of the dataset
 * 
 * @return double        Derivative at index
 */
double calculate_derivative(const ring_buffer *rb, uint16_t index, double time_interval)
{
    double derivative = 0.0;

    double x_this = ring_buffer_get_item(rb, index);
    double x_last = ring_buffer_get_item(rb, index - 1);
    double delta =  x_this - x_last;
    derivative = delta / time_interval;

    return derivative;
}

/**
 * @brief This is the function for which integral and derivatives are calculated.
 * it can be changed to anything else for testing. Right now it is y = x^3
 * 
 * @param  x        Input of function
 * 
 * @return double   Output of function
 */
double f (double x)
{
    double y;

    y = (x * x * x);

    return y;
}

int main(void)
{
    const uint16_t sample_count      = 10;                                             // number of samples in the dataset
    const uint16_t resolution_factor = 3000;                                           // 1 = no resolution increase, 2 = double resolution and so on
    const double   time_const        = 1.0 / (double)resolution_factor;                // 1 = 1 sec, 0.1 = 100 msec, 0.001 = 1 msec
    const uint16_t buffer_size       = (uint16_t)((double)sample_count / time_const);  // calculated based on sample_count and time_const

    printf("\rCreating %d samples for %d resolution factor\n", buffer_size, resolution_factor);

    // create and initialize a ring buffer for storing the data points
    ring_buffer rb;
    ring_buffer_init(&rb, buffer_size);

    // fill up all the data points
    for (uint16_t i = 1; i <= rb.size; i++)
    {
        double number = (double)i * time_const;
        ring_buffer_add(&rb, f(number));
    }

    // calculate integral of the dataset
    double integral = calculate_integral(&rb, time_const);
    printf("\r integral: %f\n", integral);

    // calculate the derivative of the dataset at each point
    for (uint16_t i = 0; i < rb.size; i++)
    {
        double derivative = calculate_derivative(&rb, i, time_const);

        // this chunk is only for printing the right data, has no other use
        {
            uint16_t internal_sample = i + 1;
            if (internal_sample % resolution_factor == 0)
            {
                uint16_t sample = internal_sample / resolution_factor;
                printf("\r derivative at %5.2f: %6.2f\n", (double)sample, derivative);
            }
        }
    }

    return 0;
}