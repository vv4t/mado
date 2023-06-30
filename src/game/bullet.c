#include "bullet.h"

bullet_t *bullet_new(bullet_t bullets[MAX_BULLETS]) {
    for (int i = 0; i < MAX_BULLETS; i ++) {
        if (bullets[i].used != true) {
            bullets[i].used = true;
            return &bullets[i];
        }
    }
    return NULL;
}