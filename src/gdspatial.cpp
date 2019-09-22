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
    bounce = false;

}

void GDSpatial::_ready() {

    Vector3 cur = this->get_global_transform().get_origin();
    unsigned int dice_rolls = 12;
    random::mt19937 rng(current_time_nanoseconds());
    random::uniform_int_distribution<> speed(min_spd,max_spd);
    x_spd = (float(speed(rng)))/100.0f;
    y_spd = (float(speed(rng)))/100.0f;
    z_spd = (float(speed(rng)))/100.0f;
    godot::Node *child = this->get_child(0);
    godot::Area *area = godot::Object::cast_to<godot::Area>(child);
    area->connect("area_entered", this, "_on_area_entered");
}

void GDSpatial::_process(float delta) {
    if(!bounce){
        Vector3 cur_pos = this->get_transform().get_origin();
        Vector3 updated_pos = Vector3(cur_pos[0] + x_spd, cur_pos[1] + y_spd, cur_pos[2] + z_spd);
        Vector3 new_pos = Vector3
        (std::max(lower, std::min(updated_pos[0], upper)), std::max(lower, std::min(updated_pos[1], upper)), std::max(lower, std::min(updated_pos[2], upper)));
        set_translation(new_pos);
    } 

}

void GDSpatial::_on_area_entered(Area *test) {
    bounce = true;
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
    Vector3 I = Vector3(x_spd, y_spd, z_spd);
    Vector3 R = I - (2.0 *N) *  (N.dot(I));
    this->x_spd = R[0];
    this->y_spd = R[1];
    this->z_spd = R[2];
}



