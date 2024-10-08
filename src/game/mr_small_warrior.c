#include <game/enemy.h>
#include <game/shoot.h>
#include <game/game.h>
#include <lib/log.h>

#include <stdio.h>

static const animation_t mr_small_warrior_attack = { .tx = 4, .ty = 10, .tw = 1, .th = 1, .framecount = 2, .frametime = 0.2 };

static shooter_t mr_small_warrior_shooter = {
  .tx = 2, .ty = 4,
  .tw = 1, .th = 1,
  .target = ENT_PLAYER,
  .damage = 8
};

static void mr_small_warrior_invoke(game_t *gs, entity_t e, event_t ev);
vector mr_small_warrior_movement(game_t *gs, entity_t e, float speed, float a2, float a3, float a4);

void enemy_spawn_mr_small_warrior(game_t *gs, vector spawn_pos)
{
  entity_t e = entity_add(gs, ENT_ENEMY);
  entity_add_component(gs, e, transform);
    transform_t *t = entity_get_component(gs, e, transform);
    t->scale = vec3(1.0, 1.0, 1.0);
    t->position = spawn_pos;
  entity_add_component(gs, e, sprite);
    sprite_t *s = entity_get_component(gs, e, sprite);
    sprite_repeat(s, &mr_small_warrior_attack);
  entity_add_component(gs, e, actor);
    actor_t *a = entity_get_component(gs, e, actor);
    actor_repeat(a, ACT0, 0.0, 0, 0.4);
  entity_add_component(gs, e, rigidbody);
    rigidbody_t *rb = entity_get_component(gs, e, rigidbody);
    rb->radius = 0.8;
  entity_add_component(gs, e, health);
    health_t *h = entity_get_component(gs, e, health);
    h->hp = 400;
    h->max_hp = 400;
  entity_add_component(gs, e, npcmove);
    npcmove_t *bm = entity_get_component(gs, e, npcmove);
    bm->speed = 6.0;
    bm->behave = BH_CHASE;
    npcmove_orbit(bm, 4.0);
  entity_bind(gs, e, mr_small_warrior_invoke);
}

void mr_small_warrior_invoke(game_t *gs, entity_t e, event_t ev)
{
  const transform_t *pt = entity_get_component(gs, gs->player, transform);
  
  transform_t *t = entity_get_component(gs, e, transform);
  health_t *h = entity_get_component(gs, e, health);

  vector forward = fdotv(11.0, normalize(vsubv(pt->position, t->position)));

  switch (ev.type) {
  case EV_ACT:
    switch (ev.act.name) {
    case ACT0:
      shoot_linear(gs, &mr_small_warrior_shooter, 4.0, t->position, forward);
      break;
    }
    break;
  case EV_TRANSITION:
    break;
  case EV_HIT:
    h->hp -= ev.hit.damage;
    break;
  case EV_NO_HEALTH:
    entity_kill(gs, e);
    break;
  case EV_ENTITY_COLLIDE:
  case EV_MAP_COLLIDE:
    break;
  }
}
