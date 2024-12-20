#include "Phong.h"
#include "Scene.h"
#include <glm/glm.hpp>

#define MAX_LEVEL 5

glm::vec3 Phong::calcColor(Scene &scene, Ray &ray, int level) {                         
    // Get the intersection point
    Intersection hit = scene.GetHit(ray);

    int status = hit.ObjectStatus;  // Object=0.0, Reflective=1, Transparent=2

    // Start with emission and ambient components
    glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);

    // Black color (no hit)
    if (!hit.hitObject) {    
        return glm::vec3(0.0f, 0.0f, 0.0f);
    }

    // Regular object
    if (status == 0) { 
        color = calcEmissionColor(scene) + calcAmbientColor(scene, hit);

        // Add diffuse and specular contributions from all lights
        for (int i = 0; i < scene.getNumLights(); i++) {
            LightSource *light = scene.getLight(i);

            if (!occluded(scene, hit, light)) {
                glm::vec3 specularColor = calcSpecularColor(scene, hit, light, ray); 
                color += (calcDiffuseColor(scene, hit, light) + specularColor) * light->intensity;
            }
        }
    }

    // Reflective object
    else if (status == 1) {
        // Base case for recursion
        if (level >= MAX_LEVEL) {
            return glm::vec3(0.0f);  // Don't add anything
        }

        // Reflective contribution
        glm::vec3 normal = hit.normal;  // Surface normal
        Ray outRay = ConstructOutRay(ray, normal, hit.point);
        outRay.objectId = hit.objectId; 

        glm::vec3 reflectedColor = calcColor(scene, outRay, level + 1);
        color += reflectedColor;
    }

    // Transparent (Refractive) object
    else if (status == 2) {
        // Base case for recursion
        if (level >= MAX_LEVEL) {
            return glm::vec3(0.0f) ;
        }


        glm::vec3 normal = hit.normal;

        // Calculate entry refraction ray
        Ray refractedRay = calcTransparencyRay(ray, normal, hit.point);

        // Simulate exit refraction
        glm::vec3 exitPoint = hit.point + 0.01f * refractedRay.direction; // Offset exit point slightly to avoid self-intersection
        Ray exitRay(exitPoint, refractedRay.direction);
        exitRay.objectId = hit.objectId; 
        glm::vec3 exitColor = calcColor(scene, exitRay, level + 1);

        color += exitColor;  
    }

    return color;
}

//  for the planes
glm:: vec3 Phong ::checkerboardColor(glm::vec3 rgbColor, glm :: vec3 hitPoint) {
// Checkerboard pattern
    float scaleParameter = 0.5f;
    float checkerboard = 0;
    if (hitPoint.x < 0) {
        checkerboard += floor((0.5 - hitPoint.x) / scaleParameter);
    }
    else {
        checkerboard += floor(hitPoint.x / scaleParameter);
    }
    if (hitPoint.y < 0) {
        checkerboard += floor((0.5 - hitPoint.y) / scaleParameter);
    }
    else {
        checkerboard += floor(hitPoint.y / scaleParameter);
    }
    checkerboard = (checkerboard * 0.5) - int(checkerboard * 0.5);
    checkerboard *= 2;
    if (checkerboard > 0.5) {
        return 0.5f * rgbColor;
    }
return rgbColor;
}


// phong funcitons

glm::vec3 Phong::calcAmbientColor(Scene &scene, Intersection &hit)
{
    glm::vec3 ambientIntensity = scene.ambient.intensity;   

    return scene.ambient.intensity *  hit.getColor();
}

glm::vec3 Phong::calcEmissionColor(Scene &scene)
{
    return glm::vec3(0.0f, 0.0f, 0.0f); // No emission
}



