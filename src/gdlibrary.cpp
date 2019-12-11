#include "sphsolver.h"
#include "droplet.h"
#include "grid.h"
#include "particle.h"


extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o) {
    godot::Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *o) {
    godot::Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void *handle) {
    godot::Godot::nativescript_init(handle);
    godot::register_class<godot::SPHSolver>();
    godot::register_class<godot::Droplet>();
    godot::register_class<godot::Particle>();
    // godot::register_class<godot::Grid>();
}
