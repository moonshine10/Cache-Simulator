//
//  block.cpp
//  
//
//  Created by QinYue Yin on 4/16/16.
//
//

#include "block.hpp"

block::block()
{
    dirty = 0;
    tag = 0;
    valid = 0;
    
}


bool block::get_dirty(){
    return dirty;
}

bool block::get_valid(){
    return valid;
}



unsigned long long int block::get_tag(){
    return tag;
}

void block::set_dirty(bool new_dirty){
    dirty = new_dirty;
    
}
void block::set_valid(bool new_valid){
    valid = new_valid;
    
}

/***********************  tag function ***************************/

void block::set_tag(unsigned long long int address,  int size, int block_size,int associativity)
{ //set tag to address's tag
    
    if(block_size == 32 && size == 8192){
        if(associativity == 1)
            tag = address & (~(0x1FFF));    ///1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1110 0000 0000 0000
        else if(associativity == 2)
            tag = address &  (~(0xFFF));  //1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 0000 0000 0000
        else if(associativity == 4)
            tag = address &   (~(0x7FF));//1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1000 0000 0000
        else if(associativity == 8)
            tag = address & (~(0x3FF));  //1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1100 0000 0000
        else if(associativity == 256)
            tag = address & (~(0x1F));  //1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1110 0000
    }
    else if(block_size == 32 && size == 4096){
        if (associativity == 1)
            tag = address & (~(0xFFF));//1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 0000 0000 0000
        if(associativity == 4)
            tag = address & (~(0x3FF));  //1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1100 0000 0000
       } 
    
    else if(block_size == 64 && size == 32768){
        if(associativity == 1)
            tag = address & (~(0x7FFF));  //1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1000 0000 0000 0000
        else if(associativity == 2)
            tag = address & (~(0x3FFF));//1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1100 0000 0000 0000
        else if(associativity == 4)
            tag = address & (~(0x1FFF));  //1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1110 0000 0000 0000

        
        else if(associativity == 512)
            tag = address & (~(0x3F));//1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1100 0000
    }
    else if(block_size == 64 && size == 16384)
        tag = address & (~(0x3FFF));//1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1100 0000 0000 0000
}

unsigned long long int block::get_addr_tag(unsigned long long int address, int size,int block_size,  int associativity)
{
    
    //get tag from the incoming address
    if(block_size == 32 && size == 8192){
        if(associativity == 1)
            return (address & (~(0x1FFF)));    ///1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1110 0000 0000 0000
        else if(associativity == 2)
            return (address &  (~(0xFFF)));  //1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 0000 0000 0000
        else if(associativity == 4)
            return (address &   (~(0x7FF)));//1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1000 0000 0000
        else if(associativity == 8)
            return (address & (~(0x3FF)));  //1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1100 0000 0000
        else if(associativity == 256)
            return (address & (~(0x1F)));  //1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1110 0000
    }
    else if(block_size == 32 && size == 4096){
        if (associativity == 1)
            return (address & (~(0xFFF)));//1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 0000 0000 0000
        if(associativity == 4)
            return (address & (~(0x3FF)));  //1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1100 0000 0000
    }
    else if(block_size == 64 && size == 32768){
        if(associativity == 1)
            return (address & (~(0x7FFF)));  //1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1000 0000 0000 0000
        else if(associativity == 2)
            return (address & (~(0x3FFF)));//1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1100 0000 0000 0000
        else if(associativity == 4)
            return (address & (~(0x1FFF)));  //1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1110 0000 0000 0000
        else if(associativity == 512)
            return (address & (~(0x3F)));//1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1100 0000
    }
    else if(block_size == 64 && size == 16384)
        return (address & (~(0x3FFF)));//1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1100 0000 0000 0000
    
}

bool block::check_tag(unsigned long long int address,  int size, int block_size,int associativity)
{
    printf("get_tag(): 0x%x \n\n",get_tag());
    printf("get_addr_tag(): 0x%x \n\n",get_addr_tag(address,size,block_size,associativity));
    return (get_tag()==get_addr_tag(address,size,block_size,associativity));
 
}
/*********************** VC tag function ***************************/
unsigned long long int block::get_addr_VC_tag(unsigned long long int address, int block_bits)
{
    if(block_bits==5)
    {
        return (address&(~(0x1F)));
    }
    else if (block_bits==6)
    {
        return (address&(~(0x2F)));
    }
    
}

void block::set_VC_tag(unsigned long long int address,  int block_bits)
{ //set tag to address's tag
    
    if(block_bits==5)
    {
        tag= (address&(~(0x1F)));
    }
    else if (block_bits==6)
    {
        tag= (address&(~(0x2F)));
    }
    
}


bool block::check_VC_tag(unsigned long long int address, int block_bits )
{
    return (get_tag()==get_addr_VC_tag(address,block_bits));
}

