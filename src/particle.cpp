#include "particle.h"


using namespace std;
using namespace godot;


void Particle::_register_methods() {
    register_method("_ready", &Particle::_ready);
    register_method("_process", &Particle::_process);
    // register_method("_on_body_entered", &Particle::_on_body_entered);
}


Particle::Particle() {
}

Particle::~Particle() {
}

void Particle::_init(){

    // Godot::print("dfdddddddd");
}



void Particle::_ready() {
   
//    Godot::print("ndsfnsdofnisdofs");
    // Node *child = this->get_child(0);
    // Node *area = this->get_child(0);
    // Godot::print(this->get_name());
    // Area *areaChild = godot::Object::cast_to<godot::Area>(area);
    // // Godot::print(areaChild->get_name());
    // // areaChild->connect("body_entered", this, "_on_body_entered");
    // // godot::Node *child2 = this->get_child(1);
    // godot::MeshInstance * sphereMesh= godot::Object::cast_to<godot::MeshInstance>(child2);
    // godot::CollisionShape *sphereColl = godot::Object::cast_to<godot::CollisionShape>(child);
    // Ref<SphereShape> sphereCollRef = Object::cast_to<SphereShape>(sphereColl->get_shape().operator->());
    // Ref<SphereMesh> sphereMeshRef = Object::cast_to<SphereMesh>(sphereMesh->get_mesh().operator->());
    // Particle_collision = sphereCollRef;
    // Particle_mesh = sphereMeshRef;

    
}

void Particle::_process(float delta) {
    // velocity.y += 0.1 * gravity;
    // if(this->is_on_floor()){
    //     velocity.y = 0.0f;
    // } else{
        //  Godot::print("kkkkkkkkkkkkkkkkkkkkkkkkkkkkk");
    // }
    // Vector3 motion = velocity;
        //  move_and_slide(motion, Vector3(0,1,0));
    // Vector3 cur = this->get_global_transform().get_origin();
    // if(!resting){
    //     Vector3 updoot = Vector3(cur.x, cur.y + (velocity * 0.001f), cur.z);
    //     this->set_translation(updoot);
    // }else{

    // }
   
    
    // Vector3 pos = this->get_global_transform().get_origin();
    // if(abs(pos.x) > 20 || abs(pos.y) > 130||abs(pos.z) > 20){
    //     this->set_translation(Vector3(0.0f, 40.0f, 0.0f));
    //     velocity.y = 0.0f;
    //     // this->queue_free();
    //     // Godot::print(this->get_global_transform().get_origin());
    // }
    
   
    
    // 
}

float Particle::_get_kernel_size(){
    return kernel_range;
}

// void Droplet::_on_body_entered(Node *body){
//     Godot::print("stop the ball");
//     resting = true;
// }





