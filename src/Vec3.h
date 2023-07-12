#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>
#include <ostream>

class Vec3{
public:
    Vec3():m_e{0,0,0}{}
    Vec3(double e0,double e1,double e2):m_e{e0,e1,e2}{}
    double x() const{return m_e[0];}
    double y() const{return m_e[1];}
    double z() const{return m_e[2];}

    Vec3 operator-() const{return Vec3(-m_e[0],-m_e[1],-m_e[2]);}
    double operator[](int i) const{return m_e[i];}
    double& operator[](int i){return m_e[i];}

    Vec3& operator+=(const Vec3 &rhs){
        m_e[0]+=rhs[0];
        m_e[1]+=rhs[1];
        m_e[2]+=rhs[2];
        return *this;
    }

    Vec3& operator-=(const Vec3 &rhs){
        m_e[0]-=rhs[0];
        m_e[1]-=rhs[1];
        m_e[2]-=rhs[2];
        return *this;
    }

    Vec3& operator*=(const double &rhs){
        m_e[0]*=rhs;
        m_e[1]*=rhs;
        m_e[2]*=rhs;
        return *this;
    }

    Vec3& operator/=(const double &rhs){
        m_e[0]/=rhs;
        m_e[1]/=rhs;
        m_e[2]/=rhs;
        return *this;
    }

    double length_squared() const{
        return m_e[0]*m_e[0]+m_e[1]*m_e[1]+m_e[2]*m_e[2];
    }

    double length() const{
        return std::sqrt(length_squared());
    }

    Vec3 normalized() const{
        const double vecLength = length();
        return Vec3(m_e[0]/vecLength,m_e[1]/vecLength,m_e[2]/vecLength);
    }

private:
    double m_e[3];
};

using Point3 = Vec3;
using Color3 = Vec3;

//util functions
inline std::ostream& operator<<(std::ostream &out,const Vec3 &rhs){
    return out << '(' << rhs[0] << ','<<rhs[1]<<','<<rhs[2]<< ')';
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
#endif