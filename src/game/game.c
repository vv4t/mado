#include <game/game.h>
#include <game/player.h>
#include <stdio.h>

static void game_animate(game_t *gs);
static void game_integrate(game_t *gs);

void game_init(game_t *gs)
{
  *gs = (game_t) {0};
  gs->player = player_create(gs);
  
  entity_t e = edict_add(&gs->edict);
  ENTITY_ADD_COMPONENT(gs->edict, e, transform);
  ENTITY_ADD_COMPONENT(gs->edict, e, sprite);
  ENTITY_ADD_COMPONENT(gs->edict, e, rigidbody);
  rigidbody_t *rb = ENTITY_GET_COMPONENT(gs->edict, e, rigidbody);
  rb->velocity.x = 2;
  rb->velocity.y = 2;
  sprite_t *s = ENTITY_GET_COMPONENT(gs->edict, e, sprite);
  s->tx = 0;
  s->ty = 0;
  s->orient = 0;
  s->stand = 0;
  s->rotation = atan2(rb->velocity.y, rb->velocity.x) - M_PI / 2.0;
  transform_t *t = ENTITY_GET_COMPONENT(gs->edict, e, transform);
  t->position.x = 3;
}

void game_update(game_t *gs, const input_t in)
{
  player_update(gs, gs->player, in);
  game_animate(gs);
  game_integrate(gs);
  gs->time += 0.015;
}

void game_integrate(game_t *gs)
{
  for (entity_t e = 0; e < gs->edict.num_entities; e++) {
    if (ENTITY_MATCH(gs->edict, e, C_transform | C_rigidbody)) {
      continue;
    }
    
    transform_t *t = ENTITY_GET_COMPONENT(gs->edict, e, transform);
    rigidbody_t *rb = ENTITY_GET_COMPONENT(gs->edict, e, rigidbody);
    
    t->position = vaddv(t->position, fdotv(0.015, rb->velocity));
  }
}

void game_animate(game_t *gs)
{
  for (entity_t e = 0; e < gs->edict.num_entities; e++) {
    if (ENTITY_MATCH(gs->edict, e, C_sprite)) {
      continue;
    }
    
    sprite_t *s = ENTITY_GET_COMPONENT(gs->edict, e, sprite);
    
    if (s->repeat) {
      s->tx = s->repeat->tx + (int) s->time % s->repeat->framecount;
      s->ty = s->repeat->ty;
      s->time += s->repeat->frametime;
    }
  }
}
