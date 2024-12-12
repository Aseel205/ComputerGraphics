#include "Scene.h"
#include "phong.h"
// Material class
Material::Material(const glm::vec3 &color, float shininess)
    : color(color), shininess(shininess) {}

void Material::print() const
{
    std::cout << "Material - Color: " << glm::to_string(color)
              << ", Shininess: " << shininess << std::endl;
}

// LightSource clas
LightSource::LightSource(const glm::vec3 &intensity, const glm::vec3 &direction)
    : intensity(intensity), direction(direction) {}

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
DirectionalLight::DirectionalLight(const glm::vec3 &intensity, const glm::vec3 &direction)
    : LightSource(intensity, direction) {}

void DirectionalLight::print() const
{
    std::cout << "Directional Light, Intensity: ("
              << intensity.x << ", " << intensity.y << ", " << intensity.z << ")"
              << ", Direction: ("
              << direction.x << ", " << direction.y << ", " << direction.z << ")"
              << std::endl;
}

// Spotlight class

Spotlight::Spotlight(const glm::vec3 &intensity, const glm::vec3 &direction, float cutoff, const glm::vec3 &position)
    : LightSource(intensity, direction), cutoff(cutoff), position(position) {};

void Spotlight::print() const
{
    std::cout << "Spotlight, Intensity:(" << intensity.x << ", " << intensity.y << ", " << intensity.z << ")"
              << ", Direction: " << direction.x << ", " << direction.y << ", " << direction.z << ")"
              << ", Position: " << position.x << ", " << position.y << ", " << position.z << ")"
              << ", Cutoff: " << cutoff << std::endl;
}


