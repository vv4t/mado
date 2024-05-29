#ifndef BOTMOVE_H
#define BOTMOVE_H

#include <game/game.h>
#include <lib/math3d.h>

typedef vector (*movement_t)(struct game_s *gs, int entity, float a1, float a2, vector v1, vector v2);

typedef struct {
  movement_t movement;
  float      a1;
  float      a2;
  vector     v1;
  vector     v2;
} botmove_t;

inline static botmove_t create_botmove()
{
  return (botmove_t) {
    .movement = NULL,
    .a1 = 0.0,
    .a2 = 0.0,
    .v1 = vec2(0.0, 0.0),
    .v2 = vec2(0.0, 0.0)
  };
}

#endif
