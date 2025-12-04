#include <iostream>
#include "Light.hpp"
#include "../raymath/Vector3.hpp"
#include "../raymath/Color.hpp"

Light::Light(const Vector3& c) : center(c)
{
}

Light::~Light()
{
}

const Vector3& Light::GetPosition() const
{
  return center;
}
