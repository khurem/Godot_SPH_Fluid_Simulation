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

    
    

    class Droplet : public Area {
        GODOT_CLASS(Droplet, Area)

    private:
        
        

    public:
        const float radius = 1.0f;
        const float init_density = 1.0f;
        float cur_density = init_density;
        float pressure = 0.0f;
        const float volume = powf(radius, 3.0f) * 3.14f * (4.0f/3.0f);
        const float mass = 5.3f;
        const float kernel_range = 2.0 ;
        float viscosity = 0.0f;

        const float viscosity_const = 0.5f;
        const float max_dist =  8.0 *  radius;
        const float stiffness = 1.5f;
        const float gravity = -9.8f;
        Vector3 velocity = Vector3(0.0f, 0.0f, 0.0f);
        Vector3 force = Vector3(0.0f, 0.0f, 0.0f);
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
