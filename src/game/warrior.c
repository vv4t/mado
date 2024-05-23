#include <game/enemy.h>
#include <game/shoot.h>
#include <game/game.h>
#include <stdio.h>

static const animation_t warrior_idle   = { .tx = 4, .ty = 6, .tw = 2, .th = 2, .framecount = 2, .frametime = 0.20 };
static const animation_t warrior_attack = { .tx = 4, .ty = 4, .tw = 2, .th = 2, .framecount = 2, .frametime = 0.15 };

static shooter_t warrior_shooter = {
  .tx = 3, .ty = 0,
  .tw = 1, .th = 1,
  .ttl = 0.5
};

static void warrior_invoke(game_t *gs, entity_t e, event_t ev);

entity_t enemy_spawn_warrior(game_t *gs)
{
  entity_t e = entity_add(gs);
  entity_add_component(gs, e, transform);
    transform_t *t = entity_get_component(gs, e, transform);
    t->scale = vec3(2.0, 2.0, 2.0);
    t->position = vec2(3, 3);
  entity_add_component(gs, e, sprite);
    sprite_t *s = entity_get_component(gs, e, sprite);
    sprite_repeat(s, &warrior_idle);
  entity_add_component(gs, e, actor);
    actor_t *a = entity_get_component(gs, e, actor);
    actor_set(a, 0, 4.0, 0); // do attack every 4s
    actor_set(a, 1, 0.30, 6); // attack 4 times (start animation first)
    actor_set(a, 2, 0.15, 1); // actually shoot the projectile at 2nd frame
    actor_start(a, 0);
  entity_bind(gs, e, warrior_invoke);
  return e;
}

void warrior_invoke(game_t *gs, entity_t e, event_t ev)
{
  const transform_t *pt = entity_get_component(gs, gs->player, transform);
  
  transform_t *t = entity_get_component(gs, e, transform);
  sprite_t *s = entity_get_component(gs, e, sprite);
  actor_t *a = entity_get_component(gs, e, actor);
  
  vector forward = fdotv(5.0, normalize(vsubv(pt->position, t->position)));
  
  switch (ev.type) {
  case EV_ACT0:
    actor_redo(a, 1);
    break;
  case EV_ACT1:
    sprite_play(s, &warrior_attack);
    actor_redo(a, 2);
    break;
  case EV_ACT2:
    shoot_linear(gs, &warrior_shooter, t->position, forward);
    break;
  }
}
