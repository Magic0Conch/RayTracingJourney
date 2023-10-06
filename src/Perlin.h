#ifndef PERLIN_H
#define PERLIN_H
#include "RTWeekend.h"
#include <cmath>
#include <corecrt_math.h>

class Perlin{
private:
    static const int m_pointCount = 256;
    int * m_permX;
    int * m_permY;
    int * m_permZ;
    Vec3* m_ranvec;

    static void permute(int *p,int n){
        for(int i = n-1;i>0;i--){
            int target = randomInt(0, i);
            int tmp = p[i];
            p[i] = p[target];
            p[target] = tmp;
        }
    }

    static int* perlinGeneratePerm(){
        auto p = new int [m_pointCount];
        for (int i = 0; i<m_pointCount; i++) {
            p[i] = i;
        }
        permute(p,m_pointCount);
        return p;
    }

    static double trilinearInterp(double c[2][2][2],double u,double v,double w){
        auto accum = 0.0;
        for(int i= 0;i<2;i++){
            for (int j = 0; j<2; j++) {
                for (int k = 0; k<2; k++){
                    accum += (i*u + (1-i)*(1-u))*(j*v + (1-j)*(1-v))*(k*w+(1-k)*(1-w))*c[i][j][k];
                }
            }
        }
        return accum;
    }

    static double hermiteCubic(double val){
        return val*val*(3-2*val);
    }

    static double perlinInterp(Vec3 c[2][2][2],double x_f,double y_f,double z_f){
        auto accum = 0.0;
        for(int i= 0;i<2;i++){
            for (int j = 0; j<2; j++) {
                for (int k = 0; k<2; k++){
                    Vec3 weight(x_f-i,y_f-j,z_f-k);
                    accum += (i*x_f + (1-i)*(1-x_f))*(j*y_f + (1-j)*(1-y_f))*(k*z_f+(1-k)*(1-z_f))*dot(weight, c[i][j][k]);
                }
            }
        }
        return accum;
    }

public:
    Perlin(){
        m_ranvec = new Vec3[m_pointCount];
        for (int i = 0; i<m_pointCount; i++) {
            m_ranvec[i] = unit_vector(Vec3::random(-1,1));
        }
        m_permX = perlinGeneratePerm();
        m_permY = perlinGeneratePerm();
        m_permZ = perlinGeneratePerm();
    }

    ~Perlin(){
        delete [] m_permX;
        delete [] m_permY;
        delete [] m_permZ;
        delete [] m_ranvec;
    }

    double noise(const Point3& p) const{
        auto x_i = static_cast<int>(floor(p.x()));
        auto y_i = static_cast<int>(floor(p.y()));
        auto z_i = static_cast<int>(floor(p.z()));

        auto x_f = hermiteCubic(p.x() - floor(p.x()));
        auto y_f = hermiteCubic(p.y() - floor(p.y()));
        auto z_f = hermiteCubic(p.z() - floor(p.z()));

        Vec3 c[2][2][2];
        for(int i= 0;i<2;i++){
            for (int j = 0; j<2; j++) {
                for (int k = 0; k<2; k++){
                    c[i][j][k] = m_ranvec[m_permX[(x_i+i)&255]^m_permY[(y_i+j)&255]^m_permZ[(z_i+k)&255]];
                }
            }
        }

        return perlinInterp(c, x_f, y_f, z_f);
    }

    double turbe(const Point3& p,int depth = 7) const{ //fbm
        auto accum = 0.0;
        auto frequency = 1;
        auto amplitude = 1.0;
        for(int i = 0;i<depth;i++){
            accum += amplitude * noise(frequency*p); 
            amplitude*=0.5; //amplitude
            frequency*=2; //frequency
        }

        return fabs(accum);
    }
};


#endif