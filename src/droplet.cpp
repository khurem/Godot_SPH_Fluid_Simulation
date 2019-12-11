#include "droplet.h"


using namespace std;
using namespace godot;


void Droplet::_register_methods() {
    register_method("_ready", &Droplet::_ready);
    register_method("_process", &Droplet::_process);
    // register_method("_on_body_entered", &Droplet::_on_body_entered);
}


Droplet::Droplet() {
}

Droplet::~Droplet() {
}

void Droplet::_init() {
    
    // position = pos;
    

}

void Droplet::_ready() {
      
    Node *child = this->get_child(0);
    Node *area = this->get_child(0);
    // Godot::print(this->get_name());
    Area *areaChild = godot::Object::cast_to<godot::Area>(area);
    // Godot::print(areaChild->get_name());
    // areaChild->connect("body_entered", this, "_on_body_entered");
    godot::Node *child2 = this->get_child(1);
    godot::MeshInstance * sphereMesh= godot::Object::cast_to<godot::MeshInstance>(child2);
    godot::CollisionShape *sphereColl = godot::Object::cast_to<godot::CollisionShape>(child);
    Ref<SphereShape> sphereCollRef = Object::cast_to<SphereShape>(sphereColl->get_shape().operator->());
    Ref<SphereMesh> sphereMeshRef = Object::cast_to<SphereMesh>(sphereMesh->get_mesh().operator->());
    droplet_collision = sphereCollRef;
    droplet_mesh = sphereMeshRef;

    
}

void Droplet::_process(float delta) {
    // velocity.y += 0.1 * gravity;
    // if(this->is_on_floor()){
    //     velocity.y = 0.0f;
    // } else{
         
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

float Droplet::_get_kernel_size(){
    return max_dist;
}

// void Droplet::_on_body_entered(Node *body){
//     Godot::print("stop the ball");
//     resting = true;
// }





