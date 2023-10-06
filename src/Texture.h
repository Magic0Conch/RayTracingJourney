#ifndef TEXTURE_H
#define TEXTURE_H

#include <cmath>
#include <corecrt_math.h>
#include <memory>
#include "rtw_stb_image.h"
#include "Perlin.h"

class Texture{
public:
    virtual ~Texture() = default;
    virtual Color3 value(double u,double v,const Point3& p) const = 0;
};

class SolidColor:public Texture{
private:
    Color3 m_colorValue;
public:
    SolidColor(Color3 colorValue):m_colorValue(colorValue){}
    SolidColor(double r,double g,double b):SolidColor(Color3(r,g,b)){}
    Color3 value(double u,double v,const Point3& p) const override{
        return m_colorValue;
    }

};


class CheckerTexture:public Texture{
private:
    double m_invScale;
    std::shared_ptr<Texture> m_even;
    std::shared_ptr<Texture> m_odd;

public:
    CheckerTexture(double scale,std::shared_ptr<Texture> even,std::shared_ptr<Texture> odd):
    m_invScale(1.0/scale),m_even(even),m_odd(odd){}
    CheckerTexture(double scale,Color3 color1,Color3 color2):
    m_invScale(1.0/scale),m_even(std::make_shared<SolidColor>(color1)),m_odd(std::make_shared<SolidColor>(color2)){}

    Color3 value(double u,double v,const Point3& p) const override{
        auto xInteger = static_cast<int>(std::floor(m_invScale*p.x()));
        auto yInteger = static_cast<int>(std::floor(m_invScale*p.y()));
        auto zInteger = static_cast<int>(std::floor(m_invScale*p.z()));

        bool isEven = (xInteger + yInteger + zInteger)%2==0;
        return isEven?m_even->value(u, v,p):m_odd->value(u, v, p);
    }
};

class ImageTexture:public Texture{
private:
    rtw_image m_image;
public:
    ImageTexture(const char* filename):m_image(filename){}
    Color3 value(double u,double v,const Point3& p) const override{
        if(m_image.height()<=0) return Color3(0,1,1);

        u = Interval(0,1).clamp(u);
        v = 1.0 - Interval(0,1).clamp(v);

        auto i = static_cast<int>(u * m_image.width());
        auto j  = static_cast<int>(v * m_image.height());
        auto pixel = m_image.pixel_data(i, j);

        auto colorScale = 1.0/255;
        return Color3(colorScale*pixel[0],colorScale*pixel[1],colorScale*pixel[2]);
    }
};

class NoiseTexture:public Texture{
private:
    Perlin m_noise;
    double m_scale;
public:
    NoiseTexture(){}
    NoiseTexture(double scale):m_scale(scale){}
    Color3 value(double u,double v,const Point3& p) const override{
        auto s = m_scale * p;
        // return Color3(1,1,1)*m_noise.turbe(s);
        return Color3(1,1,1)*0.5*(1+sin(s.z() + 10*m_noise.turbe(s)));
    }

};
#endif