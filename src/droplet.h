#pragma once
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
#include <KinematicBody.hpp>
#include <KinematicCollision.hpp>

namespace godot {

    
    

    class Droplet : public KinematicBody {
        GODOT_CLASS(Droplet, KinematicBody)

    private:
        const float radius = 0.5f;
        const float init_density = 1.0f;
        const float volume = powf(radius, 3.0f) * 3.14f * (4.0f/3.0f);
        const float mass = volume * init_density;
        const float kernel_range = 1.25 * ( radius);
        const float viscosity = 0.3f;
        const float stiffness = 1.5f;
        const float gravity = -9.8f;
        float velocity = 0.0f;
        

    public:
        static void _register_methods();
        Ref<SphereShape> droplet_collision;
        Ref<SphereMesh> droplet_mesh;

        Droplet();
        ~Droplet();

        void _init(); // our initializer called by Godot
        void _ready();
        void _process(float delta);
        void _on_body_entered(Node *body);
        float _get_kernel_size();
        bool resting = false;
       
    };
}

#endif
