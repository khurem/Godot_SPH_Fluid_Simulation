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

    
    Vector3 _particles = Vector3(max_particles /4, max_particles/2, max_particles/4);
    float num_particles = _particles.x * _particles.y * _particles.z;
    float s_width = width / 2.0f;
    float s_height = 3.0f * height / 4.0f;
    float s_depth = depth;
    AABB *particle_box = new AABB(Vector3((width - s_width) * 2.0f, height - s_height, (depth-s_depth) * 2.0f), Vector3(s_width, s_height, s_depth));
    RandomNumberGenerator *rando = RandomNumberGenerator::_new();
    Vector3 q = particle_box->size / _particles;
    for(int i = 0; i < _particles.x; i++){
        for(int j =0; j < _particles.y; j++){
            for(int k = 0; k < _particles.z; k++){
                
                Vector3 posit = Vector3(i, j, k) * q + Vector3(0.0f, 0.5f, 0.0f);
                posit.x += rando->randf_range(-0.001f, 0.001f);
                posit.y += rando->randf_range(-0.001f, 0.001f);
                posit.z += rando->randf_range(-0.001f, 0.001f);
                Particle *drop = Particle::_new();
                drop->position = posit;
                particles->append(drop);
            }
        }
    }
    printf("prticles comes out to %d \n", particles->size());

}

void SPHSolver::_ready() {
      
   
    // ResourceLoader *loader = ResourceLoader::get_singleton();
    // dropper = loader->load("res://Sphere.tscn");
    // spat_map->_reset_grid();
    // for(int ind = 0; ind < particles->size(); ind++){
    //     Particle *cur = static_cast<Particle*>(___get_from_variant(particles->operator[](ind)));

    //     spat_map->_add_to_grid(cur, ind);
    // }
    
}

Array SPHSolver::_create_neighborhood(){
    Array neighborhood = Array();
    for(int ind = 0; ind < particles->size(); ind++){
        Particle *cur = static_cast<Particle*>(___get_from_variant(particles->operator[](ind)));
        neighborhood.append(spat_map->_get_neighbors(cur, particles));
    }
    return neighborhood;
}

void SPHSolver::_process(float delta) {
   
    
    // while(particles->size() < max_particles){
    //     Particle *drop = Particle::_new();
    //     // dropposition;
    //     // drop->set_translation
        
        
    //     drop->position = Vector3(xval, yval, zval);
    //     // Godot::print(dropposition);
    //     xval += 2.0f;
    //     if(xval > 8.1f){
    //         xval = -8.8f;
    //         yval += 2.0f;
    //     }
    //     if(yval > 20.0f){
    //         yval = 10.0f;
    //         zval += 2.0f;
    //         xval = -8.8f;
    //     }
    //     Godot::print(drop->position);
    //     // this->get_parent()->add_child(drop);
    //     particles->append(drop);
    //     // count++;
    // }
    spat_map->_reset_grid();
    for(int ind = 0; ind < particles->size(); ind++){
        Particle *cur = static_cast<Particle*>(___get_from_variant(particles->operator[](ind)));

        spat_map->_add_to_grid(cur, ind);
    }
    this->neighborhood = this->_create_neighborhood();

    _calculate_density();
    _calculate_pressure();
    _calculate_force_pressure();
    _integration_step(delta);
    _collision_handling();
    // Godot::print("finished neighborhood");
    // Godot::print("finished density");
	// _calculate_pressure();
	// _calculate_force_pressure();
	// _integration_step(delta);
    // _collision_handling();
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
    Vector3 norm_delta = pos_delta / len;
    real_t surround = pow(kernel_range - len, 2);
    // Godot::print(norm_delta);
    // printf("consurround is %f \n", surround);
    return -GRAD_KERNEL_CONST * norm_delta * surround;
}

float SPHSolver::_laplace_kernel(Vector3 pos_delta){
    real_t len = pos_delta.length();
    return GRAD_KERNEL_CONST * (kernel_range - len);
}

void SPHSolver::_calculate_density(){
    for(int ind = 0; ind < particles->size(); ind++){
        Particle *cur = static_cast<Particle*>(___get_from_variant(particles->operator[](ind)));
        Array cur_nei = this->neighborhood[ind];
        
        float total_density = 0.0f;
        // Godot::print("dsfsdfsdf");
        for(int ne = 0; ne < cur_nei.size(); ne++){
            // printf("Size of temp is %d \n", cur_nei.size());
            int index = cur_nei[ne].operator unsigned int();
            // godot::PoolIntArray t =  var.split_ints(',');
            
            // Godot::print(var);
          
                
                Particle *bor = static_cast<Particle*>(___get_from_variant(particles->operator[](index)));
                // float kernel_chk = cur->position.distance_squared_to(bor->position);
                Vector3 pos_delta = cur->position - bor->position;
                float dirs = cur->position.distance_squared_to(bor->position);
                // printf("mass %f and delta pos %f %f %f and kernel return %f\n", this->particle_mass, pos_delta.x, pos_delta.y, pos_delta.z, _kernel(pos_delta));
                // printf("distance is %f dist2 is %f and kern is  %f for %d and %d\n", pos_delta.length_squared(), dirs,  cur->max_dist, ind, index);
                // if(kernel_chk <= cur->max_dist){
                    // Vector3 pos_delta = cur->position - bor->position;
             

                    total_density += this->particle_mass * _kernel(pos_delta);
                // }
            
        }
        cur->density = total_density;
        // printf("TOTAL DENSITY HERE IS %f \n", cur->density);

    }
}

