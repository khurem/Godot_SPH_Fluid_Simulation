#include "sphsolver.h"



using namespace std;
using namespace godot;


void SPHSolver::_register_methods() {
    register_method("_ready", &SPHSolver::_ready);
    register_method("_process", &SPHSolver::_process);
}


SPHSolver::SPHSolver() {
}

SPHSolver::~SPHSolver() {
}

void SPHSolver::_init() {
    
    particles = new Array();
    spat_map = new Grid();

}

void SPHSolver::_ready() {
      
   
    ResourceLoader *loader = ResourceLoader::get_singleton();
    dropper = loader->load("res://Sphere.tscn");

    
}

void SPHSolver::_process(float delta) {
    if(particles->size() < max_particles){
        Droplet *drop = static_cast<Droplet*>(dropper->instance());
        drop->translate(Vector3(0.0f, 20.0f, 0.0f));
        this->get_parent()->add_child(drop);
        particles->append(drop);
        // count++;
    }
    spat_map->_reset_grid();
    for(int ind = 0; ind < particles->size(); ind++){
        Droplet *cur = static_cast<Droplet*>(___get_from_variant(particles->operator[](ind)));
        spat_map->_add_to_grid(cur, ind);
        spat_map->_get_neighbors(cur);
    }
}



