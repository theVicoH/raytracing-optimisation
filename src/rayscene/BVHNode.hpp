#pragma once
#include <vector>
#include "../raymath/AABB.hpp"
#include "../raymath/Ray.hpp"
#include "SceneObject.hpp"
#include "Intersection.hpp"

class BVHNode
{
private:
  AABB boundingBox;

  BVHNode* left;
  BVHNode* right;

  std::vector<SceneObject*> objects;

public:
  BVHNode();
  ~BVHNode();

  bool isLeaf() const;

  void build(std::vector<SceneObject*>& sceneObjects, int maxObjectsPerLeaf = 16, int maxDepth = 24);

  bool findClosestIntersection(Ray& r, Intersection& closest, CullingType culling);

  const AABB& getBoundingBox() const;

private:
  int findLongestAxis(const AABB& box) const;

  AABB calculateBoundingBox(const std::vector<SceneObject*>& objects) const;

  double distanceToAABB(const Ray& r, const AABB& box) const;
};
