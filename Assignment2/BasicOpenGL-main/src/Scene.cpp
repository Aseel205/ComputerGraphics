#include "Scene.h"

// Material class
Material::Material(const glm::vec3& color, float shininess)
    : color(color), shininess(shininess) {}

void Material::print() const {
    std::cout << "Material - Color: " << glm::to_string(color)
              << ", Shininess: " << shininess << std::endl;
}

// LightSource base class
LightSource::LightSource(const glm::vec3& intensity) : intensity(intensity) {}

// DirectionalLight class
DirectionalLight::DirectionalLight(const glm::vec3& direction, const glm::vec3& intensity)
    : LightSource(intensity), direction(glm::normalize(direction)) {}

void DirectionalLight::print() const {
    std::cout << "Directional Light - Direction: " << glm::to_string(direction)
              << ", Intensity: " << glm::to_string(intensity) << std::endl;
}

// Spotlight class
Spotlight::Spotlight(const glm::vec3& position, float cutoff, const glm::vec3& intensity)
    : LightSource(intensity), position(position), cutoff(cutoff) {}

void Spotlight::print() const {
    std::cout << "Spotlight - Position: " << glm::to_string(position)
              << ", Cutoff: " << cutoff
              << ", Intensity: " << glm::to_string(intensity) << std::endl;
}

// Object base class
Object::Object(const Material& material, Type type) : material(material), type(type) {}

    void Object ::setMaterial(Material& material) {
            this-> material  = material ; 
    }



// Sphere class
Sphere::Sphere(const glm::vec3& center, float radius, const Material& material)
    : Object(material, Type::Sphere), center(center), radius(radius) {}

void Sphere::print() const {
    std::cout << "Sphere - Center: " << glm::to_string(center)
              << ", Radius: " << radius << std::endl;
    material.print();
}


bool Sphere::Intersect(Ray &ray) {
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

    // If the discriminant is non-negative, there are intersections
    // Calculate the two solutions (t1 and t2)
    float sqrtDiscriminant = sqrt(discriminant);
    float t1 = (-b - sqrtDiscriminant) / (2.0f * a);
    float t2 = (-b + sqrtDiscriminant) / (2.0f * a);

    // Find the smallest positive t value
    if (t1 >= 0 && t2 >= 0) {
        return true;  // Intersection in front of the ray
    }
    if (t1 >= 0) {
        return true;  // Use the first intersection if it's in front
    }
    if (t2 >= 0) {
        return true;  // Use the second intersection if it's in front
    }

    return false;  // No valid intersection (both solutions are negative)
}


// Plane class
Plane::Plane(const glm::vec4& coefficients, const Material& material)
    : Object(material, Type::Plane), coefficients(coefficients) {}


bool Plane::Intersect(Ray& ray)  {
    // Extract the plane normal and d from coefficients
    glm::vec3 normal(coefficients.x, coefficients.y, coefficients.z);
    float d = coefficients.w;

    // Calculate the denominator of the intersection formula
    float denominator = glm::dot(normal, ray.direction);

    // If the denominator is close to zero, the ray is parallel to the plane
    if (fabs(denominator) < 1e-6) {
        return false;
    }

    // Calculate the t value for the intersection point
    float t = -(glm::dot(normal, ray.origin) + d) / denominator;

    // Return true if the intersection is in the positive direction of the ray
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


Scene::~Scene() {
        // Delete dynamically allocated light sources and objects
        for (auto light : lights) {
            delete light;  // Assuming the lights are dynamically allocated
        }
        for (auto obj : objects) {
            delete obj;  // Assuming the objects are dynamically allocated
        }
}

// Intersection Class
Intersection:: Intersection() : point(glm::vec3(0.0f)), normal(glm::vec3(0.0f)), t(0.0f), hit(false) {}

Intersection::Intersection(const glm::vec3& point, const glm::vec3& normal, float t, bool hit)
    : point(point), normal(normal), t(t), hit(hit) {}

bool  Intersection:: isValid() const {
    return hit;
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





