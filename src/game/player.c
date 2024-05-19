#include <game/player.h>
#include <stdio.h>

void player_move(game_t *gs, entity_t p, const input_t in);

static const animation_t walk_forward = { .tx = 2, .ty = 1, .tw = 1, .th = 1, .framecount = 2, .frametime = 0.1 };
static const animation_t walk_back    = { .tx = 0, .ty = 1, .tw = 1, .th = 1, .framecount = 2, .frametime = 0.1 };
static const animation_t walk_left    = { .tx = 2, .ty = 2, .tw = 1, .th = 1, .framecount = 2, .frametime = 0.1 };
static const animation_t walk_right   = { .tx = 0, .ty = 2, .tw = 1, .th = 1, .framecount = 2, .frametime = 0.1 };

void die(game_t *gs, entity_t e)
{
  edict_kill(&gs->edict, e);
}

void thing(game_t *gs, entity_t p)
{
  const transform_t *pt = ENTITY_GET_COMPONENT(gs->edict, p, transform);
  
  entity_t e = edict_add(&gs->edict);
  ENTITY_ADD_COMPONENT(gs->edict, e, transform);
    transform_t *t = ENTITY_GET_COMPONENT(gs->edict, e, transform);
    t->position = pt->position;
  ENTITY_ADD_COMPONENT(gs->edict, e, rigidbody);
    rigidbody_t *rb = ENTITY_GET_COMPONENT(gs->edict, e, rigidbody);
    rb->velocity = mdotv(rotate_z(pt->rotation.z), vec2(0, 10));
  ENTITY_ADD_COMPONENT(gs->edict, e, sprite);
    sprite_t *s = ENTITY_GET_COMPONENT(gs->edict, e, sprite);
    s->tx = 0;
    s->ty = 0;
    s->orient = 0;
    s->stand = 0;
    s->rotation = atan2(rb->velocity.y, rb->velocity.x) - M_PI / 2.0;
  ENTITY_ADD_COMPONENT(gs->edict, e, actor);
    actor_t *a = ENTITY_GET_COMPONENT(gs->edict, e, actor);
    actor_do(a, die, 1.0, 1);
}

entity_t player_create(game_t *gs)
{
  entity_t p = edict_add(&gs->edict);
  ENTITY_ADD_COMPONENT(gs->edict, p, transform);
    transform_t *t = ENTITY_GET_COMPONENT(gs->edict, p, transform);
    t->position = vec2(3, 3);
  ENTITY_ADD_COMPONENT(gs->edict, p, sprite);
    sprite_t *s = ENTITY_GET_COMPONENT(gs->edict, p, sprite);
    s->tx = 0;
    s->ty = 1;
    s->repeat = &walk_forward;
  ENTITY_ADD_COMPONENT(gs->edict, p, actor);
    actor_t *a = ENTITY_GET_COMPONENT(gs->edict, p, actor);
    actor_do(a, thing, 1.0, 5);
  return p;
}

void player_update(game_t *gs, entity_t p, const input_t in)
{
  player_move(gs, p, in);
}

void player_move(game_t *gs, entity_t p, const input_t in)
{
  sprite_t *ps = ENTITY_GET_COMPONENT(gs->edict, p, sprite);
  transform_t *pt = ENTITY_GET_COMPONENT(gs->edict, p, transform);
  
  float speed = 0.1;
  float rot_speed = 0.05;
  vector walk = vec3(0, 0, 0);
  
  pt->rotation.z += input_is_key_pressed(in, 'q') * rot_speed;
  pt->rotation.z -= input_is_key_pressed(in, 'e') * rot_speed;
  
  if (input_is_key_pressed(in, 'w')) ps->repeat = &walk_forward;
  if (input_is_key_pressed(in, 's')) ps->repeat = &walk_back;
  if (input_is_key_pressed(in, 'a')) ps->repeat = &walk_left;
  if (input_is_key_pressed(in, 'd')) ps->repeat = &walk_right;
  
  walk.y += input_is_key_pressed(in, 'w');
  walk.y -= input_is_key_pressed(in, 's');
  walk.x -= input_is_key_pressed(in, 'a');
  walk.x += input_is_key_pressed(in, 'd');
  
  if (dot(walk, walk) > 0.0) {
    walk = fdotv(speed, normalize(mdotv(rotate_xyz(pt->rotation), walk)));
    pt->position = vaddv(pt->position, walk);
  } else {
    ps->repeat = NULL;
  }
}
