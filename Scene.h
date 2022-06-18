#include "_Effect.h"
#include "LinkedPointerList/LinkedPointerList.h"

class Scene : public Effect
{
public:
    virtual CRGB renderPixel(int index) override { return CRGB::Black; } // dummy implementation

    /// @brief Update all effects in the scene.
    virtual void update(void) override
    {
        for (LinkedPointerList<Effect>::iterator effect = effects.begin(); effect != effects.end(); ++effect)
        {
            (*effect)->update();
        }
    }
    virtual void addEffect(Effect* effect)
    {
        effects.add(effect);
    }
    LinkedPointerList<Effect> effects;
};