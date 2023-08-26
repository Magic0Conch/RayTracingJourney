#ifndef RTWEEKEND_H
#define RTWEEKEND_H
#pragma once
#include <memory>
#include <limits>
#include <cmath>
#include <random>

using std::sqrt;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degreesToRadians(double degrees){
    return degrees * pi/180.0;
}

inline double randomDouble(){
    static std::uniform_real_distribution<double> distribution(0.0,1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double randomDouble(double min,double max){
    return min + (max - min) * randomDouble();
}

#include "Interval.h"
#include "Ray.h"
#include "Vec3.h"
#endif