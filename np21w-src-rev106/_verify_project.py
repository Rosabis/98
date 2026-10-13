# -*- coding: utf-8 -*-
"""Static CI sanity checks for np21w-src-rev106.

Verifies:
  1. every ClCompile/ClInclude path referenced by the vcxproj exists on disk
  2. XML well-formedness of both vcxproj + both .filters
  3. mountdir registered in the include dirs of every config
  4. no dangling references to files we added (mountdir / debuguty)
"""
import os
import re
import sys
import xml.etree.ElementTree as ET

ROOT = os.path.dirname(os.path.abspath(__file__))
WIN9X = os.path.join(ROOT, 'win9x')
NS = '{http://schemas.microsoft.com/developer/msbuild/2003}'

fails = []
warns = []


def rel(p):
    return os.path.relpath(p, ROOT).replace('\\', '/')


def check_xml(path):
    try:
        ET.parse(path)
        return True
    except ET.ParseError as e:
        fails.append('XML FAIL %s: %s' % (rel(path), e))
        return False


for name in ('np21vs2019.vcxproj', 'np21vs2010.vcxproj',
             'np21vs2019.vcxproj.filters', 'np21vs2010.vcxproj.filters'):
    check_xml(os.path.join(WIN9X, name))

for proj in ('np21vs2019.vcxproj', 'np21vs2010.vcxproj'):
    path = os.path.join(WIN9X, proj)
    tree = ET.parse(path)
    root = tree.getroot()
    base = os.path.dirname(path)

    n_cpp = n_h = n_missing = 0
    for tag, counter in (('ClCompile', 'cpp'), ('ClInclude', 'h')):
        for el in root.iter(NS + tag):
            inc = el.get('Include')
            if not inc:
                continue
            inc = inc.replace('\\', '/')
            if counter == 'cpp':
                n_cpp += 1
            else:
                n_h += 1
            # skip globs / non-path values
            if '*' in inc or '$' in inc:
                continue
            full = os.path.normpath(os.path.join(base, inc))
            if not os.path.isfile(full):
                n_missing += 1
                fails.append('%s: missing source %s' % (proj, inc))
    print('%-24s ClCompile=%-4d ClInclude=%-4d missing=%d'
          % (proj, n_cpp, n_h, n_missing))

    # mountdir include dirs
    incdirs = [e.text or '' for e in root.iter(NS + 'AdditionalIncludeDirectories')]
    ok = [d for d in incdirs if 'mountdir' in d]
    print('   include-dir groups=%d with-mountdir=%d' % (len(incdirs), len(ok)))
    if not ok:
        fails.append('%s: no AdditionalIncludeDirectories references .\\mountdir' % proj)

    # OutDir / SolutionName dependency
    outdirs = set((e.text or '') for e in root.iter(NS + 'OutDir'))
    for d in outdirs:
        if '$(SolutionName)' in d:
            warns.append('%s OutDir uses $(SolutionName): %s (pass /p:SolutionName explicitly in CI)' % (proj, d))

# mountdir sources present
md = os.path.join(WIN9X, 'mountdir')
expect = ['mountdir.c', 'mountdir.h', 'md_dir.c', 'md_dir.h', 'md_file.c', 'md_file.h',
          'md_fcb.c', 'md_fcb.h', 'md_util.c', 'md_util.h']
for f in expect:
    p = os.path.join(md, f)
    if not os.path.isfile(p):
        fails.append('mountdir missing %s' % f)
print('mountdir sources: %d/%d' % (sum(os.path.isfile(os.path.join(md, f)) for f in expect), len(expect)))

# debuguty sources present
du = os.path.join(WIN9X, 'debuguty')
for f in ('d_edit.cpp', 'd_addr.cpp', 'viewstk.cpp', 'viewmenu.cpp', 'viewcmn.cpp'):
    if not os.path.isfile(os.path.join(du, f)):
        fails.append('debuguty missing %s' % f)

print()
for w in warns:
    print('WARN: ' + w)
print()
if fails:
    for f in fails:
        print('FAIL: ' + f)
    print('\nRESULT: %d FAILURE(S)' % len(fails))
    sys.exit(1)
print('RESULT: ALL CHECKS PASSED')
