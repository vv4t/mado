#include "bullet.h"

bullet_t *bullet_new(
    bullet_t bullets[MAX_BULLETS],
    sprite_t sprites[MAX_SPRITES],
    vec2_t sprite_uv,
    vec2_t pos,
    float rot,
    float lifetime,
    float decay_rate
) {
  bullet_t *new_bullet = bullet_alloc(bullets);
  sprite_t *new_sprite = sprite_alloc(sprites);
  
  if (new_bullet && new_sprite) {
    new_bullet->pos = pos;
    new_bullet->rot = rot;
    new_bullet->lifetime = lifetime;
    new_bullet->decay_rate = decay_rate;
  
    new_bullet->sprite = new_sprite;
    new_bullet->sprite->pos = pos;
    new_bullet->sprite->uv = sprite_uv;
    new_bullet->sprite->show = true;
  } else {
    if (new_sprite) {
      new_sprite->used = false;
    }
    if (new_bullet) {
      new_bullet->used = false;
    }
    return NULL;
  }
}

bullet_t *bullet_alloc(bullet_t bullets[MAX_BULLETS]) {
  for (int i = 0; i < MAX_BULLETS; i++) {
    if (!bullets[i].used) {
      bullets[i].used = true;
      return &bullets[i];
    }
  }
  return NULL;
}

void bullet_kill(bullet_t *bullet) {
  bullet->used = false;
  bullet->sprite->used = false;
  bullet->sprite->show = false;
}

void bullet_update(bullet_t *bullet) {
  bullet->lifetime -= bullet->decay_rate;

  if (bullet->lifetime <= 0) {
    bullet_kill(bullet);
  }

  bullet->pos.x += cos(bullet->rot);
  bullet->pos.y += sin(bullet->rot);

  bullet->sprite->pos = bullet->pos; 
}