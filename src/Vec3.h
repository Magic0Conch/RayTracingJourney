#pragma once
#include <corecrt_math.h>
#ifndef VEC3_H
#define VEC3_H
#include <cmath>
#include <iostream>
#include <ostream>

extern double randomDouble();
extern double randomDouble(double,double);
class Vec3{
public:
    Vec3():e{0,0,0}{}
    Vec3(double e0,double e1,double e2):e{e0,e1,e2}{}
    double x() const{return e[0];}
    double y() const{return e[1];}
    double z() const{return e[2];}

    Vec3 operator-() const{return Vec3(-e[0],-e[1],-e[2]);}
    double operator[](int i) const{return e[i];}
    double& operator[](int i){return e[i];}

    Vec3& operator+=(const Vec3 &rhs){
        e[0]+=rhs[0];
        e[1]+=rhs[1];
        e[2]+=rhs[2];
        return *this;
    }

    Vec3& operator-=(const Vec3 &rhs){
        e[0]-=rhs[0];
        e[1]-=rhs[1];
        e[2]-=rhs[2];
        return *this;
    }

    Vec3& operator*=(const double &rhs){
        e[0]*=rhs;
        e[1]*=rhs;
        e[2]*=rhs;
        return *this;
    }

    Vec3& operator/=(const double &rhs){
        e[0]/=rhs;
        e[1]/=rhs;
        e[2]/=rhs;
        return *this;
    }

    double length_squared() const{
        return e[0]*e[0]+e[1]*e[1]+e[2]*e[2];
    }

    double length() const{
        return std::sqrt(length_squared());
    }

    Vec3 normalized() const{
        const double vecLength = length();
        return Vec3(e[0]/vecLength,e[1]/vecLength,e[2]/vecLength);
    }

    static Vec3 random(){
        return Vec3(randomDouble(),randomDouble(),randomDouble());
    }

    static Vec3 random(double min,double max){
        return Vec3(randomDouble(min,max),randomDouble(min,max),randomDouble(min,max));
    }

    bool nearZero() const{
        auto s = 1e-8;
        return fabs(e[0])<s&&fabs(e[1])<s&&fabs(e[2])<s;
    }
public:
    double e[3];
};

using Point3 = Vec3;
using Color3 = Vec3;

//util functions
inline std::ostream& operator<<(std::ostream &out,const Vec3 &rhs){
    return out << '(' << rhs.e[0] << ','<<rhs.e[1]<<','<<rhs.e[2]<< ')';
}
inline Vec3 operator+ (const Vec3 &lhs,const Vec3 &rhs){
    return Vec3(lhs[0]+rhs[0],lhs[1]+rhs[1],lhs[2]+rhs[2]);
}

inline Vec3 operator- (const Vec3 &lhs,const Vec3 &rhs){
    return Vec3(lhs[0]-rhs[0],lhs[1]-rhs[1],lhs[2]-rhs[2]);
}

inline Vec3 operator*(const Vec3 &lhs,const Vec3 &rhs){
    return Vec3(lhs[0]*rhs[0],lhs[1]*rhs[1],lhs[2]*rhs[2]);
}

inline Vec3 operator*(double lhs,const Vec3 &rhs){
    return Vec3(lhs*rhs[0],lhs*rhs[1],lhs*rhs[2]);
}

inline Vec3 operator*(const Vec3 &lhs,double rhs){
    return Vec3(lhs[0]*rhs,lhs[1]*rhs,lhs[2]*rhs);
}

inline Vec3 operator/ (const Vec3 &lhs,double t){
    return 1/t * lhs;
}

inline double dot(const Vec3 &lhs,const Vec3 &rhs){
    return  lhs[0]*rhs[0]+lhs[1]*rhs[1]+lhs[2]*rhs[2];
}

inline Vec3 cross(const Vec3 &lhs,const Vec3 &rhs){
    return Vec3(lhs[1]*rhs[2]-lhs[2]*rhs[1],
                lhs[2]*rhs[0]-lhs[0]*rhs[2],
                lhs[0]*rhs[1]-lhs[1]*rhs[0]);
}

inline Vec3 unit_vector(Vec3 v) {
    return v / v.length();
}

inline Vec3 randomInUnitSphere(){
    while (true) {
        auto p = Vec3::random(-1,1);
        if(p.length_squared()<1)
            return p;
    }
}

inline Vec3 randomUnitVector(){
    return randomInUnitSphere().normalized();
}

inline Vec3 randomOnHemisphere(const Vec3& normal){
    auto unitVector = randomUnitVector();
    if(dot(unitVector, normal)>0.0){
        return unitVector;
    }
    else{
        return -unitVector;
    }
}

inline Vec3 reflect(const Vec3& v,const Vec3& n){
    return v-2*dot(v, n)*n;
}

//n/n^'
inline Vec3 refract(const Vec3& uv,const Vec3& n,double etai_over_etat){
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    Vec3 r_out_perp =  etai_over_etat * (uv + cos_theta*n);
    Vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

#endif