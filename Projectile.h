#pragma once
#include "Canvas.h"
#include "Tuple.h"
static constexpr Vector gravity = vector(0, 0.1f, 0);
static constexpr Vector wind = vector(-0.01f, 0, 0);
static constexpr Color myColor = color(1.0f, 0, 0);
static constexpr float friction = 0.98f;
class Projectile {
  Point position;
  Vector velocity;

public:
  constexpr Projectile(Point pos, Vector vel) noexcept : position(pos), velocity(vel){};

  void update() noexcept {
    position += velocity;
    velocity += wind;
    velocity += gravity;
    velocity *= friction;
  }
  void render(Canvas &col) const noexcept { col.set(position, myColor); }

  auto y() const noexcept { return position.y; };
};
