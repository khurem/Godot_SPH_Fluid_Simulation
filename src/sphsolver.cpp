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
    if(particles->size() < max_particles){
        Droplet *drop = static_cast<Droplet*>(dropper->instance());
        float addy = particles->size();
        float xsh = particles->size() % 2 ==0 ? 1 : -1;
        float zsh = -xsh;
        drop->translate(Vector3(0.0f + xsh, 20.0f + addy, 0.0f + zsh));
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
	// collision_handling();
}


float SPHSolver::_kernel(Vector3 pos_delta){
    real_t r = pos_delta.length_squared();
    real_t h = kernel_range * kernel_range;
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
    Vector3 norm_delta = pos_delta.normalized(); 
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
                Vector3 pos_delta = cur->get_global_transform().get_origin() - bor->get_global_transform().get_origin();
                total_density += cur->mass * _kernel(pos_delta);
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
                Vector3 pos_delta = cur->get_global_transform().get_origin() - bor->get_global_transform().get_origin();
                temp_pressure += bor->mass * (cur->pressure + bor->pressure) / (2.0 * bor->cur_density) * _grad_kernel(pos_delta);
                temp_viscosity += bor->mass * (bor->velocity - cur->velocity) / (bor->cur_density) * _laplace_kernel(pos_delta);
            }
        }
        temp_gravity = cur->cur_density * Vector3(0, cur->gravity, 0);
        temp_pressure * -1;
        temp_viscosity *= cur->viscosity;
        // Godot::print("start");
        // Godot::print(temp_pressure);
        // Godot::print("end");
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
        Godot::print(cur->force);
        Vector3 shift = delta * cur->velocity;
        // cur->set_translation(cur_pos);
         if(cur->is_on_floor() || cur_pos.y < 1){
            // Godot::print("HEHEWOFIDNPFOIJSDPFOJ");
            cur->velocity.y = 0.0f;
        }
        cur->move_and_slide(cur->velocity, Vector3(0,1,0));
        if(cur->is_on_floor()){
            // Godot::print("HEHEWOFIDNPFOIJSDPFOJ");
            cur->velocity.y = 0.0f;
        }
        cur->force *= 0.0f;
    }
}
/*func integration_step(delta):
	for i in range(number_particles):
		particles[i].velocity += delta * particles[i].force / particles[i].density
		particles[i].position += delta * particles[i].velocity
		
		particles[i].force *= 0*/


