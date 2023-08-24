#ifndef HITTABLE_H
#define HITTABLE_H

#include "Ray.h"
#include "Interval.h"
class HitRecord{
public:
    Point3 p;
    Vec3 normal;
    double t;
    bool frontFace;

    void setFaceNormal(const Ray& ray,const Vec3& outwardNormal){
        frontFace = dot(ray.direction(), outwardNormal);
        normal = frontFace?outwardNormal:-outwardNormal;
    }


};

class Hittable{
public:
    virtual ~Hittable() = default;
    virtual bool hit(const Ray& ray,const Interval ray_t,HitRecord& rec) const = 0;
};
#endif