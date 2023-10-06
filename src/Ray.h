#pragma once
#ifndef RAY_H
#define RAY_H
#include "Vec3.h"

class Ray{
public:
    Ray(){}
    Ray(const Point3& origin,const Vec3& direction,const double time = 0.0):m_origin(origin),m_direction(direction),m_time(time){}

    Point3 origin() const{return m_origin;}
    Vec3 direction() const{return m_direction;}
    double time() const{return m_time;};
    Point3 at(double t) const{
        return m_origin + t*m_direction;
    }

private:
    Point3 m_origin;
    Vec3 m_direction;
    double m_time;
};

#endif