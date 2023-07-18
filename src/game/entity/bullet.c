#include "bullet.h"

void xaction_bullet_die(entity_t entity, game_t *game);
void xhitmap_bullet(entity_t entity, game_t *game);
void xhit_bullet(entity_t entity, game_t *game, entity_t hit);

void bullet_shoot(game_t *game, vec2_t pos, float angle, float live_time, c_tag_t target)
{
  entity_t entity = edict_spawn(&game->edict);
  game->edict.field[entity] |= COMPONENT_TRANSFORM;
  game->edict.field[entity] |= COMPONENT_MOTION;
  game->edict.field[entity] |= COMPONENT_SPRITE;
  game->edict.field[entity] |= COMPONENT_BULLET;
  game->edict.field[entity] |= COMPONENT_BOX;
  game->edict.field[entity] |= COMPONENT_ACTOR;
  
  game->cdict.transform[entity].position = pos;
  game->cdict.transform[entity].rotation = angle;
  
  game->cdict.sprite[entity].uv = vec2_init(0, 7);
  game->cdict.sprite[entity].rotation = angle - M_PI/2.0;
  game->cdict.sprite[entity].orient = false;
  game->cdict.sprite[entity].stand = false;
  
  game->cdict.motion[entity].velocity = vec2_init(cos(angle) * 10, sin(angle) * 10);
  
  game->cdict.box[entity].min = vec2_init(-0.2, -0.2);
  game->cdict.box[entity].max = vec2_init(+0.2, +0.2);
  game->cdict.box[entity].xhit = xhit_bullet;
  game->cdict.box[entity].xhitmap = xhitmap_bullet;
  
  game->cdict.bullet[entity].target = target;
  
  c_actor_set_act(&game->cdict.actor[entity], 0, xaction_bullet_die, live_time);
}

void xhit_bullet(entity_t entity, game_t *game, entity_t hit)
{
  if (
    (game->edict.field[hit] & COMPONENT_TAG) != 0 &&
    (game->cdict.tag[hit] & game->cdict.bullet[entity].target) != 0
  ) {
    printf("e(%i) hit player\n", entity);
    edict_kill(&game->edict, entity);
  }
}

void xhitmap_bullet(entity_t entity, game_t *game)
{
  edict_kill(&game->edict, entity);
}

void xaction_bullet_die(entity_t entity, game_t *game)
{
  edict_kill(&game->edict, entity);
}
