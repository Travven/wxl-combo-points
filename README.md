# wxl-combo-points

WarcraftXL module that ports the WotLK-Extensions universal combo-point fix to the WarcraftXL extension ABI.

## Verified client data

For the supplied WotLK 3.3.5a executables:

- PE ImageBase: `0x00400000`
- Patch VA: `0x00611707`
- Patch RVA: `0x00211707`
- Original bytes: `74 11`
- Patched bytes: `EB 11`

`74 11` is `JE +0x11`; `EB 11` is `JMP +0x11`.

## Build

The current WarcraftXL v1.1 build shell auto-discovers extension folders placed under `core/extensions/`, so this repository intentionally has no standalone CMakeLists.txt of its own.

For a local build, copy this folder into:

`<wxl-core>/extensions/wxl-combo-points/`

Then configure/build the `wxl-combo-points` target with the same Win32 toolchain used by wxl-core.

## Install

Place the resulting `wxl-combo-points.dll` where your WarcraftXL installation discovers extensions.

Do not simultaneously load WotLK-Extensions' combo-point fix against the same client; both target the same instruction and one is sufficient.
