#ifndef CAMERA_H
#define CAMERA_H
#include "RTWeekend.h"
#include "Color.h"
#include "Hittable.h"
#include "Material.h"
#include <cmath>

class Camera{
private:
    int m_imageHeight;
    Point3 m_center;
    Point3 m_pixel00Location;
    Vec3 m_pixelDeltaU;
    Vec3 m_pixelDeltaV;
    //axis of camera
    Vec3 m_u,m_v,m_w;
    //defocus
    Vec3 m_defocusDistU;    
    Vec3 m_defocusDistV;    


    void initialize(){
        m_imageHeight = imageWidth/aspectRatio;
        m_imageHeight = m_imageHeight<1?1:m_imageHeight;
        m_center = lookfrom;

        // auto focalLength =(lookfrom - lookat).length();
        auto theta = degreesToRadians(fovV);
        auto h = tan(theta/2);

        auto viewportHeight = h*focusDist*2;
        auto viewportWidth = viewportHeight * (static_cast<double>(imageWidth)/m_imageHeight); //aspect ratio?

        m_w = unit_vector(lookfrom - lookat);
        m_u = unit_vector(cross(vUp, m_w));//right
        m_v = unit_vector(cross(m_w,m_u));


        auto viewportU = viewportWidth * m_u;
        auto viewportV = viewportHeight * -m_v; 

        m_pixelDeltaU = viewportU/imageWidth;
        m_pixelDeltaV = viewportV/m_imageHeight;

        auto viewportUpperLeft = m_center - (focusDist*m_w) - viewportU/2 - viewportV/2;
        m_pixel00Location = viewportUpperLeft + 0.5*(m_pixelDeltaU + m_pixelDeltaV);

        auto defocusRadius = focusDist * tan(degreesToRadians(defocusAngle/2));
        m_defocusDistU = m_u * defocusRadius;
        m_defocusDistV = m_v * defocusRadius;
    }

    Color3 rayColor(const Ray& ray,const int depth,const Hittable& world) const{
        HitRecord hitRec;
        if(depth<=0)
            return Color3(0,0,0);
        if(!world.hit(ray, Interval(0.0001,infinity), hitRec))
            return background;
        Ray scattered;
        Color3 attenuation;
        Color3 colorFromEmission = hitRec.material->emitted(hitRec.u, hitRec.v, hitRec.p);

        if (!hitRec.material->scatter(ray, hitRec,attenuation,scattered)) {     
            return colorFromEmission;
        }
        Color3 colorFromScatter = attenuation*rayColor(scattered,depth-1, world);
        return colorFromEmission+colorFromScatter;
        // Vec3 unitDirection = ray.direction().normalized();
        // auto t = .5*(unitDirection.y()+1.); //0~1
        // return (1.0-t)*Color3(1.,1.,1.) + t*Color3(.5,.7,1.); //mix
    }

    Vec3 pixelSampleSquare() const{
        auto px = -.5 + randomDouble();
        auto py = -.5 + randomDouble();
        return px * m_pixelDeltaU + py * m_pixelDeltaV;
    }

    Point3 defocusDiskSample() const{
        auto p = randomInUnitDisk();
        return m_center + (p[0]*m_defocusDistU) + (p[1]*m_defocusDistV);
    }

    Ray getRay(int i,int j) const{
        auto pixelCenter = m_pixel00Location + j*m_pixelDeltaU + i*m_pixelDeltaV;
        auto pixelSample = pixelCenter + pixelSampleSquare();

        auto rayOrigin = (defocusAngle<=0)?m_center:defocusDiskSample();
        auto rayDirection = pixelSample - rayOrigin;
        auto rayTime = randomDouble(); //0~1
        Ray ray(rayOrigin,rayDirection,rayTime);
        return ray;
    }
public:
    double aspectRatio = 1.0;
    int imageWidth = 100;
    int samplesPerPixel = 10;
    int maxDepth = 100;
    double fovV = 90;

    Point3 lookfrom = Point3(0,0,-1);
    Point3 lookat = Point3(0,0,0);
    Vec3 vUp = Vec3(0,1,0);

    double defocusAngle = 0;
    double focusDist = 10;
    Color3 background;

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