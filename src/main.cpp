#include <iostream>
#include "Color.h"
#include "Ray.h"
bool hitSphere(const Point3& center,double radius,const Ray& ray){
    Vec3 oc = ray.origin()-center;
    auto a = dot(ray.direction(), ray.direction());
    auto b = 2*dot(oc, ray.direction());
    auto c = dot(oc, oc) - radius*radius;
    auto discriminant = b*b-4*a*c;
    return discriminant>0;
}

Color3 rayColor(const Ray& ray){
    if(hitSphere(Point3(0,0,-1), 0.5, ray)){
        return Color3(1, 0, 0);
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

    //Render
    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";
    for (int i = imageHeight-1; i>=0; --i) {
        std::cerr<<"\rScanlines remaining: "<<i<<' '<<std::flush;
        for (int j = 0; j<imageWidth; ++j) {
            auto u = double(j)/(imageWidth-1);
            auto v = double(i)/(imageHeight-1);
            Point3 endPoint = lowerLeftCorner+u*horizontal+v*vertical;
            Ray ray(origin,endPoint-origin);
            Color3 pixelColor = rayColor(ray);            
            writeColor(std::cout, pixelColor);
        }
    }
    std::cerr << "\nDone.\n";
}