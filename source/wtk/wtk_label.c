
// This is free and unencumbered software released into the public domain.  

// Anyone is free to copy, modify, publish, use, compile, sell, or           
// distribute this software, either in source code form or as a compiled    
// binary, for any purpose, commercial or non-commercial, and by any        
// means.                                                                   

// In jurisdictions that recognize copyright laws, the author or authors    
// of this software dedicate any and all copyright interest in the           
// software to the public domain. We make this dedication for the benefit    
// of the public at large and to the detriment of our heirs and              
// successors. We intend this dedication to be an overt act of               
// relinquishment in perpetuity of all present and future rights to this     
// software under copyright law.                                             

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,          
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF        
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.    
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR         
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,      
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR     
// OTHER DEALINGS IN THE SOFTWARE.                                           

// For more information, please refer to <http://unlicense.org/>             


#include <wtk/wtk_label.h>

#include "_wtk_windows.h"
#include "_wtk_controls.h"
#include "_wtk_msgs.h"
#include "_wtk_layout.h"

#include <wtk/wtk_mm.h>
#include <wtk/wtk_align.h>
#include <wtk/wtk_font.h>
#include <wtk/wtk_mouse.h>
#include <wtk/wtk_keyboard.h>

static LRESULT CALLBACK wtk_label_proc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

int WTK_API wtk_label_init()
{
    return TRUE;
}

struct wtk_label* WTK_API wtk_label_create( int x, int y, int width, int height, struct wtk_control* parent )
{
    struct wtk_label* label = NULL;
    HWND hWnd;

    WTK_ASSERT(parent);

    hWnd = CreateWindowExA(0, "STATIC", NULL, SS_LEFT | WS_VISIBLE | WS_CHILD, x, y, width, height, parent->hWnd, NULL, GetModuleHandle(0), 0);
    if( !hWnd ) return NULL;

    label = wtk_alloc(sizeof(struct wtk_label));
    memset((void*)label, 0, sizeof(struct wtk_label));
    label->control.type = WTK_CONTROL_TYPE(Label);
    label->control.hWnd = hWnd;
    label->control.font = wtk_font_default();
    label->text_align = WTK_ALIGN(Left);

    SetPropA(hWnd, "_wtk_old_proc", (HANDLE)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)&wtk_label_proc));
    SetPropA(hWnd, "_wtk_ctrl_ptr", (HANDLE)label);
    PostMessage(hWnd, WM_SETFONT, (WPARAM)label->control.font->hFont, TRUE);
    PostMessage(hWnd, WTK_ON_CREATE, 0, 0);
    return label;
}

static BOOL CALLBACK wtk_on_layout_change_proc( HWND hWnd, LPARAM lParam ) {
    SendMessage(hWnd, WTK_ON_LAYOUT_CHANGED, 0, 0);
    return TRUE;
}

static LRESULT CALLBACK wtk_label_proc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    struct wtk_control* control = (struct wtk_control*)GetPropA(hWnd, "_wtk_ctrl_ptr");
    struct wtk_label* label = (struct wtk_label*)control;
    if( !control ) return DefWindowProc(hWnd, uMsg, wParam, lParam);

    switch( uMsg ) {
        case WTK_ON_CREATE: {
            HWND hWndParent = GetParent(hWnd);
            if( control->on_create_callback ) control->on_create_callback(control, WTK_EVENT(OnCreate));
            if( hWndParent ) SendMessage(hWndParent, WTK_ON_LAYOUT_CHANGED, 0, 0);
        } break;

        case WTK_ON_LAYOUT_CHANGED: {
            wtk_on_layout_changed(control);
            EnumChildWindows(hWnd, &wtk_on_layout_change_proc, 0);
            if( control->on_layout_changed_callback ) control->on_layout_changed_callback(control, WTK_EVENT(OnLayoutChanged));
        } break;

        case WM_DESTROY: {
            if( control->on_destroy_callback ) control->on_destroy_callback(control, WTK_EVENT(OnDestroy));
            wtk_free(label);
        } break;

        case WM_LBUTTONDOWN: {
            if( control->on_pressed_callback ) control->on_pressed_callback(control, WTK_EVENT(OnPressed), WTK_MOUSE_BTN_LEFT, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            goto _default;
        } break;

        case WM_MBUTTONDOWN: {
            if( control->on_pressed_callback ) control->on_pressed_callback(control, WTK_EVENT(OnPressed), WTK_MOUSE_BTN_MIDDLE, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            goto _default;
        } break;

        case WM_RBUTTONDOWN: {
            if( control->on_pressed_callback ) control->on_pressed_callback(control, WTK_EVENT(OnPressed), WTK_MOUSE_BTN_RIGHT, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            goto _default;
        } break;

        case WM_LBUTTONUP: {
            if( control->on_released_callback ) control->on_released_callback(control, WTK_EVENT(OnReleased), WTK_MOUSE_BTN_LEFT, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            goto _default;
        } break;

        case WM_MBUTTONUP: {
            if( control->on_released_callback ) control->on_released_callback(control, WTK_EVENT(OnReleased), WTK_MOUSE_BTN_MIDDLE, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            goto _default;
        } break;

        case WM_RBUTTONUP: {
            if( control->on_released_callback ) control->on_released_callback(control, WTK_EVENT(OnReleased), WTK_MOUSE_BTN_RIGHT, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            goto _default;
        } break;

        default: {
        _default:
            return CallWindowProc((WNDPROC)GetPropA(hWnd, "_wtk_old_proc"), hWnd, uMsg, wParam, lParam);
        } break;
    }

    return 0;
}
