#include <game/game.h>

void game_init(game_t *gs)
{
  *gs = (game_t) {0};
  gs->time = 0.0;
  
  sprite_t *s;
  transform_t *t;
  
  gs->player = edict_add(&gs->edict);
  ENTITY_ADD_COMPONENT(gs->edict, gs->player, transform);
  ENTITY_ADD_COMPONENT(gs->edict, gs->player, sprite);
  t = ENTITY_GET_COMPONENT(gs->edict, gs->player, transform);
  s = ENTITY_GET_COMPONENT(gs->edict, gs->player, sprite);
  s->tx = 0;
  s->ty = 1;
  
  entity_t e;
  
  e = edict_add(&gs->edict);
  ENTITY_ADD_COMPONENT(gs->edict, e, transform);
  ENTITY_ADD_COMPONENT(gs->edict, e, sprite);
  t = ENTITY_GET_COMPONENT(gs->edict, e, transform);
  s = ENTITY_GET_COMPONENT(gs->edict, e, sprite);
  s->tx = 4;
  s->ty = 3;
}

void game_update(game_t *gs, int key[], float mx, float my)
{
  transform_t *pt = ENTITY_GET_COMPONENT(gs->edict, gs->player, transform);
  sprite_t *ps = ENTITY_GET_COMPONENT(gs->edict, gs->player, sprite);
  
  float speed = 0.1;
  float rot_speed = 0.05;
  vector walk = vec3(0, 0, 0);
  
  pt->rotation.z += key['q'] * rot_speed;
  pt->rotation.z -= key['e'] * rot_speed;
  
  if (key['w']) { ps->tx = 2; ps->ty = 1; }
  if (key['s']) { ps->tx = 0; ps->ty = 1; }
  if (key['a']) { ps->tx = 2; ps->ty = 2; }
  if (key['d']) { ps->tx = 0; ps->ty = 2; }
  
  walk.y += key['w'];
  walk.y -= key['s'];
  walk.x -= key['a'];
  walk.x += key['d'];
  
  if (dot(walk, walk) > 0.0) {
    walk = fdotv(speed, normalize(mdotv(rotate_xyz(pt->rotation), walk)));
    pt->position = vaddv(pt->position, walk);
    ps->tx += (int) (gs->time * 7.0) % 2;
  }
  
  gs->time += 0.015;
}
