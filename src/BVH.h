#ifndef BVH_H
#define BVH_H

#include "RTWeekend.h"
#include "Hittable.h"
#include "HittableList.h"
#include <algorithm>
#include <memory>
#include <vcruntime.h>
#include <vector>

class BVHNode:public Hittable{
private:
    std::shared_ptr<Hittable> m_left;
    std::shared_ptr<Hittable> m_right;
    AABB m_bbox;
    static bool boxCompare(const shared_ptr<Hittable> a,const shared_ptr<Hittable> b,int axisIndex){
        return a->boundingBox().axis(axisIndex).min<b->boundingBox().axis(axisIndex).min;
    }

    static bool boxXCompare(const shared_ptr<Hittable> a,const shared_ptr<Hittable> b){
        return boxCompare(a, b, 0);
    }
    static bool boxYCompare(const shared_ptr<Hittable> a,const shared_ptr<Hittable> b){
        return boxCompare(a, b, 1);
    }
    static bool boxZCompare(const shared_ptr<Hittable> a,const shared_ptr<Hittable> b){
        return boxCompare(a, b, 2);
    }
public:
    BVHNode(const HittableList& list):BVHNode(list.objects,0,list.objects.size()){}

    BVHNode(const std::vector<shared_ptr<Hittable>>& srcObjects,size_t start,size_t end){
        auto objects = srcObjects;
        int axis = randomInt(0,2);
        
        auto comparator = axis == 0? boxXCompare:axis==1?boxYCompare:boxZCompare;

        size_t objectSpan = end - start;
        if(objectSpan == 1){
            m_left = m_right = objects[start];
        }
        else if (objectSpan == 2) {
            if(comparator(objects[start],objects[start+1])){
                m_left = objects[start];
                m_right = objects[start+1];
            }
            else {
                m_left = objects[start+1];
                m_right = objects[start];                
            }
        }
        else {
            std::sort(objects.begin()+start,objects.begin()+end,comparator);
            auto mid = start + objectSpan/2;
            m_left = make_shared<BVHNode>(objects,start,mid);
            m_right = make_shared<BVHNode>(objects,mid,end);

        }
        m_bbox = AABB(m_left->boundingBox(),m_right->boundingBox());
    }

    bool hit(const Ray& r,Interval rayTime,HitRecord& rec) const override{
        if(!m_bbox.hit(r, rayTime))
            return false;
        bool hitLeft = m_left->hit(r,rayTime, rec);
        bool hitRight = m_right->hit(r,Interval(rayTime.min,hitLeft?rec.t:rayTime.max), rec);
        return hitLeft||hitRight;
    }

    AABB boundingBox() const override{return m_bbox;}
};

#endif