#ifndef LEL_Scene_h_
#define LEL_Scene_h_

#include "Effect.h"

/// @brief A scene is a collection of effects.
/// @details description scene is a collection of effects that are rendered together.
/// The scene is responsible for rendering the effects and updating the effects.
/// The scene is also an effect in itself, and so can be added to parent scenes.
/// Like effects, the update method should be called in the loop()
/// @tparam SIZE the size of the CRGBArray used by the scene.
template <int SIZE>
class Scene : public Effect<SIZE>
{
private:
  LinkedPointerList<Effect<SIZE>> effects;

public:
  /// @brief Add an effect to the scene.
  /// @details The effect is added to the list of effects according to its z_index.
  /// @param effect the effect to add.
  void addEffect(Effect<SIZE> *effect)
  {
    for (int i = 0; i < effects.size(); i++)
    {
      if (effects.get(i)->z_index > effect->z_index)
      {
        effects.add(effects.get(i));
        effects.set(i, effect);
        return;
      }
    }
    // if no effect is a higher z_index, put at end
    effects.add(effect);
  }

  /// @brief Add an effect to the scene at a specific index.
  /// @details The effect is added to the list of effects according to the specified z_index.
  /// Note that this does override the z_index of the effect itself.
  void addEffect(Effect<SIZE> *effect, int effect_z_index)
  {
    effect->z_index = effect_z_index;
    this->addEffect(effect);
  }

protected:
  void render(void)
  {
    int min_arr_size;
    Effect<SIZE> *effect;

    fill_solid(this->output_arr, this->output_arr.size(), CRGB::Black);

    for (int i = 0; i < effects.size(); i++)
    {
      effect = effects.get(i);
      effect->update();
      min_arr_size = min(effect->output_arr.size(), this->output_arr.size());

      for (int j = 0; j < min_arr_size; j++)
      {
        blend_mode_map.at(effect->blend_mode)(this->output_arr[j], effect->output_arr[j], effect->alpha);
      }
    }
  }
};

#endif