#include "gdwall.h"

using namespace godot;

void GDWall::_register_methods() {
    register_method("_process", &GDWall::_process);
    register_method("_get_normal", &GDWall::_get_normal);
    register_property<GDWall, Vector3>("normal", &GDWall::normal, Vector3(0.0,0.0,0.0));
}


GDWall::GDWall() {
}

GDWall::~GDWall() {
    // add your cleanup here
}

void GDWall::_init() {
    //inialize normal with all 0's (changed in the editor itself)
    normal = Vector3(0.0,0.0,0.0);
}


void GDWall::_process(float delta) {  
}

Vector3 GDWall::_get_normal(){
    //returns the normal of the wall to whatever collides with it
    return this->normal;
}


