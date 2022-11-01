#pragma once

#include <Arduino.h>
#include <BasicLinearAlgebra.h>
#include "defs.h"
#include "debug.h"

using namespace BLA;

class KalmanFilter
{
private:
    int altimeter_standard_deviation = 5;
    int altimeter_variance = sq(altimeter_standard_deviation);
    float accelerometer_standard_deviation = 1;
    float accelerometer_variance = sq(accelerometer_standard_deviation);
    // we assume initial acceleration in m/s^2 is equal to gravity
    float gravity = 9.81;
    float u = gravity;
    float T = 0.05;

    // using discrete noise model
    float q1 = (pow(T, 4) / 4);
    float q2 = (pow(T, 3) / 2);
    float q3 = q2;
    float q4 = pow(T, 2);

    // The state transition matrix
    Matrix<2, 2> F = {1.0, T, 0.0, 1.0};
    // The control matrix
    Matrix<2> G = {((float)0.5 * sq(T)), T};
    // Process noise matrix
    Matrix<2, 2> Q = {q1, q2, q3, q4};
    // Measurement uncertainty
    int R = {altimeter_variance};
    // initial guess for the state
    Matrix<2> x_hat = {0.0, 0.0};
    // since initial vector is a guess we set a very high estimate uncertainty
    Matrix<2, 2> P = {500, 0.0, 0.0, 500};
    // Identity Matrix
    Matrix<2, 2> I = {1.0, 0.0, 0.0, 1.0};
    // state observation matrix
    Matrix<1, 2> H = {1.0, 0.0};

public:
    KalmanFilter();
    void begin();
    struct FilteredValues update(SendValues sv);
};
