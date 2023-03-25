#pragma once
#include "../pch.h"
#include "../Tuple.h"
#include "../Matrix.h"
#include "../Cylinder.h"
#include "../Ray.h"
#include "../Intersection.h"
DISABLE_WARNINGS_FROM_GTEST

TEST(Cylinder, rayIntersectsACylinder) {    
    const auto c = cylinder();    
    auto r = ray(point(1, 0, -5), vector(0, 0, 1)); //hit cylinder at a tangent 
    auto xs = local_intersect(c,  r);
    EXPECT_FLOAT_EQ(xs[0], 5.0f);
    EXPECT_FLOAT_EQ(xs[1], 5.0f);

    r = ray(point(0, 0, -5), vector(0, 0, 1)); //hit cylinder perpendicular, through the middle.
    xs = local_intersect(c,  r);
    EXPECT_FLOAT_EQ(xs[0], 4.0f);
    EXPECT_FLOAT_EQ(xs[1], 6.0f);
 
    r = ray(point(0.5f, 0, -5), normal_vector(0.1f, 1, 1)); //hit the cylinder at an angle
    xs = local_intersect(c,  r); 

    //using the book comparison, epsilon and expected:
    EXPECT_TRUE(math::almost_equal(xs[0], 6.80798f, math::BOOK_EPSILON));
    EXPECT_TRUE(math::almost_equal(xs[1], 7.08872f, math::BOOK_EPSILON));

    //using the universal float comparison:
    EXPECT_TRUE(math::float_cmp(xs[0], 6.80798f));
    EXPECT_TRUE(math::float_cmp(xs[1], 7.08872f));

    ////succeeds only in debug mode:
    //EXPECT_FLOAT_EQ(xs[0], 6.8080058f);
    //EXPECT_FLOAT_EQ(xs[1], 7.0886984f);

    ////succeeds only in release mode:
    //EXPECT_FLOAT_EQ(xs[0], 6.8079991);
    //EXPECT_FLOAT_EQ(xs[1], 7.0887051);
}

TEST(Cylinder, rayMissesACylinder) {
    const std::vector<Ray> rays{
        {point(1, 0, 0), normal_vector(0, 1, 0)}, //on the surface, pointing up. Ergo: perpendicular to the cylinder
        {point(0, 0, 0), normal_vector(0, 1, 0)}, //inside the the cylinder, pointing up
        {point(0, 0, -5), normal_vector(1, 1, 1)} //far away from the cylinder, pointer wherever
    };    
    const auto c = cylinder();
    for (size_t i = 0; i < rays.size(); i++) {        
        const auto xs = local_intersect(c, rays[i]);
        EXPECT_EQ(xs, MISS);       
    }
}


TEST(Cylinder, normalOnTheSurfaceOfACylinder) {   
    const std::vector<Point> points{
        point(1, 0, 0),
        point(0, 5, -1),
        point(0, -2, 1),
        point(-1, 1, 0)        
    };
    const std::vector<Vector> normals{
        vector(1, 0, 0),
        vector(0, 0, -1),
        vector(0, 0, 1),
        vector(-1, 0, 0)        
    };
    const auto c = cylinder();
    for (size_t i = 0; i < points.size(); i++) {
        const auto p = points[i];
        const auto normal = local_normal_at(c, p);
        EXPECT_EQ(normal, normals[i]);        
    }
}

TEST(Cylinder, defaultCylinderIsNearInfinite) {       
    const auto c = cylinder();
    EXPECT_FLOAT_EQ(c.minimum, math::MIN);
    EXPECT_FLOAT_EQ(c.maximum, math::MAX); 
    EXPECT_FALSE(is_bounded(c));
}

TEST(Cylinder, intersectingATruncatedCylinder) {           
    const auto c = cylinder(1.0f, 2.0f);
    EXPECT_TRUE(is_bounded(c));
    const std::vector<Ray> rays{
        {point(0, 1.5f, 0), normal_vector(0.1f, 1, 0)}, //from inside, heading up and escaping the cylinder
        {point(0, 3, -5), normal_vector(0, 0, 1)}, // perpendicular to the y-axis, but above the cylinder
        {point(0, 0, -5), normal_vector(0, 0, 1)}, // perpendicular to the y-axis, but below the cylinder
        {point(0, 2, -5), normal_vector(0, 0, 1)}, //edge case: the maximum extent is *excluded*
        {point(0, 1, -5), normal_vector(0, 0, 1)}, //edge case: the minimum extent is *excluded*
        {point(0, 1.5f, -2), normal_vector(0, 0, 1)} //perpendicular to the cylinder, at the middle of it. 
    }; 
    const std::vector<std::vector<Real>> points{
        MISS,
        MISS,
        MISS,
        MISS,
        MISS,
        {1.0f,3.0f} 
    };
    
    for (size_t i = 0; i < rays.size(); i++) {        
        const auto xs = local_intersect(c, rays[i]);        
        EXPECT_EQ(xs, points[i]);        
    }         
}

TEST(Cylinder, defaultCylinderIsOpen) {       
    const auto c = cylinder();
    EXPECT_FALSE(c.closed);
    EXPECT_FALSE(is_closed(c));    
    EXPECT_TRUE(is_open(c));    
}

TEST(Cylinder, intersectingTheCapsOfAClosedCylinder) {           
    auto c = closed_cylinder(1.0f, 2.f);
    EXPECT_TRUE(is_closed(c));  
    const std::vector<Ray> rays{
        {point(0, 3, 0), normal_vector(0, -1.0f, 0)},
        {point(0, 3, -2), normal_vector(0, -1, 2)}, 
        {point(0, 4, -2), normal_vector(0, -1, 1)}, // corner case
        {point(0, 0, -2), normal_vector(0, 1, 2)},        
        {point(0, -1, -2), normal_vector(0, 1, 1)} //corner case
    };     
    
    for (size_t i = 0; i < rays.size(); i++) {        
        const auto xs = local_intersect(c, rays[i]);        
        EXPECT_EQ(xs.size(), 2);        
    }         
}

RESTORE_WARNINGS