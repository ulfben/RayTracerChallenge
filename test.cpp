#include "pch.h"
#include "Canvas.h"
#include "Projectile.h"
#include "Tuple.h"
#include "Matrix.h"
#include "Shapes.h"

//#define RYML_SINGLE_HDR_DEFINE_NOW
//#include "external/rapidyaml.h"

#include "tests/CanvasTests.h"
#include "tests/FloatCompareTests.h"
#include "tests/ColorTests.h"
#include "tests/PointTests.h"
#include "tests/VectorTests.h"
#include "tests/MatrixTests.h"
#include "tests/MatrixTransformationTests.h"
#include "tests/RayTests.h"
#include "tests/SphereTests.h"
#include "tests/PhongReflectionTests.h"
#include "tests/WorldTests.h"
#include "tests/CameraTests.h"
#include "tests/PlaneTests.h"
#include "tests/ReflectionTests.h"
#include "tests/TransparencyTests.h"
#include "tests/PatternTests.h"
#include "tests/CubeTests.h"
#include "tests/CylinderTests.h"
#include "tests/ConeTests.h"
#include "tests/StringHelpersTest.h"
#include "tests/GroupTests.h"

TEST(DISABLED_Chapter2, CanOutputPPM) {    
    auto c = Canvas(300, 300);
    const auto bottomEdge = static_cast<Real>(c.height());
    const auto vel = normalize(vector(1, -1.8f, 0)) * 11.25f;
    auto p = Projectile(point(0, bottomEdge, 0), vel);
    while (p.y() <= bottomEdge) {
        p.update();
        p.render(c);
    }
    save_to_file(c, "output/chapter2.ppm"sv);    
}

TEST(DISABLED_Chapter4, CanCenterOrigo) {    
    auto c = Canvas(200, 200);
    const auto origo = point(c.widthf() / 2, c.heightf() / 2, 0);        
    c.set(origo, GREEN); 
    save_to_file(c, "output/chapter4_0.ppm"sv);          
}

TEST(DISABLED_Chapter4, CanDrawTwelveOClock) {    
    auto c = Canvas(200, 200);
    const auto origo = point(c.widthf() / 2, c.heightf() / 2, 0);        
    c.set(origo, GREEN);        
    const auto dist = c.widthf() * 0.375f;        
    const auto twelve = vector(0, -dist, 0);
    c.set(origo+twelve, RED);        
    save_to_file(c, "output/chapter4_1.ppm"sv);      
}

TEST(DISABLED_Chapter4, CanDrawClock) {
    auto c = Canvas(200, 200);
    const auto origo = point(c.widthf() / 2, c.heightf() / 2, 0);                 
    const auto dist = c.widthf()*0.375f; 
    const auto trans = translation(origo);
    const auto twelve = point(0, -dist, 0);
    const auto angle = (math::TWO_PI / 12.0f);//full circle is 2xPI, clock face has 12 positions. 
    for (auto i = 0; i < 12; ++i) {
        const auto rot = rotation_z(static_cast<float>(i) * angle);         
        const auto p = trans * rot * twelve;
        c.set(p, WHITE);
    }
    save_to_file(c, "output/chapter4_4.ppm"sv);    
}

TEST(DISABLED_Chapter5, CanRenderSilhouetteOfASphere) {       
    using size_type = Canvas::size_type;
    auto c = Canvas(100, 100);
    const auto shape = sphere();    
    const auto ray_origin = point(0, 0, -5);    
    const auto wall_z = 10.0f;
    const auto wall_size = 7.0f;
    const auto wall_center = wall_size / 2.0f;    
    const auto pixel_size = wall_size / c.widthf(); //assuming canvas is square
    for (size_type y = 0; y < c.width(); ++y) {
        const auto world_y = wall_center - pixel_size * y;
        for (size_type x = 0; x < c.height(); ++x) {
            const auto world_x = wall_center - pixel_size * x;
            const auto target_pos = point(world_x, world_y, wall_z);
            const auto r = ray(ray_origin, normalize(target_pos - ray_origin));
            const auto xs = intersect(shape, r);
            if (closest(xs)) {
                c.set(x, y, RED);
            }
        }
    }
    save_to_file(c, "output/chapter5_0.ppm"sv);    
}

