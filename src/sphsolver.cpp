#include "sphsolver.h"



using namespace std;
using namespace godot;


void SPHSolver::_register_methods() {
    register_method("_ready", &SPHSolver::_ready);
    register_method("_process", &SPHSolver::_process);
    register_method("_create_neighborhood", &SPHSolver::_create_neighborhood);
    register_method("_kernel", &SPHSolver::_kernel);
    register_method("_calculate_densite", &SPHSolver::_calculate_density);
    register_method("_calculate_pressure", &SPHSolver::_calculate_pressure);
    register_method("_calculate_force_pressure", &SPHSolver::_calculate_force_pressure);
    register_method("_laplace_kernel", &SPHSolver::_laplace_kernel);
    register_method("_grad_kernel", &SPHSolver::_grad_kernel);
    register_method("_integration_step", &SPHSolver::_integration_step);
}


SPHSolver::SPHSolver() {
}

SPHSolver::~SPHSolver() {
}

void SPHSolver::_init() {
    
    particles = new Array();
    spat_map = new Grid();
    neighborhood = Array();

}

void SPHSolver::_ready() {
      
   
    ResourceLoader *loader = ResourceLoader::get_singleton();
    dropper = loader->load("res://Sphere.tscn");

    
}

Array SPHSolver::_create_neighborhood(){
    Array neighborhood = Array();
    for(int ind = 0; ind < particles->size(); ind++){
        Droplet *cur = static_cast<Droplet*>(___get_from_variant(particles->operator[](ind)));
        neighborhood.append(spat_map->_get_neighbors(cur));
    }
    return neighborhood;
}

void SPHSolver::_process(float delta) {
    // Godot::print("process start");
      float xval = -8.8f;
    float yval = 10.0f;
    float zval = -8.8f;
    while(particles->size() < max_particles){
        Droplet *drop = static_cast<Droplet*>(dropper->instance());
        // drop->get_global_transform().get_origin();
        // drop->set_translation
       
   
        drop->set_translation(Vector3(xval, yval, zval));
        // Godot::print(drop->get_global_transform().get_origin());
        xval += 2.1f;
        if(xval > 1.5f){
            xval = -8.8f;
            yval += 2.1f;
        }
        if(yval > 30.0f){
            yval = 10.0f;
            zval += 2.1f;
            xval = -8.8f;
        }
        this->get_parent()->add_child(drop);
        particles->append(drop);
        // count++;
    }
    spat_map->_reset_grid();
    for(int ind = 0; ind < particles->size(); ind++){
        Droplet *cur = static_cast<Droplet*>(___get_from_variant(particles->operator[](ind)));
        spat_map->_add_to_grid(cur, ind);
    }
    this->neighborhood = this->_create_neighborhood();
    // Godot::print("finished neighborhood");
    _calculate_density();
	_calculate_pressure();
	_calculate_force_pressure();
	_integration_step(delta);
    _collision_handling();
	// collision_handling();
}


float SPHSolver::_kernel(Vector3 pos_delta){
    real_t r = pos_delta.length_squared();
    real_t h = kernel_range * kernel_range;
    // printf("THE SQUARE OF THE KERNEL IS %f %f \n", h, r);
    if( r < 0 || r > h){
        return 0.0f;
    }
    return KERNEL_CONST * pow(h-r, 3);
}

Vector3 SPHSolver::_grad_kernel(Vector3 pos_delta){
    real_t len = pos_delta.length();
    if( len == 0.0){
        return Vector3(0.0f,0.0f,0.0f);
    }
    
    //TODO: CHECK IF THIS IS CORRECT
    Vector3 norm_delta = pos_delta / pos_delta.length();
    real_t surround = pow(GRAD_KERNEL_CONST - len, 2);

    return -GRAD_KERNEL_CONST * norm_delta * surround;
}

