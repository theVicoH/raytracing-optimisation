#pragma once

#include <vector>
#include "../raymath/Ray.hpp"
#include "../raymath/Color.hpp"
#include "Light.hpp"
#include "SceneObject.hpp"
#include "BVHNode.hpp"

class Scene
{
private:
  std::vector<SceneObject *> objects;
  std::vector<Light *> lights;
  BVHNode* bvhRoot;

public:
  Scene();
  ~Scene();

  Color globalAmbient;
  bool useBVH;

  void add(SceneObject *object);
  void addLight(Light *light);
  const std::vector<Light *>& getLights() const;

  void prepare();
  Color raycast(Ray &r, Ray &camera, int castCount, int maxCastCount);

  bool closestIntersection(Ray &r, Intersection &closest, CullingType culling);
};
