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

double_t get_time() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);

    return (double_t)t.tv_sec + (double_t)t.tv_nsec * 1e-9;
}

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

    //float_t dt = 1.0f / 60.0f;
    mat4x4_f proj = perspective(convert_to_radians(90.0f), 
                                back_buffer->width / back_buffer->height,
                                0.1f, 1000.0f);

    vec3_f camera = {0.0f, 0.0f, 0.0f};
    vec3_f look_dir = {0.0f, 0.0f, 1.0f};
    float_t f_yaw = 0.0f;
    float_t f_pitch = 0.0f;

    double_t prev = get_time();

    while (running) {

        double_t curr = get_time();
        double_t dt = curr - prev;
        prev = curr;
        printf("Time : %f\r", dt);

        clear_buffer(DARKGRAY, back_buffer);

        draw_cube(back_buffer, dt, (vec3_f){1.5f, 1.5f, 5.0f}, proj, camera, look_dir, f_yaw, f_pitch);
        draw_cube(back_buffer, dt, (vec3_f){-3.5f, -1.5f, 5.0f}, proj, camera, look_dir, f_yaw, f_pitch);

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

        case KeyPress: {
            
            if (XLookupKeysym(&event.xkey, 0) == XK_Up) {
                camera.y += 10.0f * dt;
            }
            if (XLookupKeysym(&event.xkey, 0) == XK_Down) {
                camera.y -= 10.0f * dt;
            }
            if (XLookupKeysym(&event.xkey, 0) == XK_Left) {
                camera.x -= 10.0f * dt;
            }
            if (XLookupKeysym(&event.xkey, 0) == XK_Right) {
                camera.x += 10.0f * dt;
            }
            if (XLookupKeysym(&event.xkey, 0) == XK_w) {
                vec3_f forward = mul_vector(look_dir, 5.0f * dt);
                camera = add_vectors(camera, forward);
            }
            if (XLookupKeysym(&event.xkey, 0) == XK_s) {
                vec3_f forward = mul_vector(look_dir, 5.0f * dt);
                camera = sub_vectors(camera, forward);
            }
            if (XLookupKeysym(&event.xkey, 0) == XK_a) {
                f_yaw += 3.0f * dt;
            }
            if (XLookupKeysym(&event.xkey, 0) == XK_d) {
                f_yaw -= 3.0f * dt;
            }
            if (XLookupKeysym(&event.xkey, 0) == XK_Shift_L) {
                f_pitch -= 3.0f * dt;
                if (f_pitch < -89.0f) {
                    f_pitch = -89.0f;
                }
            }
            if (XLookupKeysym(&event.xkey, 0) == XK_Control_L) {
                f_pitch += 3.0f * dt;
                if (f_pitch > 89.0f) {
                    f_pitch = 89.0f;
                }
            }
            break;
        }

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

