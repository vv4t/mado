#ifndef SPRITE_H
#define SPRITE_H

#include "../common/nui_math.h"
#include <stdbool.h>

typedef struct {
  vec2_t start_uv;
  int frame_count;
  float frame_time;
} anim_t;

typedef struct {
  const anim_t *anim;
  float start_time;
  int frame_id;
} play_anim_t;

typedef struct {
  vec2_t pos;
  vec2_t uv;
  bool stand;
  bool show;
} sprite_t;

typedef struct {
  const char *path;
  int sprite_width;
  int sprite_height;
  int sheet_width;
  int sheet_height;
} sprite_sheet_t;

void play_anim_play(play_anim_t *play_anim, const anim_t *anim, float time);
void sprite_play_anim(sprite_t *sprite, const play_anim_t *play_anim);

#endif
