#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>
#include "Camera.hpp"
#include "../raymath/Ray.hpp"

#ifdef ENABLE_THREADING
#include <thread>
#endif

struct RenderSegment
{
public:
  int rowMin;
  int rowMax;
  Image *image;
  double height;
  double intervalX;
  double intervalY;
  int reflections;
  Scene *scene;
};

Camera::Camera() : position(Vector3())
{
}

Camera::Camera(Vector3 pos) : position(pos)
{
}

Camera::~Camera()
{
}

Vector3 Camera::getPosition()
{
  return position;
}

void Camera::setPosition(Vector3 &pos)
{
  position = pos;
}

/**
 * Render a segment (set of rows) of the image
 */
void renderSegment(RenderSegment *segment)
{
  for (int y = segment->rowMin; y < segment->rowMax; ++y)
  {
    double yCoord = (segment->height / 2.0) - (y * segment->intervalY);

    for (int x = 0; x < segment->image->width; ++x)
    {
      double xCoord = -0.5 + (x * segment->intervalX);

      Vector3 coord(xCoord, yCoord, 0);
      Vector3 origin(0, 0, -1);
      Ray ray(origin, coord - origin);

      Color pixel = segment->scene->raycast(ray, ray, 0, segment->reflections);
      segment->image->setPixel(x, y, pixel);
    }
  }
}

void Camera::render(Image &image, Scene &scene)
{
  double ratio = (double)image.width / (double)image.height;
  double height = 1.0 / ratio;

  double intervalX = 1.0 / (double)image.width;
  double intervalY = height / (double)image.height;

  scene.prepare();

#ifdef ENABLE_THREADING
  std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << std::endl;
  std::cout << "🚀 MULTITHREADING MODE ENABLED" << std::endl;
  std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << std::endl;
  
  unsigned int nthreads = std::thread::hardware_concurrency();
  if (nthreads == 0) nthreads = 4;
  
  std::cout << "📊 System info:" << std::endl;
  std::cout << "  - CPU cores detected: " << nthreads << std::endl;
  std::cout << "  - Image dimensions: " << image.width << "x" << image.height << std::endl;
  std::cout << "  - Total pixels: " << (image.width * image.height) << std::endl;
  
  std::vector<std::thread> threads;
  std::vector<RenderSegment*> segments;
  
  int rowsPerThread = image.height / nthreads;
  int remainingRows = image.height % nthreads;

  std::cout << "\n📦 Image division:" << std::endl;
  std::cout << "  - Rows per thread: ~" << rowsPerThread << std::endl;
  
  int currentRow = 0;
  
  for (unsigned int i = 0; i < nthreads; ++i)
  {
    RenderSegment *seg = new RenderSegment();
    seg->height = height;
    seg->image = &image;
    seg->scene = &scene;
    seg->intervalX = intervalX;
    seg->intervalY = intervalY;
    seg->reflections = Reflections;
    seg->rowMin = currentRow;
    
    int rowsForThisThread = rowsPerThread + (i < remainingRows ? 1 : 0);
    seg->rowMax = currentRow + rowsForThisThread;
    
    std::cout << "  - Thread " << i << ": rows " << seg->rowMin << " to " 
              << (seg->rowMax - 1) << " (" << rowsForThisThread << " rows)" << std::endl;
    
    currentRow = seg->rowMax;
    
    segments.push_back(seg);
    threads.push_back(std::thread(renderSegment, seg));
  }

  std::cout << "\n⚡ Starting parallel rendering..." << std::endl;
  auto startTime = std::chrono::high_resolution_clock::now();
  
  for (auto& thread : threads)
  {
    thread.join();
  }
  
  auto endTime = std::chrono::high_resolution_clock::now();
  auto totalElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
  
  for (auto seg : segments)
  {
    delete seg;
  }
  
  std::cout << "✅ All threads completed successfully!" << std::endl;
  std::cout << "⏱️  Parallel rendering time: " << totalElapsed.count() << "ms" << std::endl;
  std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << std::endl;
  
#else
  std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << std::endl;
  std::cout << "🐌 SINGLE-THREADED MODE (No threading)" << std::endl;
  std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << std::endl;
  std::cout << "📊 Image info:" << std::endl;
  std::cout << "  - Dimensions: " << image.width << "x" << image.height << std::endl;
  std::cout << "  - Total pixels: " << (image.width * image.height) << std::endl;
  std::cout << "\n⚡ Starting sequential rendering..." << std::endl;
  
  RenderSegment *seg = new RenderSegment();
  seg->height = height;
  seg->image = &image;
  seg->scene = &scene;
  seg->intervalX = intervalX;
  seg->intervalY = intervalY;
  seg->reflections = Reflections;
  seg->rowMin = 0;
  seg->rowMax = image.height;
  
  renderSegment(seg);
  
  delete seg;
  
  std::cout << "✅ Sequential rendering completed!" << std::endl;
  std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << std::endl;
#endif
}

std::ostream &operator<<(std::ostream &_stream, Camera &cam)
{
  Vector3 pos = cam.getPosition();
  return _stream << "Camera(" << pos << ")";
}