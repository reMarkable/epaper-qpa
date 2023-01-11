/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#pragma once

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qnamespace.h"
#ifdef Q_OS_FREEBSD
#include <dev/evdev/input.h>
#else
#include "linux/input.h"
#endif

// no QT_BEGIN_NAMESPACE, since we include it internally...

// Reference for Unicode names and values: https://unicode-table.com/en/
// Reference for QT key enums: https://doc.qt.io/qt-5/qt.html#Key-enum

namespace EpaperEvdevKeyboardMap {
namespace Locale {

struct Germany {
constexpr static EpaperEvdevKeyboardMap::Mapping keymap[] = {
    { 1, 0xffff, 0x01000000, 0x00, 0x00, 0x0000 },

    // KEY_1 (2)
    // "1" Digit One 0x0031
    // "!" Exclamation Mark 0x0021
    // "^" Combining Circumflex Accent 0x0302
    { KEY_1, 0x0031, Qt::Key_1, 0x00, 0x00, 0x0000 },
    { KEY_1, 0x0021, Qt::Key_Exclam, Modifiers::ModShift, 0x00, 0x0000 },
    { KEY_1, 0x0302, Qt::Key_Dead_Circumflex, Modifiers::ModAlt, Flags::IsDead, 0x0000 },
    { KEY_1, 0x0302, Qt::Key_Dead_Circumflex, Modifiers::ModAltGr, Flags::IsDead, 0x0000 },

    // KEY_2 (3)
    // "2" Digit Two 0x0032
    // """ Quotation Mark 0x0022
    { KEY_2, 0x0032, Qt::Key_2, 0x00, Flags::IsLetter, 0x0000 },
    { KEY_2, 0x0022, Qt::Key_QuoteDbl, Modifiers::ModShift, 0x00, 0x0000 },

    // KEY_3 (4)
    // "3" Digit Three 0x0033
    // "#" Number Sign 0x0023
    { KEY_3, 0x0033, Qt::Key_3, 0x00, 0x00, 0x0000 },
    { KEY_3, 0x0023, Qt::Key_NumberSign, Modifiers::ModShift, 0x00, 0x0000 },

    // KEY_4 (5)
    // "4" Digit Four 0x0034
    // "$" Dollar Sign 0x0024
    { KEY_4, 0x0034, Qt::Key_4, 0x00, 0x00, 0x0000 },
    { KEY_4, 0x0024, Qt::Key_Dollar, Modifiers::ModShift, 0x00, 0x0000 },

    // KEY_5 (6)
    // "5" Digit Five 0x0035
    // "%" Percent Sign 0x0025
    { KEY_5, 0x0035, Qt::Key_5, 0x00, 0x00, 0x0000 },
    { KEY_5, 0x0025, Qt::Key_Percent, Modifiers::ModShift, 0x00, 0x0000 },

    // KEY_6 (7)
    // "6" Digit Six 0x0036
    // "&" Ampersand 0x0026
    { KEY_6, 0x0036, Qt::Key_6, 0x00, 0x00, 0x0000 },
    { KEY_6, 0x0026, Qt::Key_Ampersand, Modifiers::ModShift, 0x00, 0x0000 },

    // KEY_7 (8)
    // "7" Digit Seven 0x0037
    // "/" Solidus 0x002f
    { KEY_7, 0x0037, Qt::Key_7, 0x00, 0x00, 0x0000 },
    { KEY_7, 0x002f, Qt::Key_Slash, Modifiers::ModShift, 0x00, 0x0000 },

    // KEY_8 (9)
    // "8" Digit Eight 0x0038
    // "(" Left Parenthesis 0x0028
    { KEY_8, 0x0038, Qt::Key_8, 0x00, 0x00, 0x0000 },
    { KEY_8, 0x0028, Qt::Key_ParenLeft, Modifiers::ModShift, 0x00, 0x0000 },

    // KEY_9 (10)
    // "9" Digit Nine 0x0039
    // ")" Right Parenthesis 0x0029
    { KEY_9, 0x0039, Qt::Key_9, 0x00, 0x02, 0x0000 },
    { KEY_9, 0x0029, Qt::Key_ParenRight, Modifiers::ModShift, 0x00, 0x0000 },

    // KEY_0 (11)
    // "0" Digit Zero 0x0030
    // "=" Equals Sign 0x003d
    // "`" Grave Accent 0x0060
    { KEY_0, 0x0030, Qt::Key_0, 0x00, 0x00, 0x0000 },
    { KEY_0, 0x003d, Qt::Key_Equal, Modifiers::ModShift, 0x00, 0x0000 },
    { KEY_0, 0x0060, Qt::Key_Dead_Grave, Modifiers::ModAlt, Flags::IsDead, 0x0000 },
    { KEY_0, 0x0060, Qt::Key_Dead_Grave, Modifiers::ModAltGr, Flags::IsDead, 0x0000 },
    { KEY_0, 0x0060, Qt::Key_Dead_Grave, Modifiers::ModShift | Modifiers::ModAlt, Flags::IsDead, 0x0000 },
    { KEY_0, 0x0060, Qt::Key_Dead_Grave, Modifiers::ModShift | Modifiers::ModAltGr, Flags::IsDead, 0x0000 },

    // Keycode 12 non-existent on Seabird.
    { 12, 0x002D, 0x0000002D, 0x00, 0x00, 0x0000 },
    { 12, 0x005F, 0x0000005F, 0x01, 0x00, 0x0000 },
    { 12, 0x005C, 0x0000005C, 0x02, 0x00, 0x0000 },
    { 12, 0x005F, 0x0400005F, 0x04, 0x00, 0x0000 },
    { 12, 0x005F, 0x0400005F, 0x05, 0x00, 0x0000 },

    // KEY_EQUAL (13)
    // "ß" Latin Small Letter Sharp S 0x00df
    // "?" Question Mark 0x003f
    // "´" Acute Accent 0x00b4
    { KEY_EQUAL, 0x00df, Qt::Key_ssharp, 0x00, 0x00, 0x0000 },
    { KEY_EQUAL, 0x003f, Qt::Key_Question, Modifiers::ModShift, 0x00, 0x0000 },
    { KEY_EQUAL, 0x00b4, Qt::Key_Dead_Acute, Modifiers::ModAlt, Flags::IsDead, 0x0000 },
    { KEY_EQUAL, 0x00b4, Qt::Key_Dead_Acute, Modifiers::ModAltGr, Flags::IsDead, 0x0000 },
    { KEY_EQUAL, 0x00b4, Qt::Key_Dead_Acute, Modifiers::ModShift | Modifiers::ModAlt, Flags::IsDead, 0x0000 },
    { KEY_EQUAL, 0x00b4, Qt::Key_Dead_Acute, Modifiers::ModShift | Modifiers::ModAltGr, Flags::IsDead, 0x0000 },

    // KEY_BACKSPACE (14)
    { KEY_BACKSPACE, 0xffff, Qt::Key_Backspace, 0x00, 0x00, 0x0000 },
    { KEY_BACKSPACE, 0xffff, Qt::Key_Delete, Modifiers::ModShift, 0x00, 0x0000 },

    // KEY_TAB (15)
    { KEY_TAB, 0xffff, 0x01000001, 0x00, 0x00, 0x0000 },

    // FIXME(modifiers)
    // KEY_Q (16)
    // "@" Commercial At 0x0040
    { KEY_Q, 0x0071, 0x00000051, 0x00, 0x02, 0x0000 },
    { KEY_Q, 0x0051, 0x00000051, 0x01, 0x02, 0x0000 },
    { KEY_Q, 0x0040, Qt::Key_At, 0x02, 0x02, 0x0000 },
    { KEY_Q, 0x0040, Qt::Key_At, 0x03, 0x02, 0x0000 },
    { KEY_Q, 0x0040, Qt::Key_At, 0x04, 0x02, 0x0000 },
    { KEY_Q, 0x0040, Qt::Key_At, 0x05, 0x02, 0x0000 },
    { KEY_Q, 0x0040, Qt::Key_At, 0x06, 0x02, 0x0000 },
    { KEY_Q, 0x0040, Qt::Key_At, 0x07, 0x02, 0x0000 },
    { KEY_Q, 0x0040, Qt::Key_At, 0x08, 0x02, 0x0000 },
    { KEY_Q, 0x0040, Qt::Key_At, 0x09, 0x02, 0x0000 },
    { KEY_Q, 0x0040, Qt::Key_At, 0x0a, 0x02, 0x0000 },
    { KEY_Q, 0x0040, Qt::Key_At, 0x0b, 0x02, 0x0000 },
    { KEY_Q, 0x0040, Qt::Key_At, 0x0c, 0x02, 0x0000 },
    { KEY_Q, 0x0040, Qt::Key_At, 0x0d, 0x02, 0x0000 },
    { KEY_Q, 0x0040, Qt::Key_At, 0x0e, 0x02, 0x0000 },
    { KEY_Q, 0x0040, Qt::Key_At, 0x0f, 0x02, 0x0000 },

    // KEY_W (17)
    { KEY_W, 0x0077, 0x00000057, 0x00, 0x02, 0x0000 },
    { KEY_W, 0x0057, 0x00000057, 0x01, 0x02, 0x0000 },

    // FIXME(modifiers)
    // KEY_E (18)
    // "€" Euro Sign 0x20ac
    { KEY_E, 0x0065, 0x00000045, 0x00, 0x02, 0x0000 },
    { KEY_E, 0x0045, 0x00000045, 0x01, 0x02, 0x0000 },
    { KEY_E, 0x20ac, 0x00000045, 0x02, 0x02, 0x0000 },
    { KEY_E, 0x20ac, 0x00000045, 0x03, 0x02, 0x0000 },
    { KEY_E, 0x20ac, 0x04000045, 0x04, 0x02, 0x0000 },
    { KEY_E, 0x20ac, 0x04000045, 0x05, 0x02, 0x0000 },
    { KEY_E, 0x20ac, 0x04000045, 0x06, 0x02, 0x0000 },
    { KEY_E, 0x20ac, 0x04000045, 0x07, 0x02, 0x0000 },
    { KEY_E, 0x20ac, 0x08000045, 0x08, 0x02, 0x0000 },
    { KEY_E, 0x20ac, 0x08000045, 0x09, 0x02, 0x0000 },
    { KEY_E, 0x20ac, 0x08000045, 0x0a, 0x02, 0x0000 },
    { KEY_E, 0x20ac, 0x08000045, 0x0b, 0x02, 0x0000 },
    { KEY_E, 0x20ac, 0x0c000045, 0x0c, 0x02, 0x0000 },
    { KEY_E, 0x20ac, 0x0c000045, 0x0d, 0x02, 0x0000 },
    { KEY_E, 0x20ac, 0x0c000045, 0x0e, 0x02, 0x0000 },
    { KEY_E, 0x20ac, 0x0c000045, 0x0f, 0x02, 0x0000 },

    // KEY_R (19)
    { KEY_R, 0x0072, 0x00000052, 0x00, 0x02, 0x0000 },
    { KEY_R, 0x0052, 0x00000052, 0x01, 0x02, 0x0000 },

    // KEY_T (20)
    { KEY_T, 0x0074, 0x00000054, 0x00, 0x02, 0x0000 },
    { KEY_T, 0x0054, 0x00000054, 0x01, 0x02, 0x0000 },

    // KEY_Y (21)
    // "z" Latin Small Letter Z 0x007a
    // "Z" Latin Capital Letter Z 0x005a
    { KEY_Y, 0x007a, 0x0000005a, 0x00, 0x02, 0x0000 },
    { KEY_Y, 0x005a, 0x0000005a, 0x01, 0x02, 0x0000 },

    // KEY_U (22)
    { KEY_U, 0x0075, 0x00000055, 0x00, 0x02, 0x0000 },
    { KEY_U, 0x0055, 0x00000055, 0x01, 0x02, 0x0000 },

    // KEY_I (23)
    { KEY_I, 0x0069, 0x00000049, 0x00, 0x02, 0x0000 },
    { KEY_I, 0x0049, 0x00000049, 0x01, 0x02, 0x0000 },

    // KEY_O (24)
    { KEY_O, 0x006f, 0x0000004f, 0x00, 0x02, 0x0000 },
    { KEY_O, 0x004f, 0x0000004f, 0x01, 0x02, 0x0000 },

    // FIXME(modifiers)
    // KEY_P (25)
    // "*" Asterisk 0x002a
    { KEY_P, 0x0070, 0x00000050, 0x00, 0x02, 0x0000 },
    { KEY_P, 0x0050, 0x00000050, Modifiers::ModShift, 0x02, 0x0000 },
    { KEY_P, 0x002a, Qt::Key_Asterisk, Modifiers::ModAlt, 0x00, 0x0000 },
    { KEY_P, 0x002a, Qt::Key_Asterisk, Modifiers::ModAltGr, 0x00, 0x0000 },
    { KEY_P, 0x002a, Qt::Key_Asterisk, Modifiers::ModAlt | Modifiers::ModShift, 0x00, 0x0000 },
    { KEY_P, 0x002a, Qt::Key_Asterisk, Modifiers::ModAltGr | Modifiers::ModShift, 0x00, 0x0000 },

    // Non-existent (26, 27)
    { KEY_LEFTBRACE, 0x005b, 0x0000005b, 0x00, 0x00, 0x0000 },
    { KEY_LEFTBRACE, 0x007b, 0x0000007b, 0x01, 0x00, 0x0000 },
    { KEY_LEFTBRACE, 0xffff, 0x01000000, 0x04, 0x00, 0x0000 },
    { KEY_RIGHTBRACE, 0x005d, 0x0000005d, 0x00, 0x00, 0x0000 },
    { KEY_RIGHTBRACE, 0x007d, 0x0000007d, 0x01, 0x00, 0x0000 },
    { KEY_RIGHTBRACE, 0x007e, 0x0000007e, 0x02, 0x00, 0x0000 },
    { KEY_RIGHTBRACE, 0x005d, 0x0400005d, 0x04, 0x00, 0x0000 },

    // KEY_ENTER (28)
    { KEY_ENTER, 0xffff, 0x01000004, 0x00, 0x00, 0x0000 },

    // KEY_LEFTCTRL (29)
    { KEY_LEFTCTRL, 0xffff, 0x01000021, 0x00, 0x04, 0x0004 }, // 0x04 => Flags::IsModifier, 0x0004 => Modifiers::ModControl, so apply ModControl when KEY_LEFTCTRL is pressed?

    // KEY_A (30)
    { KEY_A, 0x0061, 0x00000041, 0x00, 0x02, 0x0000 },
    { KEY_A, 0x0041, 0x00000041, 0x01, 0x02, 0x0000 },

    // KEY_S (31)
    { KEY_S, 0x0073, 0x00000053, 0x00, 0x02, 0x0000 },
    { KEY_S, 0x0053, 0x00000053, 0x01, 0x02, 0x0000 },

    // KEY_D (32)
    { KEY_D, 0x0064, 0x00000044, 0x00, 0x02, 0x0000 },
    { KEY_D, 0x0044, 0x00000044, 0x01, 0x02, 0x0000 },

    // KEY_F (33)
    { KEY_F, 0x0066, 0x00000046, 0x00, 0x02, 0x0000 },
    { KEY_F, 0x0046, 0x00000046, 0x01, 0x02, 0x0000 },

    // KEY_G (34)
    { KEY_G, 0x0067, 0x00000047, 0x00, 0x02, 0x0000 },
    { KEY_G, 0x0047, 0x00000047, 0x01, 0x02, 0x0000 },

    // KEY_H (35)
    { KEY_H, 0x0068, 0x00000048, 0x00, 0x02, 0x0000 },
    { KEY_H, 0x0048, 0x00000048, 0x01, 0x02, 0x0000 },

    // KEY_J (36)
    { KEY_J, 0x006a, 0x0000004a, 0x00, 0x02, 0x0000 },
    { KEY_J, 0x004a, 0x0000004a, 0x01, 0x02, 0x0000 },

    // KEY_K (37)
    { KEY_K, 0x006b, 0x0000004b, 0x00, 0x02, 0x0000 },
    { KEY_K, 0x004b, 0x0000004b, 0x01, 0x02, 0x0000 },

    // KEY_L (38)
    { KEY_L, 0x006c, 0x0000004c, 0x00, 0x02, 0x0000 },
    { KEY_L, 0x004c, 0x0000004c, 0x01, 0x02, 0x0000 },

    // FIXME(modifiers)
    // KEY_SEMICOLON (39)
    // "ö" Latin Small Letter O with Diaeresis 0x00f6
    // "Ö" Latin Capital Letter O with Diaeresis 0x00d6
    { KEY_SEMICOLON, 0x00f6, Qt::Key_Odiaeresis, 0x00, 0x02, 0x0000 },
    { KEY_SEMICOLON, 0x00d6, Qt::Key_Odiaeresis, 0x01, 0x02, 0x0000 },
    { KEY_SEMICOLON, 0x00f6, Qt::Key_Odiaeresis, 0x02, 0x02, 0x0000 },
    { KEY_SEMICOLON, 0x00d6, Qt::Key_Odiaeresis, 0x03, 0x02, 0x0000 },
    { KEY_SEMICOLON, 0x00f6, Qt::Key_Odiaeresis, 0x04, 0x02, 0x0000 },
    { KEY_SEMICOLON, 0x00d6, Qt::Key_Odiaeresis, 0x05, 0x02, 0x0000 },
    { KEY_SEMICOLON, 0x00f6, Qt::Key_Odiaeresis, 0x06, 0x02, 0x0000 },
    { KEY_SEMICOLON, 0x00d6, Qt::Key_Odiaeresis, 0x07, 0x02, 0x0000 },
    { KEY_SEMICOLON, 0x00f6, Qt::Key_Odiaeresis, 0x08, 0x02, 0x0000 },
    { KEY_SEMICOLON, 0x00d6, Qt::Key_Odiaeresis, 0x09, 0x02, 0x0000 },
    { KEY_SEMICOLON, 0x00f6, Qt::Key_Odiaeresis, 0x0a, 0x02, 0x0000 },
    { KEY_SEMICOLON, 0x00d6, Qt::Key_Odiaeresis, 0x0b, 0x02, 0x0000 },
    { KEY_SEMICOLON, 0x00f6, Qt::Key_Odiaeresis, 0x0c, 0x02, 0x0000 },
    { KEY_SEMICOLON, 0x00d6, Qt::Key_Odiaeresis, 0x0d, 0x02, 0x0000 },
    { KEY_SEMICOLON, 0x00f6, Qt::Key_Odiaeresis, 0x0e, 0x02, 0x0000 },
    { KEY_SEMICOLON, 0x00d6, Qt::Key_Odiaeresis, 0x0f, 0x02, 0x0000 },

    // FIXME(modifiers)
    // KEY_APOSTROPHE (40)
    // "ä" Latin Small Letter a with Diaeresis 0x00e4
    // "Ä" Latin Capital Letter a with Diaeresis 0x00c4
    // "'" Apostrophe 0x0027
    { KEY_APOSTROPHE, 0x00e4, Qt::Key_Adiaeresis, 0x00, 0x02, 0x0000 },
    { KEY_APOSTROPHE, 0x00c4, Qt::Key_Adiaeresis, 0x01, 0x02, 0x0000 },
    { KEY_APOSTROPHE, 0x0027, Qt::Key_Apostrophe, 0x02, 0x00, 0x0000 },
    { KEY_APOSTROPHE, 0x0027, Qt::Key_Apostrophe, 0x03, 0x00, 0x0000 },
    { KEY_APOSTROPHE, 0x0027, Qt::Key_Apostrophe, 0x04, 0x00, 0x0000 },
    { KEY_APOSTROPHE, 0x0027, Qt::Key_Apostrophe, 0x05, 0x00, 0x0000 },
    { KEY_APOSTROPHE, 0x0027, Qt::Key_Apostrophe, 0x06, 0x00, 0x0000 },
    { KEY_APOSTROPHE, 0x0027, Qt::Key_Apostrophe, 0x07, 0x00, 0x0000 },
    { KEY_APOSTROPHE, 0x0027, Qt::Key_Apostrophe, 0x08, 0x00, 0x0000 },
    { KEY_APOSTROPHE, 0x0027, Qt::Key_Apostrophe, 0x09, 0x00, 0x0000 },
    { KEY_APOSTROPHE, 0x0027, Qt::Key_Apostrophe, 0x0a, 0x00, 0x0000 },
    { KEY_APOSTROPHE, 0x0027, Qt::Key_Apostrophe, 0x0b, 0x00, 0x0000 },
    { KEY_APOSTROPHE, 0x0027, Qt::Key_Apostrophe, 0x0c, 0x00, 0x0000 },
    { KEY_APOSTROPHE, 0x0027, Qt::Key_Apostrophe, 0x0d, 0x00, 0x0000 },
    { KEY_APOSTROPHE, 0x0027, Qt::Key_Apostrophe, 0x0e, 0x00, 0x0000 },
    { KEY_APOSTROPHE, 0x0027, Qt::Key_Apostrophe, 0x0f, 0x00, 0x0000 },

    // FIXME(modifiers)
    // KEY_GRAVE (41)
    // "ü" Latin Small Letter U with Diaeresis 0x00fc
    // "Ü" Latin Capital Letter U with Diaeresis 0x00dc
    // "+" Plus Sign 0x002b
    { KEY_GRAVE, 0x00fc, Qt::Key_Udiaeresis, 0x00, Flags::IsLetter, 0x0000 },
    { KEY_GRAVE, 0x00dc, Qt::Key_Udiaeresis, Modifiers::ModShift, Flags::IsLetter, 0x0000 },
    { KEY_GRAVE, 0x002b, Qt::Key_Plus, Modifiers::ModAlt, 0x00, 0x0000 },
    { KEY_GRAVE, 0x002b, Qt::Key_Plus, Modifiers::ModAltGr, 0x00, 0x0000 },
    { KEY_GRAVE, 0x002b, Qt::Key_Plus, Modifiers::ModShift | Modifiers::ModAlt, 0x00, 0x0000 },
    { KEY_GRAVE, 0x002b, Qt::Key_Plus, Modifiers::ModShift | Modifiers::ModAltGr, 0x00, 0x0000 },

    { KEY_LEFTSHIFT, 0xffff, 0x01000020, 0x00, 0x04, 0x0001 },

    // KEY_BACKSLASH (43)
    // This key only exists on the US physical layout.
    // We implement the characters printed on the keys again
    // in case user selects German locale on a US keyboard.
    // "̃ " Combining Tilde 0x0303
    // "¨" Combining Diaeresis 0x0308
    { KEY_BACKSLASH, 0x0303, Qt::Key_Dead_Tilde, 0x00, Flags::IsDead, 0x0000 },
    { KEY_BACKSLASH, 0x0308, Qt::Key_Dead_Diaeresis, 0x01, Flags::IsDead, 0x0000 },

    // FIXME(modifiers)
    // KEY_Z (44)
    // "y" Latin Small Letter Y 0x0079
    // "Y" Latin Capital Letter Y 0x0059
    // "<" Less-Than Sign 0x003c
    { KEY_Z, 0x0079, 0x00000059, 0x00, 0x02, 0x0000 },
    { KEY_Z, 0x0059, 0x00000059, 0x01, 0x02, 0x0000 },
    { KEY_Z, 0x003c, Qt::Key_Less, 0x02, 0x02, 0x0000 },
    { KEY_Z, 0x003c, Qt::Key_Less, 0x03, 0x02, 0x0000 },
    { KEY_Z, 0x003c, Qt::Key_Less, 0x04, 0x02, 0x0000 },
    { KEY_Z, 0x003c, Qt::Key_Less, 0x05, 0x02, 0x0000 },
    { KEY_Z, 0x003c, Qt::Key_Less, 0x06, 0x02, 0x0000 },
    { KEY_Z, 0x003c, Qt::Key_Less, 0x07, 0x02, 0x0000 },
    { KEY_Z, 0x003c, Qt::Key_Less, 0x08, 0x02, 0x0000 },
    { KEY_Z, 0x003c, Qt::Key_Less, 0x09, 0x02, 0x0000 },
    { KEY_Z, 0x003c, Qt::Key_Less, 0x0a, 0x02, 0x0000 },
    { KEY_Z, 0x003c, Qt::Key_Less, 0x0b, 0x02, 0x0000 },
    { KEY_Z, 0x003c, Qt::Key_Less, 0x0c, 0x02, 0x0000 },
    { KEY_Z, 0x003c, Qt::Key_Less, 0x0d, 0x02, 0x0000 },
    { KEY_Z, 0x003c, Qt::Key_Less, 0x0e, 0x02, 0x0000 },
    { KEY_Z, 0x003c, Qt::Key_Less, 0x0f, 0x02, 0x0000 },

    // FIXME(modifiers)
    // KEY_X (45)
    // ">" Greater-Than Sign 0x003e
    { KEY_X, 0x0078, 0x00000058, 0x00, 0x02, 0x0000 },
    { KEY_X, 0x0058, 0x00000058, 0x01, 0x02, 0x0000 },
    { KEY_X, 0x003e, Qt::Key_Greater, 0x02, 0x02, 0x0000 },
    { KEY_X, 0x003e, Qt::Key_Greater, 0x03, 0x02, 0x0000 },
    { KEY_X, 0x003e, Qt::Key_Greater, 0x04, 0x02, 0x0000 },
    { KEY_X, 0x003e, Qt::Key_Greater, 0x05, 0x02, 0x0000 },
    { KEY_X, 0x003e, Qt::Key_Greater, 0x06, 0x02, 0x0000 },
    { KEY_X, 0x003e, Qt::Key_Greater, 0x07, 0x02, 0x0000 },
    { KEY_X, 0x003e, Qt::Key_Greater, 0x08, 0x02, 0x0000 },
    { KEY_X, 0x003e, Qt::Key_Greater, 0x09, 0x02, 0x0000 },
    { KEY_X, 0x003e, Qt::Key_Greater, 0x0a, 0x02, 0x0000 },
    { KEY_X, 0x003e, Qt::Key_Greater, 0x0b, 0x02, 0x0000 },
    { KEY_X, 0x003e, Qt::Key_Greater, 0x0c, 0x02, 0x0000 },
    { KEY_X, 0x003e, Qt::Key_Greater, 0x0d, 0x02, 0x0000 },
    { KEY_X, 0x003e, Qt::Key_Greater, 0x0e, 0x02, 0x0000 },
    { KEY_X, 0x003e, Qt::Key_Greater, 0x0f, 0x02, 0x0000 },

    // KEY_C (46)
    { KEY_C, 0x0063, 0x00000043, 0x00, 0x02, 0x0000 },
    { KEY_C, 0x0043, 0x00000043, 0x01, 0x02, 0x0000 },

    // KEY_V (47)
    { KEY_V, 0x0076, 0x00000056, 0x00, 0x02, 0x0000 },
    { KEY_V, 0x0056, 0x00000056, 0x01, 0x02, 0x0000 },

    // KEY_B (48)
    { KEY_B, 0x0062, 0x00000042, 0x00, 0x02, 0x0000 },
    { KEY_B, 0x0042, 0x00000042, 0x01, 0x02, 0x0000 },

    // KEY_N (49)
    { KEY_N, 0x006e, 0x0000004e, 0x00, 0x02, 0x0000 },
    { KEY_N, 0x004e, 0x0000004e, 0x01, 0x02, 0x0000 },

    // KEY_M (50)
    { KEY_M, 0x006d, 0x0000004d, 0x00, 0x02, 0x0000 },
    { KEY_M, 0x004d, 0x0000004d, 0x01, 0x02, 0x0000 },

    // KEY_COMMA (51)
    // "," Comma 0x002c
    // ";" Semicolon 0x003b
    { KEY_COMMA, 0x002c, Qt::Key_Comma, 0x00, 0x00, 0x0000 },
    { KEY_COMMA, 0x003b, Qt::Key_Semicolon, 0x01, 0x00, 0x0000 },
    { KEY_COMMA, 0x002c, Qt::Key_Comma, 0x02, 0x00, 0x0000 },
    { KEY_COMMA, 0x003b, Qt::Key_Semicolon, 0x03, 0x00, 0x0000 },
    { KEY_COMMA, 0x002c, Qt::Key_Comma, 0x04, 0x00, 0x0000 },
    { KEY_COMMA, 0x003b, Qt::Key_Semicolon, 0x05, 0x00, 0x0000 },
    { KEY_COMMA, 0x002c, Qt::Key_Comma, 0x06, 0x00, 0x0000 },
    { KEY_COMMA, 0x003b, Qt::Key_Semicolon, 0x07, 0x00, 0x0000 },
    { KEY_COMMA, 0x002c, Qt::Key_Comma, 0x08, 0x00, 0x0000 },
    { KEY_COMMA, 0x003b, Qt::Key_Semicolon, 0x09, 0x00, 0x0000 },
    { KEY_COMMA, 0x002c, Qt::Key_Comma, 0x0a, 0x00, 0x0000 },
    { KEY_COMMA, 0x003b, Qt::Key_Semicolon, 0x0b, 0x00, 0x0000 },
    { KEY_COMMA, 0x002c, Qt::Key_Comma, 0x0c, 0x00, 0x0000 },
    { KEY_COMMA, 0x003b, Qt::Key_Semicolon, 0x0d, 0x00, 0x0000 },
    { KEY_COMMA, 0x002c, Qt::Key_Comma, 0x0e, 0x00, 0x0000 },
    { KEY_COMMA, 0x003b, Qt::Key_Semicolon, 0x0f, 0x00, 0x0000 },

    // KEY_DOT (52)
    // "." Full Stop 0x002e
    // ":" Colon 0x003a
    { KEY_DOT, 0x002e, Qt::Key_Period, 0x00, 0x00, 0x0000 },
    { KEY_DOT, 0x003a, Qt::Key_Colon, 0x01, 0x00, 0x0000 },
    { KEY_DOT, 0x002e, Qt::Key_Period, 0x02, 0x00, 0x0000 },
    { KEY_DOT, 0x003a, Qt::Key_Colon, 0x03, 0x00, 0x0000 },
    { KEY_DOT, 0x002e, Qt::Key_Period, 0x04, 0x00, 0x0000 },
    { KEY_DOT, 0x003a, Qt::Key_Colon, 0x05, 0x00, 0x0000 },
    { KEY_DOT, 0x002e, Qt::Key_Period, 0x06, 0x00, 0x0000 },
    { KEY_DOT, 0x003a, Qt::Key_Colon, 0x07, 0x00, 0x0000 },
    { KEY_DOT, 0x002e, Qt::Key_Period, 0x08, 0x00, 0x0000 },
    { KEY_DOT, 0x003a, Qt::Key_Colon, 0x09, 0x00, 0x0000 },
    { KEY_DOT, 0x002e, Qt::Key_Period, 0x0a, 0x00, 0x0000 },
    { KEY_DOT, 0x003a, Qt::Key_Colon, 0x0b, 0x00, 0x0000 },
    { KEY_DOT, 0x002e, Qt::Key_Period, 0x0c, 0x00, 0x0000 },
    { KEY_DOT, 0x003a, Qt::Key_Colon, 0x0d, 0x00, 0x0000 },
    { KEY_DOT, 0x002e, Qt::Key_Period, 0x0e, 0x00, 0x0000 },
    { KEY_DOT, 0x003a, Qt::Key_Colon, 0x0f, 0x00, 0x0000 },

    // KEY_SLASH (53)
    // "-" Hyphen-Minus 0x002d
    // "_" Low Line 0x005f
    { KEY_SLASH, 0x002d, Qt::Key_Minus, 0x00, 0x00, 0x0000 },
    { KEY_SLASH, 0x005f, Qt::Key_Underscore, 0x01, 0x00, 0x0000 },
    { KEY_SLASH, 0x002d, Qt::Key_Minus, 0x02, 0x00, 0x0000 },
    { KEY_SLASH, 0x005f, Qt::Key_Underscore, 0x03, 0x00, 0x0000 },
    { KEY_SLASH, 0x002d, Qt::Key_Minus, 0x04, 0x00, 0x0000 },
    { KEY_SLASH, 0x005f, Qt::Key_Underscore, 0x05, 0x00, 0x0000 },
    { KEY_SLASH, 0x002d, Qt::Key_Minus, 0x06, 0x00, 0x0000 },
    { KEY_SLASH, 0x005f, Qt::Key_Underscore, 0x07, 0x00, 0x0000 },
    { KEY_SLASH, 0x002d, Qt::Key_Minus, 0x08, 0x00, 0x0000 },
    { KEY_SLASH, 0x005f, Qt::Key_Underscore, 0x09, 0x00, 0x0000 },
    { KEY_SLASH, 0x002d, Qt::Key_Minus, 0x0a, 0x00, 0x0000 },
    { KEY_SLASH, 0x005f, Qt::Key_Underscore, 0x0b, 0x00, 0x0000 },
    { KEY_SLASH, 0x002d, Qt::Key_Minus, 0x0c, 0x00, 0x0000 },
    { KEY_SLASH, 0x005f, Qt::Key_Underscore, 0x0d, 0x00, 0x0000 },
    { KEY_SLASH, 0x002d, Qt::Key_Minus, 0x0e, 0x00, 0x0000 },
    { KEY_SLASH, 0x005f, Qt::Key_Underscore, 0x0f, 0x00, 0x0000 },

    // KEY_RIGHTSHIFT (54)
    { KEY_RIGHTSHIFT, 0xffff, 0x01000020, 0x00, 0x04, 0x0001 },

    // Non-existent
    { 55, 0x002a, 0x2000002a, 0x00, 0x00, 0x0000 },

    // KEY_LEFTALT (56)
    { KEY_LEFTALT, 0xffff, 0x01000023, 0x00, 0x04, 0x0008 },

    // KEY_SPACE (57)
    { KEY_SPACE, 0x0020, 0x00000020, 0x00, 0x00, 0x0000 },

    // KEY_CAPSLOCK (58)
    { KEY_CAPSLOCK, 0xffff, 0x01000024, 0x00, 0x00, 0x0000 },

    // Non-existent
    { 59, 0xffff, 0x01000030, 0x00, 0x00, 0x0000 },
    { 59, 0xffff, 0x0100003c, 0x01, 0x00, 0x0000 },
    { 59, 0xffff, 0x01000048, 0x04, 0x00, 0x0000 },
    { 59, 0xffff, 0x01000000, 0x0c, 0x08, 0x0100 },
    { 60, 0xffff, 0x01000031, 0x00, 0x00, 0x0000 },
    { 60, 0xffff, 0x0100003d, 0x01, 0x00, 0x0000 },
    { 60, 0xffff, 0x01000049, 0x04, 0x00, 0x0000 },
    { 60, 0xffff, 0x01000000, 0x0c, 0x08, 0x0101 },
    { 61, 0xffff, 0x01000032, 0x00, 0x00, 0x0000 },
    { 61, 0xffff, 0x0100003e, 0x01, 0x00, 0x0000 },
    { 61, 0xffff, 0x0100004a, 0x04, 0x00, 0x0000 },
    { 61, 0xffff, 0x01000000, 0x0c, 0x08, 0x0102 },
    { 62, 0xffff, 0x01000033, 0x00, 0x00, 0x0000 },
    { 62, 0xffff, 0x0100003f, 0x01, 0x00, 0x0000 },
    { 62, 0xffff, 0x0100004b, 0x04, 0x00, 0x0000 },
    { 62, 0xffff, 0x01000000, 0x0c, 0x08, 0x0103 },
    { 63, 0xffff, 0x01000034, 0x00, 0x00, 0x0000 },
    { 63, 0xffff, 0x01000040, 0x01, 0x00, 0x0000 },
    { 63, 0xffff, 0x0100004c, 0x04, 0x00, 0x0000 },
    { 63, 0xffff, 0x01000000, 0x0c, 0x08, 0x0104 },
    { 64, 0xffff, 0x01000035, 0x00, 0x00, 0x0000 },
    { 64, 0xffff, 0x01000041, 0x01, 0x00, 0x0000 },
    { 64, 0xffff, 0x0100004d, 0x04, 0x00, 0x0000 },
    { 64, 0xffff, 0x01000000, 0x0c, 0x08, 0x0105 },
    { 65, 0xffff, 0x01000036, 0x00, 0x00, 0x0000 },
    { 65, 0xffff, 0x01000042, 0x01, 0x00, 0x0000 },
    { 65, 0xffff, 0x0100004e, 0x04, 0x00, 0x0000 },
    { 65, 0xffff, 0x01000000, 0x0c, 0x08, 0x0106 },
    { 66, 0xffff, 0x01000037, 0x00, 0x00, 0x0000 },
    { 66, 0xffff, 0x01000043, 0x01, 0x00, 0x0000 },
    { 66, 0xffff, 0x0100004f, 0x04, 0x00, 0x0000 },
    { 66, 0xffff, 0x01000000, 0x0c, 0x08, 0x0107 },
    { 67, 0xffff, 0x01000038, 0x00, 0x00, 0x0000 },
    { 67, 0xffff, 0x01000044, 0x01, 0x00, 0x0000 },
    { 67, 0xffff, 0x01000050, 0x04, 0x00, 0x0000 },
    { 67, 0xffff, 0x01000000, 0x0c, 0x08, 0x0108 },
    { 68, 0xffff, 0x01000039, 0x00, 0x00, 0x0000 },
    { 68, 0xffff, 0x01000045, 0x01, 0x00, 0x0000 },
    { 68, 0xffff, 0x01000051, 0x04, 0x00, 0x0000 },
    { 68, 0xffff, 0x01000000, 0x0c, 0x08, 0x0109 },
    { 69, 0xffff, 0x01000025, 0x00, 0x00, 0x0000 },
    { 70, 0xffff, 0x01000026, 0x00, 0x00, 0x0000 },
    { 70, 0xffff, 0x01000026, 0x08, 0x00, 0x0000 },
    { 71, 0x0037, 0x20000037, 0x00, 0x00, 0x0000 },
    { 72, 0x0038, 0x20000038, 0x00, 0x00, 0x0000 },
    { 73, 0x0039, 0x20000039, 0x00, 0x00, 0x0000 },
    { 74, 0x002d, 0x2000002d, 0x00, 0x00, 0x0000 },
    { 75, 0x0034, 0x20000034, 0x00, 0x00, 0x0000 },
    { 76, 0x0035, 0x20000035, 0x00, 0x00, 0x0000 },
    { 77, 0x0036, 0x20000036, 0x00, 0x00, 0x0000 },
    { 78, 0x002b, 0x2000002b, 0x00, 0x00, 0x0000 },
    { 79, 0x0031, 0x20000031, 0x00, 0x00, 0x0000 },
    { 80, 0x0032, 0x20000032, 0x00, 0x00, 0x0000 },
    { 81, 0x0033, 0x20000033, 0x00, 0x00, 0x0000 },
    { 82, 0x0030, 0x20000030, 0x00, 0x00, 0x0000 },
    { 83, 0x002e, 0x2000002e, 0x00, 0x00, 0x0000 },
    { 83, 0xffff, 0x01000000, 0x06, 0x08, 0x0200 },
    { 83, 0xffff, 0x01000000, 0x0c, 0x08, 0x0200 },
    { 86, 0x003c, 0x0000003c, 0x00, 0x00, 0x0000 },
    { 86, 0x003e, 0x0000003e, 0x01, 0x00, 0x0000 },
    { 86, 0x007c, 0x0000007c, 0x02, 0x00, 0x0000 },
    { 87, 0xffff, 0x0100003a, 0x00, 0x00, 0x0000 },
    { 87, 0xffff, 0x01000046, 0x01, 0x00, 0x0000 },
    { 87, 0xffff, 0x01000052, 0x04, 0x00, 0x0000 },
    { 87, 0xffff, 0x01000000, 0x0c, 0x08, 0x010a },
    { 88, 0xffff, 0x0100003b, 0x00, 0x00, 0x0000 },
    { 88, 0xffff, 0x01000047, 0x01, 0x00, 0x0000 },
    { 88, 0xffff, 0x01000000, 0x0c, 0x08, 0x010b },
    { 96, 0xffff, 0x21000005, 0x00, 0x00, 0x0000 },

    { KEY_RIGHTCTRL, 0xffff, 0x01000021, 0x00, 0x04, 0x0004 },

    // Non-existent
    { 98, 0x002f, 0x2000002f, 0x00, 0x00, 0x0000 },
    { 99, 0x005c, 0x0400005c, 0x00, 0x00, 0x0000 },

    // KEY_RIGHTALT (100)
    { KEY_RIGHTALT, 0xffff, 0x01001103, 0x00, 0x04, 0x0002 },

    // KEY_HOME (102)
    { KEY_HOME, 0xffff, 0x01000010, 0x00, 0x00, 0x0000 },

    // KEY_UP (103)
    { KEY_UP, 0xffff, 0x01000013, 0x00, 0x00, 0x0000 },

    // Non-existent
    { 104, 0xffff, 0x01000016, 0x00, 0x00, 0x0000 },

    // KEY_LEFT (105)
    { KEY_LEFT, 0xffff, 0x01000012, 0x00, 0x00, 0x0000 },
    { KEY_LEFT, 0xffff, 0x01000000, 0x0c, 0x08, 0x0180 },

    // KEY_RIGHT (106)
    { KEY_RIGHT, 0xffff, 0x01000014, 0x00, 0x00, 0x0000 },
    { KEY_RIGHT, 0xffff, 0x01000000, 0x0c, 0x08, 0x0181 },

    // KEY_END (107)
    { KEY_END, 0xffff, 0x01000011, 0x00, 0x00, 0x0000 },

    // KEY_DOWN (108)
    { KEY_DOWN, 0xffff, 0x01000015, 0x00, 0x00, 0x0000 },

    { 109, 0xffff, 0x01000017, 0x00, 0x00, 0x0000 },
    { 110, 0xffff, 0x01000006, 0x00, 0x00, 0x0000 },
    { 111, 0xffff, 0x01000007, 0x00, 0x00, 0x0000 },
    { 111, 0xffff, 0x01000000, 0x06, 0x08, 0x0200 },
    { 111, 0xffff, 0x01000000, 0x0c, 0x08, 0x0200 },

    // 113 -> 248
    { KEY_MUTE, 0xffff, Qt::Key_VolumeMute, 0x00, 0x00, 0x0000 },
    { KEY_VOLUMEDOWN, 0xffff, Qt::Key_VolumeDown, 0x00, 0x00, 0x0000 },
    { KEY_VOLUMEUP, 0xffff, Qt::Key_VolumeUp, 0x00, 0x00, 0x0000 },
    { KEY_POWER, 0xffff, Qt::Key_PowerOff, 0x00, 0x00, 0x0000 },
    { KEY_PAUSE, 0xffff, Qt::Key_Pause, 0x00, 0x00, 0x0000 },
    { KEY_STOP, 0xffff, Qt::Key_Stop, 0x00, 0x00, 0x0000 },
    { KEY_SETUP, 0xffff, Qt::Key_Settings, 0x00, 0x00, 0x0000 },
    { KEY_RECORD, 0xffff, Qt::Key_MediaRecord, 0x00, 0x00, 0x0000 },
    { KEY_REWIND, 0xffff, Qt::Key_AudioRewind, 0x00, 0x00, 0x0000 },
    { KEY_PLAYPAUSE, 0xffff, Qt::Key_MediaTogglePlayPause, 0x00, 0x00, 0x0000 },
    { KEY_PLAY, 0xffff, Qt::Key_MediaPlay, 0x00, 0x00, 0x0000 },
    { KEY_FASTFORWARD, 0xffff, Qt::Key_AudioForward, 0x00, 0x00, 0x0000 },
    { KEY_CANCEL, 0xffff, Qt::Key_Cancel, 0x00, 0x00, 0x0000 },
    { 248, 0xffff, Qt::Key_MicMute, 0x00, 0x00, 0x0000 },
    // 0x160 ->
    { KEY_SELECT, 0xffff, Qt::Key_Select, 0x00, 0x00, 0x0000 },
    { KEY_CLEAR, 0xffff, Qt::Key_Clear, 0x00, 0x00, 0x0000 },
    /* { KEY_OPTION, 0xffff, Qt::Key_Meta, 0x00, Flags::IsModifier, Modifiers::ModCapsShift }, */
    { KEY_EXIT, 0xffff, Qt::Key_Exit, 0x00, 0x00, 0x0000 },
    { KEY_INFO, 0xffff, Qt::Key_Info, 0x00, 0x00, 0x0000 },
    { KEY_PROGRAM, 0xffff, Qt::Key_Guide, 0x00, 0x00, 0x0000 },
    { KEY_CALENDAR, 0xffff, Qt::Key_Calendar, 0x00, 0x00, 0x0000 },
    { KEY_RED, 0xffff, Qt::Key_Red, 0x00, 0x00, 0x0000 },
    { KEY_GREEN, 0xffff, Qt::Key_Green, 0x00, 0x00, 0x0000 },
    { KEY_YELLOW, 0xffff, Qt::Key_Yellow, 0x00, 0x00, 0x0000 },
    { KEY_BLUE, 0xffff, Qt::Key_Blue, 0x00, 0x00, 0x0000 },
    { KEY_CHANNELUP, 0xffff, Qt::Key_ChannelUp, 0x00, 0x00, 0x0000 },
    { KEY_CHANNELDOWN, 0xffff, Qt::Key_ChannelDown, 0x00, 0x00, 0x0000 },
};

constexpr static size_t keymapSize = sizeof(keymap) / sizeof(keymap[0]);

constexpr static EpaperEvdevKeyboardMap::Composing keycompose[] = {
    { 0x0060, 0x0041, 0x00c0 },
    { 0x0060, 0x0061, 0x00e0 },
    { 0x0027, 0x0041, 0x00c1 },
    { 0x0027, 0x0061, 0x00e1 },
    { 0x005e, 0x0041, 0x00c2 },
    { 0x005e, 0x0061, 0x00e2 },
    { 0x007e, 0x0041, 0x00c3 },
    { 0x007e, 0x0061, 0x00e3 },
    { 0x0022, 0x0041, 0x00c4 },
    { 0x0022, 0x0061, 0x00e4 },
    { 0x002d, 0x0061, 0x00aa },
    { 0x002d, 0x0041, 0x00aa },
    { 0x004f, 0x0041, 0x00c5 },
    { 0x006f, 0x0061, 0x00e5 },
    { 0x0030, 0x0041, 0x00c5 },
    { 0x0030, 0x0061, 0x00e5 },
    { 0x0041, 0x0041, 0x00c5 },
    { 0x0061, 0x0061, 0x00e5 },
    { 0x00b0, 0x0041, 0x00c5 },
    { 0x00b0, 0x0061, 0x00e5 },
    { 0x0041, 0x0045, 0x00c6 },
    { 0x0061, 0x0065, 0x00e6 },
    { 0x002c, 0x0043, 0x00c7 },
    { 0x002c, 0x0063, 0x00e7 },
    { 0x005e, 0x0043, 0x00c7 },
    { 0x005e, 0x0063, 0x00e7 },
    { 0x0060, 0x0045, 0x00c8 },
    { 0x0060, 0x0065, 0x00e8 },
    { 0x0027, 0x0045, 0x00c9 },
    { 0x0027, 0x0065, 0x00e9 },
    { 0x005e, 0x0045, 0x00ca },
    { 0x005e, 0x0065, 0x00ea },
    { 0x0022, 0x0045, 0x00cb },
    { 0x0022, 0x0065, 0x00eb },
    { 0x0060, 0x0049, 0x00cc },
    { 0x0060, 0x0069, 0x00ec },
    { 0x0027, 0x0049, 0x00cd },
    { 0x0027, 0x0069, 0x00ed },
    { 0x005e, 0x0049, 0x00ce },
    { 0x005e, 0x0069, 0x00ee },
    { 0x0022, 0x0049, 0x00cf },
    { 0x0022, 0x0069, 0x00ef },
    { 0x002d, 0x0044, 0x00d0 },
    { 0x002d, 0x0064, 0x00f0 },
    { 0x005e, 0x0044, 0x00d0 },
    { 0x005e, 0x0064, 0x00f0 },
    { 0x007e, 0x004e, 0x00d1 },
    { 0x007e, 0x006e, 0x00f1 },
    { 0x005e, 0x004e, 0x00d1 },
    { 0x005e, 0x006e, 0x00f1 },
    { 0x0060, 0x004f, 0x00d2 },
    { 0x0060, 0x006f, 0x00f2 },
    { 0x0027, 0x004f, 0x00d3 },
    { 0x0027, 0x006f, 0x00f3 },
    { 0x005e, 0x004f, 0x00d4 },
    { 0x005e, 0x006f, 0x00f4 },
    { 0x007e, 0x004f, 0x00d5 },
    { 0x007e, 0x006f, 0x00f5 },
    { 0x0022, 0x004f, 0x00d6 },
    { 0x0022, 0x006f, 0x00f6 },
    { 0x002f, 0x004f, 0x00d8 },
    { 0x002f, 0x006f, 0x00f8 },
    { 0x002d, 0x006f, 0x00ba },
    { 0x002d, 0x004f, 0x00ba },
    { 0x0060, 0x0055, 0x00d9 },
    { 0x0060, 0x0075, 0x00f9 },
    { 0x0027, 0x0055, 0x00da },
    { 0x0027, 0x0075, 0x00fa },
    { 0x005e, 0x0055, 0x00db },
    { 0x005e, 0x0075, 0x00fb },
    { 0x0022, 0x0055, 0x00dc },
    { 0x0022, 0x0075, 0x00fc },
    { 0x0027, 0x0059, 0x00dd },
    { 0x0027, 0x0079, 0x00fd },
    { 0x0054, 0x0048, 0x00de },
    { 0x0074, 0x0068, 0x00fe },
    { 0x0073, 0x0073, 0x00df },
    { 0x0022, 0x0079, 0x00ff },
    { 0x0073, 0x007a, 0x00df },
    { 0x006e, 0x006e, 0x00f1 },
    { 0x006e, 0x0068, 0x00f1 },
    { 0x004e, 0x0059, 0x00d1 },
    { 0x004e, 0x004e, 0x00d1 },
    { 0x004e, 0x0048, 0x00d1 },
    { 0x004e, 0x0079, 0x00d1 },
    { 0x004e, 0x006e, 0x00d1 },
    { 0x004e, 0x0068, 0x00d1 },
    { 0x002d, 0x004c, 0x00a3 },
    { 0x003c, 0x003c, 0x00ab },
    { 0x003e, 0x003e, 0x00bb },
    { 0x003f, 0x003f, 0x00bf },
    { 0x005e, 0x003f, 0x00bf },
    { 0x0021, 0x0021, 0x00a1 },
    { 0x005e, 0x0021, 0x00a1 },
    { 0x005e, 0x0031, 0x00b9 },
    { 0x005e, 0x0032, 0x00b2 },
    { 0x005e, 0x0033, 0x00b3 },
    { 0x002b, 0x002d, 0x00b1 },
    { 0x0063, 0x003d, 0x00a2 },
    { 0x0063, 0x002f, 0x00a2 },
    { 0x002f, 0x0063, 0x00a2 },
    { 0x002d, 0x0063, 0x00a2 },
    { 0x002d, 0x0043, 0x00a2 },
    { 0x004c, 0x003d, 0x00a3 },
    { 0x002d, 0x004c, 0x00a3 },
    { 0x002d, 0x006c, 0x00a3 },
    { 0x005e, 0x002a, 0x00d7 },
    { 0x005e, 0x0078, 0x00d7 },
    { 0x0078, 0x0078, 0x00d7 },
    { 0x005e, 0x002e, 0x00b7 },
    { 0x002e, 0x002e, 0x00b7 },
    { 0x005e, 0x002f, 0x00f7 },
    { 0x005e, 0x003a, 0x00f7 },
    { 0x002d, 0x003a, 0x00f7 },
    { 0x003a, 0x002d, 0x00f7 },
    { 0x0059, 0x003d, 0x00a5 },
    { 0x002d, 0x0059, 0x00a5 },
    { 0x002d, 0x006c, 0x00a5 },
    { 0x0028, 0x0063, 0x00a9 },
    { 0x0022, 0x0063, 0x00a9 },
    { 0x002d, 0x0061, 0x00aa },
    { 0x002d, 0x0041, 0x00aa },
    { 0x002d, 0x006f, 0x00ba },
    { 0x002d, 0x004f, 0x00ba },
    { 0x0028, 0x0072, 0x00ae },
    { 0x0022, 0x0072, 0x00ae },
    { 0x006d, 0x0075, 0x00b5 },
    { 0x0031, 0x0034, 0x0152 },
    { 0x0031, 0x0032, 0x0153 },
    { 0x0033, 0x0034, 0x0178 },
    { 0x0065, 0x003d, 0x20ac },
    { 0x002d, 0x0065, 0x20ac },
    { 0x002d, 0x0045, 0x20ac },
    { 0x0076, 0x0053, 0x0160 },
    { 0x005e, 0x0053, 0x0160 },
    { 0x0076, 0x0073, 0x0161 },
    { 0x005e, 0x0073, 0x0161 },
    { 0x0076, 0x005a, 0x017d },
    { 0x005e, 0x005a, 0x017d },
    { 0x0076, 0x007a, 0x017e },
    { 0x005e, 0x007a, 0x017e },
    { 0x004f, 0x0045, 0x0152 },
    { 0x004f, 0x0065, 0x0152 },
    { 0x006f, 0x0065, 0x0153 },
    { 0x0022, 0x0059, 0x0178 },
    { 0x0069, 0x006a, 0x00ff },
    { 0x0049, 0x004a, 0x0178 },
};

constexpr static size_t keycomposeSize = sizeof(keycompose) / sizeof(keycompose[0]);
};

} // namespace Locale
} // namespace EpaperEvdevKeyboardMap

