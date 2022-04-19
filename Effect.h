#ifndef LEL_Effect_h
#define LEL_Effect_h

#include <FastLED.h>
#include "BlendModes.h"
#include "Oscillator.h"
#include "LinkedPointerList/LinkedPointerList.h"

/// @brief Base class for LED effects.
/// @details An effect is an object that updates a CRGBArray.
/// This should be subclassed to provide the render() method
/// defining the behaviour of the effect. The Effect may be
/// displayed directly or added to a Scene.
/// @tparam SIZE the size of the CRGBArray used by the effect.
template <int SIZE>
class Effect
{
public:
  Effect()
  {
    fill_solid(this->output_arr, this->output_arr.size(), CRGB(0));
  }
  /// @brief specify the blend mode for the effect.
  /// @details The blend mode specifies how the effect is blended with the
  /// effects beneath it in a Scene. The default is NORMAL.
  /// @param blend_mode the blend mode to use. See BlendModes.h for the available modes.
  void setBlendMode(RGB_BLEND_MODE mode) { blend_mode = mode; }

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

  /// @brief Update the effect.
  /// @details This method is called by the Scene to update the effect.
  /// Alternatively, the effect can be updated manually by calling this method,
  /// for example if the effect is not part of a Scene
  void update(void)
  {
    if (ms > refresh_period)
    {
      ms = 0;
      render();
    }
  }

  inline CRGB &operator[](int x) const { return this->output_arr[x]; }

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
  CRGBArray<SIZE> output_arr;
  int z_index = 0;
  uint8_t alpha = 0xFF;
  RGB_BLEND_MODE blend_mode = NORMAL;

  uint16_t refresh_period = 20;
  elapsedMillis ms;

protected:
  /// @brief Render the effect.
  /// @details Overload this method to provide the effect's behaviour.
  virtual void render(void) = 0;
};

#endif