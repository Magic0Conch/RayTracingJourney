#include <iostream>
#include "Color.h"
#include "Ray.h"

Color3 rayColor(const Ray& ray){
    Vec3 unitDirection = ray.direction().normalized();
    auto t = .5*(unitDirection.y()+1.); //0~1
    return (1.0-t)*Color3(1.,1.,1.) + t*Color3(.5,.7,1.); //mix
}

int main(){
    //Image (screen)?
    const double aspectRatio = 16.0/9.0;
    const int imageWidth = 400;
    const int imageHeight = static_cast<int>(imageWidth/aspectRatio);

    //Camera
    auto viewportHeight = 2.0;
    auto viewportWidth = aspectRatio * viewportHeight;
    auto focalLength = 1.0;

    auto origin = Point3(0,0,0);
    auto horizontal = Vec3(viewportWidth,0,0);
    auto vertical = Vec3(0,viewportHeight,0);
    auto lowerLeftCorner = origin - horizontal/2 - vertical/2 - Vec3(0,0,focalLength);

    //Render

    std::cout << "P3\n" << imageHeight << ' ' << imageHeight << "\n255\n";
    for (int i = imageHeight-1; i>=0; i--) {
        std::cerr<<"\rScanlines remaining: "<<i<<' '<<std::flush;
        for (int j = 0; j<imageWidth; j++) {
            auto u = j*1./(imageWidth-1);
            auto v = i*1./(imageHeight-1);
            Point3 endPoint = lowerLeftCorner+u*horizontal+v*vertical;
            Ray ray(origin,endPoint-origin);
            Color3 pixelColor = rayColor(ray);

            writeColor(std::cout, pixelColor);
        }
    }
    std::cerr << "\nDone.\n";
}

//为什么是条纹？