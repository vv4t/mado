#include <game/game.h>
#include <game/player.h>
#include <game/actor.h>
#include <stdio.h>

static void game_animate(game_t *gs);
static void game_integrate(game_t *gs);
static void game_perform(game_t *gs);
static void game_move_camera(game_t *gs, const input_t in);

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
  game_move_camera(gs, in);
  gs->time += 0.015;
}

void game_load_map(game_t *gs, map_t map)
{
  gs->map = map;
}

void game_move_camera(game_t *gs, const input_t in)
{
  float rot_speed = 0.05;
  transform_t *pt = ENTITY_GET_COMPONENT(gs->edict, gs->player, transform);
  
  gs->view_rotation.z += input_is_key_pressed(in, 'q') * rot_speed;
  gs->view_rotation.z -= input_is_key_pressed(in, 'e') * rot_speed;
  gs->view_position = pt->position;
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
      
      if (!action->active) {
        continue;
      }
      
      if (action->time < 0) {
        action->act(gs, e);
        action->time = action->max_time;
        
        if (action->count == 1) {
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
    
    vector new_p = vaddv(t->position, fdotv(0.015, rb->velocity));
    vector new_x = vec2(new_p.x, t->position.y);
    vector new_y = vec2(t->position.x, new_p.y);
    
    float d = 0.25;
    
    if (map_collide(gs->map, new_x, vec2(d, d))) new_p.x = t->position.x;
    if (map_collide(gs->map, new_y, vec2(d, d))) new_p.y = t->position.y;
    if (map_collide(gs->map, new_p, vec2(d, d))) new_p = t->position;
    
    t->position = new_p;
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
