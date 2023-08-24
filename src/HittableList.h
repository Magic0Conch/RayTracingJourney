#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "Hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class HittableList:public Hittable{
public:
    std::vector<shared_ptr<Hittable>> objects;

    HittableList() = default;
    HittableList(shared_ptr<Hittable> object){

    }

    void clear(){
        objects.clear();
    }

    void add(shared_ptr<Hittable> object){
        objects.emplace_back(object);
    }

    bool hit(const Ray& ray,const Interval ray_t,HitRecord& record) const override{
        HitRecord tmpHitRecord;
        bool hitAnything = false;
        auto closet_so_far = ray_t.max;
        for (const auto object : objects) {
            if(object->hit(ray, Interval(ray_t.min, closet_so_far), tmpHitRecord)){
                hitAnything = true;
                closet_so_far = tmpHitRecord.t;
                record = tmpHitRecord;
            }
        }
        return hitAnything;
    }
};

#endif