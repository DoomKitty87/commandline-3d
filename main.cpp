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
    Vector3() {
      x = 0;
      y = 0;
      z = 0;
    }
    Vector3(float xin, float yin, float zin) {
      x = xin;
      y = yin;
      z = zin;
    }
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

WorldSpaceObject rotateObject(WorldSpaceObject object, float radians, char axis) {
  WorldSpaceObject objectout = object;
  for (int i = 0; i < objectout.points.size(); i++) {
    Vector3 point = objectout.points[i];
    if (axis == 'z') {
      point.x -= objectout.position.x;
      point.y -= objectout.position.y;
      float newx = (point.x * cos(radians) - point.y * sin(radians) + objectout.position.x);
      float newy = (point.x * sin(radians) + point.y * cos(radians) + objectout.position.y);
      point.x = newx;
      point.y = newy;
      objectout.points[i] = point;
    }
    else if (axis == 'x') {
      point.y -= objectout.position.y;
      point.z -= objectout.position.z;
      float newy = (point.y * cos(radians) - point.z * sin(radians) + objectout.position.y);
      float newz = (point.y * sin(radians) + point.z * cos(radians) + objectout.position.z);
      point.y = newy;
      point.z = newz;
      objectout.points[i] = point;
    }
    else if (axis == 'y') {
      point.x -= objectout.position.x;
      point.z -= objectout.position.z;
      float newx = (point.x * cos(radians) - point.z * sin(radians) + objectout.position.x);
      float newz = (point.x * sin(radians) + point.z * cos(radians) + objectout.position.z);
      point.x = newx;
      point.z = newz;
      objectout.points[i] = point;
    }
    
  }
  return objectout;
}

WorldSpaceObject generateCube(Vector3 position, float vertexdistance) {
  WorldSpaceObject obj = WorldSpaceObject();
  obj.points.push_back(Vector3(-vertexdistance, -vertexdistance, -vertexdistance));
  obj.points.push_back(Vector3(vertexdistance, -vertexdistance, -vertexdistance));
  obj.points.push_back(Vector3(-vertexdistance, vertexdistance, -vertexdistance));
  obj.points.push_back(Vector3(vertexdistance, vertexdistance, -vertexdistance));
  obj.points.push_back(Vector3(-vertexdistance, -vertexdistance, vertexdistance));
  obj.points.push_back(Vector3(vertexdistance, -vertexdistance, vertexdistance));
  obj.points.push_back(Vector3(-vertexdistance, vertexdistance, vertexdistance));
  obj.points.push_back(Vector3(vertexdistance, vertexdistance, vertexdistance));

  obj.position = position;
  return obj;
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
          if (round(space.objects[i].points[j].x) == camera.position.x + (w - camera.resolution.first / 2) && round(space.objects[i].points[j].z) == camera.position.z + (h - camera.resolution.second / 2)) {
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
  //WorldSpaceObject object1 = WorldSpaceObject();
  //object1.position = Vector3(0, 0, 0);
  //Vector3 point1 = Vector3(0, 0, 0);
  //object1.points.push_back(point1);
  //space.objects.push_back(object1);
  WorldSpaceObject cube = generateCube(Vector3(0, 0, 0), 5);
  space.objects.push_back(cube);
  Camera cam = Camera();
  cam.position = Vector3(0, 0, 0);
  cam.resolution.first = 50;
  cam.resolution.second = 20;
  while(true) {
    for (int i = 0; i < 50; i++) {
      cout << endl;
    }
    space.objects[0] = rotateObject(space.objects[0], 0.05, 'x');
    space.objects[0] = rotateObject(space.objects[0], 0.1, 'z');
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