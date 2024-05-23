#include <game/system.h>
#include <stdio.h>

void system_perform(game_t *gs)
{
  for (entity_t e = 0; e < gs->num_entities; e++) {
    if (!entity_match(gs, e, C_actor)) {
      continue;
    }
    
    actor_t *a = entity_get_component(gs, e, actor);
    
    for (int i = 0; i < ACTION_MAX; i++) {
      action_t *action = &a->action[i];
      
      if (action->time > 0.0) action->time -= 0.015;
      else action->time = 0.0;
      if (!action->active) continue;
      
      if (action->time <= 0.0) {
        event_t ev = (event_t) { .type = EV_ACT0 + i };
        entity_invoke(gs, e, ev);
        
        action->time = action->max_time;
        
        if (action->count == 1) {
          action->active = 0;
        } else if (action->count > 1) {
          action->count--;
        }
      }
    }
  }
}

void system_integrate(game_t *gs)
{
  for (entity_t e = 0; e < gs->num_entities; e++) {
    if (!entity_match(gs, e, C_transform | C_rigidbody)) {
      continue;
    }
    
    transform_t *t = entity_get_component(gs, e, transform);
    rigidbody_t *rb = entity_get_component(gs, e, rigidbody);
    
    vector new_p = vaddv(t->position, fdotv(0.015, rb->velocity));
    vector new_x = vec2(new_p.x, t->position.y);
    vector new_y = vec2(t->position.x, new_p.y);
    
    float d = 0.45;
    
    int hit_x = map_collide(gs->map, new_x, vec2(d, d));
    int hit_y = map_collide(gs->map, new_y, vec2(d, d));
    int hit_p = map_collide(gs->map, new_p, vec2(d, d));
    int hit = hit_x || hit_y || hit_p;
    
    if (hit_x) new_p.x = t->position.x;
    else if (hit_y) new_p.y = t->position.y;
    else if (hit_p) new_p = t->position;
    
    if (hit) {
      event_t ev = (event_t) { .type = EV_MAP_COLLIDE };
      entity_invoke(gs, e, ev);
    }
    
    t->position = new_p;
  }
}

void system_animate(game_t *gs)
{
  for (entity_t e = 0; e < gs->num_entities; e++) {
    if (!entity_match(gs, e, C_sprite)) {
      continue;
    }
    
    sprite_t *s = entity_get_component(gs, e, sprite);
    
    if (s->single) {
      s->tx = s->single->tx + (int) s->time * s->single->tw;
      s->ty = s->single->ty;
      s->time += 0.015 / s->single->frametime;
      
      if ((int) s->time == s->single->framecount) {
        s->single = NULL;
      }
    } else if (s->repeat) {
      int frame = (int) s->time % s->repeat->framecount;
      s->tx = s->repeat->tx + frame * s->repeat->tw;
      s->ty = s->repeat->ty;
      s->time += 0.015 / s->repeat->frametime;
    }
  }
}

void system_update_bullet(game_t *gs)
{
  for (entity_t e = 0; e < gs->num_entities; e++) {
    if (!entity_match(gs, e, C_rigidbody | C_bullet)) {
      continue;
    }
    
    bullet_t *b = entity_get_component(gs, e, bullet);
    rigidbody_t *rb = entity_get_component(gs, e, rigidbody);
    
    if (!b->flight) {
      continue;
    }
    
    vector x = b->flight(b->time, b->a1, b->a2);
    rb->velocity = vaddv(fdotv(x.x, b->side), fdotv(x.y, b->forward));
    b->time += 0.015;
  }
}
