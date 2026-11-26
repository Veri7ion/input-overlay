# These are the presets made for version 5.0 and up

These are only the latest preset versions.
You should always use the presets shipped with the version you downloaded.

## SDL3 touchpad and capsense inputs

The SDL3 development revision used by this fork exposes up to two gamepad touchpads and four capacitive sensors. The first finger on each touchpad is available to layouts as normalized position and pressure.

To make an existing type `5` analog-stick cursor follow a touchpad, add `touchpad_index` to that layout element:

```json
{
  "type": 5,
  "touchpad_index": 0,
  "stick_radius": 30
}
```

Use index `0` for the left touchpad and `1` for the right. The cursor moves while touched and scales with pressure; pressure of `0.75` or higher uses the pressed texture.

To use a capacitive sensor with an existing type `2` gamepad-button element, add a `capsense` index:

```json
{
  "type": 2,
  "code": 0,
  "capsense": 2
}
```

Capacitive indices are `0` left stick, `1` right stick, `2` left grip, and `3` right grip. Regular button codes continue to use SDL's gamepad-button indices; SDL codes `16`–`19` are right paddle 1, left paddle 1, right paddle 2, and left paddle 2, respectively.
