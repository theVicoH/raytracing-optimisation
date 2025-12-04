#include <iostream>
#include <cmath>
#include "Sphere.hpp"
#include "../raymath/Vector3.hpp"

Sphere::Sphere(double r) : SceneObject(), radius(r)
{
}

Sphere::~Sphere()
{
}

void Sphere::applyTransform()
{
  Vector3 c;
  this->center = this->transform.apply(c);
}

bool Sphere::intersects(Ray &r, Intersection &intersection, CullingType culling)
{
  // Vector from ray origin to center of sphere
  Vector3 OC = center - r.GetPosition();

  // Project OC onto the ray (assuming ray direction is normalized)
  double opLength = OC.dot(r.GetDirection());

  // If the projection is negative, sphere is behind the ray origin
  if (opLength <= 0)
  {
    return false;
  }

  // P is the corner of the right-angle triangle formed by O-C-P
  Vector3 OP = r.GetDirection() * opLength;
  Vector3 P = r.GetPosition() + OP;

  // Is the length of CP greater than the radius of the circle ? If yes, no intersection!
  Vector3 CP = P - center;
  double distanceSquared = CP.lengthSquared();
  double radiusSquared = radius * radius;
  if (distanceSquared > radiusSquared)
  {
    return false;
  }

  // Calculate the exact point of collision: P1
  double a = sqrt(radiusSquared - distanceSquared);
  double t = opLength - a;
  Vector3 P1 = r.GetPosition() + (r.GetDirection() * t);

  // Pre-calculate some useful values for rendering
  intersection.Position = P1;
  intersection.Mat = this->material;
  intersection.Normal = (P1 - center).normalize();

  return true;
}
