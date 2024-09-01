#include <lib/map.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <lib/log.h>
#include <string.h>

struct landmark_s {
  char name[32];
  vector position;
  landmark_t next;
};

typedef struct {
  char name[32];
  landmark_t landmarks;
  int num_landmarks;
} group_t;

struct map_s {
  tile_t *data;
  int width;
  int height;
  group_t *groups;
  int num_groups;
};

struct { int name; tile_t tile; } tiledata[] = {
  {
    .name = 0,
    .tile = { .tx = 0, .ty = 0, .solid = 1, .num_block = 0 }
  },
  {
    .name = 130,
    .tile = {
      .tx = 1, .ty = 7, .solid = 1, .num_block = 2,
      .block = { { .tx = 1, .ty = 7 }, { .tx = 1, .ty = 7 } }
    }
  },
  {
    .name = 131,
    .tile = {
      .tx = 1, .ty = 7, .solid = 1, .num_block = 2,
      .block = { { .tx = 1, .ty = 7 },  { .tx = 2, .ty = 7 } }
    }
  },
  {
    .name = 146,
    .tile = {
      .tx = 0, .ty = 6, .solid = 1, .num_block = 2,
      .block = { { .tx = 1, .ty = 6 }, { .tx = 1, .ty = 6 } }
    }
  },
  {
    .name = 148,
    .tile = {
      .tx = 3, .ty = 6, .solid = 1, .num_block = 2,
      .block = { { .tx = 3, .ty = 6 }, { .tx = 3, .ty = 6 } }
    }
  },
  {
    .name = 161,
    .tile = {
      .tx = 3, .ty = 6, .solid = 1, .num_block = 2,
      .block = { { .tx = 3, .ty = 6 }, { .tx = 0, .ty = 5 } }
    }
  }
};

static tile_t tile_get(int name);
static group_t group_load(FILE *fp, const char *path);

map_t map_load(const char *path)
{
  FILE *fp = fopen(path, "r");
  if (!fp) {
    perror(path);
    exit(1);
  }

  map_t m = malloc(sizeof(struct map_s));
  
  if (fscanf(fp, "%i", &m->num_groups) < 0) {
    LOG_ERROR("%s: malformed num groups", path);
  }
  
  m->groups = calloc(m->num_groups, sizeof(group_t));
  for (int i = 0; i < m->num_groups; i++) {
    m->groups[i] = group_load(fp, path);
  }
  
  if (fscanf(fp, "%i %i", &m->width, &m->height) < 0) {
    LOG_ERROR("%s: malformed width height", path);
  }

  m->data = calloc(m->width * m->height, sizeof(tile_t));
  for (int i = 0; i < m->height; i++) {
    for (int j = 0; j < m->width; j++) {
      int name;
      if (fscanf(fp, "%x", &name) < 0) {
        LOG_ERROR("%s: malformed data", path);
      }
      
      m->data[i * m->width + j] = tile_get(name);
    }
  }

  return m;
}

group_t group_load(FILE *fp, const char *path)
{
  group_t group;
  
  if (fscanf(fp, "%32s %i", group.name, &group.num_landmarks) < 0) {
    LOG_ERROR("%s: malformed group data", path);
  }
  
  group.landmarks = calloc(group.num_landmarks, sizeof(struct landmark_s));

  for (int i = 0; i < group.num_landmarks; i++) {
    float x, y;
    if (fscanf(fp, "%32s %f %f", group.landmarks[i].name, &x, &y) < 0) {
      LOG_ERROR("%s: malformed landmark data", path);
    }
    
    group.landmarks[i].position.x = x;
    group.landmarks[i].position.y = y;
    group.landmarks[i].next = NULL;
    
    if (i > 0) {
      group.landmarks[i - 1].next = &group.landmarks[i];
    }
  }
  
  return group;
}

int map_get_width(map_t m)
{
  return m->width;
}

int map_get_height(map_t m)
{
  return m->height;
}

tile_t map_get(map_t m, int x, int y)
{
  if (x < 0 || y < 0 || x >= m->width || y >= m->height) {
    return (tile_t) {
      .tx = 0, .ty = 0, .solid = 1, .num_block = 0
    };
  }
  
  return m->data[x + y * m->width];
}

int map_collide(map_t m, vector p, vector d)
{
  int x0 = (int) floor(p.x - d.x);
  int y0 = (int) floor(p.y - d.y);
  int x1 = (int) floor(p.x + d.x);
  int y1 = (int) floor(p.y + d.y);
  
  if (map_get(m, x0, y0).solid > 0) return 1;
  if (map_get(m, x1, y0).solid > 0) return 1;
  if (map_get(m, x0, y1).solid > 0) return 1;
  if (map_get(m, x1, y1).solid > 0) return 1;
  
  return 0;
}

landmark_t map_get_group(map_t m, const char *name)
{
  for (int i = 0; i < m->num_groups; i++) {
    if (strcmp(name, m->groups[i].name) == 0) {
      return m->groups[i].landmarks;
    }
  }
  
  return NULL;
}

void map_destroy(map_t m)
{
  for (int i = 0; i < m->num_groups; i++) {
    free(m->groups[i].landmarks);
  }
  
  free(m->data);
  free(m->groups);
  free(m);
}

const char *landmark_get_name(landmark_t lm)
{
  return lm->name;
}

vector landmark_get_position(landmark_t lm)
{
  return lm->position;
}

landmark_t landmark_next(landmark_t lm)
{
  return lm->next;
}

tile_t tile_get(int name)
{
  for (int i = 0; i < sizeof(tiledata) / sizeof(tiledata[0]); i++) {
    if (tiledata[i].name == name) {
      return tiledata[i].tile;
    }
  }
  
  return (tile_t) {
    .tx = (name - 1) % 16,
    .ty = 15 - (name - 1) / 16,
    .solid = false,
    .num_block = 0
  };
}