float SPHSolver::_laplace_kernel(Vector3 pos_delta){
    real_t len = pos_delta.length();
    return GRAD_KERNEL_CONST * (kernel_range - len);
}

void SPHSolver::_calculate_density(){
    for(int ind = 0; ind < particles->size(); ind++){
        Droplet *cur = static_cast<Droplet*>(___get_from_variant(particles->operator[](ind)));
        Array cur_nei = this->neighborhood[ind];
        
        float total_density = 0.0f;
        // Godot::print("dsfsdfsdf");
        for(int ne = 0; ne < cur_nei.size(); ne++){
            // printf("Size of temp is %d \n", cur_nei.size());
            godot::String var = cur_nei[ne].operator godot::String();
            // godot::PoolIntArray t =  var.split_ints(',');
            godot::PoolStringArray r = var.split(", ");
            // Godot::print(var);
            for(int con = 0; con < r.size(); con++){
                int index = r.operator[](con).to_int();
                Droplet *bor = static_cast<Droplet*>(___get_from_variant(particles->operator[](index)));
                float kernel_chk = cur->get_global_transform().get_origin().distance_squared_to(bor->get_global_transform().get_origin());
                       Vector3 pos_delta = cur->get_global_transform().get_origin() - bor->get_global_transform().get_origin();
                    // printf("mass %f and delta pos %f %f %f and kernel return %f\n", cur->mass, pos_delta.x, pos_delta.y, pos_delta.z, _kernel(pos_delta));
                //  printf("distance is %f for %d and %d %d %d\n", pos_delta.length_squared(), bor->get_index(), cur->get_index(), ind, index);
                if(kernel_chk <= cur->max_dist){
                    // Vector3 pos_delta = cur->get_global_transform().get_origin() - bor->get_global_transform().get_origin();
             

                    total_density += cur->mass * _kernel(pos_delta);
                }
            }
        }
        cur->cur_density = total_density;
        // printf("TOTAL PRESSURE HERE IS %f \n", cur->cur_density);

    }
}

void SPHSolver::_calculate_pressure(){
     for(int ind = 0; ind < particles->size(); ind++){
        Droplet *cur = static_cast<Droplet*>(___get_from_variant(particles->operator[](ind)));
        cur->pressure = max(cur->stiffness * (cur->cur_density - cur->init_density), 0.0f);
        // Godot::print("start");
        // Godot::print(cur->pressure);
        // printf("TOTAL PRESSURE HERE IS %f \n", cur->cur_density);
        // Godot::print("end");
    }
}

void SPHSolver::_calculate_force_pressure(){
    for(int ind = 0; ind < particles->size(); ind++){
        Vector3 temp_pressure = Vector3(0.0f, 0.0f, 0.0f);
        Vector3  temp_viscosity = Vector3(0.0f, 0.0f, 0.0f);
        Vector3 temp_gravity = Vector3(0.0f,0.0f,0.0f);
        Droplet *cur = static_cast<Droplet*>(___get_from_variant(particles->operator[](ind)));
        Array cur_nei = this->neighborhood[ind];
        
        float total_density = 0.0f;
        // Godot::print("dsfsdfsdf");
        for(int ne = 0; ne < cur_nei.size(); ne++){
            // printf("Size of temp is %d \n", cur_nei.size());
            godot::String var = cur_nei[ne].operator godot::String();
            // godot::PoolIntArray t =  var.split_ints(',');
            godot::PoolStringArray r = var.split(", ");
            // Godot::print(var);
            for(int con = 0; con < r.size(); con++){
                
                int index = r.operator[](con).to_int();
                Droplet *bor = static_cast<Droplet*>(___get_from_variant(particles->operator[](index)));
                float kernel_chk = cur->get_global_transform().get_origin().distance_squared_to(bor->get_global_transform().get_origin());
                
                if(kernel_chk <= cur->max_dist){

                    Vector3 pos_delta = cur->get_global_transform().get_origin() - bor->get_global_transform().get_origin();
                    temp_pressure += bor->mass * (cur->pressure + bor->pressure) / (2.0 * bor->cur_density) * _grad_kernel(pos_delta);
                    // printf("distance is %f for %d and %d %d %d\n", pos_delta.length_squared(), bor->get_index(), cur->get_index(), ind, index);
printf("all pressure vals are %f %f %f %f %f %f %f %f\n", bor->mass, bor->cur_density, cur->pressure, bor->pressure, cur->cur_density, _grad_kernel(pos_delta)[0], _grad_kernel(pos_delta)[1], _grad_kernel(pos_delta)[2]);
                    temp_viscosity += bor->mass * (bor->velocity - cur->velocity) / (bor->cur_density) * _laplace_kernel(pos_delta);
                }
            }
        }
        temp_gravity = cur->cur_density * Vector3(0, cur->gravity, 0);
        temp_pressure *= -1;
        temp_viscosity *= cur->viscosity_const;
        Godot::print("start, pressure, visc, gravity");
        Godot::print(temp_pressure);
        Godot::print(temp_viscosity);
        Godot::print(temp_gravity);
        Godot::print("end");
        cur->viscosity = temp_viscosity.length();
        cur->force += temp_pressure + temp_viscosity + temp_gravity;

    }
}

