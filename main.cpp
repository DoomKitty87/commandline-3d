#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
using namespace std;

//Holds array of its visible points relative to its position, and its position.
class WorldSpaceObject
{
  public:
    vector<int> position;
    vector<vector<int> > points;
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
    vector<int> position;
    pair<int, int> resolution;
};

//Y axis is forward for cameras- and view is projected along it.
vector<vector<bool> > render_camera(Camera camera, Space space) {
  vector<vector<bool> > renderedPixels = vector<vector<bool> >();
  for (int h = 0; h < camera.resolution.second; h++) {
    vector<bool> currentLayer = vector<bool>();
    currentLayer.resize(camera.resolution.first);
    for (int w = 0; w < camera.resolution.first; w++) {
      for (int i = 0; i < space.objects.size(); i++) {
        for (int j = 0; j < space.objects[i].points.size(); j++) {
          if (space.objects[i].points[j][0] == camera.position[0] + (w - camera.resolution.first / 2) && space.objects[i].points[j][2] == camera.position[2] + (h - camera.resolution.second / 2)) {
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
  object1.position = vector<int>();
  object1.position.push_back(0);
  object1.position.push_back(0);
  object1.position.push_back(0);
  vector<int> point1 = vector<int>();
  point1.push_back(0);
  point1.push_back(0);
  point1.push_back(0);
  object1.points.push_back(point1);
  space.objects.push_back(object1);
  Camera cam = Camera();
  cam.position.push_back(0);
  cam.position.push_back(0);
  cam.position.push_back(0);
  cam.resolution.first = 50;
  cam.resolution.second = 10;
  while(true) {
    for (int i = 0; i < 50; i++) {
      cout << endl;
    }
    vector<int> newpoint = vector<int>();
    for (int i = 0; i < 3; i++) {
      newpoint.push_back(rand() % (cam.resolution.first) - (cam.resolution.first / 2));
    }
    space.objects[0].points.push_back(newpoint);
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