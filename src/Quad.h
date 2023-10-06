#ifndef QUAD_H
#define QUAD_H

#include "RTWeekend.h"
#include "HittableList.h"
#include <cmath>
#include <corecrt_math.h>
#include <memory>
class Quad:public Hittable{
private:
    Point3 m_Q; //left bottom corner of the quad
    Vec3 m_u; //right axis
    Vec3 m_v; //up axis
    std::shared_ptr<Material> m_material;
    AABB m_bbox;

    //plane function: m_normal·p = D
    Vec3 m_normal; 
    double m_D;
    Vec3 m_w; //scale

public:
    virtual void setBoundingBox(){
        m_bbox = AABB(m_Q,m_Q+m_u+m_v).pad();
    }

    Quad() = default;
    Quad(const Point3& Q,const Vec3& u,const Vec3& v,std::shared_ptr<Material> material)
    :m_Q(Q),m_u(u),m_v(v),m_material(material){
        auto n = cross(m_u, m_v);
        m_normal = n.normalized();
        m_D = dot(m_normal, m_Q);        
        m_w = n/dot(n, n);

        setBoundingBox();
    }

    virtual bool isInterior(double a,double b,HitRecord& rec) const{
        if((a<0)||(1<a)||(b<0)||(1<b))
            return false;
        rec.u = a;
        rec.v = b;
        return true;
    }

    virtual bool hit(const Ray& ray,const Interval ray_t,HitRecord& hitRecord) const override{
        auto denom = dot(m_normal, ray.direction());
        if(fabs(denom)<1e-8)
            return false;
        
        auto t = (m_D - dot(ray.origin(),m_normal))/denom;
        if(!ray_t.contains(t))
            return false;
        auto intersection = ray.at(t);
        Vec3 q2Intersection = intersection - m_Q;
        auto alpha = dot(m_w,cross(q2Intersection,m_v));
        auto beta =  dot(m_w,cross(m_u, q2Intersection));
        if(!isInterior(alpha,beta,hitRecord)){
            return false;
        }

        hitRecord.t = t;
        hitRecord.p = intersection;        
        hitRecord.setFaceNormal(ray, m_normal);
        hitRecord.material = m_material;
        return true;
    }

    virtual AABB boundingBox() const override{
        return m_bbox;
    }


};

inline std::shared_ptr<HittableList> box(const Point3& a,const Point3& b,shared_ptr<Material> material){
    auto sides = make_shared<HittableList>();
    auto min = Point3(fmin(a.x(), b.x()),fmin(a.y(), b.y()),fmin(a.z(), b.z()));
    auto max = Point3(fmax(a.x(), b.x()),fmax(a.y(), b.y()),fmax(a.z(), b.z()));
    auto dx = Vec3(max.x()-min.x(),0,0);
    auto dy = Vec3(0,max.y()-min.y(),0);
    auto dz = Vec3(0,0,max.z()-min.z());

    sides->add(make_shared<Quad>(Point3(min.x(),min.y(),min.z()),dx,dy,material)); //front 
    sides->add(make_shared<Quad>(Point3(min.x(),min.y(),max.z()),dy,dx,material)); //back
    sides->add(make_shared<Quad>(Point3(min.x(),max.y(),min.z()),dx,dz,material)); //top
    sides->add(make_shared<Quad>(Point3(min.x(),min.y(),min.z()),dz,dx,material)); //bottom
    sides->add(make_shared<Quad>(Point3(min.x(),min.y(),min.z()),dy,dz,material)); //left
    sides->add(make_shared<Quad>(Point3(max.x(),min.y(),min.z()),dz,dy,material)); //right
    return sides;
}
#endif