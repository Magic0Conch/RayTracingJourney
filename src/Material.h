#ifndef MATERIAL_H
#define MATERIAL_H

#include "RTWeekend.h"
#include "Hittable.h"
#include "Texture.h"
#include <cmath>
#include <memory>

class HitRecord;
class Material{

public:
    virtual ~Material() = default;
    virtual bool scatter(const Ray& r_in,const HitRecord& rec,Color3& attenuation,Ray& scattered) const = 0;
    virtual Color3 emitted(double u,double v,const Point3& p) const{
        return Color3(0,0,0);
    }
};

class Lambertian:public Material{
private:
    std::shared_ptr<Texture> m_albedo;
    // std::shared_ptr<Texture> m_mainTexture;
public:
    Lambertian(const Color3& albedo):m_albedo(std::make_shared<SolidColor>(albedo)){};
    Lambertian(const std::shared_ptr<Texture> checkerTexture):m_albedo(checkerTexture){};
    bool scatter(const Ray& rIn,const HitRecord& rec,Color3& attenuation,Ray& scattered) const override{
        auto scatterDirection = rec.normal + randomUnitVector();
        if(scatterDirection.nearZero()){
            scatterDirection = rec.normal;
        }
        scattered = Ray(rec.p,scatterDirection,rIn.time());
        attenuation = m_albedo->value(rec.u, rec.v,rec.p);
        return true;
    }

};

class Metal:public Material{
public:
    Metal(const Color3& albedo,const double fuzz):m_albedo(albedo),m_fuzz(fuzz<1?fuzz:1){};
    
    bool scatter(const Ray& rIn,const HitRecord& rec,Color3& attenuation,Ray& scattered) const override{
        Vec3 reflected = reflect(rIn.direction().normalized(), rec.normal);
        scattered = Ray(rec.p,reflected+m_fuzz*randomUnitVector(),rIn.time());
        attenuation = m_albedo;

        return dot(scattered.direction(), rec.normal)>0;
    }

private:
    Color3 m_albedo;
    double m_fuzz;
};

class Dielectric:public Material{
public:
    Dielectric(double indexOfRefraction):m_indexOfRefraction(indexOfRefraction){};
    
    bool scatter(const Ray& rIn,const HitRecord& rec,Color3& attenuation,Ray& scattered) const override{
        attenuation = Vec3(1,1,1);
        double refractionRatio = rec.frontFace?(1.0/m_indexOfRefraction):m_indexOfRefraction;
        Vec3 rInNormalized = rIn.direction().normalized();
        double cosTheta = fmin(dot(-rec.normal, rInNormalized),1.0);
        double sinTheta = sqrt(1-cosTheta*cosTheta);
        Vec3 direction;
        if(refractionRatio * sinTheta > 1.0 || reflectance(cosTheta, refractionRatio)>randomDouble()){
            direction = reflect(rInNormalized, rec.normal);
        } else {
            direction = refract(rInNormalized, rec.normal,refractionRatio);;
        }
        scattered = Ray(rec.p,direction,rIn.time());
        return true;
    }

private:
    double m_indexOfRefraction;
    static double reflectance(double cosine,double refIdx){
        auto r0 = (1-refIdx)/(1+refIdx);
        r0 = r0 * r0;
        return r0 + (1-r0)*pow((1-cosine),5);
    }
};

class DiffuseLight:public Material{
private:
    std::shared_ptr<Texture> m_emit;
public:
    DiffuseLight(std::shared_ptr<Texture> emit):m_emit(emit){}
    DiffuseLight(Color3 diffuseColor):m_emit(std::make_shared<SolidColor>(diffuseColor)){}
    bool scatter(const Ray& rIn,const HitRecord& rec,Color3& attenuation,Ray& scattered) const override{
        return false;
    }

    Color3 emitted(double u,double v,const Point3& p) const override{
        return m_emit->value(u, v, p);
    }
};

class Isotropic:public Material{
private:
    std::shared_ptr<Texture> m_albedo;
public:
    Isotropic(Color3 color):m_albedo(std::make_shared<SolidColor>(color)){};
    Isotropic(std::shared_ptr<Texture> texture):m_albedo(texture){};

    bool scatter(const Ray& rIn,const HitRecord& rec,Color3& attenuation,Ray& scattered) const override{
        scattered = Ray(rec.p,randomUnitVector(),rIn.time());
        attenuation = m_albedo->value(rec.u, rec.v, rec.p);
        return true;
    }
};
#endif

