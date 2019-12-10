#include "grid.h"


using namespace std;
using namespace godot;


void Grid::_register_methods() {
    register_method("_ready", &Grid::_ready);
    register_method("_process", &Grid::_process);
    register_method("_reset_grid", &Grid::_reset_grid);
    // register_method("_on_body_entered", &Grid::_on_body_entered);
}


Grid::Grid() {
}

Grid::~Grid() {
}

void Grid::_init() {
    
    grid = new Dictionary();
}

void Grid::_ready() {
      
    


    
}

void Grid::_process(float delta) {
   
}

Array Grid::_get_neighbors(Droplet* center){
    Vector3 drop_pos = center->get_global_transform().get_origin();
    
    int indx = int(drop_pos[0] / center->_get_kernel_size());
    int indy = int(drop_pos[1] / center->_get_kernel_size());
    int indz = int(drop_pos[2] / center->_get_kernel_size());
    Vector3 indexer = Vector3(indx, indy, indz);

    Array neighbors = Array();

    for(int x = -1; x < 2; x ++){
        for(int y = -1; y < 2; y ++){
            for(int z = -1; z < 2; z ++){
                Vector3 neighbor = Vector3(x + indx, y + indy, z + indz);
                Array temp = static_cast<Array>(grid->operator[](neighbor));
                if(grid->has(neighbor) and temp.size() > 0){
                    // Godot::print("testestest");
                    // printf("Size of temp is %d \n", temp.size());
                    neighbors.append(temp);
                } else{
                    // printf("I have nothing at %d %d %d\n", x + indx, y + indy, z + indz);
                }
            }
        }
    }
    // if(neighbors.size() > 27){
    //     printf("the size of my friends %d\n", neighbors.size());
    // }
    
    return neighbors;
}

void Grid::_add_to_grid(Droplet* new_drop, int idx){
    Vector3 drop_pos = new_drop->get_global_transform().get_origin();
    int indx = int(drop_pos[0] / new_drop->_get_kernel_size());
    int indy = int(drop_pos[1] / new_drop->_get_kernel_size());
    int indz = int(drop_pos[2] / new_drop->_get_kernel_size());
    Vector3 indexer = Vector3(indx, indy, indz);
    // Godot::print(indexer);
    if(!grid->has(indexer)){
        grid->operator[](indexer) = Array();
    }
    Array temp = static_cast<Array>(grid->operator[](indexer));
  
    temp.append(idx);
}

void Grid::_reset_grid(){
    grid = new Dictionary();
}







