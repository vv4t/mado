#include "bullet.h"

void xaction_bullet_die(entity_t entity, game_t *game);
void xhitmap_bullet(entity_t entity, game_t *game);
void xhit_bullet(entity_t entity, game_t *game, entity_t hit);

void bullet_shoot(game_t *game, vec2_t pos, vec2_t uv, float angle, float live_time, c_tag_t target, int damage)
{
  entity_t entity = edict_spawn(&game->edict);
  
  component_t field = 0;
  field |= c_transform_init(&game->cdict.transform[entity], pos, angle);
  field |= c_motion_init(&game->cdict.motion[entity]);
  field |= c_sprite_init(&game->cdict.sprite[entity], uv, false, false, angle - M_PI/2.0);
  field |= c_bullet_init(&game->cdict.bullet[entity], target, damage);
  field |= c_box_init(&game->cdict.box[entity], vec2_init(-0.2,-0.2), vec2_init(+0.2,+0.2));
  field |= c_actor_init(&game->cdict.actor[entity]);
  game->edict.field[entity] = field;
  
  c_actor_set_act(&game->cdict.actor[entity], 0, xaction_bullet_die, live_time);
  
  game->cdict.box[entity].xhit = xhit_bullet;
  game->cdict.box[entity].xhitmap = xhitmap_bullet;
  game->cdict.motion[entity].velocity = vec2_init(cos(angle) * 10, sin(angle) * 10);
}

void xhit_bullet(entity_t entity, game_t *game, entity_t hit)
{
  const component_t mask = COMPONENT_ACTOR | COMPONENT_HEALTH;
  
  if ((game->edict.field[hit] & mask) != mask)
    return;
  
  c_tag_t tag_hit = game->cdict.tag[hit];
  c_tag_t tag_target = game->cdict.bullet[entity].target;
  
  if ((tag_hit & tag_target) == 0)
    return;
  
  int damage = game->cdict.bullet[entity].damage;
  
  c_health_apply_damage(&game->cdict.health[hit], damage);
  game_kill(game, entity);
}

void xhitmap_bullet(entity_t entity, game_t *game)
{
  game_kill(game, entity);
}

void xaction_bullet_die(entity_t entity, game_t *game)
{
  edict_kill(&game->edict, entity);
}
