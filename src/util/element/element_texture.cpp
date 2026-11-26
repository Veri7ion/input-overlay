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

#include "element_texture.hpp"

#include <cmath>

extern "C" {
#include <graphics/image-file.h>
}

namespace {
int scale_to_pixels(int value, float scale)
{
    return static_cast<int>(std::lround(static_cast<float>(value) * scale));
}
}

element_texture::element_texture() : element(ET_TEXTURE)
{
    /* NO-OP */
}

element_texture::element_texture(const element_type type) : element(type)
{
    /* NO-OP */
}

void element_texture::load(const QJsonObject &obj)
{
    read_pos(obj);
    read_mapping(obj);
    m_texture_scale = static_cast<float>(obj["texture_scale"].toDouble(1.0));
}

void element_texture::draw(gs_effect_t *effect, overlay_image_t *image, sources::overlay_settings *settings)
{
    UNUSED_PARAMETER(settings);
    draw(effect, image, &m_mapping, &m_pos);
}

void element_texture::draw(gs_effect_t *effect, overlay_image_t *image, const gs_rect *rect) const
{
    draw(effect, image, rect ? rect : &m_mapping, &m_pos);
}

void element_texture::draw(gs_effect_t *effect, overlay_image_t *image, const gs_rect *rect,
                           const vec2 *pos) const
{
    gs_blend_state_push();
    gs_enable_blending(true);
    gs_blend_function(GS_BLEND_ONE, GS_BLEND_INVSRCALPHA);

    gs_matrix_push();
    gs_effect_set_texture(gs_effect_get_param_by_name(effect, "image"), image->texture);
    gs_matrix_translate3f(pos->x, pos->y, 1.f);
    gs_matrix_scale3f(1.f / m_texture_scale, 1.f / m_texture_scale, 1.f);
    gs_draw_sprite_subregion(image->texture, 0, scale_to_pixels(rect->x, m_texture_scale),
                             scale_to_pixels(rect->y, m_texture_scale),
                             scale_to_pixels(rect->cx, m_texture_scale),
                             scale_to_pixels(rect->cy, m_texture_scale));
    gs_matrix_pop();

    gs_blend_state_pop();
}

void element_texture::draw(gs_effect *effect, overlay_image_t *image, const gs_rect *rect,
                           const vec2 *pos, const float angle) const
{
    gs_effect_set_texture(gs_effect_get_param_by_name(effect, "image"), image->texture);

    gs_matrix_push();
    {
        auto cx = static_cast<float>(rect->cx);
        auto cy = static_cast<float>(rect->cy);
        const auto scale = m_texture_scale;
        /* Put into position */
        gs_matrix_translate3f(pos->x, pos->y + cy, 1.f);
        gs_matrix_scale3f(1.f / scale, 1.f / scale, 1.f);

        /* Offset to rotation center (I think :P, rotations are tricky) */
        gs_matrix_translate3f(-(cx / 2.f), -(cy / 2.f), 1.f);
        gs_matrix_rotaa4f(0.f, 0.f, 1.f, angle);

        /* Offset back */
        gs_matrix_translate3f(-(cx / 2.f), -(cy / 2.f), 1.f);

        gs_draw_sprite_subregion(image->texture, 0, scale_to_pixels(rect->x, m_texture_scale),
                                 scale_to_pixels(rect->y, m_texture_scale),
                                 scale_to_pixels(rect->cx, m_texture_scale),
                                 scale_to_pixels(rect->cy, m_texture_scale));
    }
    gs_matrix_pop();
}