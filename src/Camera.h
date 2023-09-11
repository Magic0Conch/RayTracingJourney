#ifndef CAMERA_H
#define CAMERA_H
#include "RTWeekend.h"
#include "Color.h"
#include "Hittable.h"
#include "Material.h"

class Camera{
private:
    int m_imageHeight;
    Point3 m_center;
    Point3 m_pixel00Location;
    Vec3 pixelDeltaU;
    Vec3 pixelDeltaV;

    void initialize(){
        m_imageHeight = imageWidth/aspectRatio;
        m_imageHeight = m_imageHeight<1?1:m_imageHeight;
        m_center = Vec3(0,0,0);

        auto focalLength =1.0;
        auto viewportHeight = 2.0;
        auto viewportWidth = viewportHeight * (static_cast<double>(imageWidth)/m_imageHeight); //aspect ration?

        auto viewportU = Vec3(viewportWidth,0,0);
        auto viewportV = Vec3(0,-viewportHeight,0);

        pixelDeltaU = viewportU/imageWidth;
        pixelDeltaV = viewportV/m_imageHeight;

        auto viewportUpperLeft = m_center - Vec3(0,0,focalLength) - viewportU/2 - viewportV/2;
        m_pixel00Location = viewportUpperLeft + 0.5*(pixelDeltaU + pixelDeltaV);

    }

    Color3 rayColor(const Ray& ray,const int depth,const Hittable& world) const{
        HitRecord hitRec;
        if(depth<=0)
            return Color3(0,0,0);
        if(world.hit(ray, Interval(0.0001,infinity), hitRec)){
            Ray scattered;
            Color3 attenuation;
            if (hitRec.material->scatter(ray, hitRec,attenuation,scattered)) {            
                return attenuation*rayColor(scattered,depth-1, world);
            }
            return Color3(0,0,0);

        }
        Vec3 unitDirection = ray.direction().normalized();
        auto t = .5*(unitDirection.y()+1.); //0~1
        return (1.0-t)*Color3(1.,1.,1.) + t*Color3(.5,.7,1.); //mix
    }

    Vec3 pixelSampleSquare() const{
        auto px = -.5 + randomDouble();
        auto py = -.5 + randomDouble();
        return px * pixelDeltaU + py * pixelDeltaV;
    }

    Ray getRay(int i,int j) const{
        auto pixelCenter = m_pixel00Location + j*pixelDeltaU + i*pixelDeltaV;
        auto pixelSample = pixelCenter + pixelSampleSquare();
        auto rayDirection = pixelSample - m_center;
        Ray ray(m_center,rayDirection);
        return ray;
    }
public:
    double aspectRatio = 1.0;
    int imageWidth = 100;
    int samplesPerPixel = 10;
    int maxDepth = 100;

    void render(const Hittable& world){
        initialize();
        std::cout << "P3\n" << imageWidth << ' ' << m_imageHeight << "\n255\n";
        for (int i = 0; i<m_imageHeight; ++i) {
            std::cerr<<"\rScanlines remaining: "<<m_imageHeight - i-1<<' '<<std::flush;
            for (int j = 0; j<imageWidth; ++j) {
                
                
                Color3 pixelColor(0,0,0);
                for (int sample = 0; sample<samplesPerPixel; ++sample) {
                    Ray ray = getRay(i, j);
                    pixelColor+=rayColor(ray,maxDepth, world);
                }                
                writeColor(std::cout, pixelColor,samplesPerPixel);
            }
        }
        std::cerr << "\nDone.\n";
    }


};


#endif