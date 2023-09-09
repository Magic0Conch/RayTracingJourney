#ifndef SPHERE_H
#define SPHERE_H
#include "Ray.h"
#include "Hittable.h"
#include <cmath>
#include <memory>
class Sphere:public Hittable{
private:
    Vec3 m_center;
    double m_radius;
    std::shared_ptr<Material> m_material;
public:
    Sphere() = default;
    Sphere(const Point3& center,const double radius,std::shared_ptr<Material> material):m_center(center),m_radius(radius),m_material(material){}

    virtual bool hit(const Ray& ray,const Interval ray_t,HitRecord& hitRecord) const override{
        Vec3 oc = ray.origin()-m_center;
        auto a = ray.direction().length_squared(); 
        auto half_b = dot(oc, ray.direction());
        auto c =  oc.length_squared() - m_radius*m_radius;
        auto discriminant = half_b*half_b-a*c;
        auto sqrtDiscriminant = sqrt(discriminant);
        if(discriminant<0){
            return false;
        }
        auto root = (-half_b-sqrtDiscriminant)/a;
        if(!ray_t.surrounds(root)){
            root = (-half_b+sqrtDiscriminant)/a;
            if(!ray_t.surrounds(root)){
                return false;
            }
        }

        hitRecord.t = root;
        hitRecord.p = ray.at(hitRecord.t);
        hitRecord.setFaceNormal(ray, (hitRecord.p - m_center)/m_radius);
        hitRecord.material = m_material;
        return true;
    }


};
#endif