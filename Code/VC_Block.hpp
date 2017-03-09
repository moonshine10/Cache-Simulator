//
//  VC_Block.hpp
//  
//
//  Created by QinYue Yin on 4/23/16.
//
//

#ifndef VC_Block_hpp
#define VC_Block_hpp



#include <stdio.h>

#endif


class VC_Block{
private:
    unsigned long long int VC_tag;
    unsigned long long int VC_index;
    
public:
    VC_Block();
    VC_Block(unsigned long long int index_in, unsigned long long int tag_in);
    unsigned long long int  VC_get_tag();

    unsigned long long int VC_get_addr_tag(unsigned long long int address,int size,int block_size,int associativity);
    bool  VC_check_tag(unsigned long long int address, int size, int block_size, int associativity);
    unsigned long long int  VC_get_index();
    unsigned long long int  VC_get_addr_index(unsigned long long int address,int size,int block_size,int associativity);
    bool  VC_check_index(unsigned long long int address,int size,int block_size,int associativity);
    bool  VC_check_indexNtag(unsigned long long int address,int size,int block_size,int associativity);




    
};

