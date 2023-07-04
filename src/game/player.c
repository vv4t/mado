#include "player.h"
#include "bullet.h"
#include "game.h"

vec2_t player_cmd_move(player_t *player, const usercmd_t *usercmd);
void player_collide_move(player_t *player, const map_t *map, vec2_t move_dir);
void player_sprite_animate(player_t *player, float time, vec2_t move_dir);
float rad_diff(float beta, float alpha);

void player_init(player_t *player, sprite_t *sprite)
{
  player->pos = vec2_init(2.0, 2.0);
  player->sprite = sprite;
  player->shoot_cooldown = 0;

  player->max_shoot_cooldown = 100;
  player->shoot_cooldown_decay = 5;
}

void player_move(player_t *player, const map_t *map, float time, const usercmd_t *usercmd)
{
  player->rot += usercmd->rot * 0.05;
  
  vec2_t move_dir = player_cmd_move(player, usercmd);
  player_collide_move(player, map, move_dir);
  player_sprite_animate(player, time, move_dir);
}

void player_sprite_animate(player_t *player, float time, vec2_t move_dir)
{
  float sprite_angle = rad_diff(player->rot, player->sprite_rot - M_PI / 4);
  int sprite_state = floor(sprite_angle / (M_PI / 2));
  
  if (fabs(move_dir.x) + fabs(move_dir.y) > 0.04) {
    player->sprite_rot = atan2(move_dir.y, move_dir.x);
    player->sprite->uv.x = floor(cos(time * 8) * cos(time * 8) * 2);
  }
  
  player->sprite->pos = player->pos;
}

vec2_t player_cmd_move(player_t *player, const usercmd_t *usercmd)
{
  vec2_t cmd_dir = vec2_init(usercmd->side, usercmd->forward);
  vec2_t wish_dir = vec2_normalize(cmd_dir);
  vec2_t rot_dir = vec2_rotate(wish_dir, player->rot);
  vec2_t move_dir = vec2_mulf(rot_dir, 0.05);
  
  return move_dir;
}

void player_collide_move(player_t *player, const map_t *map, vec2_t move_dir)
{
  vec2_t new_pos = vec2_add(player->pos, move_dir);
  vec2_t new_pos_x = vec2_add(player->pos, vec2_init(move_dir.x, 0.0));
  vec2_t new_pos_y = vec2_add(player->pos, vec2_init(0.0, move_dir.y));
  vec2_t box = vec2_init(0.2, 0.2);
  
  if (map_collide(map, new_pos.x, new_pos.y, box)) {
    if (!map_collide(map, player->pos.x, new_pos.y, box)) {
      player->pos.y = new_pos.y;
    } else if (!map_collide(map, new_pos.x, player->pos.y, box)) {
      player->pos.x = new_pos.x;
    } 
  } else {
    player->pos = new_pos;
  }
}

float rad_diff(float a, float b)
{
  float d = fmod(fabs(b - a), 2 * M_PI);
  float r = d > M_PI ? 2 * M_PI - d : d;
  float sign = (a - b >= 0 && a - b <= M_PI) || (a - b <= -M_PI && a - b>= -M_PI) ? 1 : -1;
  return sign < 0 ? 2 * M_PI - r : r;
}

void player_shoot(
  player_t *player, 
  bullet_t bullets[MAX_BULLETS], 
  sprite_t sprites[MAX_SPRITES], 
  const usercmd_t *usercmd
) {
  if (player->shoot_cooldown > 0) {
    player->shoot_cooldown -= player->shoot_cooldown_decay;
  }

  if (!usercmd->mouse_down) {
    return;
  }

  if (player->shoot_cooldown <= 0) {
    player->shoot_cooldown = player->max_shoot_cooldown;

    float shoot_angle = -atan2(
      usercmd->relative_cursor_y, 
      usercmd->relative_cursor_x
    ) + player->rot;

    bullet_t *new_bullet = bullet_new(
      bullets, sprites,
      vec2_init(0,5),
      player->pos, shoot_angle, 
      100, 5
    );
  }
}