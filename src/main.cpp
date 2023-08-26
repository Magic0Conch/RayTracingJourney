#include <iostream>
#include "RTWeekend.h"
#include "Color.h"
#include "Sphere.h"
#include "HittableList.h"
#include "Camera.h"


int main(){
    
    //Construct the world
    HittableList world;
    world.add(make_shared<Sphere>(Point3(0,0,-1), 0.5));
    world.add(make_shared<Sphere>(Point3(0,-100.5,-1), 100));
    
    //Camera
    Camera mainCamera;
    mainCamera.aspectRatio = 16.0/9.0;
    mainCamera.imageWidth = 400;
    mainCamera.samplesPerPixel = 100;
    //Render
    mainCamera.render(world);

}