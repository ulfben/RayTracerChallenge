#pragma once
#include "../pch.h"
#include "../Tuple.h"
#include "../Matrix.h"
#include "../Cone.h"
#include "../Ray.h"
#include "../Intersection.h"
DISABLE_WARNINGS_FROM_GTEST

template <typename T>
testing::AssertionResult IsAlmostEqual(std::string_view expr1,
                                       std::string_view expr2,
                                       T val1,
                                       T val2) {
    if (math::float_cmp(val1, val2)) {
        return testing::AssertionSuccess();
    } 
    return testing::AssertionFailure()
        << "Expected: (" << expr1 << " ~= " << expr2
        /*<< ") up to a tolerance of " << tolerance*/
        << ")\nActual: ( " << val1 << " vs " << val2 << ")";
}

template <typename T>
testing::AssertionResult SafeRangeTest(std::string_view expr1,
                                       std::string_view expr2,
                                       std::span<T> actualVales,
                                       std::span<T> expectedValues
                                       ) {
    size_t count = (actualValues.size() > expectedValues.size()) ? expectedValues.size() : actualVales.size();
    for (auto i = 0; i < count; ++i) {
        EXPECT_PRED_FORMAT2(actualValues[i], expectedValues[i]);
    }
    return EXPECT_PRED_FORMAT2(actualValues.size(), expectedValues.size());
}


TEST(Cone, intersectingAConeWithARay) {           
    const auto c = cone();
    EXPECT_FALSE(is_bounded(c));    

    auto r = ray(point(0, 0, -5), normal_vector(0, 0, 1));
    auto xs = local_intersect(c, r);   
    ASSERT_EQ(xs.size(), 2);    
    EXPECT_PRED_FORMAT2(IsAlmostEqual, xs[0], 5.0f);
    EXPECT_PRED_FORMAT2(IsAlmostEqual, xs[1], 5.0f);
    
    r = ray(point(0, 0, -5.0f), normal_vector(1.0f, 1.0f, 1.0f));
    xs = local_intersect(c, r);   
    ASSERT_EQ(xs.size(), 2);    
    EXPECT_PRED_FORMAT2(IsAlmostEqual, xs[0], 8.66025f);
    EXPECT_PRED_FORMAT2(IsAlmostEqual, xs[1], 8.66025f);
    
    r = ray(point(1, 1, -5), normal_vector(-0.5f, -1, 1));
    xs = local_intersect(c, r);   
    ASSERT_EQ(xs.size(), 2);
    EXPECT_PRED_FORMAT2(IsAlmostEqual, xs[0], 4.5500546f); //book oracle 4.55006f, which is actually 4.5500598      
    EXPECT_PRED_FORMAT2(IsAlmostEqual, xs[1], 49.44994f);    
}

TEST(Cone, intersectingAConeWithRayParallelToOneOfItsHalves) {           
    const auto c = cone();
    EXPECT_FALSE(is_bounded(c));
    const auto r = ray(point(0,0,-1), normal_vector(0,1,1));    
    const auto xs = local_intersect(c, r);        
    ASSERT_EQ(xs.size(), 1);
    EXPECT_FLOAT_EQ(xs[0], 0.35355338f);  //book oracle 0.35355f, which is actually 0.35354999f
}

TEST(Cone, intersectingAConesEndCap) {           
    auto c = closed_cone(-0.5f, 0.5f);
    EXPECT_TRUE(is_closed(c));  
    const std::vector<Ray> rays{
        {point(0, 0, -5), normal_vector(0, 1, 0)},
        {point(0, 0, -0.25f), normal_vector(0, 1, 1)},
        {point(0, 0, -0.25f), normal_vector(0, 1, 0)}        
    };    
    
    auto xs = local_intersect(c, rays[0]);        
    EXPECT_EQ(xs.size(), 0); 

    xs = local_intersect(c, rays[1]);        
    EXPECT_EQ(xs.size(), 2); 

    xs = local_intersect(c, rays[2]);        
    EXPECT_EQ(xs.size(), 4); 
}

TEST(Cone, normalOnSurfaceOfCone) {   
    const std::vector<Point> points{
        point(0.0f, 0.0f, 0.0f),
        point(1.0f, 1.0f, 1.0f),
        point(-1.0f, -1.0f, 0)
    };
    const std::vector<Vector> normals{
        vector(0, 0, 0),
        normal_vector(1, -math::sqrt(2.0f), 1),
        normal_vector(-1, 1, 0),        
    };
    const auto c = cone();
    for (size_t i = 0; i < points.size(); i++) {        
        const auto& point = points[i];
        const auto normal = local_normal_at(c, point);
        EXPECT_EQ(normal, normals[i]);        
    }
}

RESTORE_WARNINGS
