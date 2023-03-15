#include "pch.h"
#include "Canvas.h"
#include "Projectile.h"
#include "Tuple.h"
#include "Matrix.h"
#include "Shapes.h"

#include "tests/CanvasTests.h"
#include "tests/FloatCompareTests.h"
#include "tests/ColorTests.h"
#include "tests/TupleTests.h"
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
    const auto twelve = point(0, -dist, 0);
    c.set(twelve+origo, RED);        
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

    auto surface = material(checkers_pattern(BLACK, WHITE));
    surface.reflective = 0.08f;
    const auto floor = plane(surface);    
    const auto checkersBall = sphere(surface,  translation(-4, 1.0f, 0)*scaling(2, 2, 2));  

    surface = material(stripe_pattern(BLACK, WHITE, rotation_y(math::HALF_PI) * scaling(1.5f, 1.5f, 1.5f)));
    const auto back_wall = plane(surface, translation(0, 0, 5) * rotation_x(math::HALF_PI));    

    const auto left_wall = plane(material(gradient_pattern(RED, BLACK)), translation(0, 0, 5) * rotation_y(-40*math::TO_RAD) * rotation_x(90*math::TO_RAD));    
    
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
    save_to_file(img, "output/chapter10_4_sRGB.ppm"sv);
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