#include "Phong.h"
#include "Scene.h"
#include <glm/glm.hpp>

glm::vec3 Phong::calcColor(Scene& scene, Ray& ray) {
    // Get the intersection point
    Intersection hit = scene.GetHit(ray);

    // If there's no hit, return black (background)
    if (!hit.hitObject) {
        return glm::vec3(0.0f, 0.0f, 0.0f); // Black color    
    }

 
    glm::vec3 color = calcEmissionColor(scene) + calcAmbientColor(scene ,hit );   

    for (int i = 0; i < scene.getNumLights(); i++) {
        LightSource* light = scene.getLight(i); // Use pointer instead of copying

      
        
            if (hit.ObjectType.compare("Sphere") == 0 ) 
                color += calcDiffuseColor(scene, hit, light) + calcSpecularColor(scene, hit, light);
            

            if (hit.ObjectType.compare("Plane") == 0 ) {
                // Plane: Apply checkerboard pattern for diffuse color
                return   checkerboardColor(hit.material.color, hit);

            //    glm::vec3 planeDiffuseColor = checkerboardColor(hit.material.color, hit);
                
            //    color +=  + planeDiffuseColor + calcSpecularColor(scene, hit, light);

            }
            
        }
    

    return color;
}


// function from the Assigment 
glm::vec3 Phong::checkerboardColor(glm::vec3 rgbColor, Intersection& hitPoint) {
    float scaleParameter = 0.5f;

    // Calculate checkerboard coordinates by scaling and flooring
    float x = floor(hitPoint.point.x / scaleParameter);
    float y = floor(hitPoint.point.y / scaleParameter);

    // Determine if the cell is "white" or "black" based on even/odd sum of coordinates
    bool isCheckerboardWhite = static_cast<int>(x + y) % 2 == 0;

    if (isCheckerboardWhite) {
        return rgbColor; // Original color for "white" cells
    } else {
        return 0.5f * rgbColor; // Dimmed color for "black" cells
    }
}








// phong funcitons

glm::vec3 Phong::calcAmbientColor(Scene& scene, Intersection& hit ) {
    glm::vec3 ambientIntensity = scene.ambient.intensity ; 


    return ambientIntensity * hit.material.color ;
}


glm::vec3 Phong::calcEmissionColor(Scene& scene) {
// Since the emission is (0, 0, 0), return that directly
    return glm::vec3(0.0f, 0.0f, 0.0f); // No emission
}


glm::vec3 Phong::calcDiffuseColor(Scene& scene, Intersection &hit, LightSource* light) {
    glm::vec3 Li;
    glm::vec3 diffuseColor;
    glm::vec3 N = glm::normalize(hit.normal);
    glm :: vec3 KD ; 

    // Check the light type
    if (light->isDirectional()) {
        DirectionalLight* dirLight = static_cast<DirectionalLight*>(light);  // Cast to DirectionalLight

        Li = dirLight->intensity ;
        float dotLN = glm::dot(Li, N);
        dotLN = glm::max(0.0f, dotLN);  // Ensure no negative value
        KD = hit.material.color ; 

        // Calculate the diffuse color
        diffuseColor = KD * dotLN;    //  perfect 
    }
    else if (light->isSpotlight()) {
        // Spotlight: The light has a position, a cutoff angle, and intensity diminishes with angle

        Spotlight* spotLight = static_cast<Spotlight*>(light);  // Cast to Spotlight
        Li = spotLight->intensity ; 
        float dotLN = glm::dot(Li, N);
        dotLN = glm::max(0.0f, dotLN);  // Ensure no negative value
        KD = hit.material.color ; 

        diffuseColor = KD * dotLN;    //  perfect 

    }

    return diffuseColor;
}




glm::vec3 Phong::calcSpecularColor(Scene &scene, Intersection &hit, LightSource *light) {
    glm::vec3 Ks = glm::vec3(0.7, 0.7, 0.7);  // Specular reflection coefficient
    float n = hit.material.shininess;          // Shininess factor
    glm::vec3 v = glm::normalize(scene.eye.position - hit.point);  // Eye (view) vector
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
    float specDot = glm::max(0.0f, glm::dot(v, Ri));  // Avoid negative values

    // Step 5: Apply the shininess exponent (n)
    float specularFactor = glm::pow(specDot, n);  // Raise to the power of shininess

    // Step 6: Calculate the final specular color using the formula
    glm::vec3 specularColor = Ks * specularFactor * light->intensity;  // Include light intensity

    // Return the computed specular color
    return specularColor;
}


bool Phong::occluded(Intersection& hit, Scene& scene, LightSource* light) {
    glm::vec3 lightDir;

    // Determine light direction based on light type
    if (light->isDirectional()) {
        // For directional light, the direction is fixed
        lightDir = -glm::normalize(dynamic_cast<DirectionalLight*>(light)->direction);
    } else if (light->isSpotlight()) {
        // For spotlight, the direction is from the hit point to the light position
        lightDir = glm::normalize(dynamic_cast<Spotlight*>(light)->position - hit.point);
    }

    // Create a ray from the intersection point towards the light source
    Ray shadowRay(hit.point + 0.001f * hit.normal, lightDir);  // Small offset to avoid self-intersection

    // Get the intersection from the shadow ray
    Intersection shadowIntersection = scene.GetHit(shadowRay);

    // If the intersection is closer than the distance to the light, it's occluded
    if (shadowIntersection.hitObject) {
        // Ensure we are checking that the occlusion is not from the same object or already hit object
        float distanceToLight = glm::length(shadowRay.direction);
        if (shadowIntersection.t < distanceToLight) {
            return true;  // The point is occluded by another object
        }
    }

    return false;  // No occlusion, the light is not blocked
}
