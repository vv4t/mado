#include "system.h"

void sys_animate_sprite(game_t *game)
{
  const component_t mask = COMPONENT_SPRITE | COMPONENT_ANIMATOR;
  
  for (int i = 0; i < MAX_ENTITIES; i++) {
    if ((game->edict.field[i] & mask) != mask)
      continue;
    
    c_sprite_t *c_sprite = &game->cdict.sprite[i];
    c_animator_t *c_animator = &game->cdict.animator[i];
    
    const animation_t *animation = game->cdict.animator[i].do_once;
    
    if (!animation) {
      animation = game->cdict.animator[i].repeat;
    }
    
    if (!animation) {
      continue;
    }
    
    c_animator->time += DELTA_TIME / animation->frame_time;
    
    if (c_animator->time > animation->frame_count) {
      if (animation == game->cdict.animator[i].do_once) {
        game->cdict.animator[i].do_once = NULL;
      }
    }
    
    int frame_id = ((int) c_animator->time) % animation->frame_count;
    
    c_sprite->uv.x = animation->uv.x + frame_id;
    c_sprite->uv.y = animation->uv.y;
  }
}
