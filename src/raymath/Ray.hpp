#pragma once

#include <iostream>
#include "Vector3.hpp"

class Ray
{
private:
  Vector3 position;
  Vector3 direction;

public:
  Ray();
  Ray(const Vector3& pos, const Vector3& dir);
  ~Ray();

  const Vector3& GetPosition() const;
  void SetPosition(Vector3 &pos);

  const Vector3& GetDirection() const;
  void SetDirection(Vector3 &pos);

  friend std::ostream &operator<<(std::ostream &_stream, Ray &vec);
};
