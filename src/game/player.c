#include <game/player.h>
#include <stdio.h>

void player_move(game_t *gs, entity_t p, const input_t in);

static const animation_t walk_forward = { .tx = 2, .ty = 1, .tw = 1, .th = 1, .framecount = 2, .frametime = 0.1 };
static const animation_t walk_back    = { .tx = 0, .ty = 1, .tw = 1, .th = 1, .framecount = 2, .frametime = 0.1 };
static const animation_t walk_left    = { .tx = 2, .ty = 2, .tw = 1, .th = 1, .framecount = 2, .frametime = 0.1 };
static const animation_t walk_right   = { .tx = 0, .ty = 2, .tw = 1, .th = 1, .framecount = 2, .frametime = 0.1 };

void bullet_invoke(game_t *gs, entity_t e, event_t ev)
{
  switch (ev.type) {
  case EV_ACT:
    switch (ev.act.id) {
    case 0:
      edict_kill(&gs->edict, e);
      break;
    }
    break;
  case EV_HIT_MAP:
    edict_kill(&gs->edict, e);
    break;
  }
}

void shoot(game_t *gs, entity_t p)
{
  transform_t *pt = ENTITY_GET_COMPONENT(gs->edict, p, transform);
  
  entity_t e = edict_add(&gs->edict);
  ENTITY_ADD_COMPONENT(gs->edict, e, transform);
    transform_t *t = ENTITY_GET_COMPONENT(gs->edict, e, transform);
    t->position = pt->position;
  ENTITY_ADD_COMPONENT(gs->edict, e, rigidbody);
    rigidbody_t *rb = ENTITY_GET_COMPONENT(gs->edict, e, rigidbody);
    rb->velocity = mdotv(rotate_z(pt->rotation.z), vec2(0, 4));
  ENTITY_ADD_COMPONENT(gs->edict, e, sprite);
    sprite_t *s = ENTITY_GET_COMPONENT(gs->edict, e, sprite);
    s->tx = 0;
    s->ty = 0;
    s->orient = 0;
    s->stand = 0;
    s->rotation = atan2(rb->velocity.y, rb->velocity.x) - M_PI / 2.0;
  ENTITY_ADD_COMPONENT(gs->edict, e, actor);
    actor_t *a = ENTITY_GET_COMPONENT(gs->edict, e, actor);
    actor_set(a, 0, 1.0, 1);
    actor_start(a, 0);
  ENTITY_ADD_COMPONENT(gs->edict, e, listen);
    listen_t *ls = ENTITY_GET_COMPONENT(gs->edict, e, listen);
    ls->invoke = bullet_invoke;
}

void player_invoke(game_t *gs, entity_t p, event_t ev);

entity_t player_create(game_t *gs)
{
  entity_t p = edict_add(&gs->edict);
  ENTITY_ADD_COMPONENT(gs->edict, p, transform);
    transform_t *t = ENTITY_GET_COMPONENT(gs->edict, p, transform);
    t->position = vec2(1, 1);
  ENTITY_ADD_COMPONENT(gs->edict, p, sprite);
    sprite_t *s = ENTITY_GET_COMPONENT(gs->edict, p, sprite);
    s->tx = 0;
    s->ty = 1;
    s->repeat = &walk_forward;
  ENTITY_ADD_COMPONENT(gs->edict, p, actor);
    actor_t *a = ENTITY_GET_COMPONENT(gs->edict, p, actor);
    actor_set(a, 0, 0.5, 0);
    actor_set(a, 1, 0.125, 3);
  ENTITY_ADD_COMPONENT(gs->edict, p, rigidbody);
    rigidbody_t *rb = ENTITY_GET_COMPONENT(gs->edict, p, rigidbody);
  ENTITY_ADD_COMPONENT(gs->edict, p, listen);
    listen_t *ls = ENTITY_GET_COMPONENT(gs->edict, p, listen);
    ls->invoke = player_invoke;
  return p;
}

void player_invoke(game_t *gs, entity_t p, event_t ev)
{
  actor_t *pa = ENTITY_GET_COMPONENT(gs->edict, p, actor);
  
  switch (ev.type) {
  case EV_ACT:
    switch (ev.act.id) {
    case 0:
      actor_redo(pa, 1);
      break;
    case 1:
      shoot(gs, p);
      break;
    }
    break;
  }
}

void player_update(game_t *gs, entity_t p, const input_t in)
{
  player_move(gs, p, in);
  
  actor_t *a = ENTITY_GET_COMPONENT(gs->edict, p, actor);
  
  if (input_is_mouse_pressed(in, 1)) {
    actor_start(a, 0);
  } else {
    actor_stop(a, 0);
  }
}

void player_move(game_t *gs, entity_t p, const input_t in)
{
  sprite_t *ps = ENTITY_GET_COMPONENT(gs->edict, p, sprite);
  transform_t *pt = ENTITY_GET_COMPONENT(gs->edict, p, transform);
  rigidbody_t *rb = ENTITY_GET_COMPONENT(gs->edict, p, rigidbody);
  
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
    walk = fdotv(speed, normalize(mdotv(rotate_xyz(gs->view_rotation), walk)));
    rb->velocity = walk;
  } else {
    ps->repeat = NULL;
    rb->velocity = vec2(0.0, 0.0);
  }
}
