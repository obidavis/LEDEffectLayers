# LED Effects Layers

A library for creating LED effects using FastLED and layering them together

## Effect

The core of the library is the Effect class. This class is used to define how a CRGBArray should be updated over time. 

`Effect` is a class template taking a parameter `SIZE`, which is the size of the array that will be updated. The size of the array normally corresponds to the number of LEDs in the strip.

The Effect class is a pure virtual class, so you can't create an instance of it, instead you should create a subclass of Effect and implement the virtual function `render()` to describe how the LED array should be updated.

Access the array of leds using `this->output_arr`.

The following demonstrates an effect that will move a single white pixel along the array over a period of 1 second.

``` cpp
template<int SIZE>
class RisingEffect : public Effect<SIZE> {
public:
    unsigned long period = 1000;
    void render() {
        for (int i = 0; i < SIZE; i++) {
            if (millis() % period == i) {
                leds[i] = CRGB::White;
            } else {
                leds[i] = CRGB::Black;
            }
        }
    }
};
```

## Scene

The `Scene` class is used to render multiple effects on top of each other. It is also a subclass of Effect, so you can use it as an effect.

Use the `addEffect()` method to add effects to the scene. 

## Blending

When using multiple effects in a scene, they are blended together according to their `z_index`, `BLEND_MODE`, and `alpha`. `z_index` is used to determine the order in which the effects are rendered. `BLEND_MODE` is used to determine how the effects are blended together. `alpha` is used to determine the opacity of the effect. These values are all set to their defaults but can be overridden in subclasses or by using the appropriate methods.

See wikipedia for a more detailed description of the blending modes.


## Usage

Whether you are using standalone effects or scenes, you should call `update()` in your loop. This will update all effects in the scene. The Effects will only be updated if a time has elapsed greater that the refresh period of the effect, so this will not slow the loop.

## Oscillator

An `Oscillator` class is provided as an interface to allow creation of effects that take variable sources of input to control their parameters.

