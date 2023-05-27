#include <iostream>
#include <vector>
#include <cmath>
#include <math.h>
#ifdef _WINDOWS
#include <windows.h>
#else
#include<unistd.h>
#define Sleep(x) usleep((x) * 1000)
#endif
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
    vector<int> triangles;
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

vector<int> windTrianglesOrdered(vector<Vector3> vertices) {
  vector<int> triangles = vector<int>();
  triangles.resize(vertices.size() * 3);
  for (int i = 0; i < vertices.size(); i++) {
    triangles[i * 3] = i;
    triangles[i * 3 + 1] = i + 1;
    triangles[i * 3 + 2] = i + 2;
  }

  return triangles;
}

vector<int> windTrianglesClosest(vector<Vector3> vertices) {
  vector<int> triangles = vector<int>();
  triangles.resize(vertices.size() * 3);

  for (int i = 0; i < vertices.size(); i++) {
    triangles[i * 3] = i;
    float smallestDist = INFINITY;
    int closestPoint = 0;
    for (int j = 0; j < vertices.size(); j++) {
      if (j == i) continue;
      float xdist = abs(vertices[i].x - vertices[j].x);
      float ydist = abs(vertices[i].y - vertices[j].y);
      float zdist = abs(vertices[i].z - vertices[j].z);
      if (xdist + ydist + zdist < smallestDist) {
        smallestDist = xdist + ydist + zdist;
        closestPoint = j;
      }
    }
    smallestDist = INFINITY;
    float closestPoint2 = 0;
    for (int j = 0; j < vertices.size(); j++) {
      if (j == i) continue;
      if (j == closestPoint) continue;
      float xdist = abs(vertices[i].x - vertices[j].x);
      float ydist = abs(vertices[i].y - vertices[j].y);
      float zdist = abs(vertices[i].z - vertices[j].z);
      if (xdist + ydist + zdist < smallestDist) {
        smallestDist = xdist + ydist + zdist;
        closestPoint2 = j;
      }
    }

    triangles[i * 3 + 1] = closestPoint;
    triangles[i * 3 + 2] = closestPoint2;
  }

  return triangles;
}

vector<int> windTrianglesClosestRedundant(vector<Vector3> vertices) {
  vector<int> triangles = vector<int>();
  for (int i = 0; i < vertices.size(); i++) {
    float smallestDist = INFINITY;
    vector<int> closestPoints = vector<int>();
    for (int j = 0; j < vertices.size(); j++) {
      if (j == i) continue;
      float xdist = abs(vertices[i].x - vertices[j].x);
      float ydist = abs(vertices[i].y - vertices[j].y);
      float zdist = abs(vertices[i].z - vertices[j].z);
      if (xdist + ydist + zdist < smallestDist) {
        smallestDist = xdist + ydist + zdist;
        closestPoints = vector<int>();
        closestPoints.push_back(j);
      }
      else if (xdist + ydist + zdist == smallestDist) {
        closestPoints.push_back(j);
      }
    }
    for (int j = 0; j < closestPoints.size(); j++) {
      float smallestDistN = INFINITY;
      vector<int> closestPointsN = vector<int>();
      for (int k = 0; k < vertices.size(); k++) {
        if (k == i || k == closestPoints[j]) continue;
        float xdist = abs(vertices[i].x - vertices[k].x) + abs(vertices[closestPoints[j]].x - vertices[k].x);
        float ydist = abs(vertices[i].y - vertices[k].y) + abs(vertices[closestPoints[j]].y - vertices[k].y);
        float zdist = abs(vertices[i].z - vertices[k].z) + abs(vertices[closestPoints[j]].z - vertices[k].z);
        if (xdist + ydist + zdist < smallestDistN) {
          smallestDistN = xdist + ydist + zdist;
          closestPointsN = vector<int>();
          closestPointsN.push_back(k);
        }
        else if (xdist + ydist + zdist == smallestDistN) {
          closestPointsN.push_back(k);
        }
      }
      for (int k = 0; k < closestPointsN.size(); k++) {
        triangles.push_back(i);
        triangles.push_back(closestPoints[j]);
        triangles.push_back(closestPointsN[k]);
      }
    }
  }
  vector<int> trianglesCleaned = vector<int>();
  for (int i = 0; i < triangles.size() / 3; i++) {
    bool unique = true;
    for (int j = 0; j < trianglesCleaned.size() / 3; j++) {
      vector<int> pool = vector<int>(3);
      for (int k = 0; k < 3; k++) pool.push_back(trianglesCleaned[j * 3 + k]);
      for (int k = 0; k < 3; k++) {
        for (int l = 0; l < 3; l++) {
          if (triangles[i * 3 + k] == trianglesCleaned[j * 3 + l]) pool.erase(pool.begin() + l);
        }
      }
      if (pool.size() == 0) {
        unique = false;
        break;
      }
    }
    if (unique) {
      for (int j = 0; j < 3; j++) trianglesCleaned.push_back(triangles[i * 3 + j]);
    }
  }
  return trianglesCleaned;
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

  obj.triangles = windTrianglesClosestRedundant(obj.points);

  obj.position = position;
  return obj;
}

