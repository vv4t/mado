#ifndef SHOOT_H
#define SHOOT_H

#include <game/game.h>
#include <lib/math3d.h>

typedef struct {
  entname_t target;
  int damage;
  int tx, ty, tw, th;
  float ttl;
} shooter_t;

void bullet_invoke(game_t *gs, entity_t e, event_t ev);

entity_t shoot(game_t *gs, const shooter_t *sh, vector o, vector fwd, float side, flight_t fl, float a1, float a2);
entity_t shoot_linear(game_t *gs, const shooter_t *sh, vector o, vector fwd);
entity_t shoot_wave(game_t *gs, const shooter_t *sh, vector o, vector fwd, float amp, float freq, float phase);
void shoot_shotgun(game_t *gs, const shooter_t *sh, vector o, vector fwd, int count, float cone_angle);
void shoot_radial(game_t *gs, const shooter_t *sh, vector o, vector fwd, int count);
void shoot_wall(game_t *gs, const shooter_t *sh, vector o, vector fwd, int wall_bits, int length, float sep);

#endif
