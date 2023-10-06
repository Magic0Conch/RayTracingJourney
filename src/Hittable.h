#ifndef HITTABLE_H
#define HITTABLE_H

#include "Ray.h"
#include "Interval.h"
#include <cmath>
#include <memory>
#include "AABB.h"

class Material;

class HitRecord{
public:
    Point3 p;
    Vec3 normal;
    std::shared_ptr<Material> material;
    double t;
    bool frontFace;
    double u,v;
    void setFaceNormal(const Ray& ray,const Vec3& outwardNormal){
        frontFace = dot(ray.direction(), outwardNormal)<0;
        normal = frontFace?outwardNormal:-outwardNormal;
    }


};

class Hittable{
public:
    virtual ~Hittable() = default;
    virtual bool hit(const Ray& ray,const Interval ray_t,HitRecord& rec) const = 0;
    virtual AABB boundingBox() const = 0;
};

class Translate:public Hittable{
private:
    std::shared_ptr<Hittable> m_object;
    Vec3 m_offset;
    AABB m_bbox;
public:
    bool hit(const Ray& ray,const Interval rayTime,HitRecord& rec) const override{
        Ray offsetRay(ray.origin() - m_offset,ray.direction(),ray.time());
        if(!m_object->hit(offsetRay, rayTime, rec)){
            return false;
        }
        rec.p+=m_offset;
        return true;
    };

    virtual AABB boundingBox() const override{
        return m_bbox;;
    }

    Translate(std::shared_ptr<Hittable> object,const Vec3& offset):m_object(object),m_offset(offset){
        m_bbox = object->boundingBox() + m_offset;
    }
};

class RotateY:public Hittable{
private:
    std::shared_ptr<Hittable> m_object;
    AABB m_bbox;
    double m_sinTheta;
    double m_cosTheta;
public:
    bool hit(const Ray& ray,const Interval rayTime,HitRecord& rec) const override{
        auto origin = ray.origin();
        auto direction = ray.direction();

        origin[0] = m_cosTheta*ray.origin().x() - m_sinTheta*ray.origin().z();
        origin[2] = m_sinTheta*ray.origin().x() + m_cosTheta*ray.origin().z();
        direction[0] = m_cosTheta*ray.direction().x() - m_sinTheta*ray.direction().z();
        direction[2] = m_sinTheta*ray.direction().x() + m_cosTheta*ray.direction().z();

        Ray rotatedRay(origin,direction,ray.time());
        if(!m_object->hit(rotatedRay, rayTime, rec))
            return false;
        auto p = rec.p;
        p[0] = m_cosTheta*rec.p.x() + m_sinTheta*rec.p.z();
        p[2] = -m_sinTheta*rec.p.x() + m_cosTheta*rec.p.z();

        auto normal = rec.normal;
        normal[0] = m_cosTheta*rec.normal.x() + m_sinTheta*rec.normal.z();
        normal[2] = -m_sinTheta*rec.normal.x() + m_cosTheta*rec.normal.z();

        rec.p = p;
        rec.normal = normal;
        return true;
    };

    virtual AABB boundingBox() const override{
        return m_bbox;;
    }

    RotateY(std::shared_ptr<Hittable> object,double angle):m_object(object){
        auto radiance = degreesToRadians(angle);
        m_sinTheta = sin(radiance);
        m_cosTheta = cos(radiance);
        m_bbox = m_object->boundingBox();

        Point3 minPoint(infinity,infinity,infinity);
        Point3 maxPoint(-infinity,-infinity,-infinity);

        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                for (int k = 0; k < 2; k++) {
                    auto x = i * m_bbox.x.max + (1-i)*m_bbox.x.min;
                    auto y = j*m_bbox.y.max + (1-j)*m_bbox.y.min;
                    auto z = k*m_bbox.z.max + (1-k)*m_bbox.z.min;

                    auto newX = m_cosTheta*x + m_sinTheta*z;
                    auto newZ = -m_sinTheta*x + m_cosTheta*z;

                    Vec3 tester(newX,y,newZ);
                    for(int c= 0;c<3;c++){
                        minPoint[c] = fmin(minPoint[c],tester[c]);
                        maxPoint[c] = fmax(maxPoint[c],tester[c]);
                    }

                }
            }
        }
        m_bbox = AABB(minPoint,maxPoint);
    }
};
#endif