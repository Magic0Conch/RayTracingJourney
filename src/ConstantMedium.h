#ifndef CONSTANT_MEDIUM_H
#define CONSTANT_MEDIUM_H

#include "RTWeekend.h"

#include "Hittable.h"
#include "Material.h"
#include "Texture.h"
#include <corecrt_math.h>
#include <memory>

class ConstantMedium:public Hittable{
private:
    std::shared_ptr<Hittable> m_boundary;
    double m_negInvDensity;
    std::shared_ptr<Material> m_phaseFunction;
public:
    ConstantMedium(std::shared_ptr<Hittable> boundary,double density,std::shared_ptr<Texture> texture)
    :m_boundary(boundary),m_negInvDensity(-1.0/density),m_phaseFunction(std::make_shared<Isotropic>(texture)){}
    ConstantMedium(std::shared_ptr<Hittable> boundary,double density,Color3 color)
    :m_boundary(boundary),m_negInvDensity(-1.0/density),m_phaseFunction(std::make_shared<Isotropic>(color)){}

    bool hit(const Ray& ray,const Interval rayTime,HitRecord& rec) const override{
        const bool enaleDebug = false;
        const bool debugging = enaleDebug&&randomDouble()<0.00001; //???
        
        HitRecord rec1,rec2;
        //universe防止白底
        if(!m_boundary->hit(ray,Interval::universe,rec1)){
            return false;
        }
        //camera can't be in the media
        if(!m_boundary->hit(ray,Interval(rec1.t+0.0001,infinity),rec2)){
            return false;
        }

        if(debugging)
            std::clog << "\nray_tmin=" << rec1.t << ", ray_tmax=" << rec2.t << '\n';
        rec1.t = std::max(rec1.t,rayTime.min);
        rec2.t = std::min(rec2.t,rayTime.max);
        if(rec2.t<=rec1.t)
            return false;
        if(rec1.t<0)
            rec1.t = 0;

        auto rayLength = ray.direction().length();
        auto distanceInsideBoundary = (rec2.t-rec1.t)*rayLength;
        //ray marching
        auto hitDistance = log(randomDouble())*m_negInvDensity;
        
        if(hitDistance>distanceInsideBoundary)
            return false;
        //密度越大，越容易被挡在前面
        rec.t = rec1.t + hitDistance/rayLength;
        rec.p = ray.at(rec.t);
        if(debugging){
            std::clog << "hit_distance = " <<  hitDistance << '\n'
            << "rec.t = " <<  rec.t << '\n'
            << "rec.p = " <<  rec.p << '\n';
        }

        rec.normal = Vec3(0,0,0);
        rec.frontFace = true;
        rec.material = m_phaseFunction;
        return true;
    }

    virtual AABB boundingBox() const override{
        return m_boundary->boundingBox();
    }
};

#endif