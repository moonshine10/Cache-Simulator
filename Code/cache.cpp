//
//  cache.cpp
//  
//
//  Created by QinYue Yin on 4/16/16.
//
//this is the defination of cache object. It contains a number of block which stores values. For this simulator , we are not going to actually store value. It uses index to track where data is.
//

#include "cache.hpp"


cache::cache(int size_in, int asso_in, int block_size_in)
{
    size = size_in;                 //define cache size
    block_size = block_size_in;     //define block size
    associativity = asso_in;        //define associativity
    kickout_count=0;                //track kickout count for report
    VC_block= new block[8];         //define cache block

    buffer_block= new block;        //define buffer
    index_array = new index[size/(block_size*associativity)];
    for(int i = 0; i < size/(block_size*associativity); i++){
        index_array[i].index_associativity(i, associativity);
        
    }
}
int cache::get_kickout_count()
{
    return kickout_count;
}
int cache::get_size()
{
    return size;
}
int cache::get_associativity()
{
    return associativity;
}
int cache::get_block_size()
{
    return block_size;
}

unsigned long long int cache::reverse_index(unsigned long long int index)
{
    if(block_size == 32 && size == 8192){
        if(associativity == 1){
            return ((index )<<5); //0001 1111 1110 0000
        }
        else if(associativity == 2){
            //1111 1110 0000
            return ((index )<<5);
        }
        else if(associativity == 4){
            //0111 1110 0000
            return ((index)<<5);
        }
        else if(associativity == 8){
            //0111 1110 0000
            return ((index)<<5);
        }
        else if(associativity == (size/block_size)){
            
            return 0;
        }
    }
    else if(block_size == 32 && size == 4096){
        if(associativity == 1){
            
            return ((index)<<5); //0000 1111 1110 0000
        }
        else if(associativity == 4){
            //0011 1110 0000
            return ((index)<<5);
        }
    }
    else if(block_size == 64 && size == 32768){
        if(associativity == 1){
            //0111 1111 1100 0000
            return ((index  )<<6);
        }
        else if(associativity == 2){
            //0011 1111 1100 0000
            return ((index )<<6);
        }
        else if(associativity == 4){
            //0001 1111 1100 0000
            return ((index  )<<6);
        }
        else if(associativity == (size/block_size)){
            return 0;
        }
    }
    else if(block_size == 64 && size == 16384){
        //0011 1111 1100 0000
        return ((index )<<6);
    }
    
}
/*****************shifting index************************/

unsigned long long int cache::get_addr_index(unsigned long long int address)
{
#ifdef debug
    printf("block_size:%ld \n",block_size);
    printf("size:%ld \n",size);
    printf("associativity:%ld \n",associativity);
#endif
    if(block_size == 32 && size == 8192){
        if(associativity == 1){
            
            return ((address & 0x1FE0)>>5); //0001 1111 1110 0000
            
        }
        else if(associativity == 2){
            //1111 1110 0000
            return ((address & 0xFE0)>>5);
        }
        else if(associativity == 4){
            //0111 1110 0000
            return ((address & 0x7E0)>>5);
        }
        else if(associativity == 8){
            //0111 1110 0000
            return ((address & 0x3E0)>>5);
        }
        else if(associativity == (size/block_size)){
            
            return 0;
        }
    }
    else if(block_size == 32 && size == 4096){
        if(associativity == 1){
            
            return ((address & 0xFE0)>>5); //0000 1111 1110 0000
        }
        else if(associativity == 4){
            //0011 1110 0000
            return ((address & 0x3E0)>>5);
        }
    }
    else if(block_size == 64 && size == 32768){
        if(associativity == 1){
            //0111 1111 1100 0000
            return ((address & 0x7FC0)>>6);
        }
        else if(associativity == 2){
            //0011 1111 1100 0000
            return ((address & 0x3FC0)>>6);
        }
        else if(associativity == 4){
            //0001 1111 1100 0000
            return ((address & 0x1FC0)>>6);
        }
        else if(associativity == (size/block_size)){
            return 0;
        }
    }
    else if(block_size == 64 && size == 16384){
        //0011 1111 1100 0000
        return ((address & 0x3FC0)>>6);
    }
    
}

bool cache::if_hit(unsigned long long int address_in)
{
    printf("get_addr_index(address_in): %lld \n\n",get_addr_index(address_in));
    return (index_array[get_addr_index(address_in)].search_tag(address_in,size,block_size));
    
}
void cache::update_block(unsigned long long int address_in)
{
    // really is update MC block 
    index_array[get_addr_index(address_in)].update_LRU( address_in,size, block_size);
}



