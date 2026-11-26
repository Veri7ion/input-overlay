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

## More Information:
- [OBS resource page](https://obsproject.com/forum/resources/input-overlay.552/)
- [Config creation tool](https://univrsal.github.io/input-overlay/cct/)
- [Convert old *.ini presets to JSON (WIP)](https://univrsal.github.io/input-overlay/converter/)
