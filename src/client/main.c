#include <client/client.h>
#include <client/scene.h>

int main(int argc, char *argv[])
{
  cl_init();
  cl_scene_load(&s_scene1);
  while (cl_update());
  cl_deinit();
  return 0;
}
