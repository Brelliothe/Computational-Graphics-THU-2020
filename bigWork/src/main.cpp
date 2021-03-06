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
#include "Photonmap.hpp"
#include "ray_tracer.hpp"
#include "photon_tracer.hpp"
#include "sampler.hpp"

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

    SceneParser parser(argv[1]);
    cout << "parse done" << endl;
    Camera* camera = parser.getCamera();
    Group* group = parser.getGroup();
    int num_lights = parser.getNumLights();
    int w = camera->getWidth();
    int h = camera->getHeight();
    int samps = 500;
    int depth = 10;
    Image img(w, h);
    float tmin = 1e-3;

    std::vector<Light*> lights;
    for (int i = 0; i < num_lights; i++)
        lights.emplace_back(parser.getLight(i));
    RayTracer tracer(depth, group, lights, camera->getType(), parser.getBackgroundColor());

    int multiThreadCounter = 0;

    
    cout << "SPPMing ..." << endl;

    Sampler sampler(lights, camera, &img, &tracer, group, w, h);
    sampler.start();

	cout << "SPPM done." << endl;
    

    /*
    cout << "photon mapping ..." << endl;  

    PhotonTracer* photontracer = new PhotonTracer(lights, camera->getPhotons(), depth, tmin);
    photontracer->setGroup(group);
	Photonmap* photonmap = photontracer->CalcPhotonmap();
    Image* vol = photontracer->CalcVolumetricmap(camera);
    tracer.setPhotonMap(photonmap);
	delete photontracer;

    cout << "photon map set up!" << endl;

    #pragma omp parallel for
    for (int y = 0; y < h; y++) {
        // cout << "progress: " << (float) y / h * 100 << "%\n";
        for (unsigned short x = 0, Xi[3] = {y, y*y, y*y*y}; x < w; x++) {
            Vector3f ans(0, 0, 0);
            Ray ray = camera->generateRay(Vector2f(x, y));
            ans += tracer.trace(ray, Xi, 1, 0);
            img.SetPixel(x, y, ans + vol->GetPixel(x, y));
        }
        multiThreadCounter++;
        cout << "progress: " << (float) multiThreadCounter / h * 100 << "%\n";
    }

    cout << "photon mapping - tracing done." << endl;
    img.SaveBMP(argv[2]);
    */



    /*
    cout << "path tracing ..." << endl;  
    #pragma omp parallel for
    for (int it = 0; it < w * h; it++) {
        // cout << "progress: " << (float) y / h * 100 << "%\n";
        // for (unsigned short x = 0, Xi[3] = {y, y*y, y*y*y}; x < w; x++) {
            int y = it / w, x = it % w;
            unsigned short Xi[3] = {y, y*y, y*y*y};
            Vector3f ans(0, 0, 0);
            for (int sy = 0, i = (h-y-1) * w + x; sy < 2; sy++) {
                Vector3f r;
                for (int sx = 0; sx < 2; sx++, r = Vector3f::ZERO) {
                    for (int s = 0; s < samps; s++) {
                        double r1 = 2 * erand48(Xi), dx = r1 < 1 ? sqrt(r1) - 1 : 1 - sqrt(2 - r1); 
                        double r2 = 2 * erand48(Xi), dy = r2 < 1 ? sqrt(r2) - 1 : 1 - sqrt(2 - r2); 
                        Ray ray = camera->generateRay(Vector2f((sx + 0.5 + dx) / 2 + x, (sy + 0.5 + dy) / 2 + y));
                        // Ray Tracing Todo
                        ans += tracer.trace(ray, Xi, 1, 0) * 0.25 / samps;
                    }
                }
            }
            img.SetPixel(x, y, ans);
            multiThreadCounter++;
            cout << "progress: " << (float) multiThreadCounter / h / w * 100 << "%\n";
        // }
        if (it % 50 == 0) img.SaveBMP(argv[2]);
    }

    cout << "tracing done." << endl;
    img.SaveBMP(argv[2]);
    */
  
    /*
    cout << "casting ..." << endl;
    #pragma omp parallel for
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            // cout << "(" << x << ',' << y << ") ";
            Ray ray = camera->generateRay(Vector2f(x, y));
            Hit hit;
            Vector3f ans(0, 0, 0);
            // if (x == 45 && y == 19) cout << "intersect start!\n";
            if (group->intersect(ray, hit, tmin)) {
                Material* material = hit.getMaterial();
                for (int i = 0; i < num_lights; i++) {
                    Light* light = parser.getLight(i);
                    Vector3f dirToLight;
                    Vector3f lightColor;
                    Vector3f p = ray.pointAtParameter(hit.getT());
                    unsigned short Xi[3] = {i, i*i, i*i*i};
                    light->getIllumination(p, dirToLight, lightColor, group, Xi);
                    // if (x == 45 && y == 19) cout << " (" << x << ',' << y << ") " << hit.getT() << ' ';
                    ans += material->Shade(ray, hit, dirToLight, lightColor);
                    // cout << "point at: " << ray.pointAtParameter(hit.getT());
                }
            }
            else {
                ans = parser.getBackgroundColor();
            }
            // if (x == 45 && y == 19) cout << "finished!\n";
            img.SetPixel(x, y, ans);
        }
        multiThreadCounter++;
        cout << "progress: " << (float) multiThreadCounter / h * 100 << "%\n";
    }
    
    cout << "casting done." << endl;
    img.SaveBMP(argv[2]);
    */


    return 0;
}

