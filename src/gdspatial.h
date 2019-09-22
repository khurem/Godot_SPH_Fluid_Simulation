#ifndef GDSPATIAL_H
#define GDSPATIAL_H

#include <Godot.hpp>
#include <Spatial.hpp>
#include <Area.hpp>
#include <gdwall.h>
#include <random>
#include <chrono>
#include <SphereShape.hpp>
#include <CollisionShape.hpp>
#include <CollisionObject.hpp>

namespace godot {

    enum {sphere, wall};
    const unsigned int max_spd = 100;
    const unsigned int min_spd = -100;
    const real_t upper = 15.0;
    const real_t lower = -15.0;

    class GDSpatial : public Spatial {
        GODOT_CLASS(GDSpatial, Spatial)

    private:
        float time_passed;
        bool bounce;
        float x_spd;
        float y_spd;
        float z_spd;
        

    public:
        static void _register_methods();

        GDSpatial();
        ~GDSpatial();

        void _init(); // our initializer called by Godot
        void _ready();
        void _on_area_entered(Area *test);
        void _process(float delta);
        void _bounce_ball(Vector3 N);
        Vector3 _get_velocity();
        Vector3 _other_sphere_normal(GDSpatial *other);
    };
}

#endif
