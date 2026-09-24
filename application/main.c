#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "draw.h"
#include "colors.h"
#include "screen_buffer.h"

int main(void)
{

    screen_buffer* back_buffer = init_screen_buffer(800, 800);
    Display *display = XOpenDisplay(NULL);

    if (display == NULL) {
        fprintf(stderr, "Could not open X display\n");
        return 1;
    }

    int screen = DefaultScreen(display);

    Window window = XCreateSimpleWindow(
        display,
        RootWindow(display, screen),
        100, 100,
        back_buffer->width, 
        back_buffer->height,
        0,
        BlackPixel(display, screen),
        BlackPixel(display, screen)
    );

    XStoreName(display, window, "X11 Framebuffer");

    XSelectInput(
        display,
        window,
        ExposureMask |
        KeyPressMask |
        StructureNotifyMask
    );

    XMapWindow(display, window);

    XImage *image = XCreateImage(
        display,
        DefaultVisual(display, screen),
        DefaultDepth(display, screen),
        ZPixmap,
        0,
        (char *)back_buffer->mem,
        back_buffer->width,
        back_buffer->height,
        32,
        back_buffer->width * sizeof(uint8_t) * 4
    );

    if (image == NULL) {
        fprintf(stderr, "XCreateImage failed\n");

        XDestroyWindow(display, window);
        XCloseDisplay(display);

        return 1;
    }

    GC gc = XCreateGC(display, window, 0, NULL);

    int running = 1;
    XEvent event;

    float_t dt = 1.0f / 60.0f;

    while (running) {

        clear_buffer(BLACK, back_buffer);

        draw_cube(back_buffer, dt);

        XPutImage(
            display,
            window,
            gc,
            image,
            0, 0,
            0, 0,
            back_buffer->width,
            back_buffer->height
        );

        if (!XPending(display)) {
            continue;
        }

        XNextEvent(display, &event);

        switch (event.type) {

        case Expose:

            XFlush(display);

            break;

        case KeyPress:
            
            running = 0;
            break;

        case DestroyNotify:

            running = 0;
            break;
        }
    }

    XFreeGC(display, gc);

    image->data = NULL;
    XDestroyImage(image);

    XDestroyWindow(display, window);

    XCloseDisplay(display);

    free_screen_buffer(back_buffer);

    return 0;
}

