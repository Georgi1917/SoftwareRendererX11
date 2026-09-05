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

void clear_screen(screen_buffer* buffer, pixel_data p_data);

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

    // point_d p0 = {0.0, 0.5};
    // point_d p1 = {-0.5, -0.5};
    // point_d p2 = {0.5, -0.5};

    // point3_d p0d = {-0.5, 0.5, 1.0};
    // point3_d p1d = {-1.0, -0.5, 1.0};
    // point3_d p2d = {0.0, -0.5, 1.0};

    // point3_d p0b = {-0.5, 0.5, 1.25};
    // point3_d p1b = {-1.0, -0.5, 1.25};
    // point3_d p2b = {0.0, -0.5, 1.25};

    float_t dt = 1.0f / 60.0f;

    while (running) {

        clear_screen(back_buffer, LIGHTGRAY);

        // draw_triangle_fill(p0, p1, p2, RED, back_buffer);
        // draw_triangle_transform(p0, p1, p2, BLUE, back_buffer);
        // draw_triangle_transform3(p0b, p1b, p2b, RED, back_buffer);
        // draw_triangle_transform3(p0d, p1d, p2d, BLUE, back_buffer);

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

void clear_screen(screen_buffer* buffer, pixel_data p_data) {

    for (uint16_t y = 0; y < buffer->height; y++) {
        for (uint16_t x = 0; x < buffer->width; x++) {
            put_pixel(x, y, buffer, p_data);
        }
    }
}
