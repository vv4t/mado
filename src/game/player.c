#include <game/player.h>
#include <stdio.h>

void player_move(edict_t *ed, entity_t p, float rot_z, const input_t in);
void player_shoot(edict_t *ed, entity_t p, const input_t in);
void player_invoke(edict_t *ed, entity_t p, event_t ev);
void shoot(edict_t *ed, entity_t p);
void bullet_invoke(edict_t *ed, entity_t e, event_t ev);

static const animation_t walk_forward = { .tx = 2, .ty = 1, .tw = 1, .th = 1, .framecount = 2, .frametime = 0.1 };
static const animation_t walk_back    = { .tx = 0, .ty = 1, .tw = 1, .th = 1, .framecount = 2, .frametime = 0.1 };
static const animation_t walk_left    = { .tx = 2, .ty = 2, .tw = 1, .th = 1, .framecount = 2, .frametime = 0.1 };
static const animation_t walk_right   = { .tx = 0, .ty = 2, .tw = 1, .th = 1, .framecount = 2, .frametime = 0.1 };

entity_t player_create(edict_t *ed)
{
  entity_t p = edict_add(ed);
  entity_add_component(ed, p, transform);
    transform_t *t = entity_get_component(ed, p, transform);
    t->position = vec2(1, 1);
  entity_add_component(ed, p, sprite);
    sprite_t *s = entity_get_component(ed, p, sprite);
    s->tx = 0;
    s->ty = 1;
    s->repeat = &walk_forward;
  entity_add_component(ed, p, actor);
    actor_t *a = entity_get_component(ed, p, actor);
    actor_set(a, 0, 0.5, 0);
    actor_set(a, 1, 0.125, 3);
  entity_add_component(ed, p, rigidbody);
    rigidbody_t *rb = entity_get_component(ed, p, rigidbody);
  entity_add_component(ed, p, listen);
    listen_t *ls = entity_get_component(ed, p, listen);
    ls->invoke = player_invoke;
  return p;
}

void player_invoke(edict_t *ed, entity_t p, event_t ev)
{
  actor_t *pa = entity_get_component(ed, p, actor);
  
  switch (ev.type) {
  case EV_ACT0:
    actor_redo(pa, 1);
    break;
  case EV_ACT1:
    shoot(ed, p);
    break;
  }
}

void player_update(edict_t *ed, entity_t p, float rot_z, const input_t in)
{
  player_move(ed, p, rot_z, in);
  player_shoot(ed, p, in);
}

void player_shoot(edict_t *ed, entity_t p, const input_t in)
{
  actor_t *a = entity_get_component(ed, p, actor);
  
  if (input_is_mouse_pressed(in, 1)) {
    actor_start(a, 0);
  } else {
    actor_stop(a, 0);
  }
}

void player_move(edict_t *ed, entity_t p, float rot_z, const input_t in)
{
  sprite_t *ps = entity_get_component(ed, p, sprite);
  transform_t *pt = entity_get_component(ed, p, transform);
  rigidbody_t *rb = entity_get_component(ed, p, rigidbody);
  
  float speed = 5.0;
  vector walk = vec2(0, 0);
  
  if (input_is_key_pressed(in, 'w')) ps->repeat = &walk_forward;
  if (input_is_key_pressed(in, 's')) ps->repeat = &walk_back;
  if (input_is_key_pressed(in, 'a')) ps->repeat = &walk_left;
  if (input_is_key_pressed(in, 'd')) ps->repeat = &walk_right;
  pt->rotation.z = rot_z + atan2(-input_get_mouse_x(in), input_get_mouse_y(in));
  
  walk.y += input_is_key_pressed(in, 'w');
  walk.y -= input_is_key_pressed(in, 's');
  walk.x -= input_is_key_pressed(in, 'a');
  walk.x += input_is_key_pressed(in, 'd');
  
  if (dot(walk, walk) > 0.0) {
    walk = fdotv(speed, normalize(mdotv(rotate_z(rot_z), walk)));
    rb->velocity = walk;
  } else {
    ps->repeat = NULL;
    rb->velocity = vec2(0.0, 0.0);
  }
}

void shoot(edict_t *ed, entity_t p)
{
  transform_t *pt = entity_get_component(ed, p, transform);
  
  entity_t e = edict_add(ed);
  entity_add_component(ed, e, transform);
    transform_t *t = entity_get_component(ed, e, transform);
    t->position = pt->position;
  entity_add_component(ed, e, rigidbody);
    rigidbody_t *rb = entity_get_component(ed, e, rigidbody);
    rb->velocity = mdotv(rotate_z(pt->rotation.z), vec2(0, 4));
  entity_add_component(ed, e, sprite);
    sprite_t *s = entity_get_component(ed, e, sprite);
    s->tx = 0;
    s->ty = 0;
    s->orient = 0;
    s->rotation = atan2(rb->velocity.y, rb->velocity.x) - M_PI / 2.0;
  entity_add_component(ed, e, actor);
    actor_t *a = entity_get_component(ed, e, actor);
    actor_set(a, 0, 1.0, 1);
    actor_start(a, 0);
  entity_add_component(ed, e, listen);
    listen_t *ls = entity_get_component(ed, e, listen);
    ls->invoke = bullet_invoke;
}

void bullet_invoke(edict_t *ed, entity_t e, event_t ev)
{
  switch (ev.type) {
  case EV_ACT0:
    edict_kill(ed, e);
    break;
  case EV_MAP_COLLIDE:
    edict_kill(ed, e);
    break;
  }
}