TEST(DISABLED_Chapter6, CanRenderPhongShadedSphere) {       
    using size_type = Canvas::size_type;
    auto c = Canvas(100, 100);
    const auto shape = sphere(color(1, 0.2f, 1));    
    const auto light = point_light(point(10, 10, -10), WHITE);
    const auto ray_origin = point(0, 0, -5);    
    const auto wall_z = 10.0f;
    const auto wall_size = 7.0f;
    const auto wall_center = wall_size / 2.0f;    
    const auto pixel_size = wall_size / c.widthf(); //assuming canvas is square
    for (size_type y = 0; y < c.width(); ++y) {
        const auto world_y = wall_center - pixel_size * y;
        for (size_type x = 0; x < c.height(); ++x) {
            const auto world_x = wall_center - pixel_size * x;
            const auto target_pos = point(world_x, world_y, wall_z);
            const auto r = ray(ray_origin, normalize(target_pos - ray_origin));
            const auto xs = intersect(shape, r);            
            if (const auto hit = closest(xs)) {
                const auto point = position(r , hit.t); 
                const auto normal = normal_at(hit.object(), point);
                const auto eye = -r.direction; 
                const auto color = lighting(hit.surface(), light, point, eye, normal);
                c.set(x, y, color);
            }
        }
    }
    save_to_file(c, "output/chapter6_2_sRGB.ppm"sv);    
}

TEST(DISABLED_Chapter7, CanRenderScene) {    
    const auto c = Camera(400, 200, math::PI / 3, 
        view_transform(point(0.0f, 1.5f, -5.0f), point(0, 1, 0), vector(0, 1, 0)));
    
    auto floorSurface = material(color(1, 0.9f, 0.9f));       
    floorSurface.specular = 0;
    
    const auto floor = sphere(floorSurface, scaling(10, 0.01f, 10));   
    const auto left_wall = sphere(floorSurface, translation(0, 0, 5) * rotation_y(-math::PI / 4.0f) * rotation_x(math::PI / 2) * scaling(10.0f, 0.01f, 10.0f));   
    const auto right_wall = sphere(floorSurface, translation(0, 0, 5) * rotation_y(math::PI / 4.0f) * rotation_x(math::PI / 2) * scaling(10.0f, 0.01f, 10.0f));    

    auto middleSurface = material(color(0.1f, 1, 0.5f));    
    middleSurface.diffuse = 0.7f;
    middleSurface.specular = 0.4f;
    const auto middle = sphere(translation(-0.5f, 1, 0.5f));
    
    auto rightSurface = material(color(0.5f, 1, 0.1f));    
    rightSurface.diffuse = 0.7f;
    rightSurface.specular = 0.3f;
    auto right = sphere(rightSurface, translation(1.5f, 0.5f, -0.5f) * scaling(0.5f, 0.5f, 0.5f));
    

    auto leftSurface = material(color(1.0f, 0.8f, 0.1f));
    leftSurface.diffuse = 0.7f;
    leftSurface.specular = 0.3f;
    auto left = sphere(translation(-1.5f, 0.33f, -0.75f) * scaling(0.33f, 0.33f, 0.33f));    

    const auto world = World({floor, left_wall, right_wall, left, middle, right}, 
                              point_light(point(-10, 10, -10), color(1,1,1)));    
    
    const auto img = render(c, world);    
    save_to_file(img, "output/chapter7_1_sRGB.ppm"sv);    
}

TEST(DISABLED_Chapter8, CanRenderSceneWithShadows) {    
    const auto c = Camera(400, 200, math::PI / 3, 
        view_transform(point(0.0f, 1.5f, -5.0f), point(0, 1, 0), vector(0, 1, 0)));

    auto floorSurface = material(color(1, 0.9f, 0.9f));    
    floorSurface.specular = 0;
    const auto floor = sphere(floorSurface, scaling(10, 0.01f, 10));
    const auto left_wall = sphere(floorSurface, translation(0, 0, 5) * rotation_y(-math::PI / 4.0f) * rotation_x(math::PI / 2) * scaling(10.0f, 0.01f, 10.0f));
    const auto right_wall = sphere(floorSurface, translation(0, 0, 5) * rotation_y(math::PI / 4.0f) * rotation_x(math::PI / 2) * scaling(10.0f, 0.01f, 10.0f));    

    auto middleSurface = material(color(0.1f, 1, 0.5f));    
    middleSurface.diffuse = 0.7f;
    middleSurface.specular = 0.4f;
    auto middle = sphere(middleSurface, translation(-0.5f, 1, 0.5f));
    
    auto rightSurface = material(color(0.5f, 1, 0.1f));    
    rightSurface.diffuse = 0.7f;
    rightSurface.specular = 0.3f;
    const auto right = sphere(rightSurface, translation(1.5f, 0.5f, -0.5f) * scaling(0.5f, 0.5f, 0.5f));    

    auto leftSurface = material(color(1.0f, 0.8f, 0.1f));    
    leftSurface.diffuse = 0.7f;
    leftSurface.specular = 0.3f;
    const auto left = sphere(leftSurface, translation(-1.5f, 0.33f, -0.75f) * scaling(0.33f, 0.33f, 0.33f));

    const auto world = World({ floor, left_wall, right_wall, left, middle, right }, 
                             point_light(point(-10, 10, -10), color(1, 1, 1)));

    const auto img = render(c, world);
    save_to_file(img, "output/chapter8_9_sRGB.ppm"sv);
}

