#include <iostream>
#include <cmath>
#include <limits>
#include "Plane.hpp"
#include "../raymath/Vector3.hpp"

Plane::Plane(const Vector3& p, const Vector3& n) : point(p), normal(n)
{
}

Plane::~Plane()
{
}

void Plane::calculateBoundingBox()
{
  const double inf = std::numeric_limits<double>::infinity();
  Vector3 min(-inf, -inf, -inf);
  Vector3 max(inf, inf, inf);
  this->boundingBox = AABB(min, max);
}

bool Plane::intersects(Ray &r, Intersection &intersection, CullingType culling)
{

  float denom = r.GetDirection().dot(normal);

  // If denom == 0 - it is parallel to the plane
  // If denom > 0, it means plane is behind the ray
  if (denom > -0.000001)
  {
    return false;
  }

  float numer = (point - r.GetPosition()).dot(normal);
  float invDenom = 1.0f / denom;
  float t = numer * invDenom;

  intersection.Position = r.GetPosition() + (r.GetDirection() * t);
  intersection.Normal = normal;
  intersection.Mat = this->material;

  return true;
}