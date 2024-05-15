#include <game/game.h>

void game_init(game_t *gs)
{
  *gs = (game_t) {0};
  gs->time = 0.0;
  
  gs->player = edict_add(&gs->edict);
  ENTITY_ADD_COMPONENT(gs->edict, gs->player, transform);
  ENTITY_ADD_COMPONENT(gs->edict, gs->player, sprite);
}

void game_update(game_t *gs, int key[], float mx, float my)
{
  transform_t *pt = ENTITY_GET_COMPONENT(gs->edict, gs->player, transform);
  
  float speed = 0.1;
  float rot_speed = 0.05;
  vector walk = vec3(0, 0, 0);
  
  pt->rotation.z += key['q'] * rot_speed;
  pt->rotation.z -= key['e'] * rot_speed;
  
  walk.y += key['w'];
  walk.y -= key['s'];
  walk.x -= key['a'];
  walk.x += key['d'];
  
  if (dot(walk, walk) > 0.0) {
    walk = fdotv(speed, normalize(mdotv(rotate_xyz(pt->rotation), walk)));
    pt->position = vaddv(pt->position, walk);
  }
  
  gs->time += 0.015;
}
