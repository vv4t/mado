#ifndef SHOOT_H
#define SHOOT_H

#include <game/game.h>
#include <lib/math3d.h>

typedef struct {
  // name_t target;
  // int damage;
  int tx, ty, tw, th;
  float ttl;
} shooter_t;

entity_t shoot(game_t *gs, const shooter_t *sh, vector o, vector fwd, target_t target, float side, flight_t fl, float a1, float a2);
entity_t shoot_linear(game_t *gs, const shooter_t *sh, vector o, vector fwd, target_t target);
entity_t shoot_wave(game_t *gs, const shooter_t *sh, vector o, vector fwd, target_t target, float amp, float freq, float phase);
entity_t shoot_shotgun(game_t *gs, const shooter_t *sh, vector o, vector fwd, target_t target, int count, float cone_angle);
entity_t shoot_radial(game_t *gs, const shooter_t *sh, vector o, vector fwd, target_t target, int count);
entity_t shoot_wall(game_t *gs, const shooter_t *sh, vector o, vector fwd, target_t, int wall_bits, int length, float sep);

#endif
