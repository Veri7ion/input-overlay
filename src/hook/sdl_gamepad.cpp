#include "sdl_gamepad.hpp"
#include <input_data.hpp>
#include <algorithm>

void sdl_gamepad::copy_data(input_data *data, std::string name)
{
    for (int i = 0; i < SDL_GAMEPAD_BUTTON_COUNT; ++i) {
        data->gamepad_buttons[name][i] = m_buttons[i];
    }

    for (int i = 0; i < SDL_GAMEPAD_AXIS_COUNT; ++i) {
        data->gamepad_axis[name][i] = m_axis[i];
    }

    for (int i = 0; i < SDL_GAMEPAD_CAPSENSE_COUNT; ++i) {
        auto type = static_cast<SDL_GamepadCapSenseType>(i);
        data->gamepad_capsense[name][i] =
            SDL_GamepadHasCapSense(m_controller, type) && SDL_GetGamepadCapSense(m_controller, type);
    }

    auto &touchpads = data->gamepad_touchpads[name];
    touchpads = {};
    auto count = std::min(SDL_GetNumGamepadTouchpads(m_controller),
                          static_cast<int>(input_data::gamepad_touchpad_count));
    for (int i = 0; i < count; ++i) {
        bool down{};
        float x{}, y{}, pressure{};
        if (SDL_GetNumGamepadTouchpadFingers(m_controller, i) > 0 &&
            SDL_GetGamepadTouchpadFinger(m_controller, i, 0, &down, &x, &y, &pressure)) {
            touchpads[i] = {down, x, y, pressure};
        }
    }
}
