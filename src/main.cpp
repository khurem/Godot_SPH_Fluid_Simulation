#include "main.h"


using namespace std;
using namespace godot;


void Main::_register_methods() {
    register_method("_ready", &Main::_ready);
    register_method("_process", &Main::_process);
}


Main::Main() {
}

Main::~Main() {
}

void Main::_init() {
  
    // this->add_child(view);
    // OS *oss;
    // oss->_init();
    // oss->set_window_size(view->get_size());
      sph = new SPHSolver();
    sph->_init();
    sph->_ready();
    // material = new SpatialMaterial();
    // part_mesh = new MeshInstance();
    // part_mesh->set_surface_material(0, material);
  
	
 
}

void Main::_ready() {
      view = static_cast<Viewport*>(this->get_child(1)->get_child(0));
    // Godot::print(this->get_child(0)->get_class());
      Vector2 sez = 400 * Vector2(3, 1.5);
      view->set_size(sez);
      
      
    //   this->get_parent();
    //   Godot::print(this->get_parent()->get_parent()->get_class());
    // material->set_texture(material->TEXTURE_ALBEDO, text);
    // printf("I HAVE THIS MANY %d \n", sph->particles->size());
    // material->set_blend_mode(1);
      ResourceLoader* loader = ResourceLoader::get_singleton();
    //   Godot::print(this->get_path());
    text= loader->load("res://Sphere.tscn");
    for(int p = 0; p < sph->particles->size(); p++){
         Particle *cur = static_cast<Particle*>(___get_from_variant(sph->particles->operator[](p)));
        Spatial* part_draw = static_cast<Spatial *>(text->instance());
        // part_draw->add_child(part_mesh);
        Godot::print(cur->position * scale);
        view->add_child(part_draw);
        part_draw->set_translation(cur->position * scale);
        // Godot::print("And then");
        // Godot::print(part_draw->get_global_transform().get_origin());
      
        // part_draw->set_scale(def_scale); 
        
        


    }
    printf("doifsojpdof %d \n",view->get_child_count());    
}

void Main::_process(float delta) {
    if(start < 300){
        start ++;
        sph->_process(0.05);
    }else{
        sph->_process(delta);
    }
    
    for(int p = 0; p < sph->particles->size(); p++){
        Spatial* cur = static_cast<Spatial*>(view->get_child(p + 1 ));
        Particle* mirror = static_cast<Particle*>(___get_from_variant(sph->particles->operator[](p)));
       
        Vector3 loc = (1 - reset) * cur->get_global_transform().get_origin() + reset * (mirror->position * scale);
        cur->set_translation(loc);
        // Godot::print(loc);
        
    }

}










