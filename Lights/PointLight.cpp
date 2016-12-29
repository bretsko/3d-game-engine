
#include "PointLight.h"

PointLight::PointLight(Color amb, Color diff, Color spec, const node *rep,
                       vec3 att, vec3 pos)
    : Light(amb, diff, spec, rep), att(att), pos(pos) {}

void PointLight::set_pos(const vec3 &pos) { this->pos = pos; }

vec3 PointLight::get_dir(const vec3 &point) const { return pos - point; }

double PointLight::get_dist(const vec3 &point) const {
    return glm::distance(pos, point);
}

vec3 PointLight::get_att() const { return att; }