glm::vec3 Phong::calcDiffuseColor(Scene& scene, Intersection &hit, LightSource* light) {
    glm::vec3 Li; // intersection to the light
    glm::vec3 diffuseColor;
    glm::vec3 N = glm::normalize(hit.normal);   // Intersection normal 
    glm::vec3 KD = hit.getColor();   // the rgb of C 


    // Check the light type
    if (light->isDirectional()) {
        DirectionalLight* dirLight = static_cast<DirectionalLight*>(light);  // Cast to DirectionalLight

        // Directional light: direction is constant for all points
        Li = -glm::normalize(dirLight->direction); // Light vector is negative of the light's direction
        float dotLN = glm::dot(N,Li);
        dotLN = glm::max(0.0f, dotLN);  // Ensure no negative value

        // Calculate the diffuse color
        diffuseColor = KD * dotLN ; // Include light intensity
    }
    else if (light->isSpotlight()) {
        Spotlight* spotLight = static_cast<Spotlight*>(light);  // Cast to Spotlight

        // Spotlight: Vector from hit point to light position
        Li = glm::normalize(spotLight->position - hit.point);
        float dotLN = glm::dot(N,Li);
        dotLN = glm::max(0.0f, dotLN);  // Ensure no negative value

        // Calculate the diffuse color with spotlight intensity
        diffuseColor = KD * dotLN ; // Include light intensity
    }

    return diffuseColor ;
}

 glm::vec3 Phong::calcSpecularColor(Scene &scene, Intersection &hit, LightSource *light ,Ray& ray) {
    glm::vec3 Ks = glm::vec3(0.7, 0.7, 0.7);  // Specular reflection coefficient
    float n = hit.material.shininess;          // Shininess factor
    glm::vec3 v = glm::normalize(ray.origin - hit.point);  // Eye (view) vector
    glm::vec3 Ri;                             // Reflected light direction

    // Step 1: Get the normal at the intersection point
    glm::vec3 normal = glm::normalize(hit.normal);

    // Step 2: Determine the light direction
    glm::vec3 lightDir;
    if (light->isDirectional()) {
        // Directional light case
        lightDir = -glm::normalize(dynamic_cast<DirectionalLight*>(light)->direction);
    } else if (light->isSpotlight()) {
        // Spotlight case
        lightDir = glm::normalize(dynamic_cast<Spotlight*>(light)->position - hit.point);
    }

    // Step 3: Compute the reflection vector (Ri)
    Ri = glm::reflect(-lightDir, normal);

    // Step 4: Compute the dot product between the reflection vector (Ri) and the view vector (v)
    float specDot = glm::max( 0.0f, glm::dot(v, Ri));  // Avoid negative values

    // Step 5: Apply the shininess exponent (n)
    float specularFactor = glm::pow(specDot, n);  // Raise to the power of shininess

    // Step 6: Calculate the final specular color using the formula
    glm::vec3 specularColor = Ks * specularFactor ; 

    // Return the computed specular color
    return specularColor;
}

    bool Phong::occluded(Scene &scene, Intersection &hit, LightSource *light) {
    glm::vec3 lightDir;

    // Determine light direction based on light type
    if (light->isDirectional()) {
        // For directional light, the direction is fixed
        lightDir = -glm::normalize(dynamic_cast<DirectionalLight *>(light)->direction);
    } else if (light->isSpotlight()) {
        // For spotlight, the direction is from the hit point to the light position
        Spotlight *spotLight = dynamic_cast<Spotlight *>(light);
        lightDir = glm::normalize(spotLight->position - hit.point);

        // Check if the hit point is within the cutoff angle of the spotlight
        if (!spotLight->isWithinCutoff(hit.point)) {
            return true; // The point is outside the spotlight's cone, so it's occluded
        }
    }

    // Small offset to avoid self-intersection
    glm::vec3 shadowRayOrigin = hit.point + hit.normal * 1e-4f;

    // Create a ray from the intersection point towards the light source
    Ray shadowRay(shadowRayOrigin, lightDir);

    // Get the intersection from the shadow ray
    Intersection shadowIntersection = scene.GetHit(shadowRay);

    // Directional Light: Check only if there's any hit
    if (light->isDirectional()) {
        return shadowIntersection.hitObject; // Any hit means the light is occluded
    }

    // Spotlight: Check if the intersection is closer than the light position
    if (light->isSpotlight()) {
        Spotlight *spotLight = dynamic_cast<Spotlight *>(light);
        float lightDistance = glm::length(spotLight->position - shadowRayOrigin);

        // If there's a hit and it's closer than the light, the light is occluded
        if (shadowIntersection.hitObject && shadowIntersection.t < lightDistance) {
            return true; // The point is occluded by another object
        }
    }

    return false; // No occlusion, the light is not blocked
}




Ray Phong::ConstructOutRay(Ray &ray, glm::vec3 normal, glm::vec3 hitPoint) {
    // Normalize the direction and normal to ensure correctness
    glm::vec3 normalizedDirection = glm::normalize(ray.direction);
    glm::vec3 normalizedNormal = glm::normalize(normal);

    // Compute the reflected direction using the reflection formula
    glm::vec3 reflectedDirection = normalizedDirection - 2.0f * glm::dot(normalizedDirection, normalizedNormal) * normalizedNormal;

    // Offset the origin slightly along the normal to avoid self-intersections (epsilon)
    glm::vec3 offsetOrigin = hitPoint + 0.001f * normalizedNormal;

    // Return the new ray with the offset origin and reflected direction
    return Ray(offsetOrigin, glm::normalize(reflectedDirection));
}



Ray Phong::calcTransparencyRay(const Ray &ray, const glm::vec3 &normal, const glm::vec3 &hitPosition) {
    float n1 = 1.0f; // Index of refraction for air
    float n2 = 1.5f; // Refractive index for the material (sphere)
    float epsilon = 0.001f; // Small offset to prevent self-intersection

    glm::vec3 adjustedNormal = normal;

    // Compute the cosine of the angle between the ray and the normal
    float cosThetaI = glm::dot(-ray.direction, normal);

    // Adjust normal if the ray is exiting the object (from inside to outside)
    if (cosThetaI < 0) {
        cosThetaI = -cosThetaI;
        std::swap(n1, n2);
        adjustedNormal = -normal; // Invert the normal if inside
    }

    // Compute Snell's law: eta = n1 / n2
    float eta = n1 / n2;
    float sinThetaT2 = eta * eta * (1.0f - cosThetaI * cosThetaI);

    // Total internal reflection: reflect the ray
    if (sinThetaT2 > 1.0f) {
        glm::vec3 reflectionDirection = glm::reflect(ray.direction, adjustedNormal);
        glm::vec3 originOffset = hitPosition + epsilon * adjustedNormal;
        return Ray(originOffset, glm::normalize(reflectionDirection));
    }

    // Compute the refracted direction using Snell's law
    float cosThetaT = sqrt(1.0f - sinThetaT2);
    glm::vec3 refractedDirection = eta * ray.direction + (eta * cosThetaI - cosThetaT) * adjustedNormal;

    // Offset the hit position slightly to avoid self-intersection
    glm::vec3 originOffset = hitPosition + epsilon * adjustedNormal;

    return Ray(originOffset, glm::normalize(refractedDirection));
}
