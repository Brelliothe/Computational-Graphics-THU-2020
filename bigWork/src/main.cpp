#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>

#include "scene_parser.hpp"
#include "image.hpp"
#include "camera.hpp"
#include "group.hpp"
#include "light.hpp"

#include <string>

using namespace std;

int main(int argc, char *argv[]) {
    for (int argNum = 1; argNum < argc; ++argNum) {
        std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;
    }

    if (argc != 3) {
        cout << "Usage: ./bin/PA1 <input scene file> <output bmp file>" << endl;
        return 1;
    }
    string inputFile = argv[1];
    string outputFile = argv[2];  // only bmp is allowed.

    // TODO: Main RayCasting Logic
    // First, parse the scene using SceneParser.
    // Then loop over each pixel in the image, shooting a ray
    // through that pixel and finding its intersection with
    // the scene.  Write the color at the intersection to that
    // pixel in your output image.

    SceneParser parser(argv[1]);
    cout << "parse done" << endl;
    Camera* camera = parser.getCamera();
    Group* group = parser.getGroup();
    int num_lights = parser.getNumLights();
    int w = camera->getWidth();
    int h = camera->getHeight();
    int samps = 100;
    Image img(w, h);
    float tmin = 1e-8;

    cout << "path tracing ..." << endl;
    for (int y = 0; y < h; y++) {
        cout << "progress: " << (float) y / h << "\n";
        for (unsigned short x = 0, Xi[3] = {y, y*y, y*y*y}; x < w; x++) {
            Vector3f ans(0, 0, 0);
            for (int sy = 0, i = (h-y-1) * w + x; sy < 2; sy++) {
                Vector3f r;
                for (int sx = 0; sx < 2; sx++, r = Vector3f::ZERO) {
                    for (int s = 0; s < samps; s++) {
                        double r1 = 2 * erand48(Xi), dx = r1 < 1 ? sqrt(r1) - 1 : 1 - sqrt(2 - r1); 
                        double r2 = 2 * erand48(Xi), dy = r2 < 1 ? sqrt(r2) - 1 : 1 - sqrt(2 - r2); 
                        Ray ray = camera->generateRay(Vector2f((sx + 0.5 + dx) / 2 + x, (sy + 0.5 + dy) / 2 + y));
                        // Ray Tracing Todo
                    }
                }
            }
            img.SetPixel(x, y, ans);
        }
    }

    cout << "tracing done." << endl;
    img.SaveBMP(argv[2]);
    
    /*
    cout << "casting ..." << endl;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            Ray ray = camera->generateRay(Vector2f(x, y));
            Hit hit;
            Vector3f ans(0, 0, 0);
            if (group->intersect(ray, hit, tmin)) {
                Material* material = hit.getMaterial();
                for (int i = 0; i < num_lights; i++) {
                    Light* light = parser.getLight(i);
                    Vector3f dirToLight;
                    Vector3f lightColor;
                    Vector3f p = ray.pointAtParameter(hit.getT());
                    light->getIllumination(p, dirToLight, lightColor);
                    ans += material->Shade(ray, hit, dirToLight, lightColor);
                }
            }
            else {
                ans = parser.getBackgroundColor();
            }
            img.SetPixel(x, y, ans);
        }
    }

    cout << "casting done." << endl;
    img.SaveBMP(argv[2]);
    */


    return 0;
}