TEST(DISABLED_Chapter9, CanRenderPlanes) {    
    const auto c = Camera(800, 400, math::PI / 3.0f, 
        view_transform(point(0.0f, 1.5f, -5.0f), point(0, 1, 0), vector(0, 1, 0)));
    
    auto floorMat = material(color(1, 0.9f, 0.9f));
    floorMat.specular = 0.8f;    
    
    const auto floor = plane(floorMat);          
    const auto back_wall = plane(floorMat, translation(0, 0, 5) * rotation_x(math::HALF_PI));    

    auto middleMat = material(color(0.1f, 1, 0.5f));    
    middleMat.diffuse = 0.7f;
    middleMat.specular = 0.4f;
    const auto middle = sphere(middleMat, translation(-0.5f, 1, 0.5f));
   
    auto rightMat = material(color(0.5f, 1, 0.1f));    
    const auto right = sphere(translation(1.5f, 0.5f, -0.5f) * scaling(0.5f, 0.5f, 0.5f));
    
    auto leftMat = material(color(1.0f, 0.8f, 0.1f));    
    leftMat.diffuse = 0.7f;
    leftMat.specular = 0.3f;
    const auto left = sphere(leftMat, translation(-1.5f, 0.33f, -0.75f) * scaling(0.33f, 0.33f, 0.33f));    

    const auto world = World({ floor, back_wall, left, middle, right }, 
                            point_light(point(-10, 10, -10), color(1, 1, 1)));    
    const auto img = render(c, world);
    save_to_file(img, "output/chapter9_0_sRGB.ppm"sv);
}

TEST(DISABLED_Chapter10, CanRenderPatterns) {
    const auto c = Camera(600, 400, math::PI / 3.0f, 
        view_transform(point(0.0f, 5.0f, -10.0f), point(0, 1, 0), vector(0, 1, 0)));

    auto surface = material(checkers_pattern(BLACK, WHITE, scaling(0.5f, 0.5f, 0.5f)));
    surface.reflective = 0.1f;
    const auto floor = plane(surface);    
    
    surface = material(stripe_pattern(BLACK, WHITE, scaling(0.1f, 0.1f, 0.1f)*rotation_z(22*math::TO_RAD)));
    surface.reflective = 0.1f;
    const auto checkersBall = sphere(surface,  translation(-4, 2.0f, 0)*scaling(2, 2, 2));  

    surface = material(stripe_pattern(BLACK, WHITE, rotation_y(math::HALF_PI) * scaling(1.5f, 1.5f, 1.5f)));
    const auto back_wall = plane(surface, translation(0, 0, 5) * rotation_x(math::HALF_PI));    

    surface = material(gradient_pattern(RED, BLACK, scaling(8.0f, 1, 1)));
    const auto left_wall = plane(surface, translation(0, 0, 5) * rotation_y(-40*math::TO_RAD) * rotation_x(90*math::TO_RAD));    
    
    surface = material(ring_pattern(BLACK, WHITE, scaling(0.2f, .2f, .2f) * rotation(-35*math::TO_RAD, 0.0f, 45*math::TO_RAD)));                
    surface.reflective = 0.2f;
    const auto middle = sphere(surface, translation(0, 1.0f, 0));

    auto mat = glass();
    mat.color = color(0.0f, 0.0f, 0.1f);
    mat.diffuse = 0.1f;
    mat.ambient = 0.1f;
    mat.specular = 1.0f;
    mat.reflective = 0.9f;   
    mat.shininess = 300.0f;
    mat.transparency = 0.9f;      
    auto right = sphere(mat, translation(2.5f, 1.0f, -1.5f));

    const auto world = World({ floor, checkersBall, back_wall, middle, right, left_wall }, 
                              point_light(point(-10, 10, -10), color(1, 1, 1)));    
    const auto img = render(c, world);
    save_to_file(img, "output/chapter10_5_sRGB.ppm"sv);
}

