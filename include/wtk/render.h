/*===-- wtk/render.h ----------------------------------------*- mode: C -*-===*/
/*                                                                            */
/*                                     _   _                                  */
/*                               _ _ _| |_| |_                                */
/*                              | | | |  _| '_|                               */
/*                              |_____|_| |_,_|                               */
/*                                                                            */
/*       This file is distributed under the terms described in LICENSE.       */
/*                                                                            */
/*===----------------------------------------------------------------------===*/

#ifndef _WTK_RENDER_H_
#define _WTK_RENDER_H_

#include "wtk/foundation.h"

#include "wtk/window.h"
#include "wtk/canvas.h"

WTK_BEGIN_EXTERN_C

extern WTK_PRIVATE
  void wtk_renderer_init(void);

extern WTK_PRIVATE
  void wtk_renderer_shutdown(void);

extern WTK_PRIVATE
  void wtk_renderer_begin(void);

extern WTK_PRIVATE
  void wtk_renderer_end(void);

extern WTK_PRIVATE
  void wtk_renderer_mark_as_dirty(wtk_handle_t window,
                                  const wtk_rectangle_t *area);

WTK_END_EXTERN_C

#endif /* _WTK_RENDER_H_ */
