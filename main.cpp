#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <cmath>
#include <math.h>
using namespace std;

class Vector3
{
  public:
    float x;
    float y;
    float z;
};

//Holds array of its visible points relative to its position, and its position.
class WorldSpaceObject
{
  public:
    Vector3 position;
    vector<Vector3> points;
};

class Space
{
  public:
    vector<WorldSpaceObject> objects;
};

//Resolution is amount of points checked horizontally and vertically.
class Camera
{
  public:
    Vector3 position;
    pair<int, int> resolution;
};

WorldSpaceObject rotateObject(WorldSpaceObject object, float radians) {
  WorldSpaceObject objectout = object;
  for (int i = 0; i < objectout.points.size(); i++) {
    Vector3 point = objectout.points[i];
    point.x -= objectout.position.x;
    point.y -= objectout.position.y;
    int newx = round((point.x * cos(radians) - point.y * sin(radians)) + objectout.position.x);
    int newy = round((point.x * sin(radians) + point.y * cos(radians)) + objectout.position.y);
    point.x = newx;
    point.y = newy;
    objectout.points[i] = point;
  }
  
  return objectout;
}

//Y axis is forward for cameras- and view is projected along it.
vector<vector<bool> > render_camera(Camera camera, Space space) {
  vector<vector<bool> > renderedPixels = vector<vector<bool> >();
  for (int h = 0; h < camera.resolution.second; h++) {
    vector<bool> currentLayer = vector<bool>();
    currentLayer.resize(camera.resolution.first);
    for (int w = 0; w < camera.resolution.first; w++) {
      for (int i = 0; i < space.objects.size(); i++) {
        for (int j = 0; j < space.objects[i].points.size(); j++) {
          if (space.objects[i].points[j].x == camera.position.x + (w - camera.resolution.first / 2) && space.objects[i].points[j].z == camera.position.z + (h - camera.resolution.second / 2)) {
            currentLayer[w] = true;
          }
          else if (!currentLayer[w]) currentLayer[w] = false;
        }
      }
    }
    renderedPixels.push_back(currentLayer);
  }

  return renderedPixels;
}

int main() {
  Space space = Space();
  WorldSpaceObject object1 = WorldSpaceObject();
  object1.position = Vector3();
  object1.position.x = 0;
  object1.position.y = 0;
  object1.position.z = 0;
  Vector3 point1 = Vector3();
  point1.x = 0;
  point1.y = 0;
  point1.x = 0;
  object1.points.push_back(point1);
  //space.objects.push_back(object1);
  WorldSpaceObject cube = WorldSpaceObject();
  cube.position = Vector3();
  cube.position.x = 0;
  cube.position.y = 0;
  cube.position.z = 0;
  for (int i = 0; i < 8; i++) cube.points.push_back(Vector3());
  cube.points[0].x = -2;
  cube.points[0].y = -2;
  cube.points[0].z = -1;
  cube.points[1].x = 2;
  cube.points[1].y = -2;
  cube.points[1].z = -1;
  cube.points[2].x = -2;
  cube.points[2].y = 2;
  cube.points[2].z = -2;
  cube.points[3].x = 2;
  cube.points[3].y = 2;
  cube.points[3].z = -2;
  cube.points[4].x = -2;
  cube.points[4].y = -2;
  cube.points[4].z = 3;
  cube.points[5].x = 2;
  cube.points[5].y = -2;
  cube.points[5].z = 3;
  cube.points[6].x = -2;
  cube.points[6].y = 2;
  cube.points[6].z = 2;
  cube.points[7].x = 2;
  cube.points[7].y = 2;
  cube.points[7].z = 2;
  space.objects.push_back(cube);
  Camera cam = Camera();
  cam.position.x = 0;
  cam.position.y = 0;
  cam.position.z = 0;
  cam.resolution.first = 50;
  cam.resolution.second = 10;
  while(true) {
    for (int i = 0; i < 50; i++) {
      cout << endl;
    }
    space.objects[0] = rotateObject(space.objects[0], 0.5);
    //Vector3 newpoint = Vector3();
    //newpoint.x = (rand() % (cam.resolution.first) - (cam.resolution.first / 2));
    //newpoint.y = (rand() % (cam.resolution.first) - (cam.resolution.first / 2));
    //newpoint.z = (rand() % (cam.resolution.first) - (cam.resolution.first / 2));
    //space.objects[0].points.push_back(newpoint);
    vector<vector<bool> > pixels = render_camera(cam, space);
    for (int h = cam.resolution.second; h > 0; h--) {
      for (int w = 0; w < cam.resolution.first; w++) {
        if (pixels[h - 1][w]) cout << "*";
        else cout << " ";
      }
      cout << endl;
    }
    this_thread::sleep_for(chrono::milliseconds(50));
  }
}