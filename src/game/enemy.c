#include <game/enemy.h>
#include <stdio.h>

static const animation_t warrior_idle   = { .tx = 0, .ty = 4, .tw = 1, .th = 1, .framecount = 2, .frametime = 0.3 };
static const animation_t warrior_attack = { .tx = 2, .ty = 4, .tw = 1, .th = 1, .framecount = 2, .frametime = 0.3 };

void warrior_invoke(edict_t *ed, entity_t e, event_t ev);
void shoot_warrior_bullet(edict_t *ed, entity_t p);
void bullet2_invoke(edict_t *ed, entity_t e, event_t ev);

entity_t enemy_spawn_warrior(edict_t *ed)
{
  entity_t e = edict_add(ed);
  entity_add_component(ed, e, transform);
    transform_t *t = entity_get_component(ed, e, transform);
    t->scale = vec3(2.0, 2.0, 2.0);
    t->position = vec2(1, 1);
  entity_add_component(ed, e, sprite);
    sprite_t *s = entity_get_component(ed, e, sprite);
    sprite_repeat(s, &warrior_idle);
  entity_add_component(ed, e, actor);
    actor_t *a = entity_get_component(ed, e, actor);
    actor_set(a, 0, 3.0, 0);
    actor_set(a, 1, warrior_attack.frametime, 1);
    actor_start(a, 0);
  entity_add_component(ed, e, listen);
    listen_t *ls = entity_get_component(ed, e, listen);
    ls->invoke = warrior_invoke;
  return e;
}

void warrior_invoke(edict_t *ed, entity_t e, event_t ev)
{
  sprite_t *s = entity_get_component(ed, e, sprite);
  actor_t *a = entity_get_component(ed, e, actor);
  
  switch (ev.type) {
  case EV_ACT0:
    sprite_play(s, &warrior_attack);
    actor_redo(a, 1);
    break;
  case EV_ACT1:
    shoot_warrior_bullet(ed, e);
    break;
  }
}

void shoot_warrior_bullet(edict_t *ed, entity_t p)
{
  transform_t *pt = entity_get_component(ed, p, transform);
  
  entity_t e = edict_add(ed);
  entity_add_component(ed, e, transform);
    transform_t *t = entity_get_component(ed, e, transform);
    t->position = pt->position;
  entity_add_component(ed, e, rigidbody);
    rigidbody_t *rb = entity_get_component(ed, e, rigidbody);
    rb->velocity = mdotv(rotate_z(pt->rotation.z), vec2(0, 10));
  entity_add_component(ed, e, sprite);
    sprite_t *s = entity_get_component(ed, e, sprite);
    s->tx = 3;
    s->ty = 0;
    s->orient = 0;
    s->rotation = atan2(rb->velocity.y, rb->velocity.x) - M_PI / 2.0;
  entity_add_component(ed, e, actor);
    actor_t *a = entity_get_component(ed, e, actor);
    actor_set(a, 0, 0.4, 1);
    actor_start(a, 0);
  entity_add_component(ed, e, listen);
    listen_t *ls = entity_get_component(ed, e, listen);
    ls->invoke = bullet2_invoke;
}

void bullet2_invoke(edict_t *ed, entity_t e, event_t ev)
{
  switch (ev.type) {
  case EV_ACT0:
    edict_kill(ed, e);
    break;
  case EV_MAP_COLLIDE:
    edict_kill(ed, e);
    break;
  }
}
