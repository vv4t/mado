#include <game/conditions.h>

bool cond_greater_distance(game_t *gs, entity_t e, entity_t arg1, float arg2) {
    const transform_t *st = entity_get_component(gs, e, transform);
    const transform_t *ot = entity_get_component(gs, arg1, transform);
    float pdist = length(vsubv(st->position, ot->position));
    return pdist >= arg2;
}

bool cond_lesser_distance(game_t *gs, entity_t e, entity_t arg1, float arg2) {
    const transform_t *st = entity_get_component(gs, e, transform);
    const transform_t *ot = entity_get_component(gs, arg1, transform);
    float pdist = length(vsubv(st->position, ot->position));
    return pdist < arg2;
}