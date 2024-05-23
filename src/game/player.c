#include <game/player.h>
#include <stdio.h>

void player_move(game_t *gs, const input_t in);
void player_shoot(game_t *gs, const input_t in);
void player_invoke(game_t *gs, entity_t e, event_t ev);
void shoot_player_bullet(game_t *gs, entity_t p);
void bullet_invoke(game_t *gs, entity_t e, event_t ev);

static const animation_t walk_forward = { .tx = 2, .ty = 1, .tw = 1, .th = 1, .framecount = 2, .frametime = 0.15 };
static const animation_t walk_back    = { .tx = 0, .ty = 1, .tw = 1, .th = 1, .framecount = 2, .frametime = 0.15 };
static const animation_t walk_left    = { .tx = 2, .ty = 2, .tw = 1, .th = 1, .framecount = 2, .frametime = 0.15 };
static const animation_t walk_right   = { .tx = 0, .ty = 2, .tw = 1, .th = 1, .framecount = 2, .frametime = 0.15 };

void player_init(game_t *gs)
{
  entity_t e = entity_add(gs);
  entity_add_component(gs, e, transform);
    transform_t *t = entity_get_component(gs, e, transform);
    t->position = vec2(1, 1);
  entity_add_component(gs, e, sprite);
    sprite_t *s = entity_get_component(gs, e, sprite);
    s->repeat = &walk_forward;
  entity_add_component(gs, e, actor);
    actor_t *a = entity_get_component(gs, e, actor);
    actor_set(a, 0, 0.15, 0);
  entity_add_component(gs, e, rigidbody);
    rigidbody_t *rb = entity_get_component(gs, e, rigidbody);
  entity_add_component(gs, e, listen);
    listen_t *ls = entity_get_component(gs, e, listen);
    ls->invoke = player_invoke;
  gs->player = e;
}

void player_invoke(game_t *gs, entity_t e, event_t ev)
{
  actor_t *pa = entity_get_component(gs, e, actor);
  
  switch (ev.type) {
  case EV_ACT0:
    shoot_player_bullet(gs, e);
    break;
  }
}

void player_update(game_t *gs, const input_t in)
{
  player_move(gs, in);
  player_shoot(gs, in);
}

void player_shoot(game_t *gs, const input_t in)
{
  actor_t *a = entity_get_component(gs, gs->player, actor);
  
  if (input_is_mouse_pressed(in, 1)) {
    actor_start(a, 0);
  } else {
    actor_stop(a, 0);
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

void shoot_player_bullet(game_t *gs, entity_t p)
{
  const transform_t *pt = entity_get_component(gs, p, transform);
  
  entity_t e = entity_add(gs);
  entity_add_component(gs, e, transform);
    transform_t *t = entity_get_component(gs, e, transform);
    t->position = pt->position;
  entity_add_component(gs, e, rigidbody);
    rigidbody_t *rb = entity_get_component(gs, e, rigidbody);
    rb->velocity = mdotv(rotate_z(pt->rotation.z), vec2(0, 10));
  entity_add_component(gs, e, sprite);
    sprite_t *s = entity_get_component(gs, e, sprite);
    s->tx = 0;
    s->ty = 0;
    s->orient = 0;
    s->rotation = atan2(rb->velocity.y, rb->velocity.x) - M_PI / 2.0;
  entity_add_component(gs, e, actor);
    actor_t *a = entity_get_component(gs, e, actor);
    actor_set(a, 0, 0.4, 1);
    actor_start(a, 0);
  entity_add_component(gs, e, listen);
    listen_t *ls = entity_get_component(gs, e, listen);
    ls->invoke = bullet_invoke;
}

void bullet_invoke(game_t *gs, entity_t e, event_t ev)
{
  switch (ev.type) {
  case EV_ACT0:
    entity_kill(gs, e);
    break;
  case EV_MAP_COLLIDE:
    entity_kill(gs, e);
    break;
  }
}
