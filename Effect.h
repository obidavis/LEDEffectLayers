#ifndef Effect_h_
#define Effect_h_

#include <FastLED.h>
#include "BlendModes.h"


/// 
class Effect : public CPixelView<CRGB>
{
public:
    Effect(CRGB *_leds, int _len)
        : CPixelView<CRGB>(_leds, _len)
    {
    }

    Effect(CRGB *_leds, int _start, int _end)
        : CPixelView<CRGB>(_leds, _start, _end)
    {
    }

    /// @brief specify the blend mode for the effect.
    /// @details The blend mode specifies how the effect is blended with the
    /// effects beneath it in a Scene. The default is NORMAL.
    /// @param blend_mode the blend mode to use. See BlendModes.h for the available modes.
    void setBlendMode(blend_mode_fn_ptr mode) { blend_fn = mode; }

    /// @brief set the refresh rate of the effect.
    /// @details The refresh rate is the minimum interval at
    /// which the render() method is called. The default is 20 i.e. 50fps
    /// @param refresh_rate the refresh rate in milliseconds.
    void setRefreshPeriod(uint16_t period) { refresh_period = period; }

    /// @brief set the alpha value of the effect.
    /// @details The alpha value is the amount of the effect that is blended with the
    /// effects beneath it in a Scene. The default is 255 i.e. fully opaque.
    /// Note that the alpha value is only used if the effect is added to a Scene.
    /// @param alpha the alpha value.
    void setAlpha(uint8_t new_alpha) { alpha = new_alpha; }

    /// @brief get the next pixel value at the given index.
    /// @details This is the heart of the effect, governing
    /// the way the effect is rendered. It is called by the update function
    /// for each pixel in range. This should be overridden by
    /// subclasses to implement the effect.
    /// @param index the index of the pixel to render.
    virtual CRGB renderPixel(int i) {}

    /// @brief Update any internal state of the effect.
    /// @details This method is called by the update function
    /// only once to update the effect every frame. If the effect relies on
    /// data from peripherals or time-based state, it should
    /// be updated here.
    virtual void onFrameBegin(void) { }

    /// @brief Update the effect.
    /// @details Check if enough time has elapsed to render the next frame.
    /// If so, update the internal state of the effect and
    /// call the render function for each pixel in range. Blending and 
    /// sliding are applied here as well
    virtual void update(void)
    {
        if (ms >= refresh_period)
        {
            return;
        }

        ms = 0;
        onFrameBegin();
        unsigned i = 0;
        for (iterator pixel = begin(), _end = end(); pixel != _end; ++pixel, ++i)
        {
            renderSlideAndBlend(*pixel, i);
        }
    }

    /// @brief Helper method to apply slide up/slide down.
    /// @param existing the existing CRGBArray, usually the last
    /// state of the effect's CRGBArray.
    /// @param new_state the new CRGBArray, usually the next state of the effect's CRGBArray.
    /// @param slide_up a value between 0 and 255, specifying the the slide up amount
    /// @param slide_down a value between 0 and 255, specifying the the slide down amount
    static CRGB &slide(CRGB &existing, const CRGB &overlay, uint8_t slide_up, uint8_t slide_down)
    {
        // The formula is y (n) = y (n-1) + ((x (n) - y (n-1))/slide).
        uint8_t slide;
        slide = (existing.r > overlay.r) ? slide_down : slide_up;
        slide = (slide == 0) ? 1 : slide;
        existing.r += ((overlay.r - existing.r) / slide);
        slide = (existing.g > overlay.g) ? slide_down : slide_up;
        slide = (slide == 0) ? 1 : slide;
        existing.g += ((overlay.g - existing.g) / slide);
        slide = (existing.b > overlay.b) ? slide_down : slide_up;
        slide = (slide == 0) ? 1 : slide;
        existing.b += ((overlay.b - existing.b) / slide);
        return existing;
    }
protected:

    /// @brief Helper method to apply blending and slide.
    /// @param existing the existing CRGBArray, usually the last
    /// state of the effect's CRGBArray.
    /// @param index the index of the pixel to render.
    /// @details This method is called by the update function
    /// for each pixel in range. It applies the blend mode and
    /// slide to the existing CRGBArray.
    inline void renderSlideAndBlend(CRGB& existing, unsigned index)
    {
        (*blend_fn)(
            existing,
            slide(
                existing,
                renderPixel(index),
                slide_up,
                slide_down
            ),
            alpha
        );
    }

    /// @brief The blend mode to use.
    blend_mode_fn_ptr blend_fn;
    
    /// @brief The refresh rate of the effect.
    uint16_t refresh_period;
    
    /// @brief The alpha value of the effect, used for blending.
    uint8_t alpha;
    
    /// @brief The slide up amount, used for sliding.
    uint8_t slide_up;

    /// @brief The slide down amount, used for sliding.
    uint8_t slide_down;
    
    /// @brief Internal timer
    elapsedMillis ms; 
};

#endif