TEST(DISABLED_Chapter11, CanRenderReflectionsAndRefractions) {    
    const auto c = Camera(600, 400, math::PI / 3.0f,
        view_transform(point(1.0f, 3.4f, -2.5f), point(0, 1, 0), vector(0, 1, 0)));

    auto floorMat = material(color(1, 0.9f, 0.9f));
    floorMat.specular = 0.8f;
    floorMat.reflective = 0.08f;
    const auto floor = plane(floorMat);    
    
    auto mat = glass();
    mat.color = color(0.0f, 0.1f, 0.0f); //the more reflective or transparent, the darker the color need to be,
    mat.diffuse = 0.1f; //reflection and refraction is added to the surface color
    mat.ambient = 0.1f; //so we tone down the diffuse and ambient to avoid it becoming too bright
    mat.specular = 1.0f; //specular + reflective == fresnel effct
    mat.reflective = 0.9f;   
    mat.shininess = 300.0f; //reflective and transparent objects pairs well with a tight specular highlight         
    mat.transparency = 0.9f;        
    
    const auto middle = sphere(mat, translation(-0.5f, 1, 0.5f));    
    
    auto green = material(color(0.5f, 1, 0.1f));
    green.diffuse = 0.7f;
    green.specular = 0.3f;  
    green.reflective = 0.3f;
    const auto right = sphere(green, (translation(1.5f, 0.5f, -0.5f) * scaling(0.5f, 0.5f, 0.5f)));    
    
    auto behindMat = material(color(1.0f, 0.8f, 0.1f));
    behindMat.reflective = 0.0f;    
    const auto behind = sphere(behindMat, translation(-1.5f, -0.3f, 4.0f) * scaling(1.2f, 1.2f, 1.2f));    

    auto leftMat = material(color(1.0f, 0.8f, 0.1f));    
    leftMat.diffuse = 0.7f;
    leftMat.specular = 0.3f; 
    leftMat.reflective = 0.3f;
    const auto left = sphere(leftMat, translation(-1.5f, 0.33f, -0.75f) * scaling(0.33f, 0.33f, 0.33f));
    
    const auto world = World({ floor, left, middle, behind, right }, 
                                point_light(point(-10, 10, -10), color(1, 1, 1)));    
    const auto img = render(c, world);
    save_to_file(img, "output/chapter11_5_sRGB.ppm"sv);
}

TEST(DISABLED_Chapter12, CanRenderCubes) {    
    const auto c = Camera(600, 400, math::PI / 3.0f, 
        view_transform(point(0.0f, 5.0f, -10.0f), point(0, 1, 0), vector(0, 1, 0)));

    auto surface = material(checkers_pattern(BLACK, WHITE, scaling(0.5f, 0.5f, 0.5f)));
    surface.reflective = 0.1f;
    const auto floor = plane(surface);    
    
    surface = material(stripe_pattern(BLACK, WHITE, scaling(0.1f, 0.1f, 0.1f)*rotation_z(45*math::TO_RAD)));
    surface.reflective = 0.1f;
    const auto checkersBall = cube(surface,  translation(-4, 2.0f, 0)*scaling(2, 2, 2));  

    surface = material(stripe_pattern(BLACK, WHITE, rotation_y(math::HALF_PI) * scaling(1.5f, 1.5f, 1.5f)));
    const auto back_wall = plane(surface, translation(0, 0, 5) * rotation_x(math::HALF_PI));    

    surface = material(gradient_pattern(RED, BLACK, scaling(8.0f, 1, 1)));
    const auto left_wall = plane(surface, translation(0, 0, 5) * rotation_y(-40*math::TO_RAD) * rotation_x(90*math::TO_RAD));    
        
    surface.reflective = 0.0f;
    surface.transparency = 0.0f;
    const auto middle = cube(surface, translation(0, 1.0f, 0)*rotation_y(45*math::TO_RAD));

    auto mat = glass();
    mat.color = color(0.0f, 0.0f, 0.1f);
    mat.diffuse = 0.1f;
    mat.ambient = 0.1f;
    mat.specular = 1.0f;
    mat.reflective = 0.9f;   
    mat.shininess = 300.0f;
    mat.transparency = 0.9f;      
    auto right = cube(mat, translation(2.5f, 1.0f, -1.5f));

    const auto world = World({ floor, checkersBall, back_wall, middle, right, left_wall }, 
                              point_light(point(-10, 10, -10), color(1, 1, 1)));    
    const auto img = render(c, world);
    save_to_file(img, "output/chapter12_1.ppm"sv);
}

