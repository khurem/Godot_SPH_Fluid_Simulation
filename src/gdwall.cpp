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
    // initialize any variables here
    normal = Vector3(0.0,0.0,0.0);
}


void GDWall::_process(float delta) {
    
}

Vector3 GDWall::_get_normal(){
    return this->normal;
}


