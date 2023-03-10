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
    auto shape = sphere();
    shape.surface.color = color(1, 0.2f, 1);        
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
    save_to_file(c, "output/chapter6_2.ppm"sv);    
}

TEST(DISABLED_Chapter7, CanRenderScene) {    
    auto c = Camera(400, 200, math::PI / 3);
    c.transform = view_transform(point(0.0f, 1.5f, -5.0f), point(0, 1, 0), vector(0, 1, 0));

    auto floor = sphere();
    floor.transform = scaling(10, 0.01f, 10);
    floor.surface = material();
    floor.surface.color = color(1, 0.9f, 0.9f);    
    floor.surface.specular = 0;

    auto left_wall = sphere();
    left_wall.transform = translation(0, 0, 5) * rotation_y(-math::PI / 4.0f) * rotation_x(math::PI / 2) * scaling(10.0f, 0.01f, 10.0f);    
    left_wall.surface = floor.surface;    

    auto right_wall = sphere();
    right_wall.transform = translation(0, 0, 5) * rotation_y(math::PI / 4.0f) * rotation_x(math::PI / 2) * scaling(10.0f, 0.01f, 10.0f);
    right_wall.surface = floor.surface;    

    auto middle = sphere();
    middle.transform = translation(-0.5f, 1, 0.5f);
    middle.surface = material();
    middle.surface.color = color(0.1f, 1, 0.5f);
    middle.surface.diffuse = 0.7f;
    middle.surface.specular = 0.4f;

    auto right = sphere();
    right.transform = translation(1.5f, 0.5f, -0.5f) * scaling(0.5f, 0.5f, 0.5f);
    right.surface = material();
    right.surface.color = color(0.5f, 1, 0.1f);
    right.surface.diffuse = 0.7f;
    right.surface.specular = 0.3f;

    auto left = sphere();
    left.transform = translation(-1.5f, 0.33f, -0.75f) * scaling(0.33f, 0.33f, 0.33f);
    left.surface = material();
    left.surface.color = color(1.0f, 0.8f, 0.1f);
    left.surface.diffuse = 0.7f;
    left.surface.specular = 0.3f;

    auto world = World({floor, left_wall, right_wall, left, middle, right});
    world.light = point_light(point(-10, 10, -10), color(1,1,1));
    
    const auto img = render(c, world);    
    save_to_file(img, "output/chapter7_1.ppm"sv);    
}

TEST(DISABLED_Chapter8, CanRenderSceneWithShadows) {    
    auto c = Camera(400, 200, math::PI / 3);
    c.transform = view_transform(point(0.0f, 1.5f, -5.0f), point(0, 1, 0), vector(0, 1, 0));

    auto floor = sphere();
    floor.transform = scaling(10, 0.01f, 10);
    floor.surface = material();
    floor.surface.color = color(1, 0.9f, 0.9f);
    floor.surface.specular = 0;

    auto left_wall = sphere();
    left_wall.transform = translation(0, 0, 5) * rotation_y(-math::PI / 4.0f) * rotation_x(math::PI / 2) * scaling(10.0f, 0.01f, 10.0f);
    left_wall.surface = floor.surface;

    auto right_wall = sphere();
    right_wall.transform = translation(0, 0, 5) * rotation_y(math::PI / 4.0f) * rotation_x(math::PI / 2) * scaling(10.0f, 0.01f, 10.0f);
    right_wall.surface = floor.surface;

    auto middle = sphere();
    middle.transform = translation(-0.5f, 1, 0.5f);
    middle.surface = material();
    middle.surface.color = color(0.1f, 1, 0.5f);
    middle.surface.diffuse = 0.7f;
    middle.surface.specular = 0.4f;

    auto right = sphere();
    right.transform = translation(1.5f, 0.5f, -0.5f) * scaling(0.5f, 0.5f, 0.5f);
    right.surface = material();
    right.surface.color = color(0.5f, 1, 0.1f);
    right.surface.diffuse = 0.7f;
    right.surface.specular = 0.3f;

    auto left = sphere();
    left.transform = translation(-1.5f, 0.33f, -0.75f) * scaling(0.33f, 0.33f, 0.33f);
    left.surface = material();
    left.surface.color = color(1.0f, 0.8f, 0.1f);
    left.surface.diffuse = 0.7f;
    left.surface.specular = 0.3f;

    auto world = World({ floor, left_wall, right_wall, left, middle, right });
    world.light = point_light(point(-10, 10, -10), color(1, 1, 1));

    const auto img = render(c, world);
    save_to_file(img, "output/chapter8_9.ppm"sv);
}

