// Image Synthesis Main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <vector>
#include "Synthese3D.h" // personal library of vectors , colors, ray 
#include "Image.h" // personal library of images
#include <cmath>



class Material {
public:
    Vector3 L_e ;

    Material(Vector3 LightEmited) {
        this->L_e = LightEmited;
    }
};

class Sphere
{
public:
    Vector3 origin;
    float rayon;
    Color color;

    Sphere(Vector3 origine, float rayon, Color col) {
        this->origin = origine;
        this->rayon = rayon;
        this->color = col;
    }

};

class Light {
public:
    Vector3 position;
    float intensity;
    Color color;
    Light(Vector3 pos, Color col,float intense) {
        this->position = pos;
        this->color = col;
        this->intensity = intense;
    }
};





float CalculateRaySphereIntersection(Ray ray, Sphere sph) {

    //Analytic sol

    // Sphere equation:
    // x^2 + y^2 + z^2 = R^2
    // P^2 - R^2 = 0  || P(x,y,z) Implicit function, implicit shape
    // SO beacause the point is intersected by the ray we can replace P -> O+tD, O = Ray origin , D = ray Dir
    // (O+tD)^2 - R^2 = 0
    // O^2 + 2ODt + Dt^2 - R^2 = 0   =>   Dt^2 + 2ODt + (O^2 - R^2)
    // Polynome de 2nd degree  ax^2 + bx + c = 0   , a = D   t = x
    // When delta > 0 => smalles or positive solution, When delta < 0 ray does not intersect

    float t = 0;

    Vector3 oc = ray.origin - sph.origin;

    float a = Vector3::Dot(ray.direction, ray.direction);
    /*float b = 2 * Vector3::Dot(ray.origin, ray.direction);
    float c = Vector3::Dot(ray.origin, ray.origin) - sph.rayon * sph.rayon;*/

    float b = 2 * Vector3::Dot(oc, ray.direction);
    float c = Vector3::Dot(oc, oc) - sph.rayon * sph.rayon;

    float delta = (b * b) - 4 * (a * c);

    if (delta < 0)
    {
        //return Vector3::NaN;
        return NAN;
    }
    else
    {
        float t1 = (-b - std::sqrt(delta)) / (2 * a);
        float t2 = (-b + std::sqrt(delta)) / (2 * a);

        if (t1 < t2)
        {
            t = t1;
        }
        else {
            t = t2;
        }
    }

    if (t < 0)
    {
        //return  Vector3::NaN;
        return NAN;

    }
    else {
        //return  ray.origin + ray.direction * t;

        return t;
    }

}

float toneMap(float t, float maxD) {
    //1.A formula for mapping from[0, ∞) to[0, 1]
    //2.A method of applying this to the color.


    float tone = 1 - t / maxD;

    return tone;
}

Color light_Transport(Light* LSource, int nbLights, int sphIndex, Sphere* sphs, int nbSph, Vector3 rayIntersec) {

    // L_o = L_e + V(P, L_p) * L_emit / D ^ 2 * Albedo * | N . L_i |
     // L_o --> light form surface
     // L_e --> light emited by surface
     // L_i --> light incoming to the surface

     // V(P,L_p) --> visibility of point light
     // L_emit --> Quantity of light emited by the light >= 0
     // 1/ D^2 --> distance to light
     // Albedo --> quantity of light transfered by surface ( "Color of surface"),  0 and 1
     //N --> Normal to the surface 

     //intersection P->O + tD

    Vector3 N = (rayIntersec - sphs[sphIndex].origin).Normalized();
    Vector3 calculation(Vector3::Zero);
    Vector3 L_e = Vector3::Zero;
    Color albedo = sphs[sphIndex].color;

    for (int i = 0; i < nbLights; i++) {
        float V = 1;
        Vector3 L_i = (LSource[i].position - rayIntersec).Normalized();
        Ray toLight(rayIntersec, L_i);
        //float t = CalculateRaySphereIntersection(toSphere, sphs[sphIndex]);
        float Test = 0;

        for (int k = 0; k < nbSph; k++) {

            if (k == sphIndex) { continue; }

            if (!std::isnan(Test = CalculateRaySphereIntersection(toLight, sphs[k]))) {
               
                V = 0.0f;
                break;
                
            }
        }


        float L_emit = LSource[i].intensity;
        float D = Vector3::Distance(rayIntersec, LSource[i].position);

        calculation +=   V * (L_emit / (D * D)) * albedo * std::max(0.f, Vector3::Dot(N, L_i));

        calculation.x = calculation.x * (LSource[i].color.x / 255);
        calculation.y = calculation.y * (LSource[i].color.y / 255);
        calculation.z = calculation.z * (LSource[i].color.z / 255);
    }

    Color L_o( L_e + calculation);

    float x = L_o.x *sphs[sphIndex].color.x;
    float y = L_o.y *sphs[sphIndex].color.y;
    float z = L_o.z *sphs[sphIndex].color.z;


    if (x > sphs[sphIndex].color.x) { x = sphs[sphIndex].color.x; }
    if (y > sphs[sphIndex].color.y) { y = sphs[sphIndex].color.y; }
    if (z > sphs[sphIndex].color.z) { z = sphs[sphIndex].color.z; }

    Color res(x, y, z);
    return res;
}



