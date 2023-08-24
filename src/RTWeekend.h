#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <memory>
#include <limits>
#include <cmath>
#include "Interval.h"

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees){
    return degrees * pi/180.0;
}

#include "Ray.h"
#include "Vec3.h"
#endif