#include <game/enemy.h>
#include <game/shoot.h>
#include <game/movement.h>
#include <game/game.h>
#include <stdio.h>

static const animation_t mr_warrior_idle   = { .tx = 4, .ty = 6, .tw = 2, .th = 2, .framecount = 2, .frametime = 0.50 };
static const animation_t mr_warrior_attack = { .tx = 4, .ty = 4, .tw = 2, .th = 2, .framecount = 2, .frametime = 0.20 };

static shooter_t mr_warrior_shooter = {
  .tx = 3, .ty = 0,
  .tw = 1, .th = 1,
  .ttl = 1.0,
  .target = ENT_PLAYER,
  .damage = 20
};

static void mr_warrior_invoke(game_t *gs, entity_t e, event_t ev);
vector mr_warrior_movement(game_t *gs, entity_t e, float speed, float a2, float a3, float a4);

entity_t enemy_spawn_mr_warrior(game_t *gs, vector spawn_pos)
{
  entity_t e = entity_add(gs, ENT_ENEMY);
  entity_add_component(gs, e, transform);
    transform_t *t = entity_get_component(gs, e, transform);
    t->scale = vec3(2.0, 2.0, 2.0);
    t->position = spawn_pos;
  entity_add_component(gs, e, sprite);
    sprite_t *s = entity_get_component(gs, e, sprite);
    sprite_repeat(s, &mr_warrior_idle);
  entity_add_component(gs, e, actor);
    actor_t *a = entity_get_component(gs, e, actor);
    actor_repeat(a, ACT0, 0.0, 0, 0.5);
    actor_repeat(a, ACT2, 0.0, 0, 5.0);
  entity_add_component(gs, e, rigidbody);
    rigidbody_t *rb = entity_get_component(gs, e, rigidbody);
    rb->radius = 0.8;
  entity_add_component(gs, e, health);
    health_t *h = entity_get_component(gs, e, health);
    h->hp = 1000;
    h->max_hp = 1000;
  entity_add_component(gs, e, botmove);
    botmove_t *bm = entity_get_component(gs, e, botmove);
    bm->speed = 4.0;
    bm->behave = BH_CHASE;
  entity_bind(gs, e, mr_warrior_invoke);
  return e;
}

void mr_warrior_invoke(game_t *gs, entity_t e, event_t ev)
{
  const transform_t *pt = entity_get_component(gs, gs->player, transform);
  
  transform_t *t = entity_get_component(gs, e, transform);
  sprite_t *s = entity_get_component(gs, e, sprite);
  actor_t *a = entity_get_component(gs, e, actor);
  botmove_t *bm = entity_get_component(gs, e, botmove);
  
  float pdist = length(vsubv(pt->position, t->position));
  
  vector forward = fdotv(3.0, normalize(vsubv(pt->position, t->position)));
  
  switch (ev.type) {
  case EV_ACT:
    switch (ev.act.name) {
    case ACT0:
      if (pdist < 3.0) {
        botmove_retreat(bm, 3.0);
      } else if (pdist < 5.0) {
        botmove_stop(bm);
      } else {
        botmove_chase(bm, 6.0);
      }
      break;
    case ACT2:
      actor_repeat(a, ACT3, 0.0, 6, 0.3);
      break;
    case ACT3:
      sprite_play(s, &mr_warrior_attack);
      actor_do(a, ACT4, 0.15);
      break;
    case ACT4:
      shoot_shotgun(gs, &mr_warrior_shooter, t->position, forward, 1.0, flight_linear, 0.0, 0.0, 5, M_PI / 3);
      break;
    case ACT5:
      sprite_play(s, &mr_warrior_attack);
      actor_do(a, ACT6, 0.15);
      break;
    case ACT6:
      shoot_radial(gs, &mr_warrior_shooter, t->position, forward, 1.0, flight_linear, 0.0, 0.0, 10);      
      break;
    }
    break;
  case EV_HIT:
    bullet_t *b = entity_get_component(gs, ev.entcol.e, bullet);
    health_t *h = entity_get_component(gs, e, health);
    h->hp -= b->damage;
    break;
  case EV_NO_HEALTH:
    entity_kill(gs, e);
    break;
  case EV_ENTITY_COLLIDE:
  case EV_MAP_COLLIDE:
    break;
  }
}