glm::vec3 Spotlight :: getDirectionToPoint(const glm::vec3 &point)  {
        return glm::normalize(point - position);
    }



 bool Spotlight :: isWithinCutoff(const glm::vec3 &point)  {
        glm::vec3 directionToPoint = getDirectionToPoint(point);
        float cosTheta = glm::dot(glm::normalize(direction), directionToPoint);
        return cosTheta >= cutoff;
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
Sphere::Sphere(const Material &material, int status, const glm::vec3 &center, float radius)
{
    this->material = material;
    this->status = status;
    this->center = center;
    this->radius = radius;
}

Sphere::Sphere(int status, const glm::vec3 &center, float radius)
{
    this->status = status;
    this->center = center;
    this->radius = radius;
}

void Sphere::print() const
{
    std::cout << "Sphere - Center: " << glm::to_string(center)
              << ", Radius: " << radius << std::endl;
    material.print();
}

bool Sphere::Intersect(Ray &ray, float &t)
{
    // Calculate the vector from the ray origin to the sphere center
    glm::vec3 oc = ray.origin - center;

    // Calculate the coefficients of the quadratic equation
    float a = glm::dot(ray.direction, ray.direction);
    float b = 2.0f * glm::dot(oc, ray.direction);
    float c = glm::dot(oc, oc) - radius * radius;

    // Calculate the discriminant
    float discriminant = b * b - 4.0f * a * c;

    // If the discriminant is negative, there's no intersection
    if (discriminant < 0)
    {
        return false;
    }

    // Calculate the two solutions (t1 and t2)
    float sqrtDiscriminant = sqrt(discriminant);
    float t1 = (-b - sqrtDiscriminant) / (2.0f * a);
    float t2 = (-b + sqrtDiscriminant) / (2.0f * a);

    // Determine the smallest positive t value
    if (t1 >= 0 && t2 >= 0)
    {
        t = fmin(t1, t2); // Choose the closer intersection
        return true;
    }
    if (t1 >= 0)
    {
        t = t1; // Use the first intersection if it's valid
        return true;
    }
    if (t2 >= 0)
    {
        t = t2; // Use the second intersection if it's valid
        return true;
    }

    // No valid intersection
    return false;
}

// Plane Class

Plane::Plane(const Material &material, int status, const glm::vec4 &coefficients)
{
    this->material = material;
    this->status = status;
    this->coefficients = coefficients;
}

Plane::Plane(int status, const glm::vec4 &coefficients)
{
    this->status = status;
    this->coefficients = coefficients;
}

bool Plane::Intersect(Ray &ray, float &t)
{
    // Extract plane coefficients
    float a = coefficients.x;
    float b = coefficients.y;
    float c = coefficients.z;
    float d = coefficients.w;

    // Compute the dot product of the plane normal and the ray direction
    glm::vec3 normal(a, b, c);
    float denominator = glm::dot(normal, ray.direction);

    // If the denominator is close to zero, the ray is parallel to the plane
    if (glm::abs(denominator) < 1e-6)
    {
        return false; // No intersection
    }

    // Calculate the numerator
    float numerator = -(glm::dot(normal, ray.origin) + d);

    // Calculate the intersection distance (t)
    t = numerator / denominator;

    // If t is negative, the intersection is behind the ray's origin
    return t >= 0;
}

void Plane::print() const
{
    std::cout << "Plane - Coefficients: " << glm::to_string(coefficients) << std::endl;
    material.print();
}

// Eye class
Eye::Eye(const glm::vec3 &position, int modeFlag)
    : position(position), modeFlag(modeFlag) {}

void Eye::print() const
{
    std::cout << "Eye - Position: " << glm::to_string(position)
              << ", ModeFlag: " << modeFlag << std::endl;
}

// Ambient class
Ambient::Ambient(const glm::vec3 &intensity) : intensity(intensity) {}

void Ambient::print() const
{
    std::cout << "Ambient - Intensity: " << glm::to_string(intensity) << std::endl;
}

// Scene class
Scene::Scene(const Eye &eye, const Ambient &ambient)
    : eye(eye), ambient(ambient) {}

void Scene::addLight(LightSource *light)
{
    lights.push_back(light);
}

void Scene::addObject(Object *obj)
{
    objects.push_back(obj);
}

void Scene::print() const
{
    std::cout << "Scene Details:" << std::endl;
    eye.print();
    ambient.print();
    for (const auto &light : lights)
        light->print();
    for (const auto &obj : objects)
        obj->print();
}

int Scene ::getNumLights()
{
    return lights.size();
}

Scene::~Scene()
{
    // Delete dynamically allocated light sources and objects
    for (auto light : lights)
    {
        delete light; // Assuming the lights are dynamically allocated
    }
    for (auto obj : objects)
    {
        delete obj; // Assuming the objects are dynamically allocated
    }
}

Intersection Scene::GetHit(Ray &ray) {
    // Initialize closestIntersection with proper member values
    Intersection closestIntersection;
    closestIntersection.t = std::numeric_limits<float>::infinity(); // Start with a very large value
    closestIntersection.hitObject = false;

    // Iterate through all objects in the scene
    for (Object *obj : objects) {
        float zero  =  0 ;
        if (obj->tag!=ray.tag) {
            float t = 0.0f; // Parameter for the intersection
            if (obj->Intersect(ray, t)) { // If there's an intersection
                if (t < closestIntersection.t) { // Check if it's the closest one
                    closestIntersection.t = t;
                    closestIntersection.point = ray.pointAtParameter(t);

                    // Check if the object is a plane
                    if (obj->isPlane()) {
                        Plane *plane = dynamic_cast<Plane *>(obj);

                        // Extract normal from the plane coefficients (a, b, c)
                        glm::vec3 normal(plane->coefficients.x, plane->coefficients.y, plane->coefficients.z);

                        // Flip the normal if needed (depends on scene setup)
                        closestIntersection.normal = glm::normalize(-normal);  // Optionally flip the normal
                        closestIntersection.material = plane->material; // Set material from Plane
                        closestIntersection.ObjectType = "Plane";  // Set ObjectType to "Plane"
                        closestIntersection.tag = obj->tag;   

                    }
                    // Check if the object is a sphere
                    else if (obj->isSphere()) {
                        Sphere *sphere = dynamic_cast<Sphere *>(obj);
                        closestIntersection.normal = glm::normalize(closestIntersection.point - sphere->center);
                        closestIntersection.material = sphere->material; // Set material from Sphere
                        closestIntersection.ObjectType = "Sphere"; // Set ObjectType to "Sphere"
                        closestIntersection.tag = obj->tag;   

                    }

                    closestIntersection.hitObject = true; // Mark the intersection as valid
                    closestIntersection.ObjectStatus = obj->status; // Set the object status
                }
            }
        }
    }

    return closestIntersection; // Return the closest intersection
}


LightSource *Scene::getLight(int num)
{
    return lights.at(num);
}

// Intersection Class
Intersection::Intersection() : point(glm::vec3(0.0f)), normal(glm::vec3(0.0f)), t(0.0f), hitObject(false) {}

Intersection::Intersection(const glm::vec3 &point, const glm::vec3 &normal, float t, bool hitObject)
    : point(point), normal(normal), t(t), hitObject(hitObject) {}

bool Intersection::isValid() const
{
    return hitObject;
}

glm::vec3 Intersection ::getColor()
{
    return (ObjectType.compare("Plane") == 0) ? Phong ::checkerboardColor(material.color, point) : material.color;
}

// Ray Class

// Default constructor (initialize with zero vectors)
//Ray ::Ray() : origin(glm::vec3(0.0f)), direction(glm::vec3(0.0f)) {};

// Constructor with origin and direction
Ray ::Ray(const glm::vec3 &origin, const glm::vec3 &direction)
    : origin(origin), direction(glm::normalize(direction)) {
        tag = -1;
    } // Normalize the direction vector

// Function to get a point along the ray at a given distance t
glm::vec3 Ray ::pointAtParameter(float t) const
{
    return origin + t * direction;
}
