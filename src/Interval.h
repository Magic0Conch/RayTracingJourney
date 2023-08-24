#ifndef INTERVAL_H
#define INTERVAL_H
#include <limits>

// #include "RTWeekend.h"
const double infinity = std::numeric_limits<double>::infinity();
class Interval{
public:
    double min,max;
    Interval():min(+infinity),max(-infinity){}
    Interval(double min,double max):min(min),max(max){}
    bool contains(double x) const{
        return min<=x&&x<=max;
    }

    bool surrounds(double x) const{
        return min<x && x<max;
    }

    static const Interval empty,universe;

};

const static Interval empty (+infinity,-infinity);
const static Interval universe(-infinity,+infinity);

#endif