#ifndef LEL_ARGBblendmodes_h_
#define LEL_ARGBblendmodes_h_

#include <map>
#include <FastLED.h>

typedef enum
{
    NORMAL,
    DISSOLVE,
    SCREEN,
    OVERLAY,
    HARD_LIGHT,
    // SOFT_LIGHT,
    COLOUR_DODGE,
    LINEAR_DODGE,
    // COLOUR_BURN,
    // LINEAR_BURN,
    ADD,
    // SUBTRACT,
    MULTIPLY,
    DIVIDE,
    DIFFERENCE,
    DARKEN,
    LIGHTEN,
    HUE,
    // SATURATION,
    // COLOUR,
    // LUMINOSITY
} RGB_BLEND_MODE;

static CRGB &blend_normal(CRGB &existing, const CRGB &overlay, uint8_t overlay_alpha)
{
    return nblend(existing, overlay, overlay_alpha);
}

static CRGB &blend_additive(CRGB &existing, const CRGB &overlay, uint8_t overlay_alpha)
{
    existing.r = qadd8(existing.r, scale8(overlay.r, overlay_alpha));
    existing.g = qadd8(existing.g, scale8(overlay.g, overlay_alpha));
    existing.b = qadd8(existing.b, scale8(overlay.b, overlay_alpha));
    return existing;
}

static CRGB &blend_dissolve(CRGB &existing, const CRGB &overlay, uint8_t overlay_alpha)
{
    if (overlay_alpha == 255)
    {
        existing = overlay;
    }
    else if (overlay_alpha > 0)
    {
        if (random8(0xFF) < overlay_alpha)
        {
            existing = overlay;
        }
    }
    return existing;
}

static CRGB &blend_multiply(CRGB &existing, const CRGB &overlay, uint8_t overlay_alpha)
{
    existing.r = blend(existing.r, scale8(existing.r, overlay.r), overlay_alpha);
    existing.g = blend(existing.g, scale8(existing.g, overlay.g), overlay_alpha);
    existing.b = blend(existing.b, scale8(existing.b, overlay.b), overlay_alpha);
    return existing;
}

static CRGB &blend_screen(CRGB &existing, const CRGB &overlay, uint8_t overlay_alpha)
{
    existing.r = blend(existing.r, -scale8(-existing.r, -overlay.r), overlay_alpha);
    existing.g = blend(existing.g, -scale8(-existing.g, -overlay.g), overlay_alpha);
    existing.b = blend(existing.b, -scale8(-existing.b, -overlay.b), overlay_alpha);
    return existing;
}

static CRGB &blend_overlay(CRGB &existing, const CRGB &overlay, uint8_t overlay_alpha)
{
    if (existing.r < 128)
    {
        existing.r = blend(existing.r, 2 * (scale8(existing.r, overlay.r)), overlay_alpha);
    }
    else
    {
        existing.r = blend(existing.r, 2 * (255 - scale8(255 - existing.r, 255 - overlay.r)), overlay_alpha);
    }
    if (existing.g < 128)
    {
        existing.g = blend(existing.g, 2 * (scale8(existing.g, overlay.g)), overlay_alpha);
    }
    else
    {
        existing.g = blend(existing.g, 2 * (255 - scale8(255 - existing.g, 255 - overlay.g)), overlay_alpha);
    }
    if (existing.b < 128)
    {
        existing.b = blend(existing.b, 2 * (scale8(existing.b, overlay.b)), overlay_alpha);
    }
    else
    {
        existing.b = blend(existing.b, 2 * (255 - scale8(255 - existing.b, 255 - overlay.b)), overlay_alpha);
    }

    return existing;
}

static CRGB &blend_hard_light(CRGB &existing, const CRGB &overlay, uint8_t overlay_alpha)
{
    if (overlay.r < 128)
    {
        existing.r = blend(existing.r, 2 * (scale8(existing.r, overlay.r)), overlay_alpha);
    }
    else
    {
        existing.r = blend(existing.r, 2 * (-scale8(-existing.r, -overlay.r)), overlay_alpha);
    }
    if (overlay.g < 128)
    {
        existing.g = blend(existing.g, 2 * (scale8(existing.g, overlay.g)), overlay_alpha);
    }
    else
    {
        existing.g = blend(existing.g, 2 * (-scale8(-existing.g, -overlay.g)), overlay_alpha);
    }
    if (overlay.b < 128)
    {
        existing.b = blend(existing.b, 2 * (scale8(existing.b, overlay.b)), overlay_alpha);
    }
    else
    {
        existing.b = blend(existing.b, 2 * (-scale8(-existing.b, -overlay.b)), overlay_alpha);
    }

    return existing;
}

