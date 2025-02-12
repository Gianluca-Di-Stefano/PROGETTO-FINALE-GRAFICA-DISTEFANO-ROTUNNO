#ifndef HITTABLE_H
#define HITTABLE_H

#include "interval.h"
#include "ray.h"
#include "material.h"

class hit_record {
public:
  float u;
  float v;
  point3 p;
  vec3 normal;
  float t;
  bool front_face;
  material* m;

  void set_face_normal(const ray& r, const vec3& outward_normal) {
    // Sets the hit record normal vector.
    // NOTE: the parameter `outward_normal` is assumed to have unit length.

    front_face = dot(r.direction(), outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
  }
};

class hittable {
public:
  virtual ~hittable() = default;

  virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
  virtual bool hit_shadow(const ray& r, interval ray_t) const = 0;
};

#endif

