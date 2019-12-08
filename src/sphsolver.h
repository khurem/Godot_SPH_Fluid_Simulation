#pragma once
#ifndef SPHSOLVER_H
#define SPHSOLVER

#include <Godot.hpp>
#include <Spatial.hpp>
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
#include "./droplet.h"
#include "./grid.h"
#include <PackedScene.hpp>
#include <ResourceLoader.hpp>

namespace godot {

    
    

    class SPHSolver : public Spatial {
        GODOT_CLASS(SPHSolver, Spatial)

    private:
        const int max_particles = 2000;
        const Droplet temp = Droplet();
        const float radius = temp.radius;
        const float kernel_range = temp.kernel_range;
        const float KERNEL_CONST = 315.0 / (64.0 * 3 * (powf(kernel_range, 9)));
        const float GRAD_KERNEL_CONST = 45.0 / (M_PI * powf(kernel_range, 6));

        
        Array *particles;
        float velocity = 0.0f;
        int count = 0;
        Ref<PackedScene> dropper;
        Grid *spat_map;
        Array neighborhood;
        

    public:
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
