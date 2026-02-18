#pragma once

// 0: reset
#define $ "\033[0m"
// 1: bold
#define $b "\033[1m"
// 2: faint (non-std)
#define $f "\033[2m"
// 3: italic (non-std, sometimes inverse)
#define $i "\033[3m"
// 4: underline
#define $u "\033[4m"
// 5: slow blink
#define $l "\033[5m"
// 6: rapid blink (non-std)
#define $p "\033[6m"
// 7: reverse
#define $r "\033[7m"
// 8: conceal (non-std)
#define $c "\033[8m"
// 9: strikethrough (non-std)
#define $s "\033[9m"
// 21: bold off or double underline (non-std)
#define $bu "\033[21m"
// 22: normal color or intensity
#define $n "\033[22m"
// 23: not italic or Fraktur
#define $ni "\033[23m"
// 24: not underlined
#define $nu "\033[24m"
// 25: not blinking
#define $nl "\033[25m"
// 27: not inverse
#define $nn "\033[27m"
// 28: not concealed
#define $nc "\033[28m"
// 29: not strikethrough
#define $ns "\033[29m"
// 51: framed
#define $a "\033[51m"
// 52: encircled
#define $e "\033[52m"
// 53: overlined
#define $o "\033[53m"
// 54: not framed or encircled
#define $na "\033[54m"
// 55: not overlined
#define $no "\033[55m"
// 60: ideogram underline (non-std)
#define $iu "\033[60m"
// 61: ideogram double underline (non-std)
#define $id "\033[61m"
// 62: ideogram overline (non-std)
#define $io "\033[62m"
// 63: ideogram double overline (non-std)
#define $ib "\033[63m"
// 64: ideogram stress marking (non-std)
#define $is "\033[64m"
// 65: ideogram attributes off (non-std)
#define $if "\033[65m"

// 1n: alternate font (0-9)
#define $font(n) "\033[1" #n "m"
// 20: Fraktur font
#define $fraktur "\033[20m"

// 30: black foreground
#define $black "\033[30m"
// 31: red foreground
#define $red "\033[31m"
// 32: green foreground
#define $green "\033[32m"
// 33: yellow foreground
#define $yellow "\033[33m"
// 34: blue foreground
#define $blue "\033[34m"
// 35: magenta foreground
#define $magenta "\033[35m"
// 36: cyan foreground
#define $cyan "\033[36m"
// 37: white foreground
#define $white "\033[37m"
// 39: default forreground
#define $default "\033[39m"
// 90: light black foreground
#define $lblack "\033[90m"
// 91: light red foreground
#define $lred "\033[91m"
// 92: light green foreground
#define $lgreen "\033[92m"
// 93: light yellow foreground
#define $lyellow "\033[93m"
// 94: light blue foreground
#define $lblue "\033[94m"
// 95: light magenta foreground
#define $lmagenta "\033[95m"
// 96: light cyan foreground
#define $lcyan "\033[96m"
// 97: light white foreground
#define $lwhite "\033[97m"
// 38;5;n: 1-256 foreground (non-std)
#define $256(n) "\033[38;5;" #n "m"
// 38;2;r;g;b: red, green, blue foreground (non-std)
#define $rgb(r, g, b) "\033[38;2;" #r ";" #g ";" #b "m"

// 40: black background
#define $bblack "\033[40m"
// 41: red background
#define $bred "\033[41m"
// 42: green background
#define $bgreen "\033[42m"
// 43: yellow background
#define $byellow "\033[43m"
// 44: blue background
#define $bblue "\033[44m"
// 45: magenta background
#define $bmagenta "\033[45m"
// 46: cyan background
#define $bcyan "\033[46m"
// 47: white background
#define $bwhite "\033[47m"
// 49: default background
#define $bdefault "\033[49m"
// 100: light black background
#define $blblack "\033[100m"
// 101: light red background
#define $blred "\033[101m"
// 102: light green background
#define $blgreen "\033[102m"
// 103: light yellow background
#define $blyellow "\033[103m"
// 104: light blue background
#define $blblue "\033[104m"
// 105: light magenta background
#define $blmagenta "\033[105m"
// 106: light cyan background
#define $blcyan "\033[106m"
// 107: light white background
#define $blwhite "\033[107m"
// 48;5;n: 1-256 background (non-std)
#define $b256(n) "\033[48;5;" #n "m"
// 48;2;r;g;b: red, green, blue background (non-std)
#define $brgb(r, g, b) "\033[48;2;" #r ";" #g ";" #b "m"
