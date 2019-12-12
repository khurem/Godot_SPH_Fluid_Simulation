#pragma once
#ifndef MAIN_H
#define MAIN

#include <Godot.hpp>
#include <Spatial.hpp>
#include <Area.hpp>
#include <gdwall.h>
#include <random>
#include <Spatial.hpp>
#include <Texture.hpp>
#include <chrono>
#include <SphereShape.hpp>
#include <CollisionShape.hpp>
#include <SpatialMaterial.hpp>
#include <Viewport.hpp>
#include <MeshInstance.hpp>
#include <MeshInstance.hpp>
#include <OS.hpp>
#include <CollisionObject.hpp>
#include <Mesh.hpp>
#include <SphereMesh.hpp>
#include <KinematicBody.hpp>
#include <KinematicCollision.hpp>
#include <Dictionary.hpp>
#include "./particle.h"
#include "./sphsolver.h"

namespace godot {

    
    

    class Main : public Spatial {
        GODOT_CLASS(Main, Spatial)

    private:
  
        SPHSolver *sph;
        const int max_particles = 25;
        const float particle_spacing = 1.0f / max_particles;
          const float scale = 35;
        const float kernel_range = 2.0f * particle_spacing;
         const Vector3 def_scale = Vector3(1.0f, 1.0f, 1.0f)/ 64.0f * kernel_range * scale;
         SpatialMaterial *material;
         MeshInstance * part_mesh;
         Viewport *view;
         Ref<PackedScene> text;
         float reset = 1.0f;
         int start = 0;
         int child_pad = 2;

    public:
        static void _register_methods();

        Main();
        ~Main();

        void _init(); // our initializer called by Godot
        void _ready();
        void _process(float delta);

       
    };
}

#endif
