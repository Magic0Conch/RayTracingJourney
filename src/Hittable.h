#ifndef HITTABLE_H
#define HITTABLE_H

#include "Ray.h"
#include "Interval.h"
#include <memory>

class Material;

class HitRecord{
public:
    Point3 p;
    Vec3 normal;
    std::shared_ptr<Material> material;
    double t;
    bool frontFace;

    void setFaceNormal(const Ray& ray,const Vec3& outwardNormal){
        frontFace = dot(ray.direction(), outwardNormal)<0;
        normal = frontFace?outwardNormal:-outwardNormal;
    }


};

class Hittable{
public:
    virtual ~Hittable() = default;
    virtual bool hit(const Ray& ray,const Interval ray_t,HitRecord& rec) const = 0;
};
#endif