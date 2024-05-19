#include <game/player.h>

void player_move(game_t *gs, entity_t p, const input_t in);

static const animation_t walk_forward = { .tx = 2, .ty = 1, .tw = 1, .th = 1, .framecount = 2, .frametime = 0.1 };
static const animation_t walk_back    = { .tx = 0, .ty = 1, .tw = 1, .th = 1, .framecount = 2, .frametime = 0.1 };
static const animation_t walk_left    = { .tx = 2, .ty = 2, .tw = 1, .th = 1, .framecount = 2, .frametime = 0.1 };
static const animation_t walk_right   = { .tx = 0, .ty = 2, .tw = 1, .th = 1, .framecount = 2, .frametime = 0.1 };

entity_t player_create(game_t *gs)
{
  entity_t p = edict_add(&gs->edict);
  ENTITY_ADD_COMPONENT(gs->edict, p, transform);
  ENTITY_ADD_COMPONENT(gs->edict, p, sprite);
  sprite_t *s = ENTITY_GET_COMPONENT(gs->edict, p, sprite);
  s->tx = 0;
  s->ty = 1;
  s->repeat = &walk_forward;
  transform_t *t = ENTITY_GET_COMPONENT(gs->edict, p, transform);
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
