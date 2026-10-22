"""Build/run actual portable C++ and generate 320x240 PNGs. Python standard library only."""
from pathlib import Path
import os
import struct
import subprocess
import sys
import zlib

ROOT = Path(__file__).resolve().parents[1]
os.chdir(ROOT)
BUILD = ROOT / 'artifacts' / 'host'
BUILD.mkdir(parents=True, exist_ok=True)
os.environ['ZIG_GLOBAL_CACHE_DIR'] = str(BUILD / 'zig-cache')
import ziglang
ZIG = Path(ziglang.__file__).parent / 'zig.exe'
COMMON = sorted(str(p.relative_to(ROOT)) for p in (ROOT/'lib/MomGenix/src').glob('*.cpp'))

def run(args):
    print('+', ' '.join(map(str, args)), flush=True)
    subprocess.run(list(map(str, args)), check=True)

def compile_cpp(source, name):
    exe = BUILD / (name + '.exe')
    run([ZIG, 'c++', '-std=c++11', '-Wall', '-Wextra', '-Werror', '-pedantic',
         '-Ilib/MomGenix/src', source, *COMMON, '-o', exe])
    return exe

def png(path, w, h, rgb):
    def chunk(kind, data):
        return struct.pack('!I', len(data)) + kind + data + struct.pack('!I', zlib.crc32(kind+data))
    rows = b''.join(b'\x00' + rgb[y*w*3:(y+1)*w*3] for y in range(h))
    path.write_bytes(b'\x89PNG\r\n\x1a\n' + chunk(b'IHDR', struct.pack('!2I5B',w,h,8,2,0,0,0))
                     + chunk(b'IDAT',zlib.compress(rows,9)) + chunk(b'IEND',b''))

run([compile_cpp('tests/state_machine_tests.cpp', 'state_tests')])
PREVIEW = ROOT / 'preview'
PREVIEW.mkdir(exist_ok=True)
run([compile_cpp('tools/preview.cpp', 'preview'), BUILD])
frames = []
for source in sorted(BUILD.glob('*.ppm')):
    header, rgb = source.read_bytes().split(b'\n255\n', 1)
    assert header == b'P6\n320 240' and len(rgb) == 320*240*3
    png(PREVIEW/(source.stem+'.png'),320,240,rgb)
    if source.stem.startswith('message_'): frames.append(rgb)
assert len(frames) == 10
sheet = bytearray(640*1200*3)
for i, rgb in enumerate(frames):
    x, y = (i%2)*320, (i//2)*240
    for row in range(240):
        offset = ((y+row)*640+x)*3
        sheet[offset:offset+960] = rgb[row*960:(row+1)*960]
png(PREVIEW/'all_messages.png',640,1200,sheet)
images = ['startup'] + ['message_%02d'%i for i in range(1,11)] + ['paused']
html = '''<!doctype html><meta charset="utf-8"><title>MOM-GENIX firmware preview</title>
<style>body{background:#181818;color:white;font:16px sans-serif}main{display:flex;flex-wrap:wrap;gap:16px}
figure{margin:0}img{width:320px;height:240px;image-rendering:pixelated}figcaption{padding:8px}</style>
<h1>MOM-GENIX Tier 1A</h1><p>Actual shared C++ renderer at 320 by 240. LCD appearance, viewing distance,
brightness, orientation, color order and timing on the physical board remain unverified.</p><main>'''
html += ''.join(f'<figure><img src="{name}.png" alt="{name}"><figcaption>{name}</figcaption></figure>' for name in images)
(PREVIEW/'index.html').write_text(html+'</main>',encoding='utf-8')
print('PASS 10 message PNGs, startup, paused, contact sheet and local HTML gallery', flush=True)
