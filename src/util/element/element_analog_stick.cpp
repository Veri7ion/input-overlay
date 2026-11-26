/*************************************************************************
 * This file is part of input-overlay
 * git.vrsal.cc/alex/input-overlay
 * Copyright 2025 univrsal <uni@vrsal.xyz>.
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

#include "element_analog_stick.hpp"
#include "../../sources/input_source.hpp"
#include <QJsonArray>
#include <SDL3/SDL.h>
#include <keycodes.h>
#include <algorithm>
#include <cmath>

void element_analog_stick::load(const QJsonObject &obj)
{
    element_texture::load(obj);
    m_side = static_cast<element_side>(obj[CFG_SIDE].toInt());
    m_radius = static_cast<int32_t>(obj[CFG_STICK_RADIUS].toInt());
    m_touchpad_index = obj["touchpad_index"].toInt(-1);
    m_capsense = obj["capsense"].toInt(-1);
    m_touchpad_click_code = obj["touchpad_click_code"].toInt(-1);
    m_touchpad_outline = obj["touchpad_outline"].toBool(false);
    const auto transform = obj["touchpad_transform"].toArray();
    m_has_touchpad_transform = transform.size() == 4;
    if (m_has_touchpad_transform) {
        for (int i = 0; i < 4; ++i)
            m_touchpad_transform[i] = static_cast<float>(transform[i].toDouble());
    }

    const auto click_mapping = obj["touchpad_click_mapping"].toArray();
    m_has_touchpad_click_mapping = click_mapping.size() == 4;
    if (m_has_touchpad_click_mapping) {
        m_touchpad_click_mapping = {
            click_mapping[0].toInt(),
            click_mapping[1].toInt(),
            click_mapping[2].toInt(),
            click_mapping[3].toInt(),
        };
    }

    m_keycode = (m_side == element_side::LEFT) ? SDL_GAMEPAD_BUTTON_LEFT_STICK : SDL_GAMEPAD_BUTTON_RIGHT_STICK;
    m_pressed = m_mapping;
    m_pressed.y = m_mapping.y + m_mapping.cy + CFG_INNER_BORDER;
}

void element_analog_stick::draw(gs_effect_t *effect, overlay_image_t *image, sources::overlay_settings *settings)
{
    auto pos = m_pos;

    if (m_touchpad_index >= 0) {
        auto touchpad = settings->pad_touchpad(m_touchpad_index);
        const auto pressure = std::clamp(touchpad.pressure, 0.0f, 1.0f);
        const bool touched = touchpad.down || pressure > 0.02f;
        if (!touched)
            return;

        if (m_touchpad_outline) {
            element_texture::draw(effect, image, &m_pressed, &m_pos);
            return;
        }

        const auto x = std::clamp(touchpad.x, 0.0f, 1.0f);
        const auto y = std::clamp(touchpad.y, 0.0f, 1.0f);
        if (m_has_touchpad_transform) {
            const auto center_x = m_pos.x + m_mapping.cx / 2.0f;
            const auto center_y = m_pos.y + m_mapping.cy / 2.0f;
            pos.x = center_x + (x - 0.5f) * m_touchpad_transform[0] +
                    (y - 0.5f) * m_touchpad_transform[2] - m_mapping.cx / 2.0f;
            pos.y = center_y + (x - 0.5f) * m_touchpad_transform[1] +
                    (y - 0.5f) * m_touchpad_transform[3] - m_mapping.cy / 2.0f;
        } else {
            pos.x += (x - 0.5f) * 2.0f * static_cast<float>(m_radius);
            pos.y += (y - 0.5f) * 2.0f * static_cast<float>(m_radius);
        }

        const bool button_clicked = m_touchpad_click_code >= 0 &&
                                   settings->is_pad_button_pressed(m_touchpad_click_code);
        const bool clicked = button_clicked || pressure >= 0.05f;
        if (!clicked) {
            element_texture::draw(effect, image, &m_mapping, &pos);
            return;
        }

        const auto strength = std::max(pressure, button_clicked ? 0.5f : 0.0f);
        const auto scale = (12.0f + 8.0f * strength) / 20.0f;
        gs_matrix_push();
        gs_matrix_translate3f(pos.x + m_mapping.cx / 2.0f, pos.y + m_mapping.cy / 2.0f, 1.0f);
        gs_matrix_scale3f(scale, scale, 1.0f);
        gs_matrix_translate3f(-(pos.x + m_mapping.cx / 2.0f), -(pos.y + m_mapping.cy / 2.0f), 1.0f);
        element_texture::draw(effect, image, &m_pressed, &pos);
        gs_matrix_pop();
        if (m_has_touchpad_click_mapping)
            element_texture::draw(effect, image, &m_touchpad_click_mapping, &pos);
        return;
    }

    const gs_rect *temp = &m_mapping;
    float axis_x = 0.0f;
    float axis_y = 0.0f;
    if (settings->gamepad || settings->remote_input_data) {
        const auto axis_x_code = m_side == element_side::LEFT ? SDL_GAMEPAD_AXIS_LEFTX : SDL_GAMEPAD_AXIS_RIGHTX;
        const auto axis_y_code = m_side == element_side::LEFT ? SDL_GAMEPAD_AXIS_LEFTY : SDL_GAMEPAD_AXIS_RIGHTY;
        axis_x = settings->pad_axis(axis_x_code);
        axis_y = settings->pad_axis(axis_y_code);
        pos.x += axis_x * static_cast<float>(m_radius);
        pos.y += axis_y * static_cast<float>(m_radius);
    }

    if (m_capsense >= 0) {
        const bool touched = settings->is_pad_button_pressed(input_data::gamepad_capsense_code_base + m_capsense);
        const bool moving = std::sqrt(axis_x * axis_x + axis_y * axis_y) > 0.08f;
        if (!touched && !moving)
            return;
        temp = moving ? &m_pressed : &m_mapping;
    } else {
        temp = settings->is_pad_button_pressed(m_keycode) ? &m_pressed : &m_mapping;
    }

    element_texture::draw(effect, image, temp, &pos);
}
