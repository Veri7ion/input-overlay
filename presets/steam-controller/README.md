# Steam Controller

These presets use an 800 x 800 canvas and the controller artwork and colors from the native Steam Controller Gamepad Viewer. Select either `steam-controller.json` or `steam-controller-no-adaptive-trigger-fill.json` as the layout and use `steam-controller.png` as its image in the Input Overlay source.

The atlas is tightly packed at 1781 x 1463 pixels and rasterized at 4/3 density from vector artwork while the layout remains 800 x 800. The controller artwork ends at its mapped 800 x 800 boundary, keeping it separate from the sprite cells; idle reference sprites have transparent backgrounds except for the filled idle paddles. This keeps the atlas pixel area about 20% smaller than the previous 2048 x 1600 image while giving the artwork more source detail. Idle button reference sprites are included in the atlas for easier visual editing; the runtime continues to use the idle artwork in the controller background to avoid drawing these references twice.

The artwork uses a `#2f2f2f` body at 85% opacity, white idle lines at 55%, and `#25a7ff` pressed inputs at 85%. Idle trigger fill is disabled, trigger outline opacity is 1%, and grip sensing uses contour lines without shine.

`steam-controller.json` fills triggers progressively. The alternate `steam-controller-no-adaptive-trigger-fill.json` fills each trigger completely when its axis reaches 1% (or more).

## Mapped inputs

- Face buttons: A, B, X, Y (SDL button codes 0-3)
- View, Steam/Guide, Menu, stick clicks, bumpers, QAM, and paddles (codes 4-10 and 15-19)
- D-pad directions, including diagonal combinations (codes 11-14)
- Left and right trigger axes
- Left and right stick axes and capacitive touch
- Both trackpad positions, pressure, and click state (codes 20-21)
- Left and right grip capacitive touch

Capacitive indices are 0 left stick, 1 right stick, 2 left grip, and 3 right grip. Trackpad indices are 0 left and 1 right. Gyro is intentionally not included.

Touchpad cursors and outlines are hidden while idle. Their positions use the native viewer's rotated pad transforms; pressing a pad shows the pressure-sensitive cursor ring. The small stick-center dots remain visible at rest; the larger markers appear in gray on capacitive touch and turn blue when moved. Stick clicks only highlight the stick area. Grip sensing uses the native viewer's three white contour paths per side.
