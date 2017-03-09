//
//  cache.hpp
//  
//
//  Created by QinYue Yin on 4/16/16.
//
//

#ifndef cache_hpp
#define cache_hpp

#include <stdio.h>
#include "index.hpp"




#endif 

class cache{
    
private:
    int size;
    int block_size;
    int associativity;
    index * index_array;
    int kickout_count;
    
    
public:
    block *VC_block;
    block *buffer_block;
    int get_block_size();
    int get_associativity();
    int get_size();
    cache(int size_in, int asso_in, int block_size_in);
    unsigned long long int get_index(unsigned long long int address);
    unsigned long long int get_addr_index(unsigned long long int address);
    bool if_hit(unsigned long long int address_in);
    bool find_index(unsigned long long int address, int ref_num);
    unsigned long long int get_addr_tag(unsigned long long int address);
    bool if_hit_VC(unsigned long long int address_in, int offset_bits);
    void update_block(unsigned long long int address_in);
    void update_empty_block(unsigned long long int address_in);
    bool if_empty(unsigned long long int address_in);
    
    void swap_VCnC(unsigned long long int address_in, int block_bits);
    void write_to_top_VC_block(unsigned long long int address,bool dirty, int block_bit);
    unsigned long long int reverse_index(unsigned long long int index);
    unsigned long long int  cache_out(unsigned long long int address_in, int block_bits);
    int get_kickout_count();
    void print_info();
    void print_VC_info();




    
};



