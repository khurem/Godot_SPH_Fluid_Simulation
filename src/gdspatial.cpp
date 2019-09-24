#include "gdspatial.h"
#include <ctime>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/variate_generator.hpp>

using namespace std;
using namespace boost;
using namespace godot;

int current_time_nanoseconds(){
    struct timespec tm;
    clock_gettime(CLOCK_REALTIME, &tm);
    return tm.tv_nsec;
}

void GDSpatial::_register_methods() {
    register_method("_ready", &GDSpatial::_ready);
    register_method("_process", &GDSpatial::_process);
    register_method("_on_area_entered", &GDSpatial::_on_area_entered);
    register_method("_bounce_ball", &GDSpatial::_bounce_ball);
}


GDSpatial::GDSpatial() {
}

GDSpatial::~GDSpatial() {
}

void GDSpatial::_init() {
    //Establishes the random speed/direction that each ball will be going towards
    //Using std random generation, it seeds based on the nanoseconds you provide
    bounce = false;
    random::mt19937 rng(current_time_nanoseconds());
    random::uniform_int_distribution<> speed(min_spd,max_spd);
    random::uniform_int_distribution<> direction(min_dir,max_dir);
    spd = (float(speed(rng)));
    x_dir = (float(direction(rng)))/100.0f;
    y_dir = (float(direction(rng)))/100.0f;
    z_dir = (float(direction(rng)))/100.0f;
    x_sign = x_dir < 0 ? -1.0 : 1.0;
    y_sign = y_dir < 0 ? -1.0 : 1.0;
    z_sign = z_dir < 0 ? -1.0 : 1.0;
    

}

void GDSpatial::_ready() {
    
    godot::Node *child = this->get_child(0);
    godot::Area *area = godot::Object::cast_to<godot::Area>(child);
    //Connect signal from child area node to spatial(current)
    area->connect("area_entered", this, "_on_area_entered");
}

void GDSpatial::_process(float delta) {
    //Boolean bounce was included in case you want to commit to other actions alongside bouncin later on
    if(!bounce){
        //Grab current position and update based on the velocity you want to add
        Vector3 cur_pos = this->get_transform().get_origin();
        Vector3 updated_pos = cur_pos + (Vector3(x_dir, y_dir, z_dir) * (spd * 0.1));
        //Position change is clamped to prevent balls from exiting the cube
        Vector3 new_pos = Vector3
        (std::max(lower, std::min(updated_pos[0], upper)), std::max(lower, std::min(updated_pos[1], upper)), std::max(lower, std::min(updated_pos[2], upper)));
        set_translation(new_pos);
    } 

}

void GDSpatial::_on_area_entered(Area *test) {
    bounce = true;
    //Grab collision object based on if it's a sphere or wall and then compute the bounce as needed
    int collider = test->get_child_count() == 2 ? sphere : wall;
    if(collider == wall){
        godot::GDWall *testw = godot::Object::cast_to<godot::GDWall>(test->get_parent());
        _bounce_ball(testw->_get_normal());
    } else {
        godot::GDSpatial *testw = godot::Object::cast_to<godot::GDSpatial>(test->get_parent());
        _bounce_ball((this->get_transform().get_origin() - testw->get_transform().get_origin()).normalized());
    }
    bounce = false;
    
}

void GDSpatial::_bounce_ball(Vector3 N){
    //Calculate new direction post-bounce
    Vector3 I = Vector3(x_dir, y_dir, z_dir);
    Vector3 R = I - (2.0 *N) *  (N.dot(I));
    this->x_dir = R[0];
    this->y_dir = R[1];
    this->z_dir = R[2];
}



