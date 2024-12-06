#ifndef SCENE_H
#define SCENE_H

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/dual_quaternion.hpp>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <vector>
#include <stdexcept>
#define WIDTH 800
#define HEIGHT 800

class Ray {
public:
    glm::vec3 origin;   // The origin of the ray
    glm::vec3 direction; // The direction of the ray

    // Default constructor (initialize with zero vectors)
    Ray();

    // Constructor with origin and direction
    Ray(const glm::vec3& origin, const glm::vec3& direction);

    // Function to get a point along the ray at a given distance t
    glm::vec3 pointAtParameter(float t) const ;
};



// Material class to represent color and shininess
class Material {
public:
    glm::vec3 color;  // Ambient and diffuse material color (r, g, b)
    float shininess;  // Shininess value   zero or one 

    Material(const glm::vec3& color = glm::vec3(1.0f), float shininess = 0.0f);

    void print() const;
};

// Base class for all light sources
class LightSource {
public:
    glm::vec3 intensity;  // Light intensity (r, g, b)

public:
    LightSource(const glm::vec3& intensity);

    virtual ~LightSource() = default;   //  this is a destructor

    virtual void print() const = 0;
};

// Derived class for directional lights
class DirectionalLight : public LightSource {
public:
    glm::vec3 direction; // Light direction (normalized)

    DirectionalLight(const glm::vec3& direction, const glm::vec3& intensity);

    void print() const override;
};

// Derived class for spotlights
class Spotlight : public LightSource {
public:
    glm::vec3 position;  // Spotlight position
    float cutoff;        // Spotlight cutoff angle (cosine)

    Spotlight(const glm::vec3& position, float cutoff, const glm::vec3& intensity);

    void print() const override;
};

// Base class for scene objects
class Object {
public:
    enum class Type { Sphere, Plane };

    Material material;
    Type type;

    Object(const Material& material, Type type);

    virtual ~Object() = default;

    void setMaterial(Material& material );


    virtual bool Intersect(Ray& ray) = 0; // Mark const for immutability

    
    virtual void print() const = 0;
};

class Sphere : public Object {
public:
    glm::vec3 center; // Center of the sphere
    float radius;     // Radius of the sphere

    // Constructor
    Sphere(const glm::vec3& center, float radius, const Material& material) ; 
        

   bool Intersect( Ray& ray)  override;


    void print() const override;
};

class Plane : public Object {
public:
    glm::vec4 coefficients; // Plane equation coefficients (a, b, c, d)

    // Constructor
    Plane(const glm::vec4& coefficients, const Material& material) ; 

   bool Intersect( Ray& ray)  override;



    void print() const  override;
}; 

// Eye class to represent the camera
class Eye {
public:
    glm::vec3 position; // Camera position
    int modeFlag;       // Mode flag for anti-aliasing (optional)

    Eye(const glm::vec3& position, int modeFlag = 0);

    void print() const;
};

// Ambient class to represent global ambient intensity
class Ambient {
public:
    glm::vec3 intensity; // Ambient intensity (r, g, b)

    Ambient(const glm::vec3& intensity);

    void print() const;
};

// Scene class to hold all components
class Scene {
public:
    Eye eye;
    Ambient ambient;
    std::vector<LightSource*> lights;
    std::vector<Object*> objects;
    Ray rays[HEIGHT][WIDTH];

    Scene(const Eye& eye, const Ambient& ambient);

    ~Scene()  ; 

    void addLight(LightSource* light);
    void addObject(Object* obj) ;

    void print() const;
};


class Intersection {
public:
    glm::vec3 point;      // The intersection point in 3D space
    glm::vec3 normal;     // The normal vector at the intersection
    float t;              // The parameter 't' of the ray at the intersection point
    bool hit;             // Boolean flag indicating whether the intersection occurred

    // Default constructor
    Intersection(); 

    // Constructor with specific values
    Intersection(const glm::vec3& point, const glm::vec3& normal, float t, bool hit);
        

    // Function to check if the intersection is valid
    bool isValid() const ;
}; 


#endif


