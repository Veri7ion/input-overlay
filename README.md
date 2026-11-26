Everything my fork adds is 100% AI Vibe-coded.\

Steam controller 2026 support with a preset inspired by [Steam-Controller-Gamepad-Viewer-by-Ramonchi_5](https://github.com/ramonchi5/Steam-Controller-Gamepad-Viewer-by-Ramonchi_5).\

Not properly tested with the config creation tool (cct) of input-overlay.

https://github.com/user-attachments/assets/13b7abf4-2e4e-462d-96aa-d7a32016ebbc




![logo](./docs/io.png)

[![Push to master](https://github.com/univrsal/input-overlay/actions/workflows/push.yaml/badge.svg)](https://github.com/univrsal/input-overlay/actions/workflows/push.yaml)

Show keyboard, mouse and gamepad input on stream.\
Available for OBS Studio on Windows and Linux (64bit).
Head over to [releases](https://github.com/univrsal/input-overlay/releases) for binaries.

## [Wiki](https://github.com/univrsal/input-overlay/wiki)
## [Installation](https://github.com/univrsal/input-overlay/wiki/Installation)
## Credits
input-overlay depends on [libuiohook](https://github.com/kwhat/libuiohook) by [kwhat](https://github.com/kwhat) licensed under the [GNU General Public License v3.0](https://www.gnu.org/licenses/gpl-3.0.txt), [mongoose](https://github.com/cesanta/mongoose) licensed under the [GNU General Public License v2.0](https://www.gnu.org/licenses/gpl-2.0.txt), and [SDL3](https://libsdl.org) licensed under the [zlib license](https://www.zlib.net/zlib_license.html).

The current SDL3 dependency is pinned to a development revision that provides gamepad capacitive-touch APIs. See [preset notes](./presets/README.md) for using touchpad and capacitive-touch inputs in layouts.

This file separates the project's original MIT-licensed code from third-party components, references, and assets.

## Valve / Steam

- Component: Steam Controller name, Steam name, controller layout captures, and `src/SteamControllerGamepadViewer/wwwroot/assets/controller_config_controller_triton.svg`.
- Source: Steam client Controller Layout / Steam Controller tester UI.
- Notice: Steam, Steam Controller, Valve names, trademarks, UI assets, and controller artwork are property of Valve Corporation. They are not licensed under this repository's MIT license. This project is unofficial and is not endorsed by Valve.
- Reference: https://store.steampowered.com/subscriber_agreement/

## SDL3

- Component: SDL3 dynamic library and gamepad APIs.
- Source: https://github.com/libsdl-org/SDL
- License: zlib license, https://github.com/libsdl-org/SDL/blob/main/LICENSE.txt
- Use in this project: the code dynamically loads `SDL3.dll` from the app folder or an installed Steam folder. The repository does not vendor SDL3 source code.

SDL3 license notice:

```text
Copyright (C) 1997-2026 Sam Lantinga <slouken@libsdl.org>

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
```

## More Information:
- [OBS resource page](https://obsproject.com/forum/resources/input-overlay.552/)
- [Config creation tool](https://univrsal.github.io/input-overlay/cct/)
- [Convert old *.ini presets to JSON (WIP)](https://univrsal.github.io/input-overlay/converter/)
