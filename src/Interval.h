#ifndef INTERVAL_H
#define INTERVAL_H
#include <limits>

// #include "RTWeekend.h"
extern const double infinity;
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

    double clamp(double x) const{
        if(x<min) return min;
        if(x>max) return max;
        return x;
    }

    static const Interval empty,universe;

};

const static Interval empty (+infinity,-infinity);
const static Interval universe(-infinity,+infinity);

#endif