void SPHSolver::_calculate_pressure(){
     for(int ind = 0; ind < particles->size(); ind++){
        Particle *cur = static_cast<Particle*>(___get_from_variant(particles->operator[](ind)));
        cur->pressure = max(cur->stiffness * (cur->density - cur->rest_density), 0.0f);
        // Godot::print("start");
        // Godot::print(cur->pressure);
        // printf("TOTAL PRESSURE HERE IS %f \n", cur->pressure);
        // Godot::print("end");
    }
}

void SPHSolver::_calculate_force_pressure(){
    for(int ind = 0; ind < particles->size(); ind++){
        Vector3 temp_pressure = Vector3(0.0f, 0.0f, 0.0f);
        Vector3  temp_viscosity = Vector3(0.0f, 0.0f, 0.0f);
        Vector3 temp_gravity = Vector3(0.0f,0.0f,0.0f);
        Particle *cur = static_cast<Particle*>(___get_from_variant(particles->operator[](ind)));
        Array cur_nei = this->neighborhood[ind];
        
        float total_density = 0.0f;
        // Godot::print("dsfsdfsdf");
        for(int ne = 0; ne < cur_nei.size(); ne++){
            // printf("Size of temp is %d \n", cur_nei.size());
            // godot::PoolIntArray t =  var.split_ints(',');
            int index = cur_nei[ne].operator unsigned int();
                Particle *bor = static_cast<Particle*>(___get_from_variant(particles->operator[](index)));

                Vector3 pos_delta = cur->position - bor->position;
                temp_pressure += this->particle_mass * (cur->pressure + bor->pressure) / (2.0 * bor->density) * _grad_kernel(pos_delta);
                    // printf("distance is %f for %d and %d %d %d\n", pos_delta.length_squared(), bor->get_index(), cur->get_index(), ind, index);
// printf("all pressure vals are %f %f %f %f %f %f %f %f\n", this->particle_mass, bor->density, cur->pressure, bor->pressure, cur->density, _grad_kernel(pos_delta)[0], _grad_kernel(pos_delta)[1], _grad_kernel(pos_delta)[2]);
                temp_viscosity += this->particle_mass * (bor->velocity - cur->velocity) / (bor->density) * _laplace_kernel(pos_delta);
                
            
        }
        temp_gravity = cur->particle_mass * Vector3(0, cur->gravity, 0);
        temp_pressure *= -1;
        temp_viscosity *= cur->viscosity_const;
        // Godot::print("grav, pressure, visc");
        // Godot::print(temp_gravity);
        // Godot::print(temp_pressure);
        // Godot::print(temp_viscosity);
        cur->viscosity = temp_viscosity.length();
        cur->force += temp_pressure + temp_viscosity + temp_gravity;
        // Godot::print(cur->force);

    }
}

void SPHSolver::_integration_step(float delta){
    for(int ind = 0; ind < particles->size(); ind++){
        Particle *cur = static_cast<Particle*>(___get_from_variant(particles->operator[](ind)));
        cur->velocity += delta * cur->force / cur->density;
        
        cur->position += delta * cur->velocity;
        cur->force *= 0.0f;
      
    }
}

void SPHSolver::_collision_handling(){
     for(int ind = 0; ind < particles->size(); ind++){
        Particle *cur = static_cast<Particle*>(___get_from_variant(particles->operator[](ind)));
        if(cur->position.x < 0){
            cur->position.x = 0;
            cur->velocity.x *= -0.1;
        } else if(cur->position.x > width){
            cur->position.x = width;
            cur->velocity.x *= -0.1;
        }

        if(cur->position.y < 0){
            cur->position.y = 0;
            cur->velocity.y *= -0.1;
        } 
        else if(cur->position.y > height){
            cur->position.y = height;
            cur->velocity.y *= -0.1;
        }

        if(cur->position.z < 0){
            cur->position.z = 0;
            cur->velocity.z *= -0.1;
        } else if(cur->position.z > depth){
            cur->position.z = depth;
            cur->velocity.z *= -0.1;
        }
     }
}

