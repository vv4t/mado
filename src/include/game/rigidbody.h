#ifndef RIGIDBODY_H
#define RIGIDBODY_H

typedef void (*map_hit_t)(struct game_s *gs, int e);
// typedef void (*entity_hit_t)(struct game_s *gs, int e);

typedef struct {
  vector        velocity;
  map_hit_t     map_hit;
  // entity_hit_t  entity_hit;
} rigidbody_t;

inline static rigidbody_t create_rigidbody()
{
  return (rigidbody_t) {
    .velocity = vec2(0.0, 0.0)
  };
}

#endif
