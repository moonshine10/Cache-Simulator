//
//  index.hpp
//  
//
//  Created by QinYue Yin on 4/16/16.
//
//

#ifndef index_hpp
#define index_hpp

#include <stdio.h>
#include "block.hpp"

#endif 

class index{
private:
    int associativity;
    unsigned long long int index_num;
    int transfers;
    int kickouts;

    
    
public:
    block * LRU_block;
    index();
     index(unsigned long long int index_num_in, int asso_in);
    unsigned long long int index_associativity(unsigned long long int index_num_in, int asso_in);


    unsigned long long int read_index(unsigned long long int address, int size, int block_size, int associativity);
    bool search_tag(unsigned long long int address, int size, int block_size);
    void update_LRU(unsigned long long int address_in, int size, int block_size);
    bool check_LRU(unsigned long long int address,  int size,int block_size);
    unsigned long long int get_addr_tag(unsigned long long int address,  int size,int block_size, int associativity);
    void write_empty_block(unsigned long long int address,int size ,int block_size);
    bool check_not_valid(int size ,int block_size);
    
    void move_everything_down();
    void set_top_LRU(unsigned long long int address,bool dirty, int valid);
    void write_to_top_block(unsigned long long int address,int size ,int block_size, int valid, bool dirty );
    bool get_block_dirty(int block_num);
    void write_to_top_block(unsigned long long int address,int size ,int block_size, bool dirty );

    void write_to_top_VC_block(unsigned long long int address,bool dirty, int block_bits );
    int get_block_tag(int block_num);
    void print_LRU_block_info();

    
    
    
    



    

   
   
};
