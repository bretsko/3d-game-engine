#pragma once

#include "Color.h"
#include "Mesh/node.h"

struct Light_components {
    Color amb;
    Color diff;
    Color spec;
};

class Light {
public:
    Light();
    Light(Color amb, Color diff, Color spec, const node *rep);
    virtual ~Light();
    virtual void enable();
    virtual void disable();
    virtual void set_state(bool state);
    virtual bool get_state(void) const;
    virtual void set_ambient(const Color &ambient);
    virtual void set_diffuse(const Color &diffuse);
    virtual void set_specular(const Color &specular);
    virtual Color get_ambient() const;
    virtual Color get_diffuse() const;
    virtual Color get_specular() const;
    virtual vec3 get_dir(const vec3 &point) const = 0;
    virtual double get_dist(const vec3 &point) const = 0;
    virtual vec3 get_att() const = 0;
    const node *rep = nullptr;

protected:
    Light_components _comps;
    bool _enabled;
};

