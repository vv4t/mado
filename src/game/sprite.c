#include "sprite.h"

void play_anim_play(play_anim_t *play_anim, const anim_t *anim, float time)
{
  if (play_anim->anim != anim) {
    play_anim->anim = anim;
    play_anim->start_time = time;
  } else {
    float frame_time = (time - play_anim->start_time) / anim->frame_time;
    play_anim->frame_id = (int) frame_time % anim->frame_count;
    
  }
}

void sprite_play_anim(sprite_t *sprite, const play_anim_t *play_anim)
{
  if (!play_anim->anim)
    return;
  
  sprite->uv.x = play_anim->anim->start_uv.x + play_anim->frame_id;
  sprite->uv.y = play_anim->anim->start_uv.y;
}
