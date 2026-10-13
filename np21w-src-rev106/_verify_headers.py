# -*- coding: utf-8 -*-
"""Deep check of the ported files: every quoted #include must resolve.

Include search set = the vcxproj AdditionalIncludeDirectories (per config) plus
the directory of the including file itself.
"""
import os
import re
import sys
import xml.etree.ElementTree as ET

ROOT = os.path.dirname(os.path.abspath(__file__))
WIN9X = os.path.join(ROOT, 'win9x')
NS = '{http://schemas.microsoft.com/developer/msbuild/2003}'

# collect include dirs from both vcxproj (union over configs)
inc_dirs = []
for proj in ('np21vs2019.vcxproj', 'np21vs2010.vcxproj'):
    root = ET.parse(os.path.join(WIN9X, proj)).getroot()
    for e in root.iter(NS + 'AdditionalIncludeDirectories'):
        for d in (e.text or '').split(';'):
            d = d.strip()
            if not d:
                continue
            p = os.path.normpath(os.path.join(WIN9X, d))
            if p not in inc_dirs:
                inc_dirs.append(p)
inc_dirs.append(WIN9X)
print('include dirs (%d):' % len(inc_dirs))
for d in inc_dirs:
    print('   ' + os.path.relpath(d, ROOT))

# index all headers on disk under the source tree
hdr_index = {}
for dirpath, dirnames, filenames in os.walk(ROOT):
    if '.github' in dirpath or '.workbuddy' in dirpath or 'obj' in dirpath or 'bin' in dirpath:
        continue
    for f in filenames:
        if f.endswith('.h'):
            hdr_index.setdefault(f.lower(), set()).add(os.path.normpath(dirpath).lower())

TARGETS = []
for d in ('mountdir', 'debuguty'):
    base = os.path.join(WIN9X, d)
    if not os.path.isdir(base):
        continue
    for f in sorted(os.listdir(base)):
        if f.endswith(('.c', '.cpp', '.h')):
            TARGETS.append(os.path.join(base, f))
# plus the files we edited at the integration points
for f in ('menu.cpp', 'np2.cpp', 'ini.cpp'):
    TARGETS.append(os.path.join(WIN9X, f))
TARGETS.append(os.path.join(ROOT, 'pccore.c'))
TARGETS.append(os.path.join(ROOT, 'i386c', 'ia32', 'exception.c'))
TARGETS.append(os.path.join(ROOT, 'win9x', 'dialog', 'd_disk.cpp'))

# Windows SDK / CRT headers that legitimately arrive via "" from the toolchain
SDK_HEADERS = {'dbt.h', 'windows.h', 'winsock2.h', 'objbase.h', 'malloc.h'}

RE_INC = re.compile(r'^\s*#\s*include\s+"([^"]+)"', re.M)

missing = []
checked = 0
for path in TARGETS:
    if not os.path.isfile(path):
        missing.append('FILE NOT FOUND: ' + os.path.relpath(path, ROOT))
        continue
    src = open(path, 'r', encoding='utf-8', errors='replace').read()
    for m in RE_INC.finditer(src):
        inc = m.group(1).replace('\\', '/')
        checked += 1
        if inc.lower() in SDK_HEADERS:
            continue
        found = False
        for d in inc_dirs + [os.path.dirname(path)]:
            if os.path.isfile(os.path.join(d, inc)):
                found = True
                break
        if not found:
            missing.append('%s -> #include "%s"' % (os.path.relpath(path, ROOT), inc))

print('\nchecked %d quoted includes across %d files' % (checked, len(TARGETS)))
if missing:
    print('\nUNRESOLVED:')
    for m in missing:
        print('  ' + m)
    sys.exit(1)
print('RESULT: ALL INCLUDES RESOLVE')