//Y axis is forward for cameras- and view is projected along it.
pair<vector<vector<bool> >, vector<vector<bool> > > render_camera(Camera camera, Space space) {
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
  //Render points
  vector<vector<bool> > renderedLines = vector<vector<bool> >();

  for (int h = 0; h < camera.resolution.second; h++) {
    vector<bool> currentLayer = vector<bool>();
    currentLayer.resize(camera.resolution.first);
    for (int w = 0; w < camera.resolution.first; w++) {
      currentLayer[w] = false;
      for (int i = 0; i < space.objects.size(); i++) {
        for (int j = 0; j < space.objects[i].triangles.size() / 3; j++) {
          float x0 = space.objects[i].points[space.objects[i].triangles[j * 3]].x;
          float x1 = space.objects[i].points[space.objects[i].triangles[j * 3 + 1]].x;
          float z0 = space.objects[i].points[space.objects[i].triangles[j * 3]].z;
          float z1 = space.objects[i].points[space.objects[i].triangles[j * 3 + 1]].z;
          for (float k = 0; k <= 1; k += 0.1) {
            int xcheck = round(x0 * (1.0 - k) + (x1 * k));
            int zcheck = round(z0 * (1.0 - k) + (z1 * k));
            if (xcheck == camera.position.x + (w - camera.resolution.first / 2) && zcheck == camera.position.z + (h - camera.resolution.second / 2)) {
              currentLayer[w] = true;
            }
          }
          x0 = space.objects[i].points[space.objects[i].triangles[j * 3 + 1]].x;
          x1 = space.objects[i].points[space.objects[i].triangles[j * 3 + 2]].x;
          z0 = space.objects[i].points[space.objects[i].triangles[j * 3 + 1]].z;
          z1 = space.objects[i].points[space.objects[i].triangles[j * 3 + 2]].z;
          for (float k = 0; k <= 1; k += 0.1) {
            int xcheck = round(x0 * (1.0 - k) + (x1 * k));
            int zcheck = round(z0 * (1.0 - k) + (z1 * k));
            if (xcheck == camera.position.x + (w - camera.resolution.first / 2) && zcheck == camera.position.z + (h - camera.resolution.second / 2)) {
              currentLayer[w] = true;
            }
          }
          x0 = space.objects[i].points[space.objects[i].triangles[j * 3 + 2]].x;
          x1 = space.objects[i].points[space.objects[i].triangles[j * 3]].x;
          z0 = space.objects[i].points[space.objects[i].triangles[j * 3 + 2]].z;
          z1 = space.objects[i].points[space.objects[i].triangles[j * 3]].z;
          for (float k = 0; k <= 1; k += 0.1) {
            int xcheck = round(x0 * (1.0 - k) + (x1 * k));
            int zcheck = round(z0 * (1.0 - k) + (z1 * k));
            if (xcheck == camera.position.x + (w - camera.resolution.first / 2) && zcheck == camera.position.z + (h - camera.resolution.second / 2)) {
              currentLayer[w] = true;
            }
          }
        }
      }
    }
    renderedLines.push_back(currentLayer);
  }
  //Render lines
  pair<vector<vector<bool> >, vector<vector<bool> > > returnval = pair<vector<vector<bool> >, vector<vector<bool> > >();
  returnval.first = renderedPixels;
  returnval.second = renderedLines;
  return returnval;
}

int main() {
  int fps = 20;

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
    pair<vector<vector<bool> >, vector<vector<bool> > > pixels = render_camera(cam, space);
    for (int h = cam.resolution.second; h > 0; h--) {
      for (int w = 0; w < cam.resolution.first; w++) {
        if (pixels.first[h - 1][w]) cout << "*";
        else if (pixels.second[h - 1][w]) cout << "/";
        else cout << " ";
      }
      cout << endl;
    }
    Sleep(1000 / fps);
  }
}