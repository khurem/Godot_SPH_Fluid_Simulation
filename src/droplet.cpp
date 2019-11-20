#include "droplet.h"


using namespace std;
using namespace godot;


void Droplet::_register_methods() {
    register_method("_ready", &Droplet::_ready);
    register_method("_process", &Droplet::_process);
}


Droplet::Droplet() {
}

Droplet::~Droplet() {
}

void Droplet::_init() {
    
    
    

}

void Droplet::_ready() {
      
    godot::Node *child = this->get_child(0)->get_child(0);
    godot::Node *child2 = this->get_child(0)->get_child(1);
    godot::MeshInstance * sphereMesh= godot::Object::cast_to<godot::MeshInstance>(child2);
    godot::CollisionShape *sphereColl = godot::Object::cast_to<godot::CollisionShape>(child);
    Ref<SphereShape> sphereCollRef = Object::cast_to<SphereShape>(sphereColl->get_shape().operator->());
    Ref<SphereMesh> sphereMeshRef = Object::cast_to<SphereMesh>(sphereMesh->get_mesh().operator->());
    droplet_collision = sphereCollRef;
    droplet_mesh = sphereMeshRef;

    
}

void Droplet::_process(float delta) {
   

//  printf("Radius %f \n", sphor->get_radius());
}





