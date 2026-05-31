// Image Synthesis Main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <cstdlib>
#include <vector>
#include "Synthese3D.h" // personal library of vectors , colors, ray 
#include "Image.h" // personal library of images
#include <cmath>


enum  Material {
    DEFAULT,
    DIFFUSE,
    MIRROR,
    DIELECTRIC
};



class Shape {
public:
    Vector3 origin;
    Color color;
    Material mat;
    bool isLight;
    virtual ~Shape() = default;
    virtual float rayIntersection(const Ray& ray) const = 0;
    virtual Vector3 getNormalToSurface(const Vector3& rayIntersec) const = 0;

};

class Sphere : public Shape
{
public:

    float rayon;

    Sphere(Vector3 origine, float rayon, Color col) {
        this->origin = origine;
        this->rayon = rayon;
        this->color = col;
        this->mat = DEFAULT;
        this->isLight = false;
    }

    Sphere(Vector3 origine, float rayon, Color col, Material Material) {
        this->origin = origine;
        this->rayon = rayon;
        this->color = col;
        this->mat = Material;
        this->isLight = false;
    }

    Sphere(Vector3 origine, float rayon, Color col, Material Material, bool isLight) {
        this->origin = origine;
        this->rayon = rayon;
        this->color = col;
        this->mat = Material;
        this->isLight = isLight;
    }

    float rayIntersection(const Ray& ray) const override {

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

        Vector3 oc = ray.origin - this->origin;

        float a = Vector3::Dot(ray.direction, ray.direction);
        /*float b = 2 * Vector3::Dot(ray.origin, ray.direction);
        float c = Vector3::Dot(ray.origin, ray.origin) - sph.rayon * sph.rayon;*/

        float b = 2 * Vector3::Dot(oc, ray.direction);
        float c = Vector3::Dot(oc, oc) - this->rayon * this->rayon;

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

    Vector3 getNormalToSurface(const Vector3& rayIntersec) const override {
        Vector3 N = (rayIntersec - this->origin).Normalized();
        return N;
    }

};



class Plane : public Shape {
public:
    Vector3 normal;

    Plane(Vector3 origin, Vector3 n, Color color) {
        this->origin = origin;
        this->normal = n.Normalized();
        this->color = color;
        this->mat = DEFAULT;
        this->isLight = false;
    }

    float rayIntersection(const Ray& ray) const override {

        float t = NAN;
        float denominator = Vector3::Dot(ray.direction, this->normal);

        if (denominator > 1e-6) {
            float nominator = Vector3::Dot((this->origin - ray.origin), this->normal);
            t = nominator / denominator;

            if (t < 0)
            {
                t = NAN;
            }

        }

        return t;
    }

    Vector3 getNormalToSurface(const Vector3& rayIntersec) const override {
        return this->normal;
    }
};

class Triangle : public Shape {
public:
    Vector3 v1, v2, v3;
    // Plane plane;

    Triangle(Vector3 v1, Vector3 v2, Vector3 v3, Color color) {
        this->v1 = v1;
        this->v2 = v2;
        this->v2 = v3;
        this->color = color;
        this->mat = DEFAULT;
        this->isLight = false;
        //this->plane = plane;
    }

