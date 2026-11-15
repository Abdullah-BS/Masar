#!/usr/bin/env python3
"""Regenerate esp32/masar_screen/screen_page.h from ../screen.html.
Run it after every change to screen.html:   python3 esp32/make_page.py"""
import pathlib
root = pathlib.Path(__file__).resolve().parent
html = (root.parent / "screen.html").read_text(encoding="utf-8")
assert ')MASAR"' not in html
out = ("// Generated from screen.html by esp32/make_page.py. Do not edit by hand.\n"
       "#pragma once\n#include <pgmspace.h>\n"
       'const char SCREEN_HTML[] PROGMEM = R"MASAR(' + html + ')MASAR";\n')
(root / "masar_screen" / "screen_page.h").write_text(out, encoding="utf-8")
print("wrote screen_page.h,", len(html), "bytes of page")
