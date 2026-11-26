#pragma once
#include <string>
#include <array>
#include <mutex>
#include <atomic>
#include <utility>
#include <SDL3/SDL.h>

struct input_data;

class sdl_gamepad {
protected:
    std::string m_display_name{};
    std::string m_device_id{};
    std::string m_selection_id{};
    SDL_Gamepad *m_controller{};
    int m_player_index{};
    std::atomic<bool> m_valid{true};

    // We save sdl event data into these arrays and copy them in the source tick method
    // Overlay elements should not access these directly
    std::array<bool, SDL_GAMEPAD_BUTTON_COUNT> m_buttons{};
    std::array<float, SDL_GAMEPAD_AXIS_COUNT> m_axis{};
    std::mutex m_mutex{};
    SDL_JoystickID m_jid{};

public:
    explicit sdl_gamepad(SDL_JoystickID jid, std::string const &display_name)
    {
        m_controller = SDL_OpenGamepad(jid);
        m_jid = jid;
        m_display_name = display_name;
        if (m_controller) {
            const char *serial = SDL_GetGamepadSerial(m_controller);
            const char *path = SDL_GetGamepadPath(m_controller);
            if (serial && *serial)
                m_device_id = std::string("serial:") + serial;
            else if (path && *path)
                m_device_id = std::string("path:") + path;
            else
                m_device_id = std::string("instance:") + std::to_string(jid);
            m_selection_id = m_device_id;
            m_player_index = SDL_clamp(SDL_GetGamepadPlayerIndex(m_controller), 0, 4);
        } else {
            m_valid = false;
        }
    }

    operator SDL_Gamepad *() const { return m_controller; }

    SDL_JoystickID index() const { return m_jid; }      // SDL index
    int player_index() const { return m_player_index; } // player 1 through 4

    std::string display_name() const { return m_display_name; }
    std::string device_id() const { return m_device_id; }
    std::string selection_id() const { return m_selection_id; }
    void set_selection_id(std::string id) { m_selection_id = std::move(id); }

    void invalidate()
    {
        m_valid = false;
        SDL_CloseGamepad(m_controller);
        m_controller = {};
    }

    bool valid() const { return m_valid; }
    void copy_data(input_data *data, std::string name);

    auto &buttons() { return m_buttons; }
    auto &axis() { return m_axis; }
    auto &mutex() { return m_mutex; }

    ~sdl_gamepad() { invalidate(); }
};
