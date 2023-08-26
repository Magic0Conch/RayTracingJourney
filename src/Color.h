#ifndef COLOR_H
#define COLOR_H
#include "Vec3.h"
#include <iostream>
#include "Interval.h"
inline void writeColor(std::ostream &out,Color3 pixelColor,int samplesPerPixel){
    auto r = pixelColor.x();
    auto g = pixelColor.y();
    auto b = pixelColor.z();

    auto scale = 1.0/samplesPerPixel;
    r*=scale;
    g*=scale;
    b*=scale;

    static const Interval intensity(0.0,0.999);
    out << static_cast<int>(256*intensity.clamp(r))  << ' ' << static_cast<int>(256*intensity.clamp(g)) << ' ' << static_cast<int>(256*intensity.clamp(b)) << '\n';
}
#endif