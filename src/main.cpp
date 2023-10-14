#include <iostream>
#include <memory>
#include "RTWeekend.h"
#include "Color.h"
#include "Sphere.h"
#include "HittableList.h"
#include "Camera.h"
#include "Material.h"
#include "BVH.h"
#include "Texture.h"
#include "Quad.h"
#include "ConstantMedium.h"
#include "BVH.h"

void randomSpheres(){
    //Construct the world
    HittableList world;
    // auto ground_material = make_shared<Lambertian>(Color3(0.5, 0.5, 0.5));

    auto checker = make_shared<CheckerTexture>(0.32,Color3(.1,.3,.3),Color3(.9,.9,.9));

    world.add(make_shared<Sphere>(Point3(0,-1000,0), 1000, make_shared<Lambertian>(checker)));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = randomDouble();
            Point3 center(a + 0.9*randomDouble(), 0.2, b + 0.9*randomDouble());

            if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Color3::random() * Color3::random();
                    sphere_material = make_shared<Lambertian>(albedo);
                    auto center2 = center + Vec3(0,randomDouble(0,0.5),0);
                    world.add(make_shared<Sphere>(center,center2, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Color3::random(0.5, 1);
                    auto fuzz = randomDouble(0, 0.5);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center,0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<Dielectric>(1.5);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(Color3(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(Color3(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

    world = HittableList(make_shared<BVHNode>(world));

    //Camera
    Camera mainCamera;
    mainCamera.aspectRatio = 16.0/9.0;
    mainCamera.imageWidth = 400;
    mainCamera.samplesPerPixel = 100;
    mainCamera.maxDepth = 50;

    mainCamera.fovV = 20;
    mainCamera.lookfrom = Point3(13,2,3);
    mainCamera.lookat   = Point3(0,0,0);
    mainCamera.vUp      = Vec3(0,1,0);
    
    mainCamera.defocusAngle = 0.6;
    mainCamera.focusDist = 10.0;
    mainCamera.background = Color3(0.70, 0.80, 1.00);    
    //Render
    mainCamera.render(world);
}

void twoSpheres(){
    HittableList world;
    auto checker = make_shared<CheckerTexture>(0.8, Color3(.2, .3, .1), Color3(.9, .9, .9));
    world.add(make_shared<Sphere>(Point3(0,-10, 0), 10, make_shared<Lambertian>(checker)));
    world.add(make_shared<Sphere>(Point3(0, 10, 0), 10, make_shared<Lambertian>(checker)));
    Camera cam;
    
    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;

    cam.fovV = 20;
    cam.lookfrom = Point3(13,2,3);
    cam.lookat = Point3(0,0,0);
    cam.vUp = Vec3(0,1,0);

    cam.defocusAngle = 0;
    cam.background = Color3(0.70, 0.80, 1.00);
    cam.render(world);

}

void earth(){
    auto earthTexture = make_shared<ImageTexture>("earthmap.jpg");
    auto earthSurfaceMaterial = make_shared<Lambertian>(earthTexture);
    auto globe = make_shared<Sphere>(Point3(0,0,0),2,earthSurfaceMaterial);

    Camera cam;
    
    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;

    cam.fovV = 20;
    cam.lookfrom = Point3(0,0,12);
    cam.lookat = Point3(0,0,0);
    cam.vUp = Vec3(0,1,0);

    cam.defocusAngle = 0;
    cam.background = Color3(0.70, 0.80, 1.00);
    cam.render(HittableList(globe));

}

void twoPerlinSpheres(){
    HittableList world;
    auto pertexture = make_shared<NoiseTexture>(4);
    world.add(make_shared<Sphere>(Point3(0,-1000,0),1000,make_shared<Lambertian>(pertexture)));
    world.add(make_shared<Sphere>(Point3(0,2,0), 2, make_shared<Lambertian>(pertexture)));

    Camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;

    cam.fovV = 20;
    cam.lookfrom = Point3(13,2,3);
    cam.lookat = Point3(0,0,0);
    cam.vUp = Vec3(0,1,0);

    cam.defocusAngle = 0;
    cam.background = Color3(0.70, 0.80, 1.00);
    cam.render(world);

}

void quads() {
    HittableList world;

    // Materials
    auto left_red = make_shared<Lambertian>(Color3(1.0, 0.2, 0.2));
    auto back_green = make_shared<Lambertian>(Color3(0.2, 1.0, 0.2));
    auto right_blue = make_shared<Lambertian>(Color3(0.2, 0.2, 1.0));
    auto upper_orange = make_shared<Lambertian>(Color3(1.0, 0.5, 0.0));
    auto lower_teal = make_shared<Lambertian>(Color3(0.2, 0.8, 0.8));

    // Quads
    world.add(make_shared<Quad>(Point3(-3,-2, 5), Vec3(0, 0,-4), Vec3(0, 4, 0), left_red));
    world.add(make_shared<Quad>(Point3(-2,-2, 0), Vec3(4, 0, 0), Vec3(0, 4, 0), back_green));
    world.add(make_shared<Quad>(Point3( 3,-2, 1), Vec3(0, 0, 4), Vec3(0, 4, 0), right_blue));
    world.add(make_shared<Quad>(Point3(-2, 3, 1), Vec3(4, 0, 0), Vec3(0, 0, 4), upper_orange));
    world.add(make_shared<Quad>(Point3(-2,-3, 5), Vec3(4, 0, 0), Vec3(0, 0,-4), lower_teal));

    Camera cam;

    cam.aspectRatio = 1.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;

    cam.fovV = 80;
    cam.lookfrom = Point3(0,0,9);
    cam.lookat = Point3(0,0,0);
    cam.vUp = Vec3(0,1,0);

    cam.defocusAngle = 0;
    cam.background = Color3(0.70, 0.80, 1.00);
    cam.render(world);
}

void simpleLight() {
    HittableList world;

    auto pertext = make_shared<NoiseTexture>(4);
    world.add(make_shared<Sphere>(Point3(0,-1000,0), 1000, make_shared<Lambertian>(pertext)));
    world.add(make_shared<Sphere>(Point3(0,2,0), 2, make_shared<Lambertian>(pertext)));
    auto difflight = make_shared<DiffuseLight>(Color3(4,4,4));
    world.add(make_shared<Sphere>(Point3(0,7,0), 2, difflight));
    world.add(make_shared<Quad>(Point3(3,1,-2), Vec3(2,0,0), Vec3(0,2,0), difflight));

    Camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;
    cam.background = Color3(0,0,0);

    cam.fovV = 20;
    cam.lookfrom = Point3(26,3,6);
    cam.lookat = Point3(0,2,0);
    cam.vUp = Vec3(0,1,0);

    cam.defocusAngle = 0;

    cam.render(world);
}

void cornellBox() {
    HittableList world;

    auto red   = make_shared<Lambertian>(Color3(.65, .05, .05));
    auto white = make_shared<Lambertian>(Color3(.73, .73, .73));
    auto green = make_shared<Lambertian>(Color3(.12, .45, .15));
    auto light = make_shared<DiffuseLight>(Color3(15, 15, 15));

    world.add(make_shared<Quad>(Point3(555,0,0), Vec3(0,555,0), Vec3(0,0,555), green));
    world.add(make_shared<Quad>(Point3(0,0,0), Vec3(0,555,0), Vec3(0,0,555), red));
    world.add(make_shared<Quad>(Point3(343, 554, 332), Vec3(-130,0,0), Vec3(0,0,-105), light));
    world.add(make_shared<Quad>(Point3(0,0,0), Vec3(555,0,0), Vec3(0,0,555), white));
    world.add(make_shared<Quad>(Point3(555,555,555), Vec3(-555,0,0), Vec3(0,0,-555), white));
    world.add(make_shared<Quad>(Point3(0,0,555), Vec3(555,0,0), Vec3(0,555,0), white));
    

    shared_ptr<Hittable> box1 = box(Point3(0, 0, 0), Point3(165,330,165), white);
    box1 = make_shared<RotateY>(box1,15);
    box1 = make_shared<Translate>(box1,Vec3(265,0,295));
    world.add(box1);

    shared_ptr<Hittable> box2 = box(Point3(0, 0, 0), Point3(165,165,165), white);
    box2 = make_shared<RotateY>(box2,-18);
    box2 = make_shared<Translate>(box2,Vec3(130,0,65));
    
    world.add(box2);
    Camera cam;

    cam.aspectRatio = 1.0;
    cam.imageWidth = 600;
    cam.samplesPerPixel = 200;
    cam.maxDepth = 50;
    cam.background = Color3(0,0,0);

    cam.fovV = 40;
    cam.lookfrom = Point3(278, 278, -800);
    cam.lookat = Point3(278, 278, 0);
    cam.vUp = Vec3(0,1,0);

    cam.defocusAngle = 0;

    cam.render(world);
}

void cornellSmoke() {
    HittableList world;

    auto red = make_shared<Lambertian>(Color3(.65, .05, .05));
    auto white = make_shared<Lambertian>(Color3(.73, .73, .73));
    auto green = make_shared<Lambertian>(Color3(.12, .45, .15));
    auto light = make_shared<DiffuseLight>(Color3(7, 7, 7));

    world.add(make_shared<Quad>(Point3(555,0,0), Vec3(0,555,0), Vec3(0,0,555), green));
    world.add(make_shared<Quad>(Point3(0,0,0), Vec3(0,555,0), Vec3(0,0,555), red));
    world.add(make_shared<Quad>(Point3(113,554,127), Vec3(330,0,0), Vec3(0,0,305), light));
    world.add(make_shared<Quad>(Point3(0,555,0), Vec3(555,0,0), Vec3(0,0,555), white));
    world.add(make_shared<Quad>(Point3(0,0,0), Vec3(555,0,0), Vec3(0,0,555), white));
    world.add(make_shared<Quad>(Point3(0,0,555), Vec3(555,0,0), Vec3(0,555,0), white));

    shared_ptr<Hittable> box1 = box(Point3(0,0,0), Point3(165,330,165), white);
    box1 = make_shared<RotateY>(box1, 15);
    box1 = make_shared<Translate>(box1, Vec3(265,0,295));

    shared_ptr<Hittable> box2 = box(Point3(0,0,0), Point3(165,165,165), white);
    box2 = make_shared<RotateY>(box2, -18);
    box2 = make_shared<Translate>(box2, Vec3(130,0,65));

    world.add(make_shared<ConstantMedium>(box1, 0.01, Color3(0,0,0)));
    world.add(make_shared<ConstantMedium>(box2, 0.01, Color3(1,1,1)));

    Camera cam;

    cam.aspectRatio = 1.0;
    cam.imageWidth = 300;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;
    cam.background = Color3(0,0,0);

    cam.fovV = 40;
    cam.lookfrom = Point3(278, 278, -800);
    cam.lookat = Point3(278, 278, 0);
    cam.vUp = Vec3(0,1,0);

    cam.defocusAngle = 0;

    cam.render(world);
}

void finalScene(int imageWidth, int samplesPerPixel, int maxDepth) {
    HittableList boxes1;
    auto ground = make_shared<Lambertian>(Color3(0.48, 0.83, 0.53));

    int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i*w;
            auto z0 = -1000.0 + j*w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = randomDouble(1,101);
            auto z1 = z0 + w;

            boxes1.add(box(Point3(x0,y0,z0), Point3(x1,y1,z1), ground));
        }
    }

    HittableList world;

    world.add(make_shared<BVHNode>(boxes1));

    auto light = make_shared<DiffuseLight>(Color3(7, 7, 7));
    world.add(make_shared<Quad>(Point3(123,554,147),Vec3(300,0,0), Vec3(0,0,265), light));

    auto center1 = Point3(400, 400, 200);
    auto center2 = center1 + Vec3(30,0,0);
    //motion blur
    auto sphereMaterial = make_shared<Lambertian>(Color3(0.7, 0.3, 0.1));
    world.add(make_shared<Sphere>(center1, center2, 50, sphereMaterial));

    world.add(make_shared<Sphere>(Point3(260, 150, 45), 50, make_shared<Dielectric>(1.5)));
    world.add(make_shared<Sphere>(
        Point3(0, 150, 145), 50, make_shared<Metal>(Color3(0.8, 0.8, 0.9), 1.0)
    ));

    //subsurface material:volume inside a dielectric
    auto boundary = make_shared<Sphere>(Point3(360,150,145), 70, make_shared<Dielectric>(1.5));
    world.add(boundary);
    world.add(make_shared<ConstantMedium>(boundary, 0.2, Color3(0.2, 0.4, 0.9)));
    //global effects
    boundary = make_shared<Sphere>(Point3(0,0,0), 5000, make_shared<Dielectric>(1.5));
    world.add(make_shared<ConstantMedium>(boundary, .0001, Color3(1,1,1)));

    auto emat = make_shared<Lambertian>(make_shared<ImageTexture>("earthmap.jpg"));
    world.add(make_shared<Sphere>(Point3(400,200,400), 100, emat));
    auto pertext = make_shared<NoiseTexture>(0.1);
    world.add(make_shared<Sphere>(Point3(220,280,300), 80, make_shared<Lambertian>(pertext)));

    HittableList boxes2;
    auto white = make_shared<Lambertian>(Color3(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(make_shared<Sphere>(Point3::random(0,165), 10, white));
    }

    world.add(make_shared<Translate>(
        make_shared<RotateY>(
            make_shared<BVHNode>(boxes2), 15),
            Vec3(-100,270,395)
        )
    );

    Camera cam;

    cam.aspectRatio = 1.0;
    cam.imageWidth = imageWidth;
    cam.samplesPerPixel = samplesPerPixel;
    cam.maxDepth = maxDepth;
    cam.background = Color3(0,0,0);

    cam.fovV = 40;
    cam.lookfrom = Point3(478, 278, -600);
    cam.lookat = Point3(278, 278, 0);
    cam.vUp = Vec3(0,1,0);

    cam.defocusAngle = 0;

    cam.render(world);
}

int main(){
    switch (9) {
        case 1: randomSpheres();break;
        case 2: twoSpheres();break;
        case 3: earth();break;
        case 4: twoPerlinSpheres();break;
        case 5: quads(); break;
        case 6: simpleLight();break;
        case 7: cornellBox();break;
        case 8: cornellSmoke();break;
        case 9: finalScene(800, 10000, 40); break;
        default: finalScene(400, 250, 6); break;
    }

}