TEST(DISABLED_Chapter11, CanRenderBookScene) {    
    using math::TO_RAD;
    
    const auto ROT = math::HALF_PI; //book rotation 1.5708f
    const auto DIST = 5.0f;
    const auto FOV = 0.5f;

    const auto c = Camera(1024, 768, FOV,
        view_transform(point(-4.5f, 0.85f, -4.0f), point(0, 0.85f, 0), vector(0, 1, 0)));        

    auto floor_material = material(checkers_pattern(BLACK, color(0.75f)));
    floor_material.ambient = 0.5f;
    floor_material.diffuse = 0.4f;
    floor_material.specular = 0.8f;
    floor_material.reflective = 0.1f;    
    const auto floor = plane(floor_material, rotation_y(math::PI));    

    auto ceiling_material = material(checkers_pattern(color(0.85f), WHITE, scaling(0.2f)));
    ceiling_material.ambient = 0.5f;
    ceiling_material.specular = 0;

    auto transf = translation(0, DIST, 0);
    const auto ceiling = plane(ceiling_material, transf);

    auto wallpaper = material(checkers_pattern(BLACK, color(0.75f), scaling(0.5f)));
    wallpaper.specular = 0;           

    transf = translation(0, 0, DIST) * rotation_x(ROT);
    const auto north_wall = plane(wallpaper, transf);

    transf = translation(0, 0, -DIST) * rotation_x(ROT);
    const auto south_wall = plane(wallpaper, transf);
        
    transf = translation(-DIST,0,0)*rotation_z(ROT);    
    auto west_wall = plane(material(wallpaper, rotation_y(ROT)), transf);   
    
    transf = translation(DIST, 0, 0)*rotation_z(ROT);
    const auto east_wall = plane(material(wallpaper, rotation_y(ROT)), transf);

    transf = translation(4.0f, 1.0f, 4.0f);
    auto red_sphere = sphere(sRGB_to_linear(color(0.8f, 0.1f, 0.3f)), transf);
    red_sphere.surface().specular = 0;

    transf = translation(4.6f, 0.4f, 2.9f) * scaling(0.4f);
    auto green_sphere = sphere(sRGB_to_linear(color(0.1f, 0.8f, 0.2f)), transf);
    green_sphere.surface().shininess = 200;

    transf = translation(2.6f, 0.6f, 4.4f) * scaling(0.6f);
    auto blue_sphere = sphere(sRGB_to_linear(color(0.2f, 0.1f, 0.8f)), transf);
    blue_sphere.surface().shininess = 10;
    blue_sphere.surface().specular = 0.4f;

    auto glass_material = material(sRGB_to_linear(color(0.8f, 0.8f, 0.9f)));
    glass_material.ambient = 0; 
    glass_material.diffuse = 0.2f;
    glass_material.specular = 0.9f; 
    glass_material.shininess = 300.0f;
    glass_material.transparency = 0.8f;
    glass_material.refractive_index = IoR::glass;

    transf = scaling(1.0f, 1.0f, 1.0f) * translation(0.25f, 1.0f, .0f);
    auto glass_sphere = sphere(glass_material, transf);


    const auto world = World({ floor, glass_sphere, red_sphere, green_sphere, blue_sphere, ceiling, north_wall, south_wall, east_wall, west_wall }, 
                                point_light(point(-4.9f, 4.9f, 1), color(1, 1, 1)));    
    const auto img = render(c, world);
    save_to_file(img, "output/chapter11_bookscene.ppm"sv);
}

