#include <algorithm>
#include <limits>
#include <cmath>
#include "BVHNode.hpp"

BVHNode::BVHNode() : left(nullptr), right(nullptr)
{
}

BVHNode::~BVHNode()
{
  if (left != nullptr)
  {
    delete left;
  }
  if (right != nullptr)
  {
    delete right;
  }
}

bool BVHNode::isLeaf() const
{
  return (left == nullptr && right == nullptr);
}

const AABB& BVHNode::getBoundingBox() const
{
  return boundingBox;
}

int BVHNode::findLongestAxis(const AABB& box) const
{
  Vector3 size = box.getMax() - box.getMin();

  if (size.x > size.y && size.x > size.z)
  {
    return 0;
  }
  else if (size.y > size.z)
  {
    return 1;
  }
  else
  {
    return 2;
  }
}

AABB BVHNode::calculateBoundingBox(const std::vector<SceneObject*>& objects) const
{
  if (objects.empty())
  {
    return AABB();
  }

  const double inf = std::numeric_limits<double>::infinity();
  Vector3 min(inf, inf, inf);
  Vector3 max(-inf, -inf, -inf);

  for (SceneObject* obj : objects)
  {
    const AABB& objBox = obj->boundingBox;
    const Vector3& objMin = objBox.getMin();
    const Vector3& objMax = objBox.getMax();

    min.x = std::min(min.x, objMin.x);
    min.y = std::min(min.y, objMin.y);
    min.z = std::min(min.z, objMin.z);

    max.x = std::max(max.x, objMax.x);
    max.y = std::max(max.y, objMax.y);
    max.z = std::max(max.z, objMax.z);
  }

  return AABB(min, max);
}

void BVHNode::build(std::vector<SceneObject*>& sceneObjects, int maxObjectsPerLeaf, int maxDepth)
{
  if (sceneObjects.empty())
  {
    return;
  }

  this->boundingBox = calculateBoundingBox(sceneObjects);

  if (sceneObjects.size() <= maxObjectsPerLeaf || maxDepth <= 0)
  {
    this->objects = sceneObjects;
    return;
  }

  int axis = findLongestAxis(this->boundingBox);

  std::sort(sceneObjects.begin(), sceneObjects.end(),
    [axis](SceneObject* a, SceneObject* b) {
      Vector3 centerA = (a->boundingBox.getMin() + a->boundingBox.getMax()) * 0.5;
      Vector3 centerB = (b->boundingBox.getMin() + b->boundingBox.getMax()) * 0.5;

      if (axis == 0) return centerA.x < centerB.x;
      if (axis == 1) return centerA.y < centerB.y;
      return centerA.z < centerB.z;
    });

  size_t mid = sceneObjects.size() / 2;

  if (mid == 0 || mid == sceneObjects.size())
  {
    this->objects = sceneObjects;
    return;
  }

  std::vector<SceneObject*> leftObjects(sceneObjects.begin(), sceneObjects.begin() + mid);
  std::vector<SceneObject*> rightObjects(sceneObjects.begin() + mid, sceneObjects.end());

  this->left = new BVHNode();
  this->left->build(leftObjects, maxObjectsPerLeaf, maxDepth - 1);

  this->right = new BVHNode();
  this->right->build(rightObjects, maxObjectsPerLeaf, maxDepth - 1);
}

bool BVHNode::findClosestIntersection(Ray& r, Intersection& closest, CullingType culling)
{
  if (!this->boundingBox.intersects(r))
  {
    return false;
  }

  if (isLeaf())
  {
    Intersection intersection;
    double closestDistanceSquared = std::numeric_limits<double>::infinity();
    bool found = false;

    if (closest.Distance > 0)
    {
      closestDistanceSquared = closest.Distance * closest.Distance;
      found = true;
    }

    for (SceneObject* obj : objects)
    {
      if (!obj->boundingBox.intersects(r))
      {
        continue;
      }

      if (obj->intersects(r, intersection, culling))
      {
        double distSquared = (intersection.Position - r.GetPosition()).lengthSquared();
        if (distSquared < closestDistanceSquared)
        {
          closestDistanceSquared = distSquared;
          closest = intersection;
          found = true;
        }
      }
    }

    if (found)
    {
      closest.Distance = std::sqrt(closestDistanceSquared);
    }

    return found;
  }

  bool hitLeft = false;
  bool hitRight = false;

  double distLeft = (left != nullptr) ? distanceToAABB(r, left->boundingBox) : std::numeric_limits<double>::infinity();
  double distRight = (right != nullptr) ? distanceToAABB(r, right->boundingBox) : std::numeric_limits<double>::infinity();

  if (distLeft < distRight)
  {
    if (left != nullptr)
    {
      hitLeft = left->findClosestIntersection(r, closest, culling);
    }

    if (right != nullptr && (!hitLeft || distRight < closest.Distance))
    {
      hitRight = right->findClosestIntersection(r, closest, culling);
    }
  }
  else
  {
    if (right != nullptr)
    {
      hitRight = right->findClosestIntersection(r, closest, culling);
    }

    if (left != nullptr && (!hitRight || distLeft < closest.Distance))
    {
      hitLeft = left->findClosestIntersection(r, closest, culling);
    }
  }

  return hitLeft || hitRight;
}

double BVHNode::distanceToAABB(const Ray& r, const AABB& box) const
{
  const Vector3& origin = r.GetPosition();
  const Vector3& dir = r.GetDirection();
  const Vector3& bmin = box.getMin();
  const Vector3& bmax = box.getMax();

  double tmin = 0.0;
  double tmax = std::numeric_limits<double>::infinity();

  for (int i = 0; i < 3; ++i)
  {
    double invD = 1.0 / (i == 0 ? dir.x : (i == 1 ? dir.y : dir.z));
    double o = (i == 0 ? origin.x : (i == 1 ? origin.y : origin.z));
    double bmin_i = (i == 0 ? bmin.x : (i == 1 ? bmin.y : bmin.z));
    double bmax_i = (i == 0 ? bmax.x : (i == 1 ? bmax.y : bmax.z));

    double t0 = (bmin_i - o) * invD;
    double t1 = (bmax_i - o) * invD;

    if (invD < 0.0)
    {
      double temp = t0;
      t0 = t1;
      t1 = temp;
    }

    tmin = t0 > tmin ? t0 : tmin;
    tmax = t1 < tmax ? t1 : tmax;

    if (tmax < tmin)
    {
      return std::numeric_limits<double>::infinity();
    }
  }

  return tmin > 0 ? tmin : 0.0;
}
