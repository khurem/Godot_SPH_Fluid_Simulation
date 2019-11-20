#ifndef DROPLET_H
#define DROPLET

#include <Godot.hpp>
#include <Spatial.hpp>
#include <Area.hpp>
#include <gdwall.h>
#include <random>
#include <chrono>
#include <SphereShape.hpp>
#include <CollisionShape.hpp>
#include <MeshInstance.hpp>
#include <CollisionObject.hpp>
#include <Mesh.hpp>
#include <SphereMesh.hpp>

namespace godot {

    
    

    class Droplet : public Spatial {
        GODOT_CLASS(Droplet, Spatial)

    private:
        const float init_density = 1.0f;
        const float spacing = (1.0f/ 100.0f);
        const float volume = powf(spacing, 3.0f) * 3.14f * (4.0f/3.0f);
        const float mass = volume * init_density;
        const float kernel_range = 1.25 * (spacing);
        const float viscosity = 0.3f;
        const float stiffness = 1.5f;
        const float gravity = -9.8f;
        

    public:
        static void _register_methods();
        Ref<SphereShape> droplet_collision;
        Ref<SphereMesh> droplet_mesh;

        Droplet();
        ~Droplet();

        void _init(); // our initializer called by Godot
        void _ready();
        void _process(float delta);
       
    };
}

#endif