TEST(DISABLED_Chapter13, CanRenderCylinders) {    
    const auto mighty_slate = sRGB_to_linear(color(0.33f, 0.38f, 0.44f));
    const auto pacifica = sRGB_to_linear(color(0.31f, 0.81f, 0.77f));
    const auto c = Camera(600, 400, math::PI / 3.0f, 
        view_transform(point(0.0f, 5.0f, -10.0f), point(0, 1, 0), vector(0, 1, 0)));
        
    auto surface = material(checkers_pattern(mighty_slate, pacifica));   
    const auto floor = plane(surface);    
    
    surface = material(stripe_pattern(mighty_slate, pacifica, scaling(0.1f, 0.1f, 0.1f)*rotation_z(45*math::TO_RAD)));
    surface.reflective = 0.1f;
    const auto middle = cylinder(surface);  
    
    surface = material(stripe_pattern(BLACK, WHITE, scaling(0.3f, 0.3f, 0.3f)));    
    surface.transparency = 0.0f;
    const auto left = cylinder(0.0f, 4.0f, surface, rotation(0, 0, 22*math::TO_RAD)*translation(-4, 0, 0));    

    surface.reflective = 0.2f;
    const auto right = closed_cylinder(0.0f, 3.0f, surface, translation(3, 0, 0));

    const auto world = World({ floor, left, middle, right}, 
                              point_light(point(-10, 10, -10), color(1, 1, 1)));    
    const auto img = render(c, world);
    save_to_file(img, "output/chapter13_4.ppm"sv);
}

TEST(DISABLED_Chapter13, CanRenderCones) {    
    const auto mighty_slate = sRGB_to_linear(color(0.33f, 0.38f, 0.44f));
    const auto pacifica = sRGB_to_linear(color(0.31f, 0.81f, 0.77f));
    const auto c = Camera(600, 400, math::PI / 3.0f, 
        view_transform(point(0.0f, 5.0f, -10.0f), point(0, 1, 0), vector(0, 1, 0)));
        
    auto surface = material(checkers_pattern(mighty_slate, pacifica));   
    const auto floor = plane(surface);    
    
    surface = material(stripe_pattern(mighty_slate, pacifica, scaling(0.1f, 0.1f, 0.1f)*rotation_z(45*math::TO_RAD)));
    surface.reflective = 0.1f;
    const auto middle = cone(surface);  
    
    surface = material(stripe_pattern(BLACK, WHITE, scaling(0.3f, 0.3f, 0.3f)));    
    surface.transparency = 0.0f;
    const auto left = cone(0.0f, 4.0f, surface, rotation(0, 0, 22*math::TO_RAD)*translation(-4, 0, 0));    

    surface.reflective = 0.2f;
    const auto right = cone(0.0f, 3.0f, surface, translation(3, 0, 0));

    const auto world = World({ floor, left, middle, right}, 
                              point_light(point(-10, 10, -10), color(1, 1, 1)));    
    const auto img = render(c, world);
    save_to_file(img, "output/chapter13_5.ppm"sv);
}

TEST(DISABLED_Bonus01, CanTextureMap) {        
    const auto c = Camera(400, 400, 0.5f, 
        view_transform(point(0.0f, 0.0f, -5.0f), point(0, 0, 0), vector(0, 1, 0)));
    const auto light = point_light(point(-10, 10, -10), color(1, 1, 1));
    
    const auto green = color_from_srgb(0, 0.5f, 0);
    const auto texture = texture_map(uv_checkers(20, 10, green, WHITE), spherical_map);    
    Material mat = material(texture);
    mat.ambient = 0.1f;
    mat.specular = 0.4f;
    mat.shininess = 10.0f;
    mat.diffuse = 0.6f;
    const auto s = sphere(mat);
    const auto world = World({s}, 
                              light);    
    const auto img = render(c, world);
    save_to_file(img, "output/bonus_chapter_texture_01.ppm"sv);
}

TEST(DISABLED_Bonus01, CanCubeMap) {        
    const auto c = Camera(400, 400, 0.5f, 
        view_transform(point(0.0f, 0.0f, -5.0f), point(0, 0, 0), vector(0, 1, 0)));
    const auto light = point_light(point(-10, 10, -10), color(1, 1, 1));
    
    Material mat = material(CubeMap());
    mat.ambient = 0.1f;
    mat.specular = 0.4f;
    mat.shininess = 10.0f;
    mat.diffuse = 0.6f;
    const auto s = cube(mat,  scaling(0.4f)*rotation(22*math::TO_RAD, 45*math::TO_RAD, 22*math::TO_RAD));
    const auto world = World({s}, 
                              light);    
    const auto img = render(c, world);
    save_to_file(img, "output/bonus_chapter_texture_02.ppm"sv);
}