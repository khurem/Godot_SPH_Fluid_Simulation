#pragma once
#ifndef PARTICLE_H
#define PARTICLE

#include <Godot.hpp>
#include <Spatial.hpp>
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
#include <RigidBody.hpp>
#include <Reference.hpp>

namespace godot {

    
    

    class Particle : public Reference {
        GODOT_CLASS(Particle, Reference)

    private:
        
        

    public:
        const float width = 2.5;
    const float height = 1.5;
        const float scale = 40;
        const float render_width = scale * width;
        const float render_height = scale * height;
        const int max_particles = 22;
        const float rest_density = 1.0f;
        const float stiffness = 1.8f;
        const float gravity = 980.0f;
        const float particle_spacing = 1.0f / max_particles;
        const float timestep = 0.016;
        const float particle_volume = particle_spacing * particle_spacing;
        const float particle_mass = particle_volume * rest_density;
        const float kernel_range =  2 * particle_spacing;
        const float KERNEL_CONST = 315.0 / (64.0 * 3 * (powf(kernel_range, 9)));
        const float GRAD_KERNEL_CONST = 15.0 / (M_PI * powf(kernel_range, 6));
        const float viscosity_const = 0.3f;
        const float max_dist =  kernel_range * kernel_range;
        const float max_neighbors = 10;
        float density = 0.0f;
        float viscosity = 0.0f;
        float pressure = 0.0f;
        Vector3 position = Vector3(0.0f, 0.0f, 0.0f);
        Vector3 velocity = Vector3(0.0f, 0.0f, 0.0f);
        Vector3 force = Vector3(0.0f, 0.0f, 0.0f);
        static void _register_methods();
        Ref<SphereShape> Particle_collision;
        Ref<SphereMesh> Particle_mesh;

        Particle();
        ~Particle();
        void _init();
        void _ready();
        void _process(float delta);
        void _on_body_entered(Node *body);
        float _get_kernel_size();
        bool resting = false;
       
    };
}

#endif