void SPHSolver::_integration_step(float delta){
    for(int ind = 0; ind < particles->size(); ind++){
        Droplet *cur = static_cast<Droplet*>(___get_from_variant(particles->operator[](ind)));
        cur->velocity += delta * cur->force / cur->cur_density;
        
        Vector3 cur_pos = cur->get_global_transform().get_origin();
        // cur_pos += delta * cur->velocity;
        // Godot::print(cur->force);
        Vector3 shift = delta * cur->velocity;
        cur->translate(shift);
        //  if(cur->is_on_floor() || cur_pos.y < 1){
        //     // Godot::print("HEHEWOFIDNPFOIJSDPFOJ");
        //     cur->velocity.y = 0.0f;
        // }
        // cur->move_and_slide(cur->velocity, Vector3(0,1,0));
        // if(cur->is_on_floor()){
        //     // Godot::print("HEHEWOFIDNPFOIJSDPFOJ");
        //     cur->velocity.y = 0.0f;
        // }
        cur->force *= 0.0f;
    }
}

void SPHSolver::_collision_handling(){
     for(int ind = 0; ind < particles->size(); ind++){
        Droplet *cur = static_cast<Droplet*>(___get_from_variant(particles->operator[](ind)));
        Vector3 pos = cur->get_global_transform().get_origin();
        Vector3 translate = pos;
        bool fix = false;
        
        if(pos.x > 8.8){
            // translate.y = 3;
            translate.x = 8.8;
            cur->velocity.x *= -0.1f;
            cur->force.x += 1.0f;
            fix = true;
        } 
        if(pos.x < -8.8){
            // translate.y = 3;
            translate.x = -8.8;
            cur->velocity.x *= -0.1f;
            fix = true;
        } 
        if(pos.y > 25){
            translate.y = 13.7;
            cur->velocity.y *= -0.1f;
            fix = true;
        } 
        if(pos.y < 0.4){
            translate.y = 0.4;
            cur->velocity.y *= -0.5f;
            cur->force.y = cur->cur_density * cur->gravity;
            fix = true;
        } 
        if(pos.z > 8.8){
            translate.z = 8.8;
            cur->force.z += -1.0f;
            // translate.z = 0.4;
            cur->velocity.z *= -0.5f;
            fix = true;
        } 
        if(pos.z < -8.8){
            // translate.y = 4;
            cur->force.z += 1.0f;

            translate.z = -8.8;
            cur->velocity.z *= -0.5;
            fix = true;
        } 
        if(fix){
        // printf("VELOCITY INJECTION AHOY \n");
        cur->set_translation(translate);
        }
     }
}

