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
    register_method("_get_velocity", &GDSpatial::_get_velocity);
}


GDSpatial::GDSpatial() {
}

GDSpatial::~GDSpatial() {
    // add your cleanup here
}

void GDSpatial::_init() {
    // initialize any variables here
    time_passed = 0.0;
    bounce = false;

}

void GDSpatial::_ready() {

    Vector3 cur = this->get_global_transform().get_origin();
    // printf("%f %f %f\n", cur[0], cur[1], cur[2]);
    unsigned int dice_rolls = 12;
    random::mt19937 rng(current_time_nanoseconds());
    random::uniform_int_distribution<> speed(min_spd,max_spd);
    // srand(cur[1]);
    x_spd = (float(speed(rng)))/100.0f;
    y_spd = (float(speed(rng)))/100.0f;
    z_spd = (float(speed(rng)))/100.0f;
    // printf("%f %f %f\n", x_spd, y_spd, z_spd);
    godot::Node *child = this->get_child(0);
    godot::Area *area = godot::Object::cast_to<godot::Area>(child);
    // printf("the number of chiddler's the area has is %d\n", area->get_child_count());
    area->connect("area_entered", this, "_on_area_entered");
    // printf("Everythin good here %d\n", area->get_child_count());
}
//TODO PREVENT WALL HACKS BY LOWER STEP IF TOO CLOSE TO WALL
void GDSpatial::_process(float delta) {
    Vector3 cur = this->get_translation();
    

    // std::max(lower, std::min(n, upper));
    if(!bounce){
        Vector3 cur_pos = this->get_transform().get_origin();
        Vector3 updated_pos = Vector3(cur_pos[0] + x_spd, cur_pos[1] + y_spd, cur_pos[2] + z_spd);
        Vector3 new_pos = Vector3(std::max(lower, std::min(updated_pos[0], upper)), std::max(lower, std::min(updated_pos[1], upper)), std::max(lower, std::min(updated_pos[2], upper)));

        set_translation(new_pos);
    } 
    
    
    
}

void GDSpatial::_on_area_entered(Area *test) {
    bounce = true;
    // printf("the number of chiddler's the area has is %d\n", test->get_child_count());
    int collider = test->get_child_count() == 2 ? sphere : wall;

    if(collider == wall){
        godot::GDWall *testw = godot::Object::cast_to<godot::GDWall>(test->get_parent());
        _bounce_ball(testw->_get_normal());
        bounce = false;
    } else {
        
        // printf("galoogodss\n");
        godot::GDSpatial *testw = godot::Object::cast_to<godot::GDSpatial>(test->get_parent());
        // _other_sphere_normal(testw);
        // printf("%f %f %f\n", x_spd, y_spd, z_spd);
        _bounce_ball((this->get_transform().get_origin() - testw->get_transform().get_origin()).normalized());
        // printf("%f %f %f\n", x_spd, y_spd, z_spd);
        bounce = false;

    }
    
}

Vector3 GDSpatial::_other_sphere_normal(GDSpatial *other){
    godot::Node *child = this->get_child(0);
    godot::Area *area = godot::Object::cast_to<godot::Area>(child);
    godot::CollisionShape *sphereS = godot::Object::cast_to<godot::CollisionShape>(area->get_child(0));
    godot:Ref<godot::Shape> s =  sphereS->get_shape();
    godot::SphereShape * q= godot::Object::cast_to<godot::SphereShape>(s.ptr());
    // printf("the radius of the sphere is %f\n", q->get_radius());
    real_t dist = (this->get_transform().get_origin().distance_to(other->get_transform().get_origin()));
    // printf("overall disto is %f \n", dist);
    Vector3 normal = (this->get_transform().get_origin() - other->get_transform().get_origin()).normalized();
    Vector3 normal2 = ( other->get_transform().get_origin() - this->get_transform().get_origin() ).normalized();
    // printf("normals are %f %f %f and %f %f %f \n", normal[0], normal[1], normal[2], normal2[0],normal2[1],normal2[2]);
    //normalized(c1.center - c2.center) 
    // c1.center - normal * c1.radius / c2.center + normal * c2.radius)
    Vector3 point = (this->get_transform().get_origin() - normal * 1.00f) / (other->get_transform().get_origin() + normal * 1.00f);
    Vector3 point2 = (other->get_transform().get_origin() - normal2 * 1.00f )/ (this->get_transform().get_origin() + normal2 * 1.00f);
    // printf("the two radii are %f %f %f and %f %f %f\n", point[0],point[1], point[2], point2[0], point2[1], point2[2]);
    // CollisionShape *radius = area->get_child_count();
    // Vector3 point = this->get_transform().get_origin() * ;
    return Vector3(0.0, 0.0, 0.0);
}

Vector3 GDSpatial::_get_velocity(){
    return Vector3(this->x_spd, this->y_spd, this->z_spd);
}

void GDSpatial::_bounce_ball(Vector3 N){
    //R = I - 2N(N·I) 
    // printf("OTHER SPHERE%f %f %f \n", N[0], N[1], N[2]);
    Vector3 I = Vector3(x_spd, y_spd, z_spd);
    Vector3 R = I - (2.0 *N) *  (N.dot(I));
    this->x_spd = R[0];
    this->y_spd = R[1];
    this->z_spd = R[2];
    
     

}



