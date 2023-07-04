#include "sprite.h"

sprite_t *sprite_alloc(sprite_t sprites[MAX_SPRITES]) {
    for (int i = 0; i < MAX_SPRITES; i ++) {
        if (!sprites[i].used) {
            sprites[i].used = true;
            return &sprites[i];
        }
    }
    return NULL;
}