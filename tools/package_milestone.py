"""Preserve the locally verified Tier 1A source and flashing inputs, with SHA256."""
from pathlib import Path
import argparse
import hashlib
import shutil
import zipfile

root = Path(__file__).resolve().parents[1]
out = root / 'milestones' / 'tier1a-2026-10-22'
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('--refresh', action='store_true', help='Explicitly replace the initial software snapshot')
args = parser.parse_args()
if (out/'source.zip').exists() and not args.refresh:
    raise SystemExit('Milestone already exists; preserve it. Use a separate milestone for later hardware/optional work.')
out.mkdir(parents=True, exist_ok=True)
for profile in ('cyd_ili9341', 'cyd_st7789'):
    target = out / profile
    target.mkdir(exist_ok=True)
    for name in ('firmware.bin', 'bootloader.bin', 'partitions.bin'):
        shutil.copy2(root/'.pio/build'/profile/name, target/name)
    shutil.copy2(root/'.platformio/packages/framework-arduinoespressif32/tools/partitions/boot_app0.bin',
                 target/'boot_app0.bin')

(out/'README.md').write_text('''# Tier 1A software milestone

October 22, 2026. Both profiles compile and actual host state-machine tests pass.
No upload, physical validation or hardware-tested release is claimed.

`source.zip` preserves source, pinned dependencies, specification, previews, logs
and instructions. Extract into a new folder to rebuild using the root README.
The default profiles in that archive are the costume firmware. Diagnostic profiles
remain available for identification and calibration. No optional behavior is enabled.

The two profile folders contain exact build outputs plus the pinned framework's
boot_app0.bin. SHA256SUMS.txt covers this package, including the source archive.
Choose the controller only after board identification and calibration. Do not
connect the USB stick and PC simultaneously. Prefer normal PlatformIO upload.

To restore these exact binaries, after installing the pinned build dependencies,
run the command below from this milestone folder. Replace COM5 and profile as
needed. This command has NOT been run against a physical board.

```powershell
..\\..\\.venv\\Scripts\\python.exe ..\\..\\.platformio\\packages\\tool-esptoolpy\\esptool.py --chip esp32 --port COM5 --baud 115200 write_flash --flash_mode dio --flash_freq 40m --flash_size 4MB 0x1000 cyd_ili9341/bootloader.bin 0x8000 cyd_ili9341/partitions.bin 0xe000 cyd_ili9341/boot_app0.bin 0x10000 cyd_ili9341/firmware.bin
```

Offsets match the pinned classic ESP32 PlatformIO builder. Confirm the actual
board is that ESP32 variant with 4MB flash before using these inputs. Passing
hardware calibration may require a fresh build; preserve that later milestone
separately rather than calling this uncalibrated snapshot hardware-tested.
''', encoding='utf-8')

files = []
for dirname in ('src', 'include', 'lib', 'tests', 'tools', 'docs', 'preview'):
    files += [p for p in (root/dirname).rglob('*') if p.is_file() and '__pycache__' not in p.parts]
files += [root/name for name in ('.gitignore','platformio.ini','README.md','REVIEW_PACKET.md',
          'HARDWARE_CHECKLIST.md','THIRD_PARTY_NOTICES.txt','build-results.txt',
          'diagnostic-build-results.txt','test-results.txt','requirements-build.txt',
          'requirements-build-lock.txt','requirements-host.txt')]
with zipfile.ZipFile(out/'source.zip','w',zipfile.ZIP_DEFLATED) as archive:
    for path in sorted(files): archive.write(path,path.relative_to(root))
with zipfile.ZipFile(out/'source.zip','r') as archive:
    assert archive.testzip() is None
manifest = []
for path in sorted(out.rglob('*')):
    if path.is_file() and path.name != 'SHA256SUMS.txt':
        manifest.append(hashlib.sha256(path.read_bytes()).hexdigest()+'  '+path.relative_to(out).as_posix())
(out/'SHA256SUMS.txt').write_text('\n'.join(manifest)+'\n',encoding='ascii')
print(f'Preserved {len(files)} source/artifact files and both binary profiles in {out}')
print(f'{len(manifest)} checksums written; source.zip integrity verified')