static CRGB &blend_darken(CRGB &existing, const CRGB &overlay, uint8_t overlay_alpha)
{
    existing.r = blend(existing.r, min(existing.r, overlay.r), overlay_alpha);
    existing.g = blend(existing.g, min(existing.g, overlay.g), overlay_alpha);
    existing.b = blend(existing.b, min(existing.b, overlay.b), overlay_alpha);
    return existing;
}

static CRGB &blend_lighten(CRGB &existing, const CRGB &overlay, uint8_t overlay_alpha)
{
    existing.r = blend(existing.r, max(existing.r, overlay.r), overlay_alpha);
    existing.g = blend(existing.g, max(existing.g, overlay.g), overlay_alpha);
    existing.b = blend(existing.b, max(existing.b, overlay.b), overlay_alpha);
    return existing;
}

static CRGB &blend_divide(CRGB &existing, const CRGB &overlay, uint8_t overlay_alpha)
{
    existing.r = blend(existing.r, ((uint16_t)existing.r * 256) / (overlay.r + 1), overlay_alpha);
    existing.g = blend(existing.g, ((uint16_t)existing.g * 256) / (overlay.g + 1), overlay_alpha);
    existing.b = blend(existing.b, ((uint16_t)existing.b * 256) / (overlay.b + 1), overlay_alpha);
    return existing;
}

static CRGB &blend_colour_dodge(CRGB &existing, const CRGB &overlay, uint8_t overlay_alpha)
{
    return blend_divide(existing, -overlay, overlay_alpha);
}

static CRGB &blend_linear_dodge(CRGB &existing, const CRGB &overlay, uint8_t overlay_alpha)
{
    return nblend(existing, existing + overlay, overlay_alpha);
}

static CRGB &blend_hue(CRGB &existing, const CRGB &overlay, uint8_t overlay_alpha)
{
    CHSV ex_hsv = rgb2hsv_approximate(existing);
    CHSV ov_hsv = rgb2hsv_approximate(overlay);
    if (overlay_alpha == 255)
    {
        ex_hsv.h = ov_hsv.h;
    }
    else
    {
        uint8_t delta_h = ov_hsv.h - ex_hsv.h;

        if (delta_h < 127)
        {
            ex_hsv.h = ex_hsv.h + scale8(delta_h, overlay_alpha);
        }
        else
        {
            delta_h = -delta_h;
            ex_hsv.h = ex_hsv.h - scale8(delta_h, overlay_alpha);
        }
    }

    hsv2rgb_rainbow(ex_hsv, existing);
    return existing;
}

// map of blend modes to their associated finctions
const std::map<RGB_BLEND_MODE, CRGB &(*)(CRGB &, const CRGB &, uint8_t)> blend_mode_map = {
    {NORMAL, blend_normal},
    {DISSOLVE, blend_dissolve},
    {SCREEN, blend_screen},
    {OVERLAY, blend_overlay},
    {HARD_LIGHT, blend_hard_light},
    // {SOFT_LIGHT, blend_soft_light},
    {COLOUR_DODGE, blend_colour_dodge},
    {LINEAR_DODGE, blend_linear_dodge},
    // {COLOUR_BURN, blend_colour_burn},
    // {LINEAR_BURN, blend_linear_burn},
    {ADD, blend_additive},
    // {SUBTRACT, blend_subtract},
    {MULTIPLY, blend_multiply},
    {DIVIDE, blend_divide},
    // {DIFFERENCE, blend_difference},
    {DARKEN, blend_darken},
    {LIGHTEN, blend_lighten},
    {HUE, blend_hue},
    // {SATURATION, blend_saturation},
    // {COLOUR, blend_colour},
    // {LUMINOSITY, blend_luminosity}
};

typedef CRGB& (*blend_mode_fn_ptr)(CRGB &, const CRGB &, uint8_t);

#endif