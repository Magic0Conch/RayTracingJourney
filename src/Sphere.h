#ifndef SPHERE_H
#define SPHERE_H
#include "Ray.h"
#include "Hittable.h"
#include <cmath>
#include <corecrt_math.h>
#include <memory>
class Sphere:public Hittable{
private:
    Vec3 m_center;
    double m_radius;
    std::shared_ptr<Material> m_material;
    bool m_isMoving;
    Vec3 m_centerVec;
    AABB m_bbox;
    Point3 getCenter(double time) const{
        //time from 0 to 1
        return m_center + time*m_centerVec;
    }
    static void getSphereUV(const Point3& p,double& u,double& v){
        auto theta = acos(-p.y());
        auto phi = atan2(-p.z(), p.x()) + pi;

        u = phi / (2*pi);
        v = theta / pi;
    }
public:
    Sphere() = default;
    Sphere(const Point3& center,const double radius,std::shared_ptr<Material> material)
    :m_center(center),m_radius(radius),m_material(material),m_isMoving(false){
        auto rvec = Vec3(radius,radius,radius);
        m_bbox = AABB(m_center-rvec,m_center+rvec);
    }
    Sphere(const Point3& center1,const Point3& center2,const double radius,std::shared_ptr<Material> material)
    :m_center(center1),m_radius(radius),m_material(material),m_isMoving(true){
        auto rvec = Vec3(radius,radius,radius);
        AABB box1(center1-rvec,center1+rvec);
        AABB box2(center2-rvec,center2+rvec);

        m_bbox = AABB(box1,box2);


        m_centerVec = center2 - center1;
    }

    virtual bool hit(const Ray& ray,const Interval ray_t,HitRecord& hitRecord) const override{
        Point3 center = m_isMoving?getCenter(ray.time()):m_center;
        Vec3 oc = ray.origin()-center;
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
        auto outwardNormal = (hitRecord.p - m_center)/m_radius;
        hitRecord.setFaceNormal(ray, outwardNormal);
        getSphereUV(outwardNormal, hitRecord.u, hitRecord.v);
        hitRecord.material = m_material;
        return true;
    }

    virtual AABB boundingBox() const override{
        return m_bbox;
    }

};
#endif