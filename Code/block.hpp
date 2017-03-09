//
//  block.hpp
//  
//
//  Created by QinYue Yin on 4/16/16.
//
//

#ifndef block_hpp
#define block_hpp

#include <stdio.h>

#endif 
class block{
private:
    unsigned long long int tag;
    bool dirty;
    bool valid;
    
    
public:
    block();
    bool get_dirty();
    
    bool get_valid();
    unsigned long long int get_tag();
    void set_dirty(bool new_dirty);
    void set_valid(bool new_valid);
    
    void set_tag(unsigned long long int address, int size,int block_size,  int associativity);
    unsigned long long int get_addr_tag(unsigned long long int address, int size,int block_size,  int associativity);
    bool check_tag(unsigned long long int address,  int size, int block_size,int associativity);
    unsigned long long int get_addr_VC_tag(unsigned long long int address, int block_bits);
    void set_VC_tag(unsigned long long int address,  int block_bits);
    bool check_VC_tag(unsigned long long int address, int block_bits );

    

    
    
    
    
    
    
    
    
};