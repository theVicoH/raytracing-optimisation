#pragma once

#include <iostream>

#define COMPARE_ERROR_CONSTANT 0.000001

class Vector3
{
private:
public:
  double x = 0;
  double y = 0;
  double z = 0;

  Vector3();
  Vector3(double x, double y, double z);
  ~Vector3();

  // Inline operators for performance
  inline const Vector3 operator+(Vector3 const &vec) const {
    return Vector3(x + vec.x, y + vec.y, z + vec.z);
  }

  inline const Vector3 operator-(Vector3 const &vec) const {
    return Vector3(x - vec.x, y - vec.y, z - vec.z);
  }

  inline const Vector3 operator*(double const &f) const {
    return Vector3(x * f, y * f, z * f);
  }

  const Vector3 operator/(double const &f) const;
  Vector3 &operator=(Vector3 const &vec);

  double length() const;

  // Inline hot functions for performance
  inline double lengthSquared() const {
    return (x * x + y * y + z * z);
  }

  const Vector3 normalize() const;

  inline double dot(Vector3 const &vec) const {
    return (x * vec.x + y * vec.y + z * vec.z);
  }

  const Vector3 projectOn(Vector3 const &vec) const;
  const Vector3 reflect(Vector3 const &normal) const;
  const Vector3 cross(Vector3 const &vec) const;
  const Vector3 inverse() const;

  friend std::ostream &operator<<(std::ostream &_stream, Vector3 const &vec);
};
