#include "game.h"

#include "system/system.h"
#include "entity/player.h"
#include "entity/orb.h"
#include "entity/warrior.h"
#include <string.h>

void game_init(game_t *game)
{
  *game = (game_t) {0};
  
  player_init(game->player, game);
  // orb_spawn(game, vec2_init(3, 3));
  warrior_spawn(game, vec2_init(5, 4));
}

void game_update(game_t *game, const usercmd_t *usercmd)
{
  game->usercmd = *usercmd;
  
  player_update(game->player, game);
  
  sys_perform_action(game);
  sys_physics_update(game);
  sys_animate_sprite(game);
  sys_check_health(game);
}

void game_load_map(game_t *game, map_t *map)
{
  game->map = map;
}

void game_kill(game_t *game, entity_t entity)
{
  edict_kill(&game->edict, entity);
  
  memset(&game->cdict.tag[entity], 0, sizeof(game->cdict.tag[entity]));
  memset(&game->cdict.box[entity], 0, sizeof(game->cdict.box[entity]));
  memset(&game->cdict.actor[entity], 0, sizeof(game->cdict.actor[entity]));
  memset(&game->cdict.motion[entity], 0, sizeof(game->cdict.motion[entity]));
  memset(&game->cdict.health[entity], 0, sizeof(game->cdict.health[entity]));
  memset(&game->cdict.sprite[entity], 0, sizeof(game->cdict.sprite[entity]));
  memset(&game->cdict.bullet[entity], 0, sizeof(game->cdict.bullet[entity]));
  memset(&game->cdict.ai_move[entity], 0, sizeof(game->cdict.ai_move[entity]));
  memset(&game->cdict.transform[entity], 0, sizeof(game->cdict.transform[entity]));
}
