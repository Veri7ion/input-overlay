/*************************************************************************
 * This file is part of input-overlay
 * git.vrsal.xyz/alex/input-overlay
 * Copyright 2023 univrsal <uni@vrsal.xyz>.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *************************************************************************/

#pragma once

#include <mutex>
#include <memory>
#include <vector>
#include <map>
#include <atomic>
#include <algorithm>
#include <array>
#include <string>
#include <SDL3/SDL.h>
#include "util/log.h"

#include "sdl_gamepad.hpp"

struct input_data;

namespace gamepad_hook {
extern std::atomic<bool> state;

inline const char *controller_description(SDL_JoystickID id)
{
    switch (SDL_GetGamepadTypeForID(id)) {

#if SDL_VERSION_ATLEAST(2, 24, 0)
    case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_LEFT:
    case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT:
    case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_PAIR:
        return "Nintendo Switch Joy-Con";
#endif
    case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_PRO:
        return "Nintendo Switch Pro Controller";
    case SDL_GAMEPAD_TYPE_PS3:
        return "PS3 Controller";
    case SDL_GAMEPAD_TYPE_PS4:
        return "PS4 Controller";
    case SDL_GAMEPAD_TYPE_PS5:
        return "PS5 Controller";
    case SDL_GAMEPAD_TYPE_XBOX360:
        return "XBox 360 Controller";
    case SDL_GAMEPAD_TYPE_XBOXONE:
        return "XBox One Controller";
    case SDL_GAMEPAD_TYPE_UNKNOWN:
    case SDL_GAMEPAD_TYPE_STANDARD:
        return "Virtual Game Controller";
    default:
        return "Game Controller";
    }
}

class gamepads {
    std::mutex m_mutex{};
    std::map<SDL_JoystickID, std::shared_ptr<sdl_gamepad>> m_pads{};

    static const char *get_sensor_name(SDL_SensorType sensor)
    {
        switch (sensor) {
        case SDL_SENSOR_ACCEL:
            return "accelerometer";
#if SDL_VERSION_ATLEAST(2, 26, 0)
        case SDL_SENSOR_GYRO:
            return "gyro";
        case SDL_SENSOR_ACCEL_L:
            return "accelerometer (L)";
        case SDL_SENSOR_GYRO_L:
            return "gyro (L)";
        case SDL_SENSOR_ACCEL_R:
            return "accelerometer (R)";
        case SDL_SENSOR_GYRO_R:
            return "gyro (R)";
#endif
        default:
            return "UNKNOWN";
        }
    }

public:
    gamepads();

    auto &mutex() { return m_mutex; }
    auto &pads() { return m_pads; }

    void add_controller(SDL_JoystickID id, std::string const &display_name)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_pads.find(id) != m_pads.end()) {
            bdebug("Ignoring duplicate gamepad add event for id %u", static_cast<unsigned>(id));
            return;
        }
        auto pad = std::make_shared<sdl_gamepad>(id, display_name);
        if (!pad->valid() || !static_cast<SDL_Gamepad *>(*pad)) {
            bwarn("Couldn't open gamepad with id %u: %s", static_cast<unsigned>(id), SDL_GetError());
            return;
        }
        const auto base_id = pad->selection_id();
        const auto duplicate_id = std::any_of(m_pads.begin(), m_pads.end(), [&](const auto &entry) {
            return entry.second->selection_id() == base_id;
        });
        if (duplicate_id)
            pad->set_selection_id(base_id + "#instance:" + std::to_string(id));
        m_pads[id] = std::move(pad);
    }

    void remove_controller(SDL_JoystickID id)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_pads.find(id) != m_pads.end()) {
            m_pads[id]->invalidate();
            m_pads.erase(id);
        }
    }

    std::shared_ptr<sdl_gamepad> get_controller_from_selection_id(const std::string &selection_id)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (selection_id.empty() && m_pads.size() == 1)
            return m_pads.begin()->second;

        std::shared_ptr<sdl_gamepad> legacy_match;
        for (const auto &pad : m_pads) {
            if (pad.second->selection_id() == selection_id)
                return pad.second;
            const auto display_label =
                pad.second->display_name() + " (ID " + std::to_string(pad.second->index()) + ")";
            if (display_label == selection_id)
                return pad.second;
            if (pad.second->display_name() == selection_id && !legacy_match)
                legacy_match = pad.second;
        }
        return legacy_match;
    }

    std::shared_ptr<sdl_gamepad> get_controller_from_instance_id(SDL_JoystickID id)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        const auto pad = m_pads.find(id);
        return pad != m_pads.end() ? pad->second : nullptr;
    }

    void event_loop();
};

void start();
void stop();

extern gamepads *local_gamepads;
}
