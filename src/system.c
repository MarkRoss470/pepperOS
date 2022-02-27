#include "system.h"
#include "screen.h"
#include "graphics.h"
#include "timer.h"

#define NOTIFICATION_DURATION_TICKS (TIMER_TPS * 2)

static struct {
    char content[512];
    u64 ticks;
} notification = {
    "", -1
};

static u32 rseed = 1;

void seed(u32 s) {
    rseed = s;
}

u32 rand() {
    static u32 x = 123456789;
    static u32 y = 362436069;
    static u32 z = 521288629;
    static u32 w = 88675123;

    x *= 23786259 - rseed;

    u32 t;

    t = x ^ (x << 11);
    x = y; y = z; z = w;
    return w = w ^ (w >> 19) ^ t ^ (t >> 8);
}

void panic(const char *err) {
    screen_clear(0xff0000);

    if (err != NULL) {
        draw_text(err, 0, 0, 0xffffff, 0xff0000);
    }

    screen_swap();
    for (;;) {}
}

const char *get_notification() {
    return timer_get() - notification.ticks <= NOTIFICATION_DURATION_TICKS ?
        ((const char *) &notification.content) : NULL;
}

void notify(const char *message) {
    memcpy(
        &notification.content,
        message,
        MIN(strlen(message) + 1, sizeof(notification.content))
    );

    notification.content[sizeof(notification.content) - 1] = 0;
    notification.ticks = timer_get();
}
