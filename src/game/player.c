#include <game/player.h>
#include <game/shoot.h>
#include <stdio.h>

static void player_move(game_t *gs, const usercmd_t *usercmd);
static void player_attack(game_t *gs, const usercmd_t *usercmd);
static void player_invoke(game_t *gs, entity_t e, event_t ev);

static const animation_t walk_forward = { .tx = 2, .ty = 1, .tw = 1, .th = 1, .framecount = 2, .frametime = 0.15 };
static const animation_t walk_back    = { .tx = 0, .ty = 1, .tw = 1, .th = 1, .framecount = 2, .frametime = 0.15 };
static const animation_t walk_left    = { .tx = 2, .ty = 2, .tw = 1, .th = 1, .framecount = 2, .frametime = 0.15 };
static const animation_t walk_right   = { .tx = 0, .ty = 2, .tw = 1, .th = 1, .framecount = 2, .frametime = 0.15 };

static shooter_t player_shooter = {
  .tx = 0, .ty = 0,
  .tw = 1, .th = 1,
  .target = ENT_ENEMY,
  .damage = 20
};

typedef enum {
  PS_DEAD,
  PS_ACTIVE
} pstate_t;

struct playerctx {
  pstate_t state;
  float aim_rot;
};

void player_init(game_t *gs)
{
  entity_t e = entity_add(gs, ENT_PLAYER);
  entity_add_component(gs, e, transform);
  gs->player = e;
  player_kill(gs);
}

void player_kill(game_t *gs)
{
  health_t *h = entity_get_component(gs, gs->player, health);
  h->hp = 0;
  
  entity_bind(gs, gs->player, NULL);
  entity_remove_component(gs, gs->player, health);
  entity_remove_component(gs, gs->player, sprite);
  entity_remove_component(gs, gs->player, actor);
  entity_remove_component(gs, gs->player, rigidbody);
  
  struct playerctx *ctx = entity_get_context(gs, gs->player, sizeof(struct playerctx));
  ctx->state = PS_DEAD;
}

void player_spawn(game_t *gs, vector position)
{
  transform_t *pt = entity_get_component(gs, gs->player, transform);
  pt->position = position;
  
  entity_add_component(gs, gs->player, sprite);
    sprite_t *s = entity_get_component(gs, gs->player, sprite);
    sprite_repeat(s, &walk_forward);
  entity_add_component(gs, gs->player, actor);
    actor_t *a = entity_get_component(gs, gs->player, actor);
    actor_repeat(a, ACT0, 2.0, 0, 0.15);
    actor_stop(a, ACT0);
  entity_add_component(gs, gs->player, rigidbody);
    rigidbody_t *rb = entity_get_component(gs, gs->player, rigidbody);
    rb->radius = 0.1;
  entity_add_component(gs, gs->player, health);
    health_t *h = entity_get_component(gs, gs->player, health);
    h->hp = 100;
  entity_bind(gs, gs->player, player_invoke);
  
  struct playerctx *ctx = entity_get_context(gs, gs->player, sizeof(struct playerctx));
  ctx->state = PS_ACTIVE;
}

void player_invoke(game_t *gs, entity_t e, event_t ev)
{
  transform_t *pt = entity_get_component(gs, e, transform);
  health_t *h = entity_get_component(gs, e, health);
  struct playerctx *ctx = entity_get_context(gs, e, sizeof(struct playerctx));
  
  vector forward = mdotv(rotate_z(ctx->aim_rot), vec2(0, 16));

  switch (ev.type) {
  case EV_ACT:
    switch (ev.act.name) {
    case ACT0:
      shoot_wave(gs, &player_shooter, 0.6, pt->position, forward, 1.0, 36.0, 0.0);
      shoot_wave(gs, &player_shooter, 0.6, pt->position, forward, 1.0, 36.0, M_PI);
      break;
    }
    break;
  case EV_HIT:
    h->hp -= ev.hit.damage;
    break;
  case EV_NO_HEALTH:
    player_kill(gs);
    break;
  case EV_MAP_COLLIDE:
  case EV_ENTITY_COLLIDE:
  case EV_TRANSITION:
    break;
  }
}

void player_update(game_t *gs, const usercmd_t *usercmd)
{
  struct playerctx *ctx = entity_get_context(gs, gs->player, sizeof(struct playerctx));
  
  switch (ctx->state) {
  case PS_ACTIVE:
    player_move(gs, usercmd);
    player_attack(gs, usercmd);
    break;
  case PS_DEAD:
  default:
    break;
  }
}

void player_attack(game_t *gs, const usercmd_t *usercmd)
{
  actor_t *a = entity_get_component(gs, gs->player, actor);
  
  if (usercmd->attack) {
    actor_start(a, ACT0);
  } else {
    actor_stop(a, ACT0);
  }
}

void player_move(game_t *gs, const usercmd_t *usercmd)
{
  sprite_t *ps = entity_get_component(gs, gs->player, sprite);
  transform_t *pt = entity_get_component(gs, gs->player, transform);
  rigidbody_t *rb = entity_get_component(gs, gs->player, rigidbody);
  
  float speed = 9.0;
  float rot_speed = 0.05;
  
  vector walk = vec2(0, 0);
  
  if (usercmd->forward) ps->repeat = &walk_forward;
  if (usercmd->back) ps->repeat = &walk_back;
  if (usercmd->left) ps->repeat = &walk_left;
  if (usercmd->right) ps->repeat = &walk_right;
  
  walk.y += usercmd->forward;
  walk.y -= usercmd->back;
  walk.x -= usercmd->left;
  walk.x += usercmd->right;
  
  struct playerctx *ctx = entity_get_context(gs, gs->player, sizeof(struct playerctx));
  pt->rotation += usercmd->rotate_left * rot_speed;
  pt->rotation -= usercmd->rotate_right * rot_speed;
  ctx->aim_rot = pt->rotation + atan2(-usercmd->aim_x, -usercmd->aim_y);
  
  if (dot(walk, walk) > 0.0) {
    walk = fdotv(speed, normalize(mdotv(rotate_z(pt->rotation), walk)));
    rb->velocity = walk;
  } else {
    ps->repeat = NULL;
    rb->velocity = vec2(0.0, 0.0);
  }
}
