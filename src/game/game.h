#ifndef GAME_H
#define GAME_H

#include "../common/nui_math.h"
#include <stdbool.h>

#define MAX_SPRITES 8

typedef enum {
  TILE_DATA_EXISTS = (1 << 4)
} tile_t;

typedef struct {
  bool block;
  bool solid;
  int uv;
} tile_data_t;

typedef struct {
  tile_t *data;
  tile_data_t *tile_data;
  int width;
  int height;
} map_t;

typedef struct {
  float forward;
  float side;
  float rot;
} usercmd_t;

typedef struct {
  vec2_t pos;
  vec2_t uv;
  bool show;
  bool stand;
} sprite_t;

typedef struct {
  vec2_t pos;
  float rot;
  sprite_t *sprite;
} player_t;

typedef struct {
  float time;
  player_t player;
  sprite_t sprites[MAX_SPRITES];
} game_t;

void game_init(game_t *game);
void game_update(game_t *game, const usercmd_t *usercmd);

#endif