bool cache::if_hit_VC(unsigned long long int address_in, int offset_bits)
{
    
    for(int i = 0; i < 8; i++){
        if(VC_block[i].check_VC_tag(address_in,offset_bits)){
            return 1;
        }
    }
    return 0;
}



bool cache::if_empty(unsigned long long int address_in)
{
    return (index_array[get_addr_index(address_in)].check_not_valid(size,block_size));
}
void cache::update_empty_block(unsigned long long int address_in)
{
    index_array[get_addr_index(address_in)].write_empty_block( address_in, size , block_size);
}
unsigned long long int cache::get_addr_tag(unsigned long long int address)
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
void cache::write_to_top_VC_block(unsigned long long int address,bool dirty, int block_bits)
{
#ifdef debug
    printf("write VC block \n\n");
#endif
    
    VC_block[0].set_VC_tag(address, block_bits);
    VC_block[0].set_valid(1);
    VC_block[0].set_dirty(dirty);
    
}

void cache::swap_VCnC(unsigned long long int address_in, int block_bits)
{
    //grab Last cache block
    bool LastCache_block_dirty=index_array[get_addr_index(address_in)].get_block_dirty(associativity-1);
    unsigned long long LastCache_block_address=(index_array[get_addr_index(address_in)].get_block_tag(associativity-1)|reverse_index(get_addr_index(address_in))); //return address without offset bits
    
    //move everything cache down
    index_array[get_addr_index(address_in)].move_everything_down();
    
    //LRU VC
    int rank;
    //if non of the block is empty
    
    for(int i = 0; i < 7; i++)
    {
        
        if (VC_block[i].check_VC_tag(address_in,block_bits))//if tag matches
        {
            rank=i;
            int VC_block_dirty=VC_block[i].get_dirty(); //grab that block info
            //VC_block_address=incoming address
            for (int j=(rank-1); j>=0; j--)//rearrange order
            {
                VC_block[j+1]=VC_block[j];
            }
            //LastCache on top of VC
            VC_block[0].set_VC_tag(address_in, block_bits);
            VC_block[0].set_dirty(LastCache_block_dirty);
            
            //VC_block (hit data) on top of cache
            index_array[get_addr_index(address_in)].write_to_top_block(address_in, size ,block_size,VC_block_dirty);
        }
    }
    
}

unsigned long long int  cache::cache_out(unsigned long long int address_in, int block_bits)
{
    int j;
    //L1: grab LastL1 value
    int LastL1_dirty=index_array[get_addr_index(address_in)].get_block_dirty(associativity-1);
    unsigned long long int LastL1_address=(index_array[get_addr_index(address_in)].get_block_tag(associativity-1)|reverse_index(get_addr_index(address_in))); //return address without offset bits

    //L1: move everything down
    index_array[get_addr_index(address_in)].move_everything_down();
    //L1: hit block top of L1
    index_array[get_addr_index(address_in)].write_to_top_block(address_in, size, block_size, 0);
    //VC:check if last block is dirty
    if(!VC_block[7].get_dirty()) //if not
    {
        //VC : last out (overwrite), everything down, LastL1 in
        for ( j=6; j>=0; j--)//rearrange order
        {
            VC_block[j+1]=VC_block[j];
        }
        //VC: LastL1 top
        VC_block[0].set_VC_tag(LastL1_address, block_bits);
        VC_block[0].set_dirty(LastL1_dirty);
        return 0;
    }
    else//if dirty
    {
        kickout_count++;
        //grab last VC block
        unsigned long long int kickout= ((VC_block[7].get_tag()|0));//return address witout offset bits
        //move everthing down
        for ( j=6; j>=0; j--)//rearrange order
        {
            VC_block[j+1]=VC_block[j];
        }
        //VC: LastL1 top
        VC_block[0].set_VC_tag(LastL1_address, block_bits);
        VC_block[0].set_dirty(LastL1_dirty);
                printf("kickout_count: %d \n\n");
        return kickout;
    }
                }
void cache::print_info()
{
    for(int i=0; i<size/(block_size*associativity); i++)
    {
        if(!index_array[i].check_not_valid( size , block_size))
        {
            printf("##index: 0X%x ###\n",i);
            index_array->print_LRU_block_info();
        }
    }
}


void cache::print_VC_info()
{
    
    
    for (int i=0;i<8; i++)
    {
        printf("VC_block # %d \n",i);
        printf("block tag: %lld \n",VC_block[i].get_tag());
        printf("block valid: %d \n",VC_block[i].get_valid());
        printf("block dirty: %d \n\n",VC_block[i].get_dirty());
    }
}