int main()
{

    int w = 1000, h = 1000;
    std::vector<std::vector<Color>> mat(h, std::vector<Color>(w));
    for (int i = 0; i < h; i++) {

        for (int j = 0; j < w; j++) {
            mat[i][j] = Color::Black;
        }
    }


    Sphere CenterSphere(Vector3(w / 2, h / 2 , 1300), 100, Color::White);
    Sphere CenterDownSphere(Vector3(w / 2, h / 2 + 250, 1300), 100, Color(255, 215, 0,1));
    Sphere bSphere(Vector3(200, 300, 1500), 150, Color::Blue);
    Sphere cSphere(Vector3(800, 800, 1400), 70, Color::Green);

    Sphere backGround(Vector3(w / 2, h / 2, 3100), 1600, Color(192, 192, 192));
    Sphere floor(Vector3(w / 2, h + 1600, 2100), 1600, Color(75, 0, 130));
    Sphere ceiling(Vector3(w / 2, -1600, 2100), 1600, Color(230, 230, 250));
    Sphere RWall(Vector3(w + 1600, h / 2, 2100), 1600, Color(0, 206, 209));
    Sphere LWall(Vector3(-1600, h / 2, 2100), 1600, Color(0, 206, 209));

    Sphere spheres[] = { CenterSphere, CenterDownSphere, bSphere, cSphere, backGround ,ceiling, floor , RWall,LWall };
    int nbSpheres = 9;

    /*Sphere spheres[] = { CenterSphere ,CenterDownSphere,  backGround ,ceiling, floor , RWall,LWall };
    int nbSpheres = 6; */

    float maxD = 3100;
    float focale = 1000;

    Light aLight(Vector3(w / 2 , h / 2 + 50 ,1000), Color::White, 2000);

    Light bLight(Vector3(10, 10, 1300), Color::White,  500);
    Light cLight(Vector3(w - 10, h - 10, 1100), Color::White,  500);
    Light dLight(Vector3(100, h , 1450), Color::White,  500);

    Light lights[] = { aLight, bLight, cLight, dLight };
    int nbLights = 4;

    /*Light lights[] = { aLight };
    int nbLights = 1;*/

    Vector3 camera(w / 2, h / 2, 0);



    for (int i = 0; i < h; i++) {

        for (int j = 0; j < w; j++) {

            Vector3 pixelPos(j, i, focale);
            Vector3 rayDir = (pixelPos - camera).Normalized();
            Ray aRay(camera, rayDir);


            float min = 99999999999;

            float res = 0;
            for (int k = 0; k < nbSpheres; k++)
            {

                if (!std::isnan(res = CalculateRaySphereIntersection(aRay, spheres[k])))
                {

                    if (res < min) {

                        min = res;
                        //float toneDeep = toneMap(res, maxD);
                        Vector3 rayIntersec = aRay.origin + aRay.direction * res;
                        Color pixelColor = light_Transport(lights, nbLights, k ,spheres, nbSpheres, rayIntersec);
                        mat[i][j] = pixelColor;
                    }

                }
            }



        }
    }




    Image img(w, h, mat);

    img.WriteImage("C:\\Dev");




}




void GeometricSolutionRaytrace() {

    Ray camRay(Vector3(1, 1, 0), Vector3(1, 0.5, 0));

    Sphere Sph(Vector3(4, 4, 0), 1, Color::White);

    // Geometric sol to raytrace a sphere

    // t0 = Tca -  Thc
    // t1 = Tca +  Thc
    // Thc ? tca?
    // P ( first point where ray intersects sphere ) = rayOrigin + t0*rayDir
    // P'( Last point where ray intersects sphere) = rayOrigin + t1*rayDir

    //we got a triagle were 
    // L = distance from camera origin to Sphere origin, 
    // Tca  = distance between origin of camera  and the point that is perpandicular to Sphere origin
    // d = distance between he point that is perpandicular to Sphere origin and the Sphere origin

    Vector3 L = Sph.origin - camRay.origin;

    // vector a =  camera ray direction
    // vector b = camera to sphere origin direction 
    float Tca = Vector3::Dot(camRay.direction, L);
    Vector3 TcaV = camRay.direction * Tca;

    // L = sqrt( tca^2 + dtoSphOrigin^2)
    // dtoSphOrigin = sqrt(L^2 _ Tca^2)

    float x = Vector3::Dot(L, L); // =>   L.Magnitude() * L.Magnitude()
    float y = Vector3::Dot(TcaV, TcaV);
    float d = std::sqrt(x - y);

    // if d > sphere radius = ray missed the sphere so next ray

    // Sphere_radius = sqrt(Thc^2 + d^2)
    // Thc = sqrt(Sphere_radius^2 - d^2)

    x = Sph.rayon * Sph.rayon;
    y = d * d;

    float Thc = std::sqrt(x - y);

    //Now whe find t0 t1
    float t0 = Tca - Thc;
    float t1 = Tca + Thc;

    Vector3 P = camRay.origin + camRay.direction * t0;
    Vector3 P_ = camRay.origin + camRay.direction * t1;


}

void TestDrawCircle(std::vector<std::vector<Color>> mat) {
    Sphere sph(Vector3(32, 32, 0), 5, Color::White);

    double PI = 2 * acos(0.0);

    std::vector<Vector3> circleCoord;

    for (float i = 0; i < (PI * 2); i = i + 0.1f) {
        Vector3 test(sph.origin.x + std::cos(i) * sph.rayon, sph.origin.y + std::sin(i) * sph.rayon, 0);

        test = Vector3(roundf(test.x), roundf(test.y), roundf(test.z));
        std::cout << "test = " << test << std::endl;

        circleCoord.push_back(test);
    }

    for (int k = 0; k < circleCoord.size(); k++) {
        mat[circleCoord[k].x][circleCoord[k].y] = Color::Red;
    }
}