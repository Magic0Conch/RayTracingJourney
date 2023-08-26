#pragma once
#ifndef RAY_H
#define RAY_H
#include "Vec3.h"

class Ray{
public:
    Ray(){}
    Ray(const Point3& origin,const Vec3& direction):m_origin(origin),m_direction(direction){}

    Point3 origin() const{return m_origin;}
    Vec3 direction() const{return m_direction;}
    Point3 at(double t) const{
        return m_origin + t*m_direction;
    }

private:
    Point3 m_origin;
    Vec3 m_direction;
};

#endif