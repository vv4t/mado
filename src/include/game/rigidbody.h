#ifndef RIGIDBODY_H
#define RIGIDBODY_H

typedef struct {
  vector  velocity;
  vector  prev_pos;
  float   radius;
} rigidbody_t;

inline static rigidbody_t create_rigidbody()
{
  return (rigidbody_t) {
    .velocity = vec2(0.0, 0.0),
    .prev_pos = vec2(0.0, 0.0),
    .radius = 1.0
  };
}

#endif
