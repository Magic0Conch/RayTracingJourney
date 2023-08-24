#include <iostream>
#include "RTWeekend.h"
#include "Color.h"
#include "Sphere.h"
#include "HittableList.h"


Color3 rayColor(const Ray& ray,const HittableList& world){
    HitRecord hitRec;
    if(world.hit(ray, Interval(0,infinity), hitRec)){        
        return 0.5*(Color3(hitRec.normal)+Color3(1,1,1));
    }
    Vec3 unitDirection = ray.direction().normalized();
    auto t = .5*(unitDirection.y()+1.); //0~1
    return (1.0-t)*Color3(1.,1.,1.) + t*Color3(.5,.7,1.); //mix
}



int main(){
    //Image (screen)?
    const auto aspectRatio = 16.0/9.0;
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

    //Construct the world
    HittableList world;
    world.add(make_shared<Sphere>(Point3(0,0,-1), 0.5));
    world.add(make_shared<Sphere>(Point3(0,-100.5,-1), 100));

    //Render
    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";
    for (int i = imageHeight-1; i>=0; --i) {
        std::cerr<<"\rScanlines remaining: "<<i<<' '<<std::flush;
        for (int j = 0; j<imageWidth; ++j) {
            auto u = double(j)/(imageWidth-1);
            auto v = double(i)/(imageHeight-1);
            Point3 endPoint = lowerLeftCorner+u*horizontal+v*vertical;
            Ray ray(origin,endPoint-origin);
            Color3 pixelColor = rayColor(ray,world);            
            writeColor(std::cout, pixelColor);
        }
    }
    std::cerr << "\nDone.\n";
}