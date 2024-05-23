#ifndef SPRITE_H
#define SPRITE_H

#include <stdlib.h>
#include <lib/math3d.h>

typedef struct {
  int tx, ty, tw, th;
  int framecount;
  float frametime;
} animation_t;

typedef struct {
  float rotation;
  int orient;
  int tx, ty, tw, th;
  float time;
  const animation_t *repeat;
  const animation_t *single;
} sprite_t;

inline static sprite_t create_sprite()
{
  return (sprite_t) {
    .rotation = 0.0,
    .orient = 1,
    .tx = 0, .ty = 0, .tw = 1, .th = 1,
    .time = 0.0,
    .repeat = NULL
  };
}

inline static void sprite_repeat(sprite_t *s, const animation_t *repeat)
{
  s->tx = repeat->tx;
  s->ty = repeat->ty;
  s->tw = repeat->tw;
  s->th = repeat->th;
  s->repeat = repeat;
}

inline static void sprite_play(sprite_t *s, const animation_t *single)
{
  s->tx = single->tx;
  s->ty = single->ty;
  s->tw = single->tw;
  s->th = single->th;
  s->single = single;
  s->time = 0.0;
}

#endif
