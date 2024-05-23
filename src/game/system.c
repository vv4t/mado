#include <game/system.h>
#include <stdio.h>

void system_perform(edict_t *ed)
{
  for (entity_t e = 0; e < ed->num_entities; e++) {
    if (!entity_match(ed, e, C_actor | C_listen)) {
      continue;
    }
    
    actor_t *a = entity_get_component(ed, e, actor);
    listen_t *ls = entity_get_component(ed, e, listen);
    
    for (int i = 0; i < ACTION_MAX; i++) {
      action_t *action = &a->action[i];
      
      if (action->time > 0.0) action->time -= 0.015;
      else action->time = 0.0;
      if (!action->active) continue;
      
      if (action->time <= 0.0) {
        event_t ev = (event_t) { .type = EV_ACT0 + i };
        ls->invoke(ed, e, ev);
        
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

void system_integrate(edict_t *ed, map_t map)
{
  for (entity_t e = 0; e < ed->num_entities; e++) {
    if (!entity_match(ed, e, C_transform | C_rigidbody)) {
      continue;
    }
    
    transform_t *t = entity_get_component(ed, e, transform);
    rigidbody_t *rb = entity_get_component(ed, e, rigidbody);
    listen_t *ls = entity_get_component(ed, e, listen);
    
    vector new_p = vaddv(t->position, fdotv(0.015, rb->velocity));
    vector new_x = vec2(new_p.x, t->position.y);
    vector new_y = vec2(t->position.x, new_p.y);
    
    float d = 0.25;
    
    int hit_x = map_collide(map, new_x, vec2(d, d));
    int hit_y = map_collide(map, new_y, vec2(d, d));
    int hit_p = map_collide(map, new_p, vec2(d, d));
    int hit = hit_x || hit_y || hit_p;
    
    if (hit_x) new_p.x = t->position.x;
    if (hit_y) new_p.y = t->position.y;
    if (!hit_x && !hit_y && hit_p) new_p = t->position;
    
    if (hit && entity_match(ed, e, C_listen)) {
      event_t ev = (event_t) { .type = EV_MAP_COLLIDE };
      ls->invoke(ed, e, ev);
    }
    
    t->position = new_p;
  }
}

void system_animate(edict_t *ed)
{
  for (entity_t e = 0; e < ed->num_entities; e++) {
    if (!entity_match(ed, e, C_sprite)) {
      continue;
    }
    
    sprite_t *s = entity_get_component(ed, e, sprite);
    
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
