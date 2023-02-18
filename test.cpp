#include "pch.h"
#include "Canvas.h"
#include "Projectile.h"
#include "Tuple.h"
#include "Matrix.h"

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

//TEST(Chapter2, CanOutputPPM) {
//    auto c = Canvas(300, 300);
//    const auto bottomEdge = static_cast<Real>(c.height());
//    const auto vel = normalize(vector(1, -1.8f, 0)) * 11.25f;
//    auto p = Projectile(point(0, bottomEdge, 0), vel);
//    while (p.y() <= bottomEdge) {
//        p.update();
//        p.render(c);
//    }
//    std::ofstream ofs("output/chapter2.ppm", std::ofstream::out);
//    ofs << c.to_ppm();
//    EXPECT_TRUE(true);
//}


//TEST(Chapter4, CanCenterOrigo) {
//    const auto GREEN = color(0, 1, 0);
//    auto c = Canvas(200, 200);
//    const auto origo = point(c.widthf() / 2, c.heightf() / 2, 0);        
//    c.set(origo, GREEN);    
//    std::ofstream ofs("output/chapter4_0.ppm", std::ofstream::out);
//    ofs << c.to_ppm();   
//}
//
//TEST(Chapter4, CanDrawTwelveOClock) {
//    const auto RED = color(1, 0, 0);
//    const auto GREEN = color(0, 1, 0);    
//    auto c = Canvas(200, 200);
//    const auto origo = point(c.widthf() / 2, c.heightf() / 2, 0);        
//    c.set(origo, GREEN);        
//    const auto dist = c.widthf() * 0.375f;        
//    const auto twelve = point(0, -dist, 0);
//    c.set(twelve+origo, RED);    
//    std::ofstream ofs("output/chapter4_1.ppm", std::ofstream::out);
//    ofs << c.to_ppm();   
//}
//
//TEST(Chapter4, CanDraw12_3_6_9) {
//    const auto RED = color(1, 0, 0);    
//    const auto GREEN = color(0, 1, 0);
//    auto c = Canvas(200, 200);
//    const auto origo = point(c.widthf() / 2, c.heightf() / 2, 0);        
//    c.set(origo, GREEN);        
//    const auto dist = c.widthf() * 0.375f;        
//    const auto twelve = translation(0, -dist, 0);
//    const auto three = translation(dist, 0, 0);
//    const auto six = translation(0, dist, 0);
//    const auto nine = translation(-dist, 0, 0);
//    c.set(twelve*origo, RED);  
//    c.set(three*origo, RED);  
//    c.set(six*origo, RED);  
//    c.set(nine*origo, RED);             
//    std::ofstream ofs("output/chapter4_2.ppm", std::ofstream::out);
//    ofs << c.to_ppm();  
//}

//TEST(Chapter4, CanDraw12_3_6_9Dynamically) {
//    const auto RED = color(1, 0, 0);    
//    const auto GREEN = color(0, 1, 0);
//    const auto WHITE = color(1, 1, 1);
//    auto c = Canvas(200, 200);
//    const auto origo = point(c.widthf() / 2, c.heightf() / 2, 0);        
//    c.set(origo, GREEN);        
//    const auto dist = c.widthf()*0.375f;        
//    const auto twelve = point(0, -dist, 0);
//    const auto three = point(dist, 0, 0);
//    const auto six = point(0, dist, 0);
//    const auto nine = point(-dist, 0, 0);
//    c.set(twelve+origo, RED);  
//    c.set(three+origo, RED);  
//    c.set(six+origo, RED);  
//    c.set(nine+origo, RED); 
//        
//    const auto angle = (math::TWO_PI / 4.0f);//full circle is 2xPI, divide by the four cardinal directions
//    for (auto i = 0; i < 4; ++i) {
//        const auto rot = rotation_z(static_cast<float>(i) * angle);//full cirle is 2*PI. 12 slices = 2*PI/12 = PI/6.
//        const auto p = rot * twelve;        
//        c.set(p + origo, WHITE);
//    }
//    std::ofstream ofs("output/chapter4_3.ppm", std::ofstream::out);
//    ofs << c.to_ppm();  
//}

//TEST(Chapter4, CanDrawClock) {       
//    const auto WHITE = color(1, 1, 1);
//    auto c = Canvas(200, 200);
//    const auto origo = point(c.widthf() / 2, c.heightf() / 2, 0);                 
//    const auto dist = c.widthf()*0.375f; 
//    const auto trans = translation(origo);
//    const auto twelve = point(0, -dist, 0);
//    const auto angle = (math::TWO_PI / 12.0f);//full circle is 2xPI, clock face has 12 positions. 
//    for (auto i = 0; i < 12; ++i) {
//        const auto rot = rotation_z(static_cast<float>(i) * angle);         
//        const auto p = trans * rot * twelve;
//        c.set(p, WHITE);
//    }
//    std::ofstream ofs("output/chapter4_4.ppm", std::ofstream::out);
//    ofs << c.to_ppm();  
//}

//TEST(Chapter5, CanRenderSilhouetteOfASphere) {       
//    using size_type = Canvas::size_type;
//    auto c = Canvas(100, 100);
//    const auto shape = sphere();
//    const auto RED = color(1, 0, 0);
//    const auto ray_origin = point(0, 0, -5);    
//    const auto wall_z = 10.0f;
//    const auto wall_size = 7.0f;
//    const auto wall_center = wall_size / 2.0f;    
//    const auto pixel_size = wall_size / c.widthf(); //assuming canvas is square
//    for (size_type y = 0; y < c.width(); ++y) {
//        const auto world_y = wall_center - pixel_size * y;
//        for (size_type x = 0; x < c.height(); ++x) {
//            const auto world_x = wall_center - pixel_size * x;
//            const auto target_pos = point(world_x, world_y, wall_z);
//            const auto r = ray(ray_origin, normalize(target_pos - ray_origin));
//            const auto xs = intersect(shape, r);
//            if (hit(xs)) {
//                c.set(x, y, RED);
//            }
//        }
//    }
//    std::ofstream ofs("output/chapter5_0.ppm", std::ofstream::out);
//    ofs << c.to_ppm();  
//}

//TEST(Chapter6, CanRenderPhongShading) {       
//    using size_type = Canvas::size_type;
//    auto c = Canvas(100, 100);
//    auto shape = sphere();
//    shape.surface.color = color(1, 0.2f, 1);        
//    const auto light = point_light(point(10, 10, -10), color(1, 1, 1));
//    const auto ray_origin = point(0, 0, -5);    
//    const auto wall_z = 10.0f;
//    const auto wall_size = 7.0f;
//    const auto wall_center = wall_size / 2.0f;    
//    const auto pixel_size = wall_size / c.widthf(); //assuming canvas is square
//    for (size_type y = 0; y < c.width(); ++y) {
//        const auto world_y = wall_center - pixel_size * y;
//        for (size_type x = 0; x < c.height(); ++x) {
//            const auto world_x = wall_center - pixel_size * x;
//            const auto target_pos = point(world_x, world_y, wall_z);
//            const auto r = ray(ray_origin, normalize(target_pos - ray_origin));
//            const auto xs = intersect(shape, r);            
//            if (const auto closest = hit(xs)) {
//                const auto point = position(r , closest.t); 
//                const auto normal = normal_at(closest.obj, point);
//                const auto eye = -r.direction; 
//                const auto color = lighting(closest.obj.surface, light, point, eye, normal);
//                c.set(x, y, color);
//            }
//        }
//    }
//    std::ofstream ofs("output/chapter6_0.ppm", std::ofstream::out);
//    ofs << c.to_ppm();  
//}