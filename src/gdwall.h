#ifndef GDWALL_H
#define GDWALL_H

#include <Godot.hpp>
#include <Spatial.hpp>
#include <Area.hpp>

namespace godot {

    class GDWall : public Spatial {
        GODOT_CLASS(GDWall, Spatial)

    private:
        Vector3 normal;
        

    public:
        static void _register_methods();

        GDWall();
        ~GDWall();

        void _init(); // our initializer called by Godot
        void _process(float delta);
        Vector3 _get_normal();
    };
}

#endif
