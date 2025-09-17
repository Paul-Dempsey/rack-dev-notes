# Jitter periodic updates

A common technique for reducing CPU usage in a module is to throttle parameter updates.
Parameters are usually controlled by knobs, sliders, and switches.
In other words, by the user via the user interface.
Human users operate at speeds much slower than audio, so you don't need to update parameters as fast as you do control voltage.

The industry terms used are "audio rate" versus "control rate".

The same principle applies to lights: light changes don't need to happen any faster than humans can perceive, which is much slower than audio rate.

The usual way to throttle updates is with something like this:

```cpp
constexpr const int64_t PARAM_INTERVAL = 64;

void MyModule::process(const ProcessArgs &args) {
    ...
    if (0 == (args.frame % PARAM_INTERVAL)) {
        // update parameters ...
    }
```

So we update params or do other expensive computation every 64 samples (aka frames).

This is all well and good, but what happens when there are many modules using the same interval?

Powers of two are programmer's favorite for arbitrary numbers (for no particular good reason for this application), and programmers love to copy and paste such boilerplate, so collisions among modules are common, and plugins often use the same interval for all modules in that plugin.

This results in a CPU spike every `PARAM_INTERVAL` frames.
This is bad. CPU spikes _can_ cause audio artifacts.

What we want is to spread these periodic parameter updates across different frames for different modules.
Plus, we want to do that precisely and efficiently.

Rack has `::rack::random::uniform()` we can call to get a nicely distributed random number for that purpose that we can save as a member in our module. But there's a better option.

We take advantage of the fact that module ids are large (broad range), unique (no collision with another module), and randomly distributed integers: the perfect characteristics we want for jittering periodic updates. Plus, Rack computes it for us, and the value is ready at hand at minimal cost to access.

Thus, jittered periodic intervals:

```cpp
constexpr const int64_t PARAM_INTERVAL = 67;

void MyModule::process(const ProcessArgs &args) {
    ...

    if (0 == ((args.frame + getId()) % PARAM_INTERVAL)) {
        // update parameters ...
    }

```

| | |
|--|--|
| ![pachde (#d) logo](./assets/Logo.svg) | Copyright © Paul Chase Dempsey |
