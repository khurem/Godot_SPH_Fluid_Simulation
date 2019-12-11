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
        Particle *cur = static_cast<Particle*>(___get_from_variant(particles->operator[](ind)));
        neighborhood.append(spat_map->_get_neighbors(cur));
    }
    return neighborhood;
}

void SPHSolver::_process(float delta) {
    // Godot::print("process start");
      float xval = -8.8f;
    float yval = 10.0f;
    float zval = -8.8f;
    
    Vector3 _particles = Vector3(max_particles /2, max_particles /2, max_particles);
    float num_particles = _particles.x * _particles.y * _particles.z;
    float s_width = width / 4.2;
    float s_height = 3.0f * height / 4.0f;
    float s_depth = 3.0f * depth / 4.0f;
    AABB *particle_box = new AABB(Vector3((width - s_width) /2.0f, height - s_height, depth-s_depth), Vector3(width, height, depth));
    RandomNumberGenerator *rando = new RandomNumberGenerator();
    Vector3 q = particle_box->size / _particles;
    for(int i = 0; i < _particles.x; i++){
        for(int j =0; j < _particles.y; j++){
            for(int k = 0; k < _particles.z; k++){
                
                Vector3 posit = Vector3(i, j, k) * q + Vector3(0, 0.25, 0.25);
                posit.x += rando->randf_range(-0.001, 0.001);
                posit.y += rando->randf_range(-0.001, 0.001);
                posit.z += rando->randf_range(-0.001, 0.001);
                Particle *drop = Particle::_new();
                drop->position = posit;
                particles->append(drop);
            }
        }
    }
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
    // spat_map->_reset_grid();
    // for(int ind = 0; ind < particles->size(); ind++){
    //     Particle *cur = static_cast<Particle*>(___get_from_variant(particles->operator[](ind)));
    //     spat_map->_add_to_grid(cur, ind);
    // }
    // this->neighborhood = this->_create_neighborhood();
    // Godot::print("finished neighborhood");
    // _calculate_density();
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
        Particle *cur = static_cast<Particle*>(___get_from_variant(particles->operator[](ind)));
        Array cur_nei = this->neighborhood[ind];
        
        float total_density = 0.0f;
        // Godot::print("dsfsdfsdf");
        for(int ne = 0; ne < cur_nei.size(); ne++){
            printf("Size of temp is %d \n", cur_nei.size());
            godot::String var = cur_nei[ne].operator godot::String();
            // godot::PoolIntArray t =  var.split_ints(',');
            godot::PoolStringArray r = var.split(", ");
            // Godot::print(var);
            for(int con = 0; con < r.size(); con++){
                int index = r.operator[](con).to_int();
                Particle *bor = static_cast<Particle*>(___get_from_variant(particles->operator[](index)));
                float kernel_chk = cur->position.distance_squared_to(bor->position);
                       Vector3 pos_delta = cur->position - bor->position;
                    // printf("mass %f and delta pos %f %f %f and kernel return %f\n", this->particle_mass, pos_delta.x, pos_delta.y, pos_delta.z, _kernel(pos_delta));
                //  printf("distance is %f for %d and %d %d %d\n", pos_delta.length_squared(), bor->get_index(), cur->get_index(), ind, index);
                if(kernel_chk <= cur->max_dist){
                    // Vector3 pos_delta = cur->position - bor->position;
             

                    total_density += this->particle_mass * _kernel(pos_delta);
                }
            }
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
        // printf("TOTAL PRESSURE HERE IS %f \n", cur->density);
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
            godot::String var = cur_nei[ne].operator godot::String();
            // godot::PoolIntArray t =  var.split_ints(',');
            godot::PoolStringArray r = var.split(", ");
            // Godot::print(var);
            for(int con = 0; con < r.size(); con++){
                
                int index = r.operator[](con).to_int();
                Particle *bor = static_cast<Particle*>(___get_from_variant(particles->operator[](index)));
                float kernel_chk = cur->position.distance_squared_to(bor->position);
                
                if(kernel_chk <= cur->max_dist){

                    Vector3 pos_delta = cur->position - bor->position;
                    temp_pressure += this->particle_mass * (cur->pressure + bor->pressure) / (2.0 * bor->density) * _grad_kernel(pos_delta);
                    // printf("distance is %f for %d and %d %d %d\n", pos_delta.length_squared(), bor->get_index(), cur->get_index(), ind, index);
printf("all pressure vals are %f %f %f %f %f %f %f %f\n", this->particle_mass, bor->density, cur->pressure, bor->pressure, cur->density, _grad_kernel(pos_delta)[0], _grad_kernel(pos_delta)[1], _grad_kernel(pos_delta)[2]);
                    temp_viscosity += this->particle_mass * (bor->velocity - cur->velocity) / (bor->density) * _laplace_kernel(pos_delta);
                }
            }
        }
        temp_gravity = cur->density * Vector3(0, cur->gravity, 0);
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
        Particle *cur = static_cast<Particle*>(___get_from_variant(particles->operator[](ind)));
        cur->velocity += delta * cur->force / cur->density;
        
        Vector3 cur_pos = cur->position;
        // cur_pos += delta * cur->velocity;
        // Godot::print(cur->force);
        Vector3 shift = delta * cur->velocity;
        // cur->translate(shift);
        //  if(cur->is_on_floor() || cur_pos.y < 1){
        //     // Godot::print("HEHEWOFIDNPFOIJSDPFOJ");
        //     cur->velocity.y = 0.0f;
        // }
        // godot::Ref<KinematicCollision> col = cur->move_and_collide(0.1 * shift, true, true, true);
        // if(col != nullptr){
        //     // Vector3 dir = cur->position - col->get_class()
        //     // cur->velocity = 
        //     // char* body = "KinematicBody";
        //     if(col->get_collider()->get_class() == "KinematicBody"){
                
        //         Particle* other = static_cast<Particle*>(col->get_collider());
        //         Vector3 dir = (cur->position - other->position).normalized();
        //         Vector3 parVel = (cur->velocity.dot(dir)) * dir;
        //         Vector3 opdir = (other->position - cur->position).normalized();
        //         Vector3 othVel = (other->velocity.dot(opdir)) * opdir;
        //         other->velocity = other->velocity - othVel;
        //         Vector3 prev = cur->velocity;
        //         cur->velocity = cur->velocity - parVel;

        //     } else{
        //        printf("the string here is %s \n", col->get_collider()->get_class().alloc_c_string());
        //     }
        //     // Godot::print();
        // }

        // // cur->add_central_force(cur->velocity);
        // // cur->set_linear_velocity(cur->velocity);
        // // cur->add_force(cur->force, cur_pos);
        // // if(cur->is_on_floor()){
        // //     // Godot::print("HEHEWOFIDNPFOIJSDPFOJ");
        // //     cur->velocity.y = 0.0f;
        // // }
        // cur->force *= 0.0f;
    }
}

void SPHSolver::_collision_handling(){
     for(int ind = 0; ind < particles->size(); ind++){
        Particle *cur = static_cast<Particle*>(___get_from_variant(particles->operator[](ind)));
        Vector3 pos = cur->position;
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
        if(pos.y > 35){
            translate.y = 13.7;
            cur->velocity.y *= -0.1f;
            fix = true;
        } 
        if(pos.y < 0.4){
            translate.y = 0.4;
            cur->velocity.y *= -0.1f;
            cur->force.y = cur->density * cur->gravity;
            fix = true;
        } 
        if(pos.z > 8.8){
            translate.z = 8.8;
            // cur->force.z += -0.1f;
            // translate.z = 0.4;
            cur->velocity.z *= -0.1f;
            fix = true;
        } 
        if(pos.z < -8.8){
            // translate.y = 4;
            // cur->force.z += 0.1f;

            translate.z = -8.8;
            cur->velocity.z *= -0.1;
            fix = true;
        } 
        if(fix){
        // printf("VELOCITY INJECTION AHOY \n");
        cur->position = translate;
        }
     }
}

