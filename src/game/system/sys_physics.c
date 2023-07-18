#include "system.h"

static void box_hit_map(game_t *game);
static void box_hit_box(game_t *game);
static void integrate_motion(game_t *game);
static void predict_motion(game_t *game);

void sys_physics_update(game_t *game)
{
  predict_motion(game);
  box_hit_map(game);
  box_hit_box(game);
  predict_motion(game);
  integrate_motion(game);
}

static void integrate_motion(game_t *game)
{
  const component_t mask = COMPONENT_TRANSFORM | COMPONENT_MOTION;
  
  for (int i = 0; i < MAX_ENTITIES; i++) {
    if ((game->edict.field[i] & mask) != mask)
      continue;
    
    game->cdict.transform[i].position = game->cdict.motion[i].new_pos;
  }
}

static void predict_motion(game_t *game)
{
  const component_t mask = COMPONENT_TRANSFORM | COMPONENT_MOTION;
  
  for (int i = 0; i < MAX_ENTITIES; i++) {
    if ((game->edict.field[i] & mask) != mask)
      continue;
    
    vec2_t delta_pos = vec2_mulf(game->cdict.motion[i].velocity, DELTA_TIME);
    vec2_t new_pos = vec2_add(game->cdict.transform[i].position, delta_pos);
    
    game->cdict.motion[i].new_pos = new_pos;
  }
}

static void box_hit_box(game_t *game)
{
  const component_t mask = COMPONENT_TRANSFORM | COMPONENT_MOTION | COMPONENT_BOX;
  
  for (int i = 0; i < MAX_ENTITIES; i++) {
    if ((game->edict.field[i] & mask) != mask)
      continue;
    
    vec2_t a_min = vec2_add(game->cdict.motion[i].new_pos, game->cdict.box[i].min);
    vec2_t a_max = vec2_add(game->cdict.motion[i].new_pos, game->cdict.box[i].max);
    
    for (int j = i + 1; j < MAX_ENTITIES; j++) {
      if ((game->edict.field[j] & mask) != mask)
        continue;
      
      vec2_t b_min = vec2_add(game->cdict.motion[j].new_pos, game->cdict.box[j].min);
      vec2_t b_max = vec2_add(game->cdict.motion[j].new_pos, game->cdict.box[j].max);
      
      if (
        a_min.x <= b_max.x &&
        a_max.x >= b_min.x &&
        a_min.y <= b_max.y &&
        a_max.y >= b_min.y
      ) {
        if (game->cdict.box[i].xhit)
          game->cdict.box[i].xhit(i, game, j);
        
        if (game->cdict.box[j].xhit)
          game->cdict.box[j].xhit(j, game, i);
      }
    }
  }
}
    

static void box_hit_map(game_t *game)
{
  const component_t mask = COMPONENT_TRANSFORM | COMPONENT_MOTION | COMPONENT_BOX;
  
  for (int i = 0; i < MAX_ENTITIES; i++) {
    if ((game->edict.field[i] & mask) != mask)
      continue;
    
    vec2_t old_pos = game->cdict.transform[i].position;
    vec2_t new_pos = game->cdict.motion[i].new_pos;
    
    vec2_t new_pos_x = vec2_init(new_pos.x, old_pos.y);
    vec2_t new_pos_y = vec2_init(old_pos.x, new_pos.y);
    
    vec2_t min = game->cdict.box[i].min;
    vec2_t max = game->cdict.box[i].max;
    
    if (map_collide(game->map, new_pos, min, max)) {
      if (!map_collide(game->map, new_pos_y, min, max)) {
        game->cdict.motion[i].velocity.x = 0.0;
      } else if (!map_collide(game->map, new_pos_x, min, max)) {
        game->cdict.motion[i].velocity.y = 0.0;
      } else {
        game->cdict.motion[i].velocity.x = 0.0;
        game->cdict.motion[i].velocity.y = 0.0;
      }
      
      if (game->cdict.box[i].xhitmap) {
        game->cdict.box[i].xhitmap(i, game);
      }
    }
  }
}
