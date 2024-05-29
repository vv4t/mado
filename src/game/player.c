#include <game/player.h>
#include <game/shoot.h>
#include <stdio.h>

void player_move(game_t *gs, const input_t in);
void player_attack(game_t *gs, const input_t in);
void player_invoke(game_t *gs, entity_t e, event_t ev);

static const animation_t walk_forward = { .tx = 2, .ty = 1, .tw = 1, .th = 1, .framecount = 2, .frametime = 0.15 };
static const animation_t walk_back    = { .tx = 0, .ty = 1, .tw = 1, .th = 1, .framecount = 2, .frametime = 0.15 };
static const animation_t walk_left    = { .tx = 2, .ty = 2, .tw = 1, .th = 1, .framecount = 2, .frametime = 0.15 };
static const animation_t walk_right   = { .tx = 0, .ty = 2, .tw = 1, .th = 1, .framecount = 2, .frametime = 0.15 };

static shooter_t player_shooter = {
  .tx = 0, .ty = 0,
  .tw = 1, .th = 1,
  .ttl = 0.6,
  .target = ENT_ENEMY,
  .damage = 5
};

void player_init(game_t *gs)
{
  entity_t e = entity_add(gs, ENT_PLAYER);
  entity_add_component(gs, e, transform);
    transform_t *t = entity_get_component(gs, e, transform);
    t->position = vec2(1, 1);
  entity_add_component(gs, e, sprite);
    sprite_t *s = entity_get_component(gs, e, sprite);
    sprite_repeat(s, &walk_forward);
  entity_add_component(gs, e, actor);
    actor_t *a = entity_get_component(gs, e, actor);
    actor_repeat(a, ACT0, 0.0, 0, 0.15);
    actor_stop(a, ACT0);
  entity_add_component(gs, e, rigidbody);
    rigidbody_t *rb = entity_get_component(gs, e, rigidbody);
    rb->radius = 0.5;
  entity_bind(gs, e, player_invoke);
  gs->player = e;
}

void player_invoke(game_t *gs, entity_t e, event_t ev)
{
  actor_t *pa = entity_get_component(gs, e, actor);
  transform_t *pt = entity_get_component(gs, e, transform);
  
  vector forward = mdotv(rotate_z(pt->rotation.z), vec2(0, 10));
  
  switch (ev.type) {
  case EV_ACT:
    switch (ev.act.name) {
    case ACT0:
      shoot_wave(gs, &player_shooter, pt->position, forward, 1.0, 22.0, 0.0);
      shoot_wave(gs, &player_shooter, pt->position, forward, 1.0, 22.0, M_PI);
      break;
    }
    break;
  }
}

void player_update(game_t *gs, const input_t in)
{
  player_move(gs, in);
  player_attack(gs, in);
}

void player_attack(game_t *gs, const input_t in)
{
  actor_t *a = entity_get_component(gs, gs->player, actor);
  
  if (input_is_mouse_pressed(in, 1)) {
    actor_start(a, ACT0);
  } else {
    actor_stop(a, ACT0);
  }
}

void player_move(game_t *gs, const input_t in)
{
  sprite_t *ps = entity_get_component(gs, gs->player, sprite);
  transform_t *pt = entity_get_component(gs, gs->player, transform);
  rigidbody_t *rb = entity_get_component(gs, gs->player, rigidbody);
  
  float speed = 5.0;
  vector walk = vec2(0, 0);
  
  if (input_is_key_pressed(in, 'w')) ps->repeat = &walk_forward;
  if (input_is_key_pressed(in, 's')) ps->repeat = &walk_back;
  if (input_is_key_pressed(in, 'a')) ps->repeat = &walk_left;
  if (input_is_key_pressed(in, 'd')) ps->repeat = &walk_right;
  pt->rotation.z = gs->view_rotation.z + atan2(-input_get_mouse_x(in), input_get_mouse_y(in));
  
  walk.y += input_is_key_pressed(in, 'w');
  walk.y -= input_is_key_pressed(in, 's');
  walk.x -= input_is_key_pressed(in, 'a');
  walk.x += input_is_key_pressed(in, 'd');
  
  if (dot(walk, walk) > 0.0) {
    walk = fdotv(speed, normalize(mdotv(rotate_z(gs->view_rotation.z), walk)));
    rb->velocity = walk;
  } else {
    ps->repeat = NULL;
    rb->velocity = vec2(0.0, 0.0);
  }
}
