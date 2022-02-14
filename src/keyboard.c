#include "keyboard.h"
#include "irq.h"
#include "system.h"
#include "timer.h"
#include "graphics.h"
#include "screen.h"

u8 keyboard_layout_us[2][128] = {
    {
        KEY_NULL, KEY_ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
        '-', '=', KEY_BACKSPACE, KEY_TAB, 'q', 'w', 'e', 'r', 't', 'y', 'u',
        'i', 'o', 'p', '[', ']', KEY_ENTER, 0, 'a', 's', 'd', 'f', 'g', 'h', 'j',
        'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm',
        ',', '.', '/', 0, 0, 0, ' ', 0, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5,
        KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, 0, 0, KEY_HOME, KEY_UP,
        KEY_PAGE_UP, '-', KEY_LEFT, '5', KEY_RIGHT, '+', KEY_END, KEY_DOWN,
        KEY_PAGE_DOWN, KEY_INSERT, KEY_DELETE, 0, 0, 0, KEY_F11, KEY_F12
    }, {
        KEY_NULL, KEY_ESC, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')',
        '_', '+', KEY_BACKSPACE, KEY_TAB, 'Q', 'W',   'E', 'R', 'T', 'Y', 'U',
        'I', 'O', 'P',   '{', '}', KEY_ENTER, 0, 'A', 'S', 'D', 'F', 'G', 'H',
        'J', 'K', 'L', ':', '\"', '~', 0, '|', 'Z', 'X', 'C', 'V', 'B', 'N',
        'M', '<', '>', '?', 0, 0, 0, ' ', 0, KEY_F1, KEY_F2, KEY_F3, KEY_F4,
        KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, 0, 0, KEY_HOME, KEY_UP,
        KEY_PAGE_UP, '-', KEY_LEFT, '5', KEY_RIGHT, '+', KEY_END, KEY_DOWN,
        KEY_PAGE_DOWN, KEY_INSERT, KEY_DELETE, 0, 0, 0, KEY_F11, KEY_F12
    }
};

struct Keyboard keyboard;
struct Keybuffer keybuffer;

// bad hack! for a better RNG
static bool seeded = false;

inline char char_from_scancode(u16 scancode)
{
	return(keyboard_layout_us[!!(keyboard.mods & KEY_MOD_SHIFT)][scancode & 0x7f]);
}

static void keyboard_handler(struct Registers *regs) {

    u16 scancode = (u16) inb(0x60);

    if (!seeded) {
        seed(((u32) scancode) * 17 + timer_get());
        seeded = true;
    }

    if (KEY_SCANCODE(scancode) == KEY_LALT ||
        KEY_SCANCODE(scancode) == KEY_RALT) {
        keyboard.mods = BIT_SET(keyboard.mods, HIBIT(KEY_MOD_ALT), KEY_IS_PRESS(scancode));
        return;
    } else if (
        KEY_SCANCODE(scancode) == KEY_LCTRL ||
        KEY_SCANCODE(scancode) == KEY_RCTRL) {
        keyboard.mods = BIT_SET(keyboard.mods, HIBIT(KEY_MOD_CTRL), KEY_IS_PRESS(scancode));
        return;
    } else if (
        KEY_SCANCODE(scancode) == KEY_LSHIFT ||
        KEY_SCANCODE(scancode) == KEY_RSHIFT) {
        keyboard.mods = BIT_SET(keyboard.mods, HIBIT(KEY_MOD_SHIFT), KEY_IS_PRESS(scancode));
        return;
    } else if (KEY_SCANCODE(scancode) == KEY_CAPS_LOCK) {
        keyboard.mods = BIT_SET(keyboard.mods, HIBIT(KEY_MOD_CAPS_LOCK), KEY_IS_PRESS(scancode));
        return;
    } else if (KEY_SCANCODE(scancode) == KEY_NUM_LOCK) {
        keyboard.mods = BIT_SET(keyboard.mods, HIBIT(KEY_MOD_NUM_LOCK), KEY_IS_PRESS(scancode));
        return;
    } else if (KEY_SCANCODE(scancode) == KEY_SCROLL_LOCK) {
        keyboard.mods = BIT_SET(keyboard.mods, HIBIT(KEY_MOD_SCROLL_LOCK), KEY_IS_PRESS(scancode));
        return;
    }

    keyboard.keys[(u8) (scancode & 0x7F)] = KEY_IS_PRESS(scancode);
    keyboard.chars[KEY_CHAR(scancode)] = KEY_IS_PRESS(scancode);
    
    if(KEY_IS_PRESS(scancode)){
    	//left arrow
    	if((scancode & 0x7f) == 0x4b)
    	{
    		keybuffer.buffer[keybuffer.end] = 0x00;
    		keybuffer.buffer[keybuffer.end + 1] = 0x01;
    		keybuffer.end += 2;
    	}
    	//right arrow
    	else if((scancode & 0x7f) == 0x4d)
    	{
    		keybuffer.buffer[keybuffer.end] = 0x00;
    		keybuffer.buffer[keybuffer.end + 1] = 0x02;
    		keybuffer.end += 2;
    	}
    	//up arrow
    	else if((scancode & 0x7f) == 0x48)
    	{
    		keybuffer.buffer[keybuffer.end] = 0x00;
    		keybuffer.buffer[keybuffer.end + 1] = 0x03;
    		keybuffer.end += 2;
    	}
    	//down arrow
    	else if((scancode & 0x7f) == 0x50)
    	{
    		keybuffer.buffer[keybuffer.end] = 0x00;
    		keybuffer.buffer[keybuffer.end + 1] = 0x04;
    		keybuffer.end += 2;
    	}
    	//escape
    	else if((scancode & 0x7f) == 0x01)
    	{
    		keybuffer.buffer[keybuffer.end] = 0x00;
    		keybuffer.buffer[keybuffer.end + 1] = 0x05;
    		keybuffer.end += 2;
    	}
    	//backslash
    	else if((scancode & 0x7f) == 0x56)
    	{
    		keybuffer.buffer[keybuffer.end] = '\\';
    		keybuffer.end += 1;
    	}
    	else
    	{
    		keybuffer.buffer[keybuffer.end] = char_from_scancode(scancode);
    		keybuffer.end += 1;
    	}
    }
 
}

void keyboard_init() {
    irq_install(1, keyboard_handler);
    keybuffer.start = 0;
    keybuffer.end = 0;
}
