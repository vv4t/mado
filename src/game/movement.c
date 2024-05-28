#include <game/movement.h>

vector movement_chase(game_t *gs, entity_t e, float speed, float a2, vector v1, vector v2)
{
  transform_t *pt = entity_get_component(gs, gs->player, transform);
  transform_t *et = entity_get_component(gs, e, transform);
  
  return fdotv(speed, normalize(vsubv(pt->position, et->position)));
}

vector movement_none(game_t *gs, entity_t e, float a1, float a2, vector v1, vector v2)
{  
  return vec2(0.0, 0.0);
}

vector movement_pivot(game_t *gs, entity_t e, float radius, float time, vector pivot, vector v2)
{  
  float pivot_dir = (rand() % 256) / 256.0 * M_PI * 2;
  
  transform_t *t = entity_get_component(gs, e, transform);
  vector pos = t->position;

  vector delta_pos = vsubv(pivot, pos);
  vector new_delta = mdotv(rotate_z(pivot_dir), vec2(0.0, radius));
  vector move_delta = vaddv(delta_pos, new_delta);
  
  return fdotv(length(move_delta) / time, normalize(move_delta));
}