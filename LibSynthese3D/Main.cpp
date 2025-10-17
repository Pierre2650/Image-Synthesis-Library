// Test_Lib_Synthese3D.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <vector>
#include "Synthese3D.h" // personal library of vectors , colors, ray 
#include "Image.h" // personal library of images
#include <cmath>




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

    Light(Vector3 pos, float intense) {
        this->position = pos;
        this->intensity = intense;
    }
};



float CalculateRaySphereIntersection(Ray ray, Sphere sph ) {

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

    //chatgp test 
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

Color toneMap(float t, Sphere sph, float maxD) {
   //1.A formula for mapping from[0, ∞) to[0, 1]
   //2.A method of applying this to the color.


    float tone = 1 - t / maxD;
    
    //float tone = t/ t + Sph.rayon;
    //float tone = pixelDistance/ maxDistance;
    //std::cout << "tone =  " << tone << std::endl;
    float x = tone * sph.color.x;
    float y = tone * sph.color.y;
    float z = tone * sph.color.z;

    if (x > sph.color.x) { sph.color.x; }
   if (y > sph.color.y) { y = sph.color.y; }
   if (z > sph.color.z) { z = sph.color.z; }
   

    Color res(x, y, z);

    return res;
}

Color toneMap2(float t, Sphere Sph, Ray ray) {
    
    Vector3 rayHit = ray.origin + ray.direction * t ;

    //surface normal N
    Vector3 N = (rayHit - ray.origin) / Sph.rayon;

   //the amount of illumination at a point depends on how directly that surface faces the camera(or the light).
   //That’s given by the dot product between the light direction and the surface normal :
    // Brigness factor I

    float I = Vector3::Dot(N, -ray.direction);
    float tone = 0;
    //std::cout << "tone =  " << tone << std::endl;
    float x = tone * Sph.color.x;
    float y = tone * Sph.color.y;
    float z = tone * Sph.color.z;

    //if (x > SphColor.x) { x = SphColor.x; }
     //if (y > SphColor.y) { y = SphColor.y; }
    // if (z > SphColor.z) { z = SphColor.z; }


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

   float focale = 1000;
 
   Sphere CenterSphere(Vector3(w / 2, h / 2, 1500), 300, Color::Red);
   Sphere bSphere(Vector3(100, 0, 2000), 300, Color::Blue);
   Sphere cSphere(Vector3(1000, 1000, 2000), 100, Color::Green);
   Sphere backGround(Vector3(w / 2, h / 2, 2600), 1200, Color(192, 192, 192));
   Sphere floor(Vector3(w/2, h + 1000, 2100), 1000, Color(75, 0, 130));
   Sphere ceiling(Vector3(w / 2, -1000, 2100), 1000, Color(230, 230, 250));
   Sphere RWall(Vector3(w + 1100, h / 2, 1800), 1000, Color(0, 206, 209));
   Sphere LWall(Vector3(-1100, h / 2, 1800), 1000 ,Color(0, 206, 209));
   Sphere spheres[] = { CenterSphere, bSphere, cSphere, backGround ,ceiling, floor , RWall,LWall};
   int nbSpheres = 8;

   float maxD = 3000;

   Vector3 camera(w/2, h/2, 0);


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
                      Color Mapped = toneMap(res, spheres[k], maxD);
                      mat[i][j] = Mapped;
                  }

              }
          }
         

          
      }
   }

  
   //// a test of printing a J
   ////--------- J -------
  /* mat[0][4] = Color::Red;
   mat[1][4] = Color::Red;
   mat[2][4] = Color::Red;
   mat[3][4] = Color::Red;
   mat[4][4] = Color::Red;
   mat[5][4] = Color::Red;
   mat[6][4] = Color::Red;

   mat[6][0] = Color::Red;

   mat[7][1] = Color::Red;
   mat[7][2] = Color::Red;
   mat[7][3] = Color::Red;*/
   ////-------------------

   Image img(w, h, mat);

   img.WriteImage("C:\\Users\\jlarmat\\Pictures\\Test");




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