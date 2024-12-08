#include "Scene.h"

// Material class
Material::Material(const glm::vec3& color, float shininess)
    : color(color), shininess(shininess) {}

void Material::print() const {
    std::cout << "Material - Color: " << glm::to_string(color)
              << ", Shininess: " << shininess << std::endl;
}

// LightSource clas
LightSource::LightSource(const glm::vec3& intensity , const glm::vec3& direction)
    : intensity(intensity), direction(direction)  {}

// ignore thesase functions /////////////////////////


void LightSource::print() const
{
 }

bool LightSource::isDirectional() const
{
    return false;
}

bool LightSource::isSpotlight() const
{
    return false;
}

////////////////////////////////////////////
// DirectionalLight class
DirectionalLight::DirectionalLight(const glm::vec3& intensity , const glm::vec3& direction)
    : LightSource(intensity, direction)  {} 



void DirectionalLight::print() const {
    std::cout << "Directional Light, Intensity: (" 
              << intensity.x << ", " << intensity.y << ", " << intensity.z << ")"
              << ", Direction: (" 
              << direction.x << ", " << direction.y << ", " << direction.z << ")"
              << std::endl;
}


// Spotlight class


Spotlight::Spotlight(const glm::vec3& intensity,const glm::vec3& direction,float cutoff, const glm::vec3& position)
    : LightSource(intensity, direction), cutoff(cutoff), position(position) {};


void Spotlight::print() const {
    std::cout << "Spotlight, Intensity:(" << intensity.x << ", " << intensity.y << ", " << intensity.z << ")"
              << ", Direction: "<< direction.x << ", " << direction.y << ", " << direction.z << ")"
              << ", Position: " << position.x << ", " << position.y << ", " << position.z << ")"
              << ", Cutoff: " << cutoff << std::endl;
}


// you can ignore this two methods
bool Object::isSphere() const
{
    return false;
}

bool Object::isPlane() const
{
    return false;
}
//////////////////////////////////


void Object ::setMaterial(Material &material)
{
    this->material = material;
}

// Sphere Class
Sphere::Sphere(const Material& material, int status, const glm::vec3& center, float radius) {
    this->material = material;
    this->status = status;
    this->center = center;
    this->radius = radius;
}

Sphere::Sphere(int status, const glm::vec3& center, float radius) {
    this->status = status;
    this->center = center;
    this->radius = radius;
}

void Sphere::print() const {
    std::cout << "Sphere - Center: " << glm::to_string(center)
              << ", Radius: " << radius << std::endl;
    material.print();
}

bool Sphere::Intersect(Ray& ray, float& t) {
    // Calculate the vector from the ray origin to the sphere center
    glm::vec3 oc = ray.origin - center;

    // Calculate the coefficients of the quadratic equation
    float a = glm::dot(ray.direction, ray.direction);
    float b = 2.0f * glm::dot(oc, ray.direction);
    float c = glm::dot(oc, oc) - radius * radius;

    // Calculate the discriminant
    float discriminant = b * b - 4.0f * a * c;

    // If the discriminant is negative, there's no intersection
    if (discriminant < 0) {
        return false;
    }

    // Calculate the two solutions (t1 and t2)
    float sqrtDiscriminant = sqrt(discriminant);
    float t1 = (-b - sqrtDiscriminant) / (2.0f * a);
    float t2 = (-b + sqrtDiscriminant) / (2.0f * a);

    // Determine the smallest positive t value
    if (t1 >= 0 && t2 >= 0) {
        t = fmin(t1, t2); // Choose the closer intersection
        return true;
    }
    if (t1 >= 0) {
        t = t1; // Use the first intersection if it's valid
        return true;
    }
    if (t2 >= 0) {
        t = t2; // Use the second intersection if it's valid
        return true;
    }

    // No valid intersection
    return false;
}


// Plane Class

Plane::Plane(const Material& material, int status, const glm::vec4& coefficients) {
    this->material = material;
    this->status = status;
    this->coefficients = coefficients;
}

Plane::Plane(int status, const glm::vec4& coefficients) {
    this->status = status;
    this->coefficients = coefficients;
}

