#include "gl.h"

#include "../common/log.h"

bool gl_init()
{
#ifdef __EMSCRIPTEN__
  return true;
#else
  glewExperimental = true;
  
  GLenum status = glewInit();
  if (status != GLEW_OK)
    LOG_ERROR("failed to initialize GLEW: %s", glewGetErrorString(status));
  
  return status == GLEW_OK;
#endif
}
