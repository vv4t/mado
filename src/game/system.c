#include <game/system.h>
#include <stdio.h>

void system_collide(game_t *gs)
{
  for (entity_t a = 0; a < gs->num_entities; a++) {
    if (!entity_match(gs, a, C_transform | C_rigidbody)) {
      continue;
    }
    
    transform_t *at = entity_get_component(gs, a, transform);
    rigidbody_t *a_rb = entity_get_component(gs, a, rigidbody);
    
    vector new_x = vec2(at->position.x, a_rb->prev_pos.y);
    vector new_y = vec2(a_rb->prev_pos.x, at->position.y);
    
    float d = 0.45;
    
    int hit_x = map_collide(gs->map, new_x, vec2(d, d));
    int hit_y = map_collide(gs->map, new_y, vec2(d, d));
    int hit_p = map_collide(gs->map, at->position, vec2(d, d));
    int hit = hit_x || hit_y || hit_p;
    
    if (hit_p) {
      if (!hit_x) at->position.y = a_rb->prev_pos.y;
      if (!hit_y) at->position.x = a_rb->prev_pos.x;
    }
    
    if (hit) {
      entity_invoke(gs, a, (event_t) { .type = EV_MAP_COLLIDE });
    }
    
    for (entity_t b = a + 1; b < gs->num_entities; b++) {
      if (!entity_match(gs, b, C_transform | C_rigidbody)) {
        continue;
      }
      
      transform_t *bt = entity_get_component(gs, b, transform);
      rigidbody_t *b_rb = entity_get_component(gs, b, rigidbody);
      
      if (length(vsubv(bt->position, at->position)) <= a_rb->radius + b_rb->radius) {
        entity_invoke(gs, a, (event_t) { .type = EV_ENTITY_COLLIDE, .entcol.e = b });
        entity_invoke(gs, b, (event_t) { .type = EV_ENTITY_COLLIDE, .entcol.e = a });
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
    
    rb->prev_pos = t->position;
    t->position = vaddv(t->position, fdotv(0.015, rb->velocity));
  }
}

void system_perform(game_t *gs)
{
  for (entity_t e = 0; e < gs->num_entities; e++) {
    if (!entity_match(gs, e, C_actor)) {
      continue;
    }
    
    actor_t *a = entity_get_component(gs, e, actor);
    
    for (actname_t i = 0; i < ACTION_MAX; i++) {
      action_t *action = &a->action[i];
      
      if (action->time > 0.0) action->time -= 0.015;
      else action->time = 0.0;
      if (!action->active) continue;
      
      if (action->time <= 0.0) {
        entity_invoke(gs, e, (event_t) { .type = EV_ACT, .act.name = i });
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
      s->tw = s->single->tw;
      s->th = s->single->th;
      s->time += 0.015 / s->single->frametime;
      
      if ((int) s->time == s->single->framecount) {
        s->single = NULL;
      }
    } else if (s->repeat) {
      int frame = (int) s->time % s->repeat->framecount;
      s->tx = s->repeat->tx + frame * s->repeat->tw;
      s->ty = s->repeat->ty;
      s->tw = s->repeat->tw;
      s->th = s->repeat->th;
      s->time += 0.015 / s->repeat->frametime;
    }
  }
}

void system_update_bullet(game_t *gs)
{
  for (entity_t e = 0; e < gs->num_entities; e++) {
    if (!entity_match(gs, e, C_rigidbody | C_bullet | C_sprite)) {
      continue;
    }
    
    bullet_t *b = entity_get_component(gs, e, bullet);
    sprite_t *s = entity_get_component(gs, e, sprite);
    rigidbody_t *rb = entity_get_component(gs, e, rigidbody);
    struct bulletctx *bulletctx = entity_get_context(gs, e, sizeof(struct bulletctx));
    
    if (!b->flight) {
      continue;
    }
    
    vector v = b->flight(b->time, bulletctx->a1, bulletctx->a2);
    rb->velocity = vaddv(fdotv(v.x, b->side), fdotv(v.y, b->forward));
    s->rotation = atan2(rb->velocity.y, rb->velocity.x) - M_PI / 2.0;
    b->time += 0.015;
  }
}

void system_update_health(game_t *gs)
{
  for (entity_t e = 0; e < gs->num_entities; e++) {
    if (!entity_match(gs, e, C_health)) {
      continue;
    }
    
    health_t *h = entity_get_component(gs, e, health);
    if (h->hp <= 0) {
      entity_invoke(gs, e, (event_t) { .type = EV_NO_HEALTH });
    }
  }
}

void system_update_botmove(game_t *gs)
{
  for (entity_t e = 0; e < gs->num_entities; e++) {
    if (!entity_match(gs, e, C_botmove | C_transform)) {
      continue;
    }

    rigidbody_t *r = entity_get_component(gs, e, rigidbody);
    botmove_t *m = entity_get_component(gs, e, botmove);

    r->velocity = m->movement(gs, e, m->a1, m->a2, m->v1, m->v2);
  }
}
