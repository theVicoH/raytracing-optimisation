#include <iostream>
#include <cmath>
#include <limits>
#include "Scene.hpp"
#include "Intersection.hpp"
#include "Mesh.hpp"
#include "Triangle.hpp"

Scene::Scene() : bvhRoot(nullptr), useBVH(true)
{
}

Scene::~Scene()
{
  for (int i = 0; i < objects.size(); ++i)
  {
    delete objects[i];
  }

  for (int i = 0; i < lights.size(); ++i)
  {
    delete lights[i];
  }

  if (bvhRoot != nullptr)
  {
    delete bvhRoot;
  }
}

void Scene::add(SceneObject *object)
{
  objects.push_back(object);
}

void Scene::addLight(Light *light)
{
  lights.push_back(light);
}

void Scene::prepare()
{
  for (int i = 0; i < objects.size(); ++i)
  {
    objects[i]->applyTransform();
    objects[i]->calculateBoundingBox();
  }

  if (useBVH)
  {
    std::vector<SceneObject*> allPrimitives;

    for (SceneObject* obj : objects)
    {
      Mesh* mesh = dynamic_cast<Mesh*>(obj);
      if (mesh != nullptr)
      {
        const std::vector<Triangle*>& triangles = mesh->getTriangles();
        for (Triangle* tri : triangles)
        {
          allPrimitives.push_back(tri);
        }
      }
      else
      {
        allPrimitives.push_back(obj);
      }
    }

    std::cout << "🌳 Building BVH with " << allPrimitives.size() << " primitives (triangles + objects)..." << std::endl;
    std::cout << "   Max objects per leaf: 16, Max depth: 24" << std::endl;
    if (bvhRoot != nullptr)
    {
      delete bvhRoot;
    }
    bvhRoot = new BVHNode();
    bvhRoot->build(allPrimitives, 16, 24);
    std::cout << "✅ BVH construction complete!" << std::endl;
  }
}

const std::vector<Light *>& Scene::getLights() const
{
  return lights;
}

bool Scene::closestIntersection(Ray &r, Intersection &closest, CullingType culling)
{
  if (useBVH && bvhRoot != nullptr)
  {
    return bvhRoot->findClosestIntersection(r, closest, culling);
  }

  Intersection intersection;

  double closestDistanceSquared = std::numeric_limits<double>::infinity();
  bool found = false;
  for (int i = 0; i < objects.size(); ++i)
  {
    if (!objects[i]->boundingBox.intersects(r))
    {
      continue;
    }

    if (objects[i]->intersects(r, intersection, culling))
    {

      double distSquared = (intersection.Position - r.GetPosition()).lengthSquared();
      if (distSquared < closestDistanceSquared)
      {
        intersection.Distance = std::sqrt(distSquared);
        closestDistanceSquared = distSquared;
        closest = intersection;
        found = true;
      }
    }
  }
  return found;
}

Color Scene::raycast(Ray &r, Ray &camera, int castCount, int maxCastCount)
{

  Color pixel;

  Intersection intersection;

  if (closestIntersection(r, intersection, CULLING_FRONT))
  {
    // Add the view-ray for convenience (the direction is normalised in the constructor)
    intersection.View = (camera.GetPosition() - intersection.Position).normalize();

    if (intersection.Mat != NULL)
    {
      pixel = pixel + (intersection.Mat)->render(r, camera, &intersection, this);

      // Reflect
      if (castCount < maxCastCount & intersection.Mat->cReflection > 0)
      {
        Vector3 reflectDir = r.GetDirection().reflect(intersection.Normal);
        Vector3 origin = intersection.Position + (reflectDir * COMPARE_ERROR_CONSTANT);
        Ray reflectRay(origin, reflectDir);

        pixel = pixel + raycast(reflectRay, camera, castCount + 1, maxCastCount) * intersection.Mat->cReflection;
      }
    }
  }

  return pixel;
}