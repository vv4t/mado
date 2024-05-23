#include <game/enemy.h>
#include <game/shoot.h>
#include <game/game.h>
#include <stdio.h>

static const animation_t archmage_idle   = { .tx = 0, .ty = 3, .tw = 1, .th = 1, .framecount = 2, .frametime = 0.20 };
static const animation_t archmage_attack = { .tx = 2, .ty = 3, .tw = 1, .th = 1, .framecount = 2, .frametime = 0.15 };

static shooter_t archmage_shooter = {
  .tx = 2, .ty = 0,
  .tw = 1, .th = 1,
  .ttl = 8.0
};

static void archmage_invoke(game_t *gs, entity_t e, event_t ev);

entity_t enemy_spawn_archmage(game_t *gs)
{
  entity_t e = entity_add(gs);
  entity_add_component(gs, e, transform);
    transform_t *t = entity_get_component(gs, e, transform);
    t->scale = vec3(2.0, 2.0, 2.0);
    t->position = vec2(3, 3);
  entity_add_component(gs, e, sprite);
    sprite_t *s = entity_get_component(gs, e, sprite);
    sprite_repeat(s, &archmage_idle);
  entity_add_component(gs, e, actor);
    actor_t *a = entity_get_component(gs, e, actor);
    actor_set(a, 0, 5.0, 0);
    actor_set(a, 1, 0.30, 1);
    actor_set(a, 2, 0.15, 5);
    actor_start(a, 0);
    a->action[0].time = 2.0;
  entity_bind(gs, e, archmage_invoke);
  return e;
}

vector flight_archmage_bullet(float time, float a1, float a2) {
  if (time < 0.5) {
    return vec2(0, -1);
  }
  
  time -= 0.5;
  
  float r = 2.0;
  return vec2(cos(time * r), sin(time * r));
}

void archmage_invoke(game_t *gs, entity_t e, event_t ev)
{
  const transform_t *pt = entity_get_component(gs, gs->player, transform);
  
  transform_t *t = entity_get_component(gs, e, transform);
  sprite_t *s = entity_get_component(gs, e, sprite);
  actor_t *a = entity_get_component(gs, e, actor);
  
  switch (ev.type) {
  case EV_ACT0:
    actor_redo(a, 1);
    break;
  case EV_ACT1:
    sprite_play(s, &archmage_attack);
    actor_redo(a, 2);
    break;
  case EV_ACT2:
    shoot(gs, &archmage_shooter, t->position, vec2(4, 0), 1.0, flight_archmage_bullet, 22.0, 0.0);
    break;
  }
}
