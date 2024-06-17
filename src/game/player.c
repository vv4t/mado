#include <game/player.h>
#include <game/shoot.h>
#include <stdio.h>

void player_move(game_t *gs, const usercmd_t *usercmd);
void player_attack(game_t *gs, const usercmd_t *usercmd);
void player_invoke(game_t *gs, entity_t e, event_t ev);

static const animation_t walk_forward = { .tx = 2, .ty = 1, .tw = 1, .th = 1, .framecount = 2, .frametime = 0.15 };
static const animation_t walk_back    = { .tx = 0, .ty = 1, .tw = 1, .th = 1, .framecount = 2, .frametime = 0.15 };
static const animation_t walk_left    = { .tx = 2, .ty = 2, .tw = 1, .th = 1, .framecount = 2, .frametime = 0.15 };
static const animation_t walk_right   = { .tx = 0, .ty = 2, .tw = 1, .th = 1, .framecount = 2, .frametime = 0.15 };

static shooter_t player_shooter = {
  .tx = 0, .ty = 0,
  .tw = 1, .th = 1,
  .target = ENT_ENEMY,
  .damage = 20
};

void player_init(game_t *gs)
{
  entity_t e = entity_add(gs, ENT_PLAYER);
  entity_add_component(gs, e, transform);
    transform_t *t = entity_get_component(gs, e, transform);
    t->position = vec2(24, 24);
  entity_add_component(gs, e, sprite);
    sprite_t *s = entity_get_component(gs, e, sprite);
    sprite_repeat(s, &walk_forward);
  entity_add_component(gs, e, actor);
    actor_t *a = entity_get_component(gs, e, actor);
    actor_repeat(a, ACT0, 0.0, 0, 0.15);
    actor_stop(a, ACT0);
  entity_add_component(gs, e, rigidbody);
    rigidbody_t *rb = entity_get_component(gs, e, rigidbody);
    rb->radius = 0.1;
  entity_bind(gs, e, player_invoke);
  gs->player = e;
}

void player_invoke(game_t *gs, entity_t e, event_t ev)
{
  transform_t *pt = entity_get_component(gs, e, transform);
  
  vector forward = mdotv(rotate_z(pt->rotation.z), vec2(0, 10));

  switch (ev.type) {
  case EV_ACT:
    switch (ev.act.name) {
    case ACT0:
      shoot_wave(gs, &player_shooter, 0.6, pt->position, forward, 1.0, 22.0, 0.0);
      shoot_wave(gs, &player_shooter, 0.6, pt->position, forward, 1.0, 22.0, M_PI);
      break;
    }
    break;
  case EV_MAP_COLLIDE:
  case EV_ENTITY_COLLIDE:
  case EV_HIT:
  case EV_NO_HEALTH:
    break;
  }
}

void player_update(game_t *gs, const usercmd_t *usercmd)
{
  player_move(gs, usercmd);
  player_attack(gs, usercmd);
}

void player_attack(game_t *gs, const usercmd_t *usercmd)
{
  actor_t *a = entity_get_component(gs, gs->player, actor);
  
  if (usercmd->attack) {
    actor_start(a, ACT0);
  } else {
    actor_stop(a, ACT0);
  }
}

void player_move(game_t *gs, const usercmd_t *usercmd)
{
  sprite_t *ps = entity_get_component(gs, gs->player, sprite);
  transform_t *pt = entity_get_component(gs, gs->player, transform);
  rigidbody_t *rb = entity_get_component(gs, gs->player, rigidbody);
  
  float speed = 9.0;
  vector walk = vec2(0, 0);
  
  if (usercmd->forward) ps->repeat = &walk_forward;
  if (usercmd->back) ps->repeat = &walk_back;
  if (usercmd->left) ps->repeat = &walk_left;
  if (usercmd->right) ps->repeat = &walk_right;
  pt->rotation.z = gs->view_rotation.z + atan2(-usercmd->aim_x, -usercmd->aim_y);
  
  walk.y += usercmd->forward;
  walk.y -= usercmd->back;
  walk.x -= usercmd->left;
  walk.x += usercmd->right;
  
  if (dot(walk, walk) > 0.0) {
    walk = fdotv(speed, normalize(mdotv(rotate_z(gs->view_rotation.z), walk)));
    rb->velocity = walk;
  } else {
    ps->repeat = NULL;
    rb->velocity = vec2(0.0, 0.0);
  }
}
