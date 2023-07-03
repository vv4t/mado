#include "orb.h"

void orb_init(orb_t *orb, sprite_t *sprite)
{
  orb->pos = vec2_init(2.0, 2.0);
  orb->sprite = sprite;
  
  orb->sprite->pos = vec2_init(2.0, 3.0);
  orb->sprite->uv = vec2_init(2, 4);
  orb->sprite->show = true;
  orb->sprite->stand = true;
  
  orb->idle_anim = (anim_t) { .start_uv = {2,4}, .frame_count = 2, .frame_time = 0.25 };
}

void orb_update(orb_t *orb, float time)
{
  play_anim_play(&orb->play_anim, &orb->idle_anim, time);
  
  sprite_play_anim(orb->sprite, &orb->play_anim);
}
