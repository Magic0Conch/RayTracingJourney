#ifndef COLOR_H
#define COLOR_H
#include "Vec3.h"
#include <iostream>

inline void writeColor(std::ostream &out,Color3 pixelColor){
    int ir = static_cast<int>(255.999*pixelColor.x());
    int ig = static_cast<int>(255.999*pixelColor.y());
    int ib = static_cast<int>(255.999*pixelColor.z());
    out << ir << ' ' << ig << ' ' << ib << '\n';
}

#endif