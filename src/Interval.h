#ifndef INTERVAL_H
#define INTERVAL_H
#include <limits>
#include <xutility>

// #include "RTWeekend.h"
extern const double infinity;
class Interval{
public:
    double min,max;
    Interval():min(+infinity),max(-infinity){}
    Interval(double min,double max):min(min),max(max){}
    Interval(const Interval& a,const Interval& b):min(fmin(a.min,b.min)),max(fmax(a.max,b.max)){}
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
    double size() const{
        return max - min;
    }
    Interval expand(double delta) const{
        auto padding = delta/2;
        return Interval(min-padding,max+padding);
    }
    static const Interval empty,universe;

};

const Interval Interval::empty (+infinity,-infinity);
const Interval Interval::universe(-infinity,+infinity);

Interval operator+(const Interval& interval,double offset){
    return Interval(interval.min + offset,interval.max + offset);
}

Interval operator+(double offset,const Interval& interval){
    return interval + offset;
}

#endif