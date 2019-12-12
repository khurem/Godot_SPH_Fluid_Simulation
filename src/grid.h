#pragma once
#ifndef GRID_H
#define GRID

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
#include <Dictionary.hpp>
#include "./particle.h"

namespace godot {

    
    

    class Grid : public Spatial {
        GODOT_CLASS(Grid, Spatial)

    private:
        Dictionary * grid;
        

    public:
        static void _register_methods();

        Grid();
        ~Grid();

        void _init(); // our initializer called by Godot
        void _ready();
        void _process(float delta);
        void _add_to_grid(Particle* new_drop, int idx);
        void _reset_grid();
        Array _get_neighbors(Particle* center, Array* particles);
       
    };
}

#endif
