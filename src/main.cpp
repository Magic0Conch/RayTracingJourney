#include <iostream>
#include "RTWeekend.h"
#include "Color.h"
#include "Sphere.h"
#include "HittableList.h"
#include "Camera.h"
#include "Material.h"

int main(){
    
    //Construct the world
    HittableList world;
    auto materialGround = make_shared<Lambertian>(Color3(0.8, 0.8, 0.0));
    auto materialCenter = make_shared<Lambertian>(Color3(0.7, 0.3, 0.3));
    auto materialLeft   = make_shared<Metal>(Color3(0.8, 0.8, 0.8),.3);
    auto materialRight  = make_shared<Metal>(Color3(0.8, 0.6, 0.2),1.0);

    world.add(make_shared<Sphere>(Point3( 0.0, -100.5, -1.0), 100.0, materialGround));
    world.add(make_shared<Sphere>(Point3( 0.0,    0.0, -1.0),   0.5, materialCenter));
    world.add(make_shared<Sphere>(Point3(-1.0,    0.0, -1.0),   0.5, materialLeft));
    world.add(make_shared<Sphere>(Point3( 1.0,    0.0, -1.0),   0.5, materialRight));
        
    //Camera
    Camera mainCamera;
    mainCamera.aspectRatio = 16.0/9.0;
    mainCamera.imageWidth = 400;
    mainCamera.samplesPerPixel = 100;
    //Render
    mainCamera.render(world);

}