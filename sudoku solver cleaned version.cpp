#include<iostream>
#include<vector>
#include<cmath>
#include<algorithm>
using namespace std;


class game{
private:
    int grid_size;
    int zero_counter;
    std::vector<int> grid_values;

    void set_grid_size(){
        std::cin>>grid_size;
        grid_values.resize(grid_size);
    }
    std::vector< pair<int, pair<int, int> > > input_data(){
        std::vector< pair<int, pair<int, int> > > zero_position;
        int temp;
        int block_number = 0;
        int block_counter = 0;
        for(int i = 0; i<grid_size; i++){
            for(int j = 0; j<grid_size; j++){
                std::cin>>temp;
                grid_values.push_back(temp);
                if(temp == 0){
                        zero_counter++;
                    block_number = (j/3) + (i/3)*3;
                    zero_position.push_back( make_pair( block_number, make_pair(j, i)) );
                }

            }
        }
        grid_values.erase(grid_values.begin(), grid_values.begin()+9);//Brute Force solution (MUST BE REMOVED)
        return zero_position;
    }

public:
    void set_grid_value(int x, int y, int value){
        int index = y*grid_size;
        index += x;
        grid_values[index] = value;
    }
    int get_zero_counter(){
        return zero_counter;
    }
    int get_grid_size(){
        return grid_size;
    }
    std::vector<int> get_grid_values(){
        return grid_values;
    }
    std::vector< pair<int, pair<int, int> > > set_game(){
        set_grid_size();
        return input_data();
    }
};

//Global object grid
game grid;

class cell{
private:
    std::pair <int, int> cell_position;
    int block_num;
    std::vector<int> possible_values;
    int current_index = 0;
    int num_of_values;

public:
    int get_num_of_values(){
        return num_of_values;
    }
    int get_current_index(){
        return current_index;
    }
    void set_cell_position(int x, int y){
        cell_position = make_pair(x, y);
    }
    void set_block_number(int z){
        block_num = z;
    }
    void set_current_index(int k){
        current_index = k;
    }
    void remove_possible_value(int k){
        possible_values.erase(find(possible_values.begin(), possible_values.end(), k));
    }

    int get_block_number(){
        return block_num;
    }
    int get_value(){
        return current_index;
    }
    std::pair<int, int> get_cell_position(){
        return cell_position;
    }
    std::vector<int> get_possible_values(){
        return possible_values;
    }
    void possible_entries(){
        std::vector<int> temp_vector;
        std::vector<int> grid_values = grid.get_grid_values();
        int grid_size = grid.get_grid_size();
        temp_vector.resize(grid_size);
        possible_values.resize(grid_size);

        for(int i = 0; i<grid_size; i++)
            temp_vector[i] = 0;

        possible_values.clear();

        for(int i = 0; i<grid_size; i++){
            if(grid_values[grid_size*cell_position.second +i] != 0)
                temp_vector[grid_values[grid_size*cell_position.second +i] - 1] = 1;
            if(grid_values[cell_position.first +i*grid_size] != 0)
                temp_vector[grid_values[cell_position.first +i*grid_size] - 1] = 1;
        }
        int level = floor(block_num/sqrt(grid_size));
        int start_index = sqrt(grid_size)* grid_size * level + (block_num - level*sqrt(grid_size)) * sqrt(grid_size);

        for(int i = 0; i<sqrt(grid_size);i++){
            for(int j = start_index + i*grid_size; j<start_index + i*grid_size +3; j++){
                if(grid_values[j] != 0)
                    temp_vector[grid_values[j] -1] = 1;
            }
        }

        for(unsigned int i = 0; i<temp_vector.size(); i++)
            if(temp_vector[i] == 0)
                possible_values.push_back(i + 1);
        num_of_values = possible_values.size();
    }
};


bool solved = true;

void game_solver(cell unsolved_cell[], int current_cell, int zero_counter){
    if(zero_counter == 0){
        std::cout<<"Done!";
        return;
    }
    std::pair<int, int> position = unsolved_cell[current_cell].get_cell_position();//getting x,y coordinates of the cell
    if(unsolved_cell[current_cell].get_current_index() < unsolved_cell[current_cell].get_num_of_values() && unsolved_cell[current_cell].get_num_of_values()){ //Still has possibilities
        //Setting the current empty cell with the possibility[current_index]
        grid.set_grid_value(position.first, position.second, unsolved_cell[current_cell].get_possible_values()[unsolved_cell[current_cell].get_current_index()]);
        //Incrementing current index
        current_cell++;
        zero_counter--;
        //setting the new possibilities
        for(int i = current_cell; i<grid.get_zero_counter(); i++)
            unsolved_cell[i].possible_entries();
    } else { //Ran out of possibilities
        grid.set_grid_value(position.first, position.second, 0);
        unsolved_cell[current_cell].set_current_index(0);
        current_cell--;
        zero_counter++;
        unsolved_cell[current_cell].set_current_index(unsolved_cell[current_cell].get_current_index() + 1);
    }
    if(current_cell < 0){
        solved = false;
        std::cout<<"No Solution!";
        return;
    } else {
        game_solver(unsolved_cell, current_cell, zero_counter);
    }

}

int main(){

    int empty_cells;
    std::vector< pair<int, pair<int, int> > > zero_position = grid.set_game();
    cell unsolved_cell[grid.get_zero_counter()];
    empty_cells = zero_position.size();

    for(unsigned int  i = 0; i<zero_position.size();i++){
        unsolved_cell[i].set_block_number(zero_position[i].first);
        unsolved_cell[i].set_cell_position(zero_position[i].second.first, zero_position[i].second.second);
    }

    for(int i = 0; i<empty_cells; i++){
        unsolved_cell[i].possible_entries();
    }




    game_solver(unsolved_cell, 0, grid.get_zero_counter());

    if(solved){
        std::cout<<"\n\ngrid_values after solving\n";
        for(int i = 0; i<grid.get_grid_size(); i++){
            for(int j = 0; j< grid.get_grid_size(); j++){
                std::cout<<grid.get_grid_values()[j + i*grid.get_grid_size()]<<" ";
            }
            std::cout<<"\n";
        }
    }


    return 0;
}
