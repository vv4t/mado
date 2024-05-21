#include <lib/map.h>
#include <stdio.h>
#include <stdlib.h>
#include <lib/log.h>

struct map_s {
  int *data;
  int width;
  int height;
};

map_t map_load(const char *path)
{
  FILE *fp = fopen(path, "r");
  if (!fp) {
    perror(path);
    exit(1);
  }
  
  map_t m = malloc(sizeof(*m));
  
  if (fscanf(fp, "%i %i", &m->width, &m->height) < 0) {
    LOG_ERROR("%s: bad header", path);
  }
  
  m->data = calloc(m->width * m->height, sizeof(int));
  
  for (int i = 0; i < m->height; i++) {
    for (int j = 0; j < m->width; j++) {
      if (fscanf(fp, "%i", &m->data[i * m->width + j]) < 0) {
        LOG_ERROR("%s: bad data", path);
      }
    }
  }
  
  return m;
}

int map_get_width(map_t m)
{
  return m->width;
}

int map_get_height(map_t m)
{
  return m->height;
}

int map_get(map_t m, int x, int y)
{
  if (x < 0 || y < 0 || x >= m->width || y >= m->height) {
    return 0;
  }
  
  return m->data[x + y * m->width];
}

int map_collide(map_t m, vector p, vector d)
{
  int x0 = (int) (p.x - d.x);
  int y0 = (int) (p.y - d.y);
  int x1 = (int) (p.x + d.x);
  int y1 = (int) (p.y + d.y);
  
  if (map_get(m, x0, y0) > 0) return 1;
  if (map_get(m, x1, y0) > 0) return 1;
  if (map_get(m, x0, y1) > 0) return 1;
  if (map_get(m, x1, y1) > 0) return 1;
  
  return 0;
}

void map_destroy(map_t m)
{
  free(m->data);
  free(m);
}
