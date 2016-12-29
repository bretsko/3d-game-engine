#include "Light.h"
//------------------ Properties ------------------------

//------------------ Utilities ------------------------
Light::Light() : _enabled(false) {}

Light::Light(Color amb, Color diff, Color spec, const node *rep)
    : _comps({ amb, diff, spec }), rep(rep) {}

Light::~Light() {}

void Light::enable() { _enabled = true; }

void Light::disable() { _enabled = false; }

void Light::set_state(bool state) { _enabled = state; }

bool Light::get_state(void) const { return _enabled; }

void Light::set_ambient(const Color &ambient) { _comps.amb = ambient; }

void Light::set_diffuse(const Color &diffuse) { _comps.diff = diffuse; }

void Light::set_specular(const Color &specular) { _comps.spec = specular; }

Color Light::get_ambient() const { return _comps.amb; }

Color Light::get_diffuse() const { return _comps.diff; }

Color Light::get_specular() const { return _comps.spec; }
