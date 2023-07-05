#include "bullet.h"

bullet_t *bullet_new(
    bullet_t bullets[MAX_BULLETS],
    sprite_t sprites[MAX_SPRITES],
    vec2_t sprite_uv,
    vec2_t pos,
    float rot,
    float speed,
    float lifetime,
    float decay_rate
) {
  bullet_t *new_bullet = bullet_alloc(bullets);
  sprite_t *new_sprite = sprite_alloc(sprites);
  
  if (new_bullet && new_sprite) {
    new_bullet->pos = pos;
    new_bullet->rot = rot;

    new_bullet->speed = speed;
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

void bullet_update(bullet_t *bullet, float delta_time) {
  bullet->lifetime -= bullet->decay_rate * (delta_time * 10);

  if (bullet->lifetime <= 0) {
    bullet_kill(bullet);
  }

  vec2_t mag_dir = vec2_init(0, bullet->speed * delta_time);
  vec2_t rot_dir = vec2_rotate(mag_dir, bullet->rot - M_PI_2);

  bullet->pos = vec2_add(bullet->pos, rot_dir);

  bullet->sprite->pos = bullet->pos; 
}