    float rayIntersection(const Ray& ray) const override {

        Vector3 e1 = this->v2 - this->v1;
        Vector3 e2 = this->v3 - this->v1;

        Vector3 crossRayE2 = Vector3::Cross(ray.direction, e2);
        float determinant = Vector3::Dot(e1,crossRayE2);

        float epsilon = 0.001;

        if (determinant > -epsilon && determinant < epsilon) { return NAN; }

        float inv_det = 1 / determinant;

        Vector3 v1ToRayO = this->v1 - ray.origin;
        float u = Vector3::Dot(v1ToRayO, crossRayE2)*inv_det;
        if (u < 0 || u > 1) { return NAN; }

        Vector3 crossv1ToRay0E1 = Vector3::Cross(v1ToRayO, e1);
        float v = Vector3::Dot(ray.direction, crossv1ToRay0E1) * inv_det;
        if (v < 0 || v + u > 1) { return NAN; }



        float t = Vector3::Dot(e2, v1ToRayO) * inv_det;

        return t;
    }
};



class Light {
public:
    Vector3 position;
    float intensity;
    Color color;
    Light(Vector3 pos, Color col, float intense) {
        this->position = pos;
        this->color = col;
        this->intensity = intense;
    }
};


Vector3 reflect(Vector3 N, Vector3 rayDir) {

    float b = Vector3::Dot(rayDir, N);
    Vector3 reflectedRayDir = rayDir - (2 * b) * N;
    return reflectedRayDir;
}

Vector3 refract(float n, float nP, Vector3 Normal, Vector3 rayDir) {
    //η = index of refraction of material the ray was previously in
    // η' = index of refraction of the new material

    float theta;

    Vector3 Rpperpandicular = (n / nP) * (rayDir - (Vector3::Dot(rayDir, Normal) * Normal));
    Vector3 Rpparallel = -Normal * (std::sqrt(1 - Rpperpandicular.Magnitude() * Rpperpandicular.Magnitude()));

    Vector3 Rp = Rpperpandicular + Rpparallel;

    return Rp;
}

Vector3 randomVector() {


    int nbSign = 0;
    float vectComponents[3];

    for (int i = 0; i < 3; i++)
    {
        nbSign = rand() % 2;
        if (nbSign == 1) { nbSign = -1; }
        else { nbSign = 1; }

        vectComponents[i] = (float)(std::rand() / (RAND_MAX + 1.0f));
        vectComponents[i] *= nbSign;
    }

    return Vector3(vectComponents[0], vectComponents[1], vectComponents[2]);
}

Vector3 randomVectorRejectionMethod() {
    int treshold = 0;

    while (true) {

        Vector3 Test = randomVector();

        if (1e-160 < Test.Magnitude() && Test.Magnitude() <= 1) {
            return Test.Normalized();
        }

        treshold++;
        if (treshold >= 200) {
            std::cout << "Error loop treshold reached breaking loop" << std::endl;
            return Vector3::NaN;
        }
    }
}

/*float CalculateRayPlaneIntersection(Ray ray, Plane pl) {
    float t = NAN;
    float denominator = Vector3::Dot(ray.direction, pl.normal);

    if (denominator > 1e-6) {
        float nominator  = Vector3::Dot((pl.origin - ray.origin), pl.normal);
        t = nominator / denominator;

        if (t < 0)
        {
            t = NAN;
        }

    }

    return t;
}
*/

/* float CalculateRaySphereIntersection(Ray ray, Sphere sph) {

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
    //float b = 2 * Vector3::Dot(ray.origin, ray.direction);
    //float c = Vector3::Dot(ray.origin, ray.origin) - sph.rayon * sph.rayon;

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

}*/

int getClosestShape(float* min, Ray sampleRay, Shape** shapes, int nbShapes) {

    float t = 0;
    int result = -1;

    for (int k = 0; k < nbShapes; k++)
    {

        t = shapes[k]->rayIntersection(sampleRay);

        if (!std::isnan(t))
        {
            if (t < *min) {
                *min = t;
                result = k;
            }

        }
    }

    return result;
}

float toneMap(float t, float maxD) {
    //1.A formula for mapping from[0, ∞) to[0, 1]
    //2.A method of applying this to the color.


    float tone = 1 - t / maxD;

    return tone;
}


Color directLightTransport(Light* LSource, int nbLights, int shpIndex, Shape** shapes, int nbShp, Vector3 rayIntersec) {

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


    Vector3 N = shapes[shpIndex]->getNormalToSurface(rayIntersec);
    Vector3 calculation(Vector3::Zero);
    Vector3 L_e = Vector3::Zero; // light emited by the object?
    Color albedo = shapes[shpIndex]->color;


    for (int i = 0; i < nbLights; i++) {
        float V = 1;
        Vector3 L_i = (LSource[i].position - rayIntersec).Normalized();
        Ray toLight(rayIntersec, L_i);
        float Test = 0;

        for (int k = 0; k < nbShp; k++) {

            if (k == shpIndex || shapes[k]->isLight) { continue; }

            Test = shapes[k]->rayIntersection(toLight);

            if (!std::isnan(Test)) {

                V = 0.0f;
                break;

            }
        }

        float L_emit = LSource[i].intensity;
        float D = Vector3::Distance(rayIntersec, LSource[i].position);


        calculation += V * (L_emit / (D * D)) * albedo * std::max(0.f, Vector3::Dot(N, L_i));

        calculation.x *= (LSource[i].color.x / 255);
        calculation.y *= (LSource[i].color.y / 255);
        calculation.z *= (LSource[i].color.z / 255);
    }

    Color L_o(L_e + calculation);

    float x = L_o.x * 255;
    float y = L_o.y * 255;
    float z = L_o.z * 255;

    Color res(x, y, z);
    res.Clamp();

    return res;
}


Color diffuseLightTransport(int nbBounces, int shpIndex, Shape** shapes, int nbShp, Vector3 rayIntersec) {

    if (nbBounces >= 3) {

        return Color::Black;
    }

    Vector3 N = shapes[shpIndex]->getNormalToSurface(rayIntersec);
    float epsilon = 0.001;
    //float epsilon = 1;
    Vector3 rayOrigin = rayIntersec + N * epsilon;

    const int nbSamples = 5;
    Color result = Color::Black;
    //Color L = Color::Black;
    float colorRetention = 1;
    float avgX = 0, avgY = 0, avgZ = 0;
    for (int i = 0; i < nbSamples; i++)
    {
        Vector3 randomDir = randomVectorRejectionMethod();
        if (Vector3::Dot(randomDir, N) < 0.0) { // Not in the same hemisphere as the normal
            randomDir = -randomDir;
        }

        Ray sampleRay(rayOrigin, randomDir);

        float min = 9999999999999;
        int newShapeIndex = 0;

        newShapeIndex = getClosestShape(&min, sampleRay, shapes, nbShp);
        Color found = Color::Black;



        if (newShapeIndex == -1) {
            //No sphere
            found = Color(0, 0, 0);
        }
        else if (shapes[newShapeIndex]->isLight) {
            //Light sphere
            found = Color(shapes[newShapeIndex]->color.x / 255, shapes[newShapeIndex]->color.y / 255, shapes[newShapeIndex]->color.z / 255);
        }
        else
        {
            found = shapes[newShapeIndex]->color;

            found = found / 255;

            //nbBounces++;
            Vector3 newIntersec = rayOrigin + randomDir * min;
            found = found * colorRetention * diffuseLightTransport(nbBounces + 1, newShapeIndex, shapes, nbShp, newIntersec);
        }


        avgX += found.x;
        avgY += found.y;
        avgZ += found.z;

    }


    avgX /= nbSamples;
    avgY /= nbSamples;
    avgZ /= nbSamples;

    Color average(avgX, avgY, avgZ);

    return average;

}

Color mirrorLightTransport(Light* LSource, int nbLights, int shpIndex, Shape** shapes, int nbShp, Vector3 rayIntersec, Vector3 rayDir) {

    Vector3 N = shapes[shpIndex]->getNormalToSurface(rayIntersec);
    float b = Vector3::Dot(rayDir, N);
    Vector3 reflectedRayDir = reflect(N, rayDir);

    float epsilon = 1;
    Vector3 rayOrigin = rayIntersec + N * epsilon;

    Ray reflectedRay(rayOrigin, reflectedRayDir);
    float min = 9999999999999;

    int foundSphere = getClosestShape(&min, reflectedRay, shapes, nbShp - 1);
    //Color foundColor = Color::Black;
    Color foundColor = Color(255, 20, 147);

    if (foundSphere != -1)
    {

        rayIntersec = rayOrigin + reflectedRayDir * min;

        foundColor = directLightTransport(LSource, nbLights, foundSphere, shapes, nbShp, rayIntersec);

    }


    return foundColor;
}

Color dielectricsLightTransport(Light* LSource, int nbLights, int shpIndex, Shape** shapes, int nbShp, Vector3 rayIntersec, Vector3 rayDir) {
    //randomly choosing between reflection and refraction,

    Vector3 N = shapes[shpIndex]->getNormalToSurface(rayIntersec);
    float b = Vector3::Dot(rayDir, N);
    Vector3 reflectedRayDir = refract(1, 1.7, N, rayDir);

    float epsilon = 1;
    Vector3 rayOrigin = rayIntersec + N * epsilon;

    Ray reflectedRay(rayOrigin, reflectedRayDir);
    float min = 9999999999999;

    int foundSphere = getClosestShape(&min, reflectedRay, shapes, nbShp - 1);
    //Color foundColor = Color::Black;
    Color foundColor = Color(255, 20, 147);

    if (foundSphere != -1)
    {

        rayIntersec = rayOrigin + reflectedRayDir * min;

        foundColor = directLightTransport(LSource, nbLights, foundSphere, shapes, nbShp, rayIntersec);

    }


    return foundColor;

}


int main()
{
    srand(time(0));

    int w = 1000, h = 1000;
    std::vector<std::vector<Color>> mat(h, std::vector<Color>(w));
    for (int i = 0; i < h; i++) {

        for (int j = 0; j < w; j++) {
            mat[i][j] = Color::Black;
        }
    }


    // ----------------- SHAPES ----------------------------//
    Sphere CenterSphere(Vector3(w / 2, h / 2, 1300), 100, Color::White, MIRROR);
    Sphere CenterDownSphere(Vector3(w / 2, h / 2 + 250, 1300), 100, Color(255, 215, 0, 1));
    Sphere bSphere(Vector3(200, 300, 1500), 150, Color::Blue);
    Sphere cSphere(Vector3(800, 800, 1400), 70, Color::Green);

    Sphere backGround(Vector3(w / 2, h / 2, 3100), 1600, Color(192, 192, 192));
    Sphere floor(Vector3(w / 2, h + 1600, 2100), 1600, Color(75, 0, 130));
    Sphere ceiling(Vector3(w / 2, -1600, 2100), 1600, Color(230, 230, 250));
    Sphere RWall(Vector3(w + 1600, h / 2, 2100), 1600, Color(0, 206, 209));
    Sphere LWall(Vector3(-1600, h / 2, 2100), 1600, Color(0, 206, 209));

    //Sphere noPointLight(Vector3(w / 2 - 300, h / 2 + 200, 1300), 180, Color::White, DEFAULT, true);

    Sphere noPointLight(Vector3(w / 2 - 300, h / 2 + 200, 1300), 180, Color::White, DEFAULT, true);

    //Sphere spheres[] = { CenterSphere, CenterDownSphere, bSphere, cSphere, backGround ,ceiling, floor , RWall,LWall , noPointLight};
    //int nbSpheres = 10;

    Plane centerPlane(Vector3(w / 2 + 300, h / 2 + 300, 1200), Vector3(15,5, 10), Color::White);
    centerPlane.isLight = true;

    Shape* shapes[] = {/* &CenterSphere, &CenterDownSphere, */ &bSphere, &cSphere, &centerPlane , &backGround ,&ceiling, &floor , &RWall,&LWall, &noPointLight};
    int nbShapes = 8;

    // ----------------- SHAPES ----------------------------//

    Light aLight(Vector3(w / 2, h / 2 + 60, 1000), Color::White, 1000);
    Light bLight(Vector3(10, 10, 1300), Color::White, 500);
    Light cLight(Vector3(w - 10, h - 10, 1100), Color::White, 500);
    Light dLight(Vector3(100, h, 1450), Color::White, 500);

    Light lights[] = { aLight, cLight /*,  dLight */ };
    int nbLights = 2;

    float maxD = 3100;
    float focale = 1000;
    const int nbSamples = 10;


    Vector3 camera(w / 2, h / 2, 0);

    for (int i = 0; i < h; i++) {

        for (int j = 0; j < w; j++) {

            Vector3 pixelCenter(j, i, focale);
            Vector3 rayDir = (pixelCenter - camera).Normalized();
            Ray centerRay(camera, rayDir);


            float avgX = 0, avgY = 0, avgZ = 0;
            int shapeIndex = 0;

            for (int s = 0; s < nbSamples; s++)
            {
                Color sampleColor = Color::Black;
                float x = (float)(std::rand() / (RAND_MAX + 1.0f));
                float y = (float)(std::rand() / (RAND_MAX + 1.0f));

                //A sample ray
                Vector3 aSample(j + x, i + y, focale);
                Vector3 aRayDir = (aSample - camera).Normalized();
                Ray aRay(camera, aRayDir);

                //Closes sphere calculation
                float t = 99999999999;
                shapeIndex = getClosestShape(&t, aRay, shapes, nbShapes);
                if (shapeIndex == -1) {
                    //std::cout << "No sphere hit" << std::endl;
                    continue;
                }

                Vector3 rayIntersec = aRay.origin + aRay.direction * t;

                int nbBounces = 0;


                switch (shapes[shapeIndex]->mat)
                {
                case DIFFUSE:
                    sampleColor = diffuseLightTransport(nbBounces, shapeIndex, shapes, nbShapes, rayIntersec);
                    //careful value should be 0-1 re convert to 255
                    sampleColor = sampleColor * 255;
                    break;

                case MIRROR:

                    shapes[shapeIndex]->color = mirrorLightTransport(lights, nbLights, shapeIndex, shapes, nbShapes, rayIntersec, aRay.direction);
                    sampleColor = directLightTransport(lights, nbLights, shapeIndex, shapes, nbShapes, rayIntersec);
                    break;

                case DIELECTRIC:

                    sampleColor = dielectricsLightTransport(lights, nbLights, shapeIndex, shapes, nbShapes, rayIntersec, aRay.direction);
                    break;


                default:

                    if (shapes[shapeIndex]->isLight)
                    {
                        sampleColor = shapes[shapeIndex]->color;
                    }
                    else {
                        sampleColor = directLightTransport(lights, nbLights, shapeIndex, shapes, nbShapes, rayIntersec);
                    }
                    break;
                }




                avgX += sampleColor.x;
                avgY += sampleColor.y;
                avgZ += sampleColor.z;


            }


            avgX /= nbSamples;
            avgY /= nbSamples;
            avgZ /= nbSamples;

            Color pixelColor(avgX, avgY, avgZ);
            pixelColor.Clamp();


            mat[i][j] = pixelColor;


        }
    }




    Image img(w, h, mat);

    //img.WriteImage("C:\\Users\\jlarmat\\Pictures\\Test");
    img.WriteImage("C:\\Dev");

    std::cout << "DONE !";

}
    






