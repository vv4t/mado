#include "system.h"

void sys_animate_sprite(game_t *game)
{
  const component_t mask = COMPONENT_SPRITE | COMPONENT_ANIMATOR;
  
  for (int i = 0; i < MAX_ENTITIES; i++) {
    if ((game->edict.field[i] & mask) != mask)
      continue;
    
    c_sprite_t *c_sprite = &game->cdict.sprite[i];
    c_animator_t *c_animator = &game->cdict.animator[i];
    
    if (!c_animator->animation)
      continue;
    
    c_animator->time += DELTA_TIME / c_animator->animation->frame_time;
    
    int frame_id = ((int) c_animator->time) % c_animator->animation->frame_count;
    
    c_sprite->uv.x = c_animator->animation->uv.x + frame_id;
    c_sprite->uv.y = c_animator->animation->uv.y;
  }
}