bool Plane::Intersect(Ray& ray, float& t) {
    // Extract plane coefficients
    float a = coefficients.x;
    float b = coefficients.y;
    float c = coefficients.z;
    float d = coefficients.w;

    // Compute the dot product of the plane normal and the ray direction
    glm::vec3 normal(a, b, c);
    float denominator = glm::dot(normal, ray.direction);

    // If the denominator is close to zero, the ray is parallel to the plane
    if (glm::abs(denominator) < 1e-6) {
        return false; // No intersection
    }

    // Calculate the numerator
    float numerator = -(glm::dot(normal, ray.origin) + d);

    // Calculate the intersection distance (t)
    t = numerator / denominator;

    // If t is negative, the intersection is behind the ray's origin
    return t >= 0;
}





void Plane::print() const {
    std::cout << "Plane - Coefficients: " << glm::to_string(coefficients) << std::endl;
    material.print();
}

// Eye class
Eye::Eye(const glm::vec3& position, int modeFlag)
    : position(position), modeFlag(modeFlag) {}

void Eye::print() const {
    std::cout << "Eye - Position: " << glm::to_string(position)
              << ", ModeFlag: " << modeFlag << std::endl;
}

// Ambient class
Ambient::Ambient(const glm::vec3& intensity) : intensity(intensity) {}

void Ambient::print() const {
    std::cout << "Ambient - Intensity: " << glm::to_string(intensity) << std::endl;
}

// Scene class
Scene::Scene(const Eye& eye, const Ambient& ambient)
    : eye(eye), ambient(ambient) {}



void Scene::addLight(LightSource* light) {
    lights.push_back(light);
}

void Scene::addObject(Object* obj) {
    objects.push_back(obj);
}

void Scene::print() const {
    std::cout << "Scene Details:" << std::endl;
    eye.print();
    ambient.print();
    for (const auto& light : lights) light->print();
    for (const auto& obj : objects) obj->print();

}

int  Scene :: getNumLights ()   {
    return lights.size() ;

} 



Scene::~Scene() {
        // Delete dynamically allocated light sources and objects
        for (auto light : lights) {
            delete light;  // Assuming the lights are dynamically allocated
        }
        for (auto obj : objects) {
            delete obj;  // Assuming the objects are dynamically allocated
        }
}


Intersection Scene::GetHit(Ray& ray) {
    Intersection closestIntersection;
    closestIntersection.t = std::numeric_limits<float>::infinity(); // Start with a very large value
    closestIntersection.hitObject = false;

    for (Object* obj : objects) {
        float t = 0.0f;
        if (obj->Intersect(ray, t)) {
            if (t < closestIntersection.t) {
                closestIntersection.t = t;
                closestIntersection.point = ray.pointAtParameter(t);

                // Compute the normal at the intersection point and get the material
                if (obj->isPlane()) {
                    Plane* plane = dynamic_cast<Plane*>(obj);
                    glm::vec3 normal(plane->coefficients.x, plane->coefficients.y, plane->coefficients.z);
                    closestIntersection.normal = glm::normalize(normal);
                    closestIntersection.material = plane->material;  // Set material from Plane
                    closestIntersection.ObjectType = "Plane"; // Set ObjectType to "Plane"
                } else if (obj->isSphere()) {
                    Sphere* sphere = dynamic_cast<Sphere*>(obj);
                    closestIntersection.normal = glm::normalize(closestIntersection.point - sphere->center);
                    closestIntersection.material = sphere->material;  // Set material from Sphere
                    closestIntersection.ObjectType = "Sphere"; // Set ObjectType to "Sphere"
                }

                closestIntersection.hitObject = true;
            }
        }
    }

    return closestIntersection;
}



 LightSource *   Scene::getLight(int num)
{
    return lights.at(num) ; 
}



// Intersection Class
Intersection:: Intersection() : point(glm::vec3(0.0f)), normal(glm::vec3(0.0f)), t(0.0f), hitObject(false) {}

Intersection::Intersection(const glm::vec3& point, const glm::vec3& normal, float t, bool hitObject)
    : point(point), normal(normal), t(t), hitObject(hitObject) {}

bool  Intersection:: isValid() const {
    return hitObject;
}



//Ray Class

// Default constructor (initialize with zero vectors)
Ray :: Ray() : origin(glm::vec3(0.0f)), direction(glm::vec3(0.0f)) {}

// Constructor with origin and direction
Ray :: Ray(const glm::vec3& origin, const glm::vec3& direction)
    : origin(origin), direction(glm::normalize(direction)) {} // Normalize the direction vector

// Function to get a point along the ray at a given distance t
glm::vec3  Ray :: pointAtParameter(float t) const {
    return origin + t * direction;
}





