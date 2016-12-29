#pragma once

#include "Common.h"

struct Color
{
    // float r, g, b, a;
    vec4 rgba;
    double r, g, b, a;

    //    unsigned char r() const { return static_cast<unsigned char>(rgba.x * 255);
    //                            }

    //    unsigned char g() const { return static_cast<unsigned char>(rgba.y * 255); }

    //    unsigned char b() const { return static_cast<unsigned char>(rgba.z * 255); }

    //    unsigned char a() const { return static_cast<unsigned char>(rgba.w * 255); }

    explicit Color(void) : rgba({ 0, 0, 0, 1 }) {}

    //    Color(float r, float g, float b, float a = 1.0f) : r(r), g(g), b(b), a(a)
    //    {}

    explicit Color(double r, double g, double b, double a = 1)
        : rgba({ r, g, b, a }) {}

    explicit Color(const vec4 &rgba) : rgba(rgba) {}



    explicit Color(double val) : rgba({ val, val, val, 1 }) {}


    //    void Set(float r, float g, float b, float a = 1.0f)
    //    {
    //        this->r = r;
    //        this->g = g;
    //        this->b = b;
    //        this->a = a;
    //    }

    void Set(double r, double g, double b, double a = 1.0f)
    {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }



    float* operator & ()
    {
        return (float*)this;
    }

};

//Color Red = Color(1.0f, 0.0f, 0.0f);
//Color Green = Color(0.0f, 1.0f, 0.0f);
//Color Blue = Color(0.0f, 0.0f, 1.0f);
//Color Black = Color(0.0f, 0.0f, 0.0f);
//Color White = Color(1.0f, 1.0f, 1.0f);
//Color Orange = Color(1.0f, 0.43f, 0.0f);
//Color Pink = Color(1.0f, 0.0f, 0.95f);
//Color Yellow = Color(0.95f, 1.0f, 0.0f);
