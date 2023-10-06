#ifndef AABB_H
#define AABB_H
#include "RTWeekend.h"
#include <algorithm>
#include <cmath>
#include <xutility>
class AABB{
public:
    Interval x,y,z;
    
    AABB() = default;
    AABB(const Interval& ix,const Interval& iy,const Interval& iz):x(ix),y(iy),z(iz){}
    AABB(const Point3& a,const Point3& b){
        x = Interval(fmin(a[0],b[0]), fmax(a[0],b[0]));
        y = Interval(fmin(a[1],b[1]), fmax(a[1],b[1]));
        z = Interval(fmin(a[2],b[2]), fmax(a[2],b[2]));
    }
    AABB(const AABB& a,const AABB& b){
        x = Interval(a.x, b.x);
        y = Interval(a.y,b.y);
        z = Interval(a.z,b.z);
    }

    AABB pad(){
        double delta = 1e-4;
        Interval nx = x.size()>=delta?x:x.expand(delta);
        Interval ny = y.size()>=delta?y:y.expand(delta);
        Interval nz = z.size()>=delta?z:z.expand(delta);
        return AABB(nx,ny,nz);
    }

    const Interval& axis(int n) const{
        switch(n){
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return z;
            default:
                return x;
        }
    }

    bool hit(const Ray& r,Interval rayTime) const{
        for (int a = 0; a<3; a++) {
            auto invD = 1.0/r.direction()[a];

            auto t0 = fmin((axis(a).min - r.origin()[a])*invD,
            (axis(a).max - r.origin()[a])*invD);
            auto t1 = fmax((axis(a).min - r.origin()[a])*invD,
            (axis(a).max - r.origin()[a])*invD);
            rayTime.min = fmax(t0,rayTime.min);
            rayTime.max = fmin(t1,rayTime.max);
            if(rayTime.max<=rayTime.min)
                return false;
        }
        return true;
    }

};

AABB operator+(const AABB& bbox,const Vec3& offset){
    return AABB(bbox.x + offset.x(),bbox.y + offset.y(),bbox.z + offset.z());
}

AABB operator+(const Vec3& offset,const AABB& bbox){
    return bbox+offset;
}

#endif