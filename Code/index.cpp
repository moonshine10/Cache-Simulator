//
//  index.cpp
//  
//
//  Created by QinYue Yin on 4/16/16.
//
//

#include "index.hpp"




index::index()
{
    kickouts = 0;
    transfers = 0;
    associativity = 1;
    
}

index::index(unsigned long long int index_num_in, int asso_in)
{
    index_num=index_num_in;
    kickouts = 0;
    transfers = 0;
    associativity = asso_in;
    LRU_block= new block[associativity];
    
    
}

unsigned long long int index::index_associativity(unsigned long long int index_num_in, int asso_in){
    index_num=index_num_in;
    kickouts = 0;
    transfers = 0;
    associativity = asso_in;
    LRU_block= new block[associativity];
    return associativity;
}

unsigned long long int index::read_index(unsigned long long int address,  int size, int block_size,int associativity)
{
    if(block_size==32)
    {//L1 cache
        if (associativity==1&&size==8)
        {
        return (address & (0x1FE0)); // 0001 1111 1110 0000
        }
    }

}
bool index::check_not_valid(int size ,int block_size)
{
    for(int i = 0; i < associativity; i++){
        if(!(LRU_block[i].get_valid())){
            return 1;
        }
    }
        return 0;
}


bool index::check_LRU(unsigned long long int address,int size ,int block_size)
{
    for(int i = 0; i < associativity; i++){
        if(LRU_block[i].check_tag(address, size, block_size,  associativity)){
            // grab the tag value of the input address  ( without rightshitfing) .
            // it it equals to 'pre-set tag value' return 1
            return 1;
        }
    }
    return 0;
}



void index::move_everything_down()
{
    for (int i=(associativity-2); i>=0; i--)
    {
        LRU_block[i+1]=LRU_block[i];
        
    }
}

void index::update_LRU(unsigned long long int address_in,  int size, int block_size)
{
    
    int rank;
    block temp_block;
    
    for(int i = 0; i < associativity; i++)
    
      
        if (LRU_block[i].check_tag(address_in,size,block_size,associativity))//if tag matches
        {
            temp_block=LRU_block[i];
            rank=i;
            for (int j=(rank-1); j>=0; j--)//rearrange order
            {
                LRU_block[j+1]=LRU_block[j];
            }
            LRU_block[0]=temp_block;
            
        }
    
}

bool index::search_tag(unsigned long long int address, int size, int block_size)
{
    
    for (int i=0; i<associativity; i++)
    {
        if(LRU_block[i].check_tag(address,size,block_size,associativity))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    
}
void index::print_LRU_block_info()
{
    for (int i=0;i<associativity; i++)
    {
    printf("block tag: 0x%x \n",LRU_block[i].get_tag());
    printf("block valid: %d \n",LRU_block[i].get_valid());
    printf("block dirty: %d \n\n",LRU_block[i].get_dirty());
    }
}

void index::write_empty_block(unsigned long long int address,int size ,int block_size)
{
    
    for(int i = 0; i < associativity; i++){
        if(!(LRU_block[i].get_valid())){
            LRU_block[i].set_tag(address, size, block_size,  associativity);
            LRU_block[i].set_valid(1);
            
        }
    }
}


void index::write_to_top_block(unsigned long long int address,int size ,int block_size, bool dirty )
{
#ifdef debug
    printf("write block \n\n");
#endif
    
    LRU_block[0].set_tag(address, size, block_size,  associativity);
    LRU_block[0].set_valid(1);
    LRU_block[0].set_dirty(dirty);
    
}

bool index::get_block_dirty(int block_num)
{
    return  LRU_block[block_num].get_dirty();
}
int index::get_block_tag(int block_num)
{
    return  LRU_block[block_num].get_tag();
}
