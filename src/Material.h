#ifndef MATERIAL_H
#define MATERIAL_H

#include "RTWeekend.h"
#include "Hittable.h"
#include <cmath>

class HitRecord;
class Material{
public:
    virtual ~Material() = default;
    virtual bool scatter(const Ray& r_in,const HitRecord& rec,Color3& attenuation,Ray& scattered) const = 0;
};

class Lambertian:public Material{
public:
    Lambertian(const Color3& albedo):m_albedo(albedo){};
    
    bool scatter(const Ray& r_in,const HitRecord& rec,Color3& attenuation,Ray& scattered) const override{
        auto scatterDirection = rec.normal + randomUnitVector();
        if(scatterDirection.nearZero()){
            scatterDirection = rec.normal;
        }
        scattered = Ray(rec.p,scatterDirection);
        attenuation = m_albedo;
        return true;
    }

private:
    Color3 m_albedo;
};

class Metal:public Material{
public:
    Metal(const Color3& albedo,const double fuzz):m_albedo(albedo),m_fuzz(fuzz){};
    
    bool scatter(const Ray& r_in,const HitRecord& rec,Color3& attenuation,Ray& scattered) const override{
        Vec3 reflected = reflect(r_in.direction().normalized(), rec.normal);
        scattered = Ray(rec.p,reflected+m_fuzz*randomUnitVector());
        attenuation = m_albedo;

        return dot(scattered.direction(), rec.normal)>0;
    }

private:
    Color3 m_albedo;
    double m_fuzz;
};
#endif

