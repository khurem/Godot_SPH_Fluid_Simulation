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
        const int max_particles = 200;
        Array *particles;
        float velocity = 0.0f;
        int count = 0;
        Ref<PackedScene> dropper;
        Grid *spat_map;
        

    public:
        static void _register_methods();

        SPHSolver();
        ~SPHSolver();

        void _init(); 
        void _ready();
        void _process(float delta);
       
    };
}

#endif
