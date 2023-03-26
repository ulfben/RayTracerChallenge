#pragma once
#include "../pch.h"
#include "../Tuple.h"
#include "../Matrix.h"
#include "../Cone.h"
#include "../Ray.h"
#include "../Intersection.h"
DISABLE_WARNINGS_FROM_GTEST


TEST(Cone, intersectingAConeWithARay) {           
    const auto c = cone();
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

TEST(Cone, intersectingTheCapsOfAClosedCylinder) {           
    auto c = closed_cylinder(1.0f, 2.f);
    EXPECT_TRUE(is_closed(c));  
    const std::vector<Ray> rays{
        {point(0, 3, 0), vector(0, -1, 0)}, //start above the cylinder pointing straight down through it. hits both caps
        {point(0, 3, -2), vector(0, -1, 2)}, //diagonal through top cap and cylinder wall 
        {point(0, 4, -2), vector(0, -1, 1)}, // corner case, diagonal through top cap + where second cap meets cylinder wall
        {point(0, 0, -2), vector(0, 1, 2)},  //diagonal through bottom cap and cylinder wall       
        {point(0, -1, -2), vector(0, 1, 1)} //corner case, diagonal through bottom cap + where second cap meets cylinder wall
    };     
    
    for (size_t i = 0; i < rays.size(); i++) {        
        const auto xs = local_intersect(c, rays[i]);        
        EXPECT_EQ(xs.size(), 2);        
    }         
}

TEST(Cone, normalOnCylinderEndCap) {   
    const std::vector<Point> points{
        point(0.0f, 1.0f, 0.0f),
        point(0.5f, 1.0f, 0.0f),
        point(0.0f, 1.0f, 0.5f),
        point(0.0f, 2.0f, 0.0f),        
        point(0.5f, 2.0f, 0.0f),
        point(0.0f, 2.0f, 0.5f)
    };
    const std::vector<Vector> normals{
        vector(0, -1, 0),
        vector(0, -1, 0),
        vector(0, -1, 0),
        vector(0, 1, 0),        
        vector(0, 1, 0),        
        vector(0, 1, 0)        
    };
    const auto c = closed_cylinder(1, 2);
    for (size_t i = 0; i < points.size(); i++) {        
        const auto& point = points[i];
        const auto normal = local_normal_at(c, point);
        EXPECT_EQ(normal, normals[i]);        
    }
}

RESTORE_WARNINGS