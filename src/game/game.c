#include <game/game.h>
#include <game/player.h>
#include <game/actor.h>
#include <stdio.h>

static void game_animate(game_t *gs);
static void game_integrate(game_t *gs);
static void game_perform(game_t *gs);

void game_init(game_t *gs)
{
  *gs = (game_t) {0};
  gs->player = player_create(gs);
}

void game_update(game_t *gs, const input_t in)
{
  player_update(gs, gs->player, in);
  game_animate(gs);
  game_integrate(gs);
  game_perform(gs);
  gs->time += 0.015;
}

void game_perform(game_t *gs)
{
  for (entity_t e = 0; e < gs->edict.num_entities; e++) {
    if (ENTITY_MATCH(gs->edict, e, C_actor)) {
      continue;
    }
    
    actor_t *a = ENTITY_GET_COMPONENT(gs->edict, e, actor);
    
    for (int i = 0; i < ACTION_MAX; i++) {
      action_t *action = &a->action[i];
      
      if (action->act == NULL || action->time == 0.0) {
        continue;
      }
      
      action->time -= 0.015;
      
      if (action->time < 0) {
        action->act(gs, e);
        action->time = action->maxtime;
        
        if (action->count == 1) {
          if (i > ACTION_FIXED_MAX) {
            action->act = NULL;
          }
          action->time = 0.0;
        } else if (action->count > 1) {
          action->count--;
        }
      }
    }
  }
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
