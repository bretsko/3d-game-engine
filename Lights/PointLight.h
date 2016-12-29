#pragma once

#include "Light.h"

class PointLight : public Light {

public:

  PointLight(Color amb, Color diff, Color spec, const node *rep, vec3 att,
              vec3 pos);

  void set_pos(const vec3 &pos);

  vec3 get_dir(const vec3 &point) const override;

  double get_dist(const vec3 &point) const override;

  vec3 get_att() const override;

//private:

  vec3 att;
  vec3 pos;
};

