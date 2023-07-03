#include "orb.h"

void orb_init(orb_t *orb, sprite_t *sprite)
{
  orb->pos = vec2_init(2.0, 2.0);
  orb->sprite = sprite;
  
  orb->sprite->pos = vec2_init(2.0, 3.0);
  orb->sprite->uv = vec2_init(2, 4);
  orb->sprite->show = true;
  orb->sprite->stand = true;
}

void orb_update(orb_t *orb, float time)
{
  orb->sprite->uv.x = 2 + floor(cos(time * 8) * cos(time * 8) * 2);
}
