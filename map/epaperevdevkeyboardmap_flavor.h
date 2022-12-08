// Mapping for ctrl, alt, home (bullet) and end (opt) keys are handled here.
// They map to different values depending on apple/windows flavor and they're independent of the keyboard language.

#include "qnamespace.h"
#ifdef Q_OS_FREEBSD
#include <dev/evdev/input.h>
#else
#include "linux/input.h"
#endif

namespace EpaperEvdevKeyboardMap {
namespace Flavor {

struct Windows {
constexpr static Mapping keymap[] = {
    // KEY_LEFTCTRL (29)
    { KEY_LEFTCTRL, 0xffff, Qt::Key_Control, 0x00, 0x04, 0x0004 },

    // KEY_LEFTALT (56)
    { KEY_LEFTALT, 0xffff, Qt::Key_Alt, 0x00, 0x04, 0x0008 },

    // KEY_RIGHTALT (100)
    { KEY_RIGHTALT, 0xffff, Qt::Key_AltGr, 0x00, 0x04, 0x0002 },

    // TODO(Cem):
    // Home (bullet/rM) key is dropped after PVT. This line is now redundant,
    // but can be kept until we're sure there are no EVT/DVT birds lying around.
    // KEY_HOME (102)
    { KEY_HOME, 0xffff, Qt::Key_Home, 0x00, 0x00, 0x0000 },

    // KEY_END (107)
    { KEY_END, 0xffff, Qt::Key_End, 0x00, 0x00, 0x0000 },
};

constexpr static size_t keymapSize = sizeof(keymap) / sizeof(keymap[0]);
};

struct Apple {
constexpr static Mapping keymap[] = {
    // KEY_LEFTCTRL (29)
    { KEY_LEFTCTRL, 0xffff, Qt::Key_End, 0x00, 0x00, 0x0000 },

    // KEY_LEFTALT (56)
    { KEY_LEFTALT, 0xffff, Qt::Key_Control, 0x00, 0x04, 0x0004 },

    // KEY_RIGHTALT (100)
    { KEY_RIGHTALT, 0xffff, Qt::Key_AltGr, 0x00, 0x04, 0x0002 },

    // TODO(Cem):
    // Home (bullet/rM) key is dropped after PVT. This line is now redundant,
    // but can be kept until we're sure there are no EVT/DVT birds lying around.
    // KEY_HOME (102)
    { KEY_HOME, 0xffff, Qt::Key_AltGr, 0x00, 0x04, 0x0002 },

    // KEY_END (107)
    { KEY_END, 0xffff, Qt::Key_Alt, 0x00, 0x04, 0x0008 },
};

constexpr static size_t keymapSize = sizeof(keymap) / sizeof(keymap[0]);
};

} // namespace Flavor
} // namespace EpaperEvdevKeyboardMap