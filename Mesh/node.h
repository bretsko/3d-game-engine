#pragma once

#include "Common.h"
#include "Lights/Material.h"

class node {

public:
  void add_child(unique_ptr<node> &&child);
  virtual Color get_color(const vec3 &point) const = 0;
  virtual vec3 get_normal(const vec3 &point) const = 0;
  virtual material get_material() const = 0;
  virtual ~node();
  vector<const node *> children() const;
  void set_lcs(const mat4 &lcs);
  mat4 get_lcs() const;
  mat4 get_ilcs() const;

protected:
  mat4 _lcs;
  mat4 _ilcs;
  mat4 _tlcs;
  vector<unique_ptr<node> > _children;
};
