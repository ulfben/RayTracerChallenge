#pragma once
#include "../pch.h"
#include "../Material.h"


DISABLE_WARNINGS_FROM_GTEST

TEST(Material, canBeDefaultConstructed) {    
    constexpr auto m = material();
    EXPECT_EQ(m.color, WHITE);
    EXPECT_EQ(m.ambient, 0.1f);
    EXPECT_EQ(m.diffuse, 0.9f);
    EXPECT_EQ(m.specular, 0.9f);
    EXPECT_EQ(m.shininess, 200.0f);
}

TEST(Material, hasReflectivity) {    
    constexpr auto m = material();
    EXPECT_EQ(m.reflective, 0.0f);    
}

TEST(Material, hasTransparencyAndRefractiveIndex) {    
    constexpr auto m = material();
    EXPECT_EQ(m.transparency, 0.0f);
    EXPECT_EQ(m.refractive_index, 1.0f);
}


RESTORE_WARNINGS