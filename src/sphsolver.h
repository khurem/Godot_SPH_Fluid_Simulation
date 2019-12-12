#pragma once
#ifndef SPHSOLVER_H
#define SPHSOLVER

#include <Godot.hpp>
#include <Reference.hpp>
#include <Spatial.hpp>
#include <Node.hpp>
#include <Area.hpp>
#include <random>
#include <chrono>
#include <SphereShape.hpp>
#include <CollisionShape.hpp>
#include <MeshInstance.hpp>
#include <CollisionObject.hpp>
#include <Mesh.hpp>
#include <SphereMesh.hpp>
#include <Spatial.hpp>
// #include "./droplet.h"
#include "./particle.h"
#include "./grid.h"
#include <PackedScene.hpp>
#include <AABB.hpp>
#include <ResourceLoader.hpp>
#include <RandomNumberGenerator.hpp>

namespace godot {

    
    

    class SPHSolver : public Reference {
        GODOT_CLASS(SPHSolver, Reference)

    private:
    const float width = 1.25;
    const float height = 1.25;
    const float depth = 2.0f;
         const float scale = 35;

        const float render_width = scale * width;
        const float render_height = scale * height;
        const int max_particles = 25;
        const float rest_density = 1.0f;
        const float stiffness = 1.5f;
        const float viscosity = 0.3f;
        const float gravity = 9.8f;
        const float particle_spacing = 1.0f / max_particles;
        const float timestep = 0.016;
        const float particle_volume = particle_spacing * particle_spacing;
        const float particle_mass = particle_volume * rest_density;
         const float kernel_range = 3.0 * particle_spacing;
        const float KERNEL_CONST = 360.0 / (64.0 * 3 * (powf(kernel_range, 9)));
        const float GRAD_KERNEL_CONST = 15.0 / (M_PI * powf(kernel_range, 6));

        
        

    public:

        Array *particles;
        int count = 0;
        Ref<PackedScene> dropper;
        Grid *spat_map;
        Array neighborhood;
        static void _register_methods();

        SPHSolver();
        ~SPHSolver();

        void _init(); 
        void _ready();
        void _process(float delta);
        Array _create_neighborhood();
        void _calculate_density();
        void _calculate_pressure();
        void _calculate_force_pressure();
        void _integration_step(float delta);
        void _collision_handling();
        float _kernel(Vector3 pos_delta);
        float _laplace_kernel(Vector3 pos_delta);
        Vector3 _grad_kernel(Vector3 pos_delta);

    };
}

#endif
