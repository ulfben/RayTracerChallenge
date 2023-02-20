#pragma once
#include "../pch.h"
#include "../Tuple.h"
#include "../Lights.h"

class Phong : public ::testing::Test {
 protected:
     Phong() noexcept = default;
     Material surface = material();
     Point position = point(0,0,0); //world space point that the light is bouncing off
     Vector normal = vector(0, 0, -1);
};

DISABLE_WARNINGS_FROM_GTEST

TEST_F(Phong, lightingWithEyeBetweenLightAndSurface) {
    const Vector eye = vector(0, 0, -1);    
    const auto light = point_light(point(0, 0, -10), WHITE);
    const auto result = lighting(surface, light, position, eye, normal);
    EXPECT_EQ(result, color(1.9f, 1.9f, 1.9f));
}

TEST_F(Phong, lightingWithEyeOffset45Deg) {
    const auto sq = std::sqrtf(2) / 2;
    const Vector eye = vector(0, sq, sq);    
    const auto light = point_light(point(0, 0, -10), WHITE);
    const auto result = lighting(surface, light, position, eye, normal);
    EXPECT_EQ(result, color(1.0f, 1.0f, 1.0f));
}

TEST_F(Phong, lightingWithEyeOppositeSurfaceLightOffset45Deg) {    
    const Vector eye = vector(0, 0, -1);    
    const auto light = point_light(point(0, 10, -10), WHITE);
    const auto result = lighting(surface, light, position, eye, normal);
    EXPECT_EQ(result, color(0.7364f, 0.7364f, 0.7364f));
}

TEST_F(Phong, lightingWithEyeInThePathOfReflectionVector) {
    const auto sq = std::sqrtf(2) / 2;
    const Vector eye = vector(0, -sq, -sq);    
    const auto light = point_light(point(0, 10, -10), WHITE);
    const auto result = lighting(surface, light, position, eye, normal);
    EXPECT_EQ(result, color(1.6364f, 1.6364f, 1.6364f));
}

TEST_F(Phong, lightingWithEyeBehindTheSurface) {  
    const Vector eye = vector(0, 0, -1);    
    const auto light = point_light(point(0, 0, 10), WHITE);
    const auto result = lighting(surface, light, position, eye, normal);
    EXPECT_EQ(result, color(0.1f, 0.1f, 0.1f)); //only the ambient component remains
}

TEST_F(Phong, lightingWithSurfaceInShadow) {
    const Vector eyev = vector(0, 0, -1);
    const Vector normalv = vector(0, 0, -1);
    const auto light = point_light(point(0, 0, -10), WHITE);
    const auto result = lighting_shadow(surface, light);
    EXPECT_EQ(result, color(0.1f, 0.1f, 0.1f)); //only the ambient component remains
}

TEST_F(Phong, noShadowWhenNothingIsCollinearWithPointAndLight) {
    const auto w = World(); //default world puts light at point(-10, 10, -10) and unit spheres at origo.
    const auto p = point(0, 10, 0);  //our point is well above the sphere.      
    EXPECT_FALSE(is_shadowed(w, p));
}

TEST_F(Phong, inShadowWhenObjectIsBetweenPointAndLight) {
    const auto w = World(); 
    const auto p = point(10, -10, 10);  //our point is opposite the light, with spheres in between 
    EXPECT_TRUE(is_shadowed(w, p));
}

TEST_F(Phong, noShadowWhenObjectIsBehindTheLight) {
    const auto w = World(); 
    const auto p = point(-20, 20, -20);  //our point far behind the light
    EXPECT_FALSE(is_shadowed(w, p));
}

TEST_F(Phong, noShadowWhenObjectIsBehindThePoint) {
    const auto w = World(); 
    const auto p = point(-2, 2, -2);  //our point far behind the light
    EXPECT_FALSE(is_shadowed(w, p));
}
RESTORE_WARNINGS