TEST(DISABLED_Chapter9, CanRenderPlanes) {    
    auto c = Camera(800, 400, math::PI / 3.0f);
    c.transform = view_transform(point(0.0f, 1.5f, -5.0f), point(0, 1, 0), vector(0, 1, 0));

    auto floor = plane();  
    floor.surface.color =  color(1, 0.9f, 0.9f);
    floor.surface.specular = 0.8f;

    auto back_wall = plane();
    back_wall.transform = translation(0, 0, 5) * rotation_x(math::HALF_PI);
    back_wall.surface = floor.surface;    

    auto middle = sphere();
    middle.transform = translation(-0.5f, 1, 0.5f);
    middle.surface = material();
    middle.surface.color = color(0.1f, 1, 0.5f);
    middle.surface.diffuse = 0.7f;
    middle.surface.specular = 0.4f;

    auto right = sphere();
    right.transform = translation(1.5f, 0.5f, -0.5f) * scaling(0.5f, 0.5f, 0.5f);
    right.surface = material();
    right.surface.color = color(0.5f, 1, 0.1f);
    right.surface.diffuse = 0.7f;
    right.surface.specular = 0.3f;

    auto left = sphere();
    left.transform = translation(-1.5f, 0.33f, -0.75f) * scaling(0.33f, 0.33f, 0.33f);
    left.surface = material();
    left.surface.color = color(1.0f, 0.8f, 0.1f);
    left.surface.diffuse = 0.7f;
    left.surface.specular = 0.3f;

    auto world = World({ floor, back_wall, left, middle, right });
    world.light = point_light(point(-10, 10, -10), color(1, 1, 1));

    const auto img = render(c, world);
    save_to_file(img, "output/chapter9_0.ppm"sv);
}

TEST(Chapter10, CanRenderPatterns) {    
    auto c = Camera(600, 400, math::PI / 3.0f);
    c.transform = view_transform(point(1.0f, 3.4f, -2.5f), point(0, 1, 0), vector(0, 1, 0));

    auto floor = plane(color(1, 0.9f, 0.9f));
    surface(floor).pattern = ring_pattern(WHITE, BLACK);
    surface(floor).reflective = 0.08f;
    
    auto mat = material();
    mat.pattern = checkers_pattern(RED, BLUE);    
    const auto middle = sphere(mat, translation(-0.5f, 1, 0.5f));    
    
    auto green = material(color(0.5f, 1, 0.1f));
    green.diffuse = 0.7f;
    green.specular = 0.3f;  
    green.reflective = 0.3f;
    const auto right = sphere(green, (translation(1.5f, 0.5f, -0.5f) * scaling(0.5f, 0.5f, 0.5f)));    

    auto left = sphere();
    left.transform = translation(-1.5f, 0.33f, -0.75f) * scaling(0.33f, 0.33f, 0.33f);
    left.surface = material();
    surface(left).pattern = stripe_pattern(GREEN, BLUE);
    transform(surface(left).pattern) = rotation_y(math::HALF_PI);
    left.surface.diffuse = 0.7f;
    left.surface.specular = 0.3f; 
    left.surface.reflective = 0.3f;

    auto world = World({ floor, left, middle, right });
    world.light = point_light(point(-10, 10, -10), color(1, 1, 1));

    const auto img = render(c, world);
    save_to_file(img, "output/chapter10_1.ppm"sv);
}
TEST(DISABLED_Chapter11, CanRenderReflectionsAndRefractions) {    
    auto c = Camera(600, 400, math::PI / 3.0f);
    c.transform = view_transform(point(1.0f, 3.4f, -2.5f), point(0, 1, 0), vector(0, 1, 0));

    auto floor = plane(color(1, 0.9f, 0.9f));
    surface(floor).specular = 0.8f;
    surface(floor).reflective = 0.08f;
    
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
    
    auto behind = right;
    surface(behind).color = color(1.0f, 0.8f, 0.1f);
    transform(behind) = translation(-1.5f, -0.3f, 4.0f) * scaling(1.2f, 1.2f, 1.2f);
    surface(behind).reflective = 0.0f;

    auto left = sphere();
    left.transform = translation(-1.5f, 0.33f, -0.75f) * scaling(0.33f, 0.33f, 0.33f);
    left.surface = material();
    left.surface.color = color(1.0f, 0.8f, 0.1f);
    left.surface.diffuse = 0.7f;
    left.surface.specular = 0.3f; 
    left.surface.reflective = 0.3f;

    auto world = World({ floor, left, middle, behind, right });
    world.light = point_light(point(-10, 10, -10), color(1, 1, 1));

    const auto img = render(c, world);
    save_to_file(img, "output/chapter11_5.ppm"sv);
}