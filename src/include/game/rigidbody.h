#ifndef RIGIDBODY_H
#define RIGIDBODY_H

typedef struct {
  vector velocity;
} rigidbody_t;

inline static rigidbody_t create_rigidbody()
{
  return (rigidbody_t) {
    .velocity = vec3(0.0, 0.0, 0.0)
  };
}

#endif
