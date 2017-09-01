/*===-- wtk/render.c ----------------------------------------*- mode: C -*-===*/
/*                                                                            */
/*                                     _   _                                  */
/*                               _ _ _| |_| |_                                */
/*                              | | | |  _| '_|                               */
/*                              |_____|_| |_,_|                               */
/*                                                                            */
/*       This file is distributed under the terms described in LICENSE.       */
/*                                                                            */
/*===----------------------------------------------------------------------===*/

#include "wtk/app.h"

#include "wtk/window.h"
#include "wtk/render.h"

#if WTK_PLATFORM == WTK_PLATFORM_WINDOWS
  #include <windows.h>
#elif WTK_PLATFORM == WTK_PLATFORM_MAC
#elif WTK_PLATFORM == WTK_PLATFORM_LINUX
#endif

/* TODO(mtwilliams): Handle overlap by delaying hit tests by one frame. */

/*
  Frames
   Retained mode quad-tree of `frames`.
    Calls to user specified function to perform presentation.
   Maps `n` frames to `m` windows.
    Seamless tabs and docking!
  Widgets
   Immediate mode.
  Canvas
   Immediate mode.
    Dirty tracking to reduce redraw.
*/

WTK_BEGIN_EXTERN_C

typedef struct wtk_app {
#if 0
  wtk_frame_t *frames;
#endif
} wtk_app_t;

static wtk_app_t *app = NULL;

void wtk_app_init(void)
{
  app = (wtk_app_t *)wtk_allocate_s(sizeof(wtk_app_t), 16);

  wtk_renderer_init();
}

void wtk_app_shutdown(void)
{
  wtk_renderer_shutdown();

  wtk_free((void *)app);

  app = NULL;
}

void wtk_app_draw(void)
{
  /* TODO(mtwilliams): Iterate over frames, calling presentation callback. */

  /* HACK(mtwilliams): Force redraw. */
#if 0
  for (window in windows)
    InvalidateRect(window->handle, NULL, TRUE);
#endif
}

void wtk_app_pump(void)
{
  wtk_renderer_begin();

#if WTK_PLATFORM == WTK_PLATFORM_WINDOWS
  MSG msg;
  while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
    ::TranslateMessage(&msg);
    ::DispatchMessage(&msg);
  }
#elif WTK_PLATFORM == WTK_PLATFORM_MAC
#elif WTK_PLATFORM == WTK_PLATFORM_LINUX
#endif

  wtk_app_draw();

  wtk_renderer_end();
}

WTK_END_EXTERN_C
