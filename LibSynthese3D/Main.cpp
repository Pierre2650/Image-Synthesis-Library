// Test_Lib_Synthese3D.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <vector>
#include "Synthese3D.h" // personal library of vectors , colors, ray 
#include "Image.h" // personal library of images
#include <cmath>




class sphere
{
public:
    Vector3 origin;
    float rayon;

    sphere(Vector3 origine, float rayon) {
        this->origin = origine;
        this->rayon = rayon;
    }

};


Vector3 CalculateRaySphereIntersection(Ray ray, sphere sph) {

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
        return Vector3::NaN;
    }
    else
    {
        float t1 = (-b - std::sqrt(delta)) / (2 * a);
        float t2 = (-b + std::sqrt(delta)) / (2 * a);

        if (t1 > t2)
        {
            t = t1;
        }
        else {
            t = t2;
        }
    }

    if (t < 0)
    {
        return  Vector3::NaN;

    }
    else {
        return  ray.origin + ray.direction * t;
    }

}



int main()
{
    

   int w = 1000, h = 1000;
   std::vector<std::vector<Color>> mat(h, std::vector<Color>(w));

   sphere aSphere(Vector3(3, 6, 4), 100);
   Vector3 camera(0, 0, 0);

   for (int i = 0; i < h; i++) {

      for (int j = 0; j < w; j++) {
          Vector3 pixelPos(j, i, -1);
          Vector3 rayDir = (pixelPos - camera).Normalized();
          Ray aRay(camera, rayDir);

          Vector3 Intersection = CalculateRaySphereIntersection(aRay, aSphere);

          if (Vector3::IsNaN(Intersection)) {
              mat[i][j] = Color::Black;
          }
          else
          {
              mat[i][j] = Color::Blue;
          }

         //mat[i][j] = Color::White; // a test
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

    sphere Sph(Vector3(4, 4, 0), 1);

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
    sphere sph(Vector3(32, 32, 0), 5);

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