
#include "cache.hpp"

//#include "back_func.hpp"
//#include "default.hpp"
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>

#define debug

using namespace std;


int i_size_default = 8192;
int d_size_default = 8192;
int l2_size_default = 32768;
int i_associative_default = 1;
int d_associative_default = 1;
int l2_associative_default = 1;

// define cache sizes
int i_size = i_size_default;
int d_size = d_size_default;
int l2_size = l2_size_default;
int i_associative = i_associative_default;
int d_associative = d_associative_default;
int l2_associative = l2_associative_default;

//default block size
int l1_block_size_default=32;
int l2_block_size_default=64;

//define block sizes
int l1_block_size=l1_block_size_default;
int l2_block_size=l2_block_size_default;


// hit time & miss time  & transfer time default
int l1_hit_time_default=1;
int l1_miss_time_default=1;
int l2_hit_time_default=8;
int l2_miss_time_default=10;
int l2_transfer_time_default=10;
int l2_bus_width_default=16;

//hit time & miss time  & transfer time
int l1_hit_time=l1_hit_time_default;
int l1_miss_time=l1_miss_time_default;
int l2_hit_time=l2_hit_time_default;
int l2_miss_time=l2_miss_time_default;
int l2_transfer_time=l2_transfer_time_default;
int l2_bus_width=l2_bus_width_default;



//define default main memory para
int mem_sendaddr_default=10;
int mem_ready_default=50;
int mem_chucktime_default=15;
int mem_chucksize_default=8;

//define main memory para
int mem_sendaddr=mem_sendaddr_default;
int mem_ready=mem_ready_default;
int mem_chucktime=mem_chucktime_default;
int mem_chucksize=mem_chucktime_default;


//vitim cache
int victim_size;


unsigned long long int ref_Read=0;
unsigned long long int ref_write=0;
unsigned long long int ref_inst=0;
unsigned long long int ref_total=0;

unsigned long long int cycle_Read=0;
unsigned long long int cycle_write=0;
unsigned long long int cycle_inst=0;
unsigned long long int cycle_total=0;

char test_op= 'I';
unsigned long long int test_address=0x7f81ce2240Fc;
int test_bytesize=4;


cache *LIcache;
cache *LDcache;
cache *L2cache;
cache *LI_v_cache;
cache *LD_v_cache;
cache *L2_v_cache;



int main ()
{
    //setup file
    int time =0;
    char input[30];
    FILE *config;
    config=fopen("default_config.txt", "r");
    fscanf(config,"%d %d %d %d %d %d",&i_size,&d_size,&l2_size,&i_associative, &d_associative,&l2_associative);
    printf ("Conig INFO : Icache Size: %d Dcache size: %d L2cache size:  %d Icache associativity: %d Dcache associativity: %d L2cache associativity:%d \n",i_size,d_size,l2_size,i_associative, d_associative,l2_associative);
    
    //define variables
    char op=test_op;
    unsigned long long int address=test_address;
    int bytesize=test_bytesize;
    LIcache=new cache(i_size,  i_associative,  l1_block_size);
    LDcache=new cache( d_size,  d_associative,  l1_block_size);
    L2cache=new cache( l2_size,  l2_associative,  l2_block_size);
    int test;
    bool flag;
    unsigned long long int kickout_address;
    int i_ref=0, r_ref=0, w_ref=0,LI_hit=0 , LD_hit=0,L2_hit=0, L2VC_hit=0, LIVC_hit=0, LDVC_hit=0;
    int iHi ,iHd ,iH2 ,rHi ,rHd ,rH2 ,wHi ,wHd,wH2;
    unsigned long long int cache_index = LIcache->get_addr_index(test_address);
    
    printf(" programming is starting ... \n\n");

    //ready trance file
    FILE *yoyo;
    yoyo=fopen("tr5", "r");
    int line_count=0;
    
    
    
    //SIMULATION
    while (fscanf(yoyo,"%c %llx %d\n",&op,&address,&bytesize) == 3) {
        line_count++;
        printf("***********%d************\n",line_count);
        printf("%c %llx %d\n\n",op,address,bytesize);
        flag=0;
        
        if (op=='I')
        {
            i_ref++;
            
            if(!flag&&LIcache->if_empty(address))//if LI empty
            {
                
                printf(" empty \n\n");
                LIcache->update_empty_block(address);
                flag=1;
                //write on L2
                L2cache->update_empty_block(address);
                printf("Add L1i miss time (+ 1) \n");
                time++;
                printf("Add L2 miss time (+ 10) \n");
                time=time+10;
                printf("Add memory to L2 transfer time (+ 180) \n");
                time=time+180;
                printf("Add L2 to L1 transfer time (+ 20)\n");
                time=time+20;
                printf("Add L2 hit replay time (+ 8)\n");
                time=time+8;
                printf("Add L1i hit replay time (+ 1) \n");
                time++;
                printf("Add +1 for instruction execute time \n");
                time++;
                
            }
            else if (!flag&&LIcache->if_hit(address)) //if hit LI
            {
                
                printf(" HIT LI \n\n");
                LIcache->update_block(address);//update LRU
                
                ref_inst++;
                ref_total++;
                flag=1;
                printf("Add L1i hit time (+ 1) \n");
                time++;
                printf("Add +1 for instruction execute time \n");
                time++;
                
            }
            else if(!flag&&LIcache->if_hit_VC(address,5))  //if hit L1 victim cache
            {
                iHi++;
                printf(" HIT LIVC \n\n");
                
                //L1 VC swap
                LIcache->swap_VCnC( address,5);
                flag=1;
                printf("Add L1i miss time (+ 1) \n");
                time++;
                printf("Add L1iVC hit time (+ 1) \n");
                time++;
                printf("Add +1 for instruction execute time \n");
                time++;
                
                
            }
            else if(!flag&&L2cache->if_hit(address)) //if hit L2 cache
            {
                iH2++;
                printf(" HIT L2 \n\n");
                
                kickout_address=LIcache->cache_out(address,5);
                if(kickout_address==0)//if not have dirty kickout
                {
                    L2cache->update_block(address);//L2: block top of L2
                }
                else //if has dirty kickout
                {
                    kickout_address=L2cache->cache_out(kickout_address,6);
                    L2cache->update_block(address);//L2: block top of L2
                }
                flag=1;
                printf("Add L1i miss time (+ 1) \n");
                time++;
                printf("Add L2 hit time (+ 8) \n");
                time=time+8;
                printf("Add L2 to L1 transfer time (+ 20)\n");
                time=time+20;
                printf("Add L1i hit replay time (+ 1) \n");
                time++;
                printf("Add L1i hit time (+ 1)\n");
                time++;
                printf("Add +1 for instruction execute time \n");
                time++;
            }
            else if(!flag&&L2cache->if_hit_VC(address,6)) //L2VC
            {
                
                printf(" HIT L2VC \n\n");
                //L2 VC swap
                LIcache->swap_VCnC( address,6);
                
                kickout_address=LIcache->cache_out(address,5);
                if(kickout_address==0) //if not have dirty kickout
                {
                    kickout_address=0; //do nothing
                    
                }
                else //if has dirty kickout
                {
                    
                    kickout_address=L2cache->cache_out(kickout_address,6);
                    L2cache->update_block(address);//L2: block top of L2
                    
                }
                flag=1;
                printf("Add L1i miss time (+ 1) \n");
                time++;
                printf("Add L2 hit time (+ 8) \n");
                time=time+8;
                printf("Add L2 to L1 transfer time (+ 20)\n");
                time=time+20;
                printf("Add L1i hit replay time (+ 1) \n");
                time++;
                printf("Add L1i hit time (+ 1)\n");
                time++;
                printf("Add +1 for instruction execute time \n");
                time++;
            }
            else  //main memory
            {
                
                printf(" main memory  \n\n");
                
                //place block to top of L1
                kickout_address=LIcache->cache_out(address,5);
                if(kickout_address==0)//if not have dirty kickout
                {
                    kickout_address=0; //do nothing
                }
                else //if has dirty kickout
                {
                    kickout_address=L2cache->cache_out(kickout_address,6);
                    
                }
                
                //place block to top of L2
                kickout_address=L2cache->cache_out(address,6);
                
                
                flag=1;
                printf("Add L1i miss time (+ 1) \n");
                time++;
                printf("Add L2 miss time (+ 10) \n");
                time=time+10;
                printf("Add memory to L2 transfer time (+ 180) \n");
                time=time+180;
                printf("Add L2 to L1 transfer time (+ 20)\n");
                time=time+20;
                printf("Add L2 hit replay time (+ 8)\n");
                time=time+8;
                printf("Add L1i hit replay time (+ 1) \n");
                time++;
                printf("Add +1 for instruction execute time \n");
                time++;
                
            }
            
        }
        
        else if(op == 'R')
        {
            r_ref++;
            rHi++;
            if(!flag&&LDcache->if_empty(address))//if LI empty
            {
                printf(" empty \n\n");
                LDcache->update_empty_block(address);
                flag=1;
                //write on L2
                LDcache->update_empty_block(address);
                printf("Add LDcache miss time (+ 1) \n");
                time++;
                printf("Add L2 miss time (+ 10) \n");
                time=time+10;
                printf("Add memory to L2 transfer time (+ 180) \n");
                time=time+180;
                printf("Add L2 to L1 transfer time (+ 20)\n");
                time=time+20;
                printf("Add L2 hit replay time (+ 8)\n");
                time=time+8;
                printf("Add L1i hit replay time (+ 1) \n");
                time++;
                printf("Add +1 for instruction execute time \n");
                time++;
                
            }
            else if (!flag&&LDcache->if_hit(address)) //if hit LI
            {
                
                printf(" HIT LDcache \n\n");
                LDcache->update_block(address);//update LRU
                
                ref_inst++;
                ref_total++;
                flag=1;
                printf("Add LDcache hit time (+ 1) \n");
                time++;
                printf("Add +1 for instruction execute time \n");
                time++;
                
                
            }
            else if(!flag&&LDcache->if_hit_VC(address,5))  //L1 victim cache
            {
                rHd++;
                printf(" HIT LDcacheVC \n\n");
                
                //L1 VC swap
                LDcache->swap_VCnC( address,5);
                flag=1;
                printf("Add LDcache miss time (+ 1) \n");
                time++;
                printf("Add LDcacheiVC hit time (+ 1) \n");
                time++;
                printf("Add +1 for instruction execute time \n");
                time++;
                
                
            }
            else if(!flag&&L2cache->if_hit(address)) //L2 cache
            {
                rH2++;
                printf(" HIT L2 \n\n");
                
                kickout_address=LDcache->cache_out(address,5);
                if(kickout_address==0)//if not have dirty kickout
                {
                    L2cache->update_block(address);//L2: block top of L2
                }
                else //if has dirty kickout
                {
                    kickout_address=L2cache->cache_out(kickout_address,6);
                    L2cache->update_block(address);//L2: block top of L2
                }
                flag=1;
                printf("Add LDcache miss time (+ 1) \n");
                time++;
                printf("Add L2 hit time (+ 8) \n");
                time=time+8;
                printf("Add L2 to L1 transfer time (+ 20)\n");
                time=time+20;
                printf("Add LDcache hit replay time (+ 1) \n");
                time++;
                printf("Add LDcache hit time (+ 1)\n");
                time++;
                printf("Add +1 for instruction execute time \n");
                time++;
            }
            else if(!flag&&L2cache->if_hit_VC(address,6)) //L2VC
            {
                printf(" HIT L2VC \n\n");
                //L2 VC swap
                LIcache->swap_VCnC( address,6);
                
                kickout_address=LIcache->cache_out(address,5);
                if(kickout_address==0)//if not have dirty kickout
                {
                    kickout_address=0; //do nothing
                    
                }
                else //if has dirty kickout
                {
                    
                    kickout_address=L2cache->cache_out(kickout_address,6);
                    L2cache->update_block(address);//L2: block top of L2
                    
                }
                flag=1;
                printf("Add LDcache miss time (+ 1) \n");
                time++;
                printf("Add L2 hit time (+ 8) \n");
                time=time+8;
                printf("Add L2 to L1 transfer time (+ 20)\n");
                time=time+20;
                printf("Add LDcache hit replay time (+ 1) \n");
                time++;
                printf("Add LDcache hit time (+ 1)\n");
                time++;
                printf("Add +1 for instruction execute time \n");
                time++;
            }
            else  //main memory
            {
                
                printf(" main memory  \n\n");
                
                //place block to top of L1
                kickout_address=LIcache->cache_out(address,5);
                if(kickout_address==0)//if not have dirty kickout
                {
                    kickout_address=0; //do nothing
                }
                else //if has dirty kickout
                {
                    kickout_address=L2cache->cache_out(kickout_address,6);
                    
                }
                
                //place block to top of L2
                kickout_address=L2cache->cache_out(address,6);
                
                
                flag=1;
                printf("Add LDcache miss time (+ 1) \n");
                time++;
                printf("Add L2 miss time (+ 10) \n");
                time=time+10;
                printf("Add memory to L2 transfer time (+ 180) \n");
                time=time+180;
                printf("Add L2 to L1 transfer time (+ 20)\n");
                time=time+20;
                printf("Add L2 hit replay time (+ 8)\n");
                time=time+8;
                printf("Add L1i hit replay time (+ 1) \n");
                time++;
                printf("Add +1 for instruction execute time \n");
                time++;
                
            }
            
        }
        
        
         else if(op == 'W')
         {
             wHi++;
             w_ref++;
            
             if(!flag&&LDcache->if_empty(address))//if LI empty
             {
                 printf(" empty \n\n");
                 LDcache->update_empty_block(address);
                 flag=1;
                 //write on L2
                 LDcache->update_empty_block(address);
                 printf("Add LDcache miss time (+ 1) \n");
                 time++;
                 printf("Add L2 miss time (+ 10) \n");
                 time=time+10;
                 printf("Add memory to L2 transfer time (+ 180) \n");
                 time=time+180;
                 printf("Add L2 to L1 transfer time (+ 20)\n");
                 time=time+20;
                 printf("Add L2 hit replay time (+ 8)\n");
                 time=time+8;
                 printf("Add L1i hit replay time (+ 1) \n");
                 time++;
                 printf("Add +1 for instruction execute time \n");
                 time++;
                 
             }
             else if (!flag&&LDcache->if_hit(address)) //if hit LI
             {
                 
                 printf(" HIT LDcache \n\n");
                 LDcache->update_block(address);//update LRU
                 
                 ref_inst++;
                 ref_total++;
                 flag=1;
                 printf("Add LDcache hit time (+ 1) \n");
                 time++;
                 printf("Add +1 for instruction execute time \n");
                 time++;
                 
                 
             }
             else if(!flag&&LDcache->if_hit_VC(address,5))  //L1 victim cache
             {
                 
                 printf(" HIT LDcacheVC \n\n");
                 
                 //L1 VC swap
                 LDcache->swap_VCnC( address,5);
                 flag=1;
                 printf("Add LDcache miss time (+ 1) \n");
                 time++;
                 printf("Add LDcacheiVC hit time (+ 1) \n");
                 time++;
                 printf("Add +1 for instruction execute time \n");
                 time++;
                 
                 
             }
             else if(!flag&&L2cache->if_hit(address)) //L2 cache
             {
                 
                 printf(" HIT L2 \n\n");
                 
                 kickout_address=LDcache->cache_out(address,5);
                 if(kickout_address==0)//if not have dirty kickout
                 {
                     L2cache->update_block(address);//L2: block top of L2
                 }
                 else //if has dirty kickout
                 {
                     kickout_address=L2cache->cache_out(kickout_address,6);
                     L2cache->update_block(address);//L2: block top of L2
                 }
                 flag=1;
                 printf("Add LDcache miss time (+ 1) \n");
                 time++;
                 printf("Add L2 hit time (+ 8) \n");
                 time=time+8;
                 printf("Add L2 to L1 transfer time (+ 20)\n");
                 time=time+20;
                 printf("Add LDcache hit replay time (+ 1) \n");
                 time++;
                 printf("Add LDcache hit time (+ 1)\n");
                 time++;
                 printf("Add +1 for instruction execute time \n");
                 time++;
             }
             else if(!flag&&L2cache->if_hit_VC(address,6)) //L2VC
             {
                 printf(" HIT L2VC \n\n");
                 //L2 VC swap
                 LIcache->swap_VCnC( address,6);
                 
                 kickout_address=LIcache->cache_out(address,5);
                 if(kickout_address==0)//if not have dirty kickout
                 {
                     kickout_address=0; //do nothing
                     
                 }
                 else //if has dirty kickout
                 {
                     
                     kickout_address=L2cache->cache_out(kickout_address,6);
                     L2cache->update_block(address);//L2: block top of L2
                     
                     
                 }
                 flag=1;
                 printf("Add LDcache miss time (+ 1) \n");
                 time++;
                 printf("Add L2 hit time (+ 8) \n");
                 time=time+8;
                 printf("Add L2 to L1 transfer time (+ 20)\n");
                 time=time+20;
                 printf("Add LDcache hit replay time (+ 1) \n");
                 time++;
                 printf("Add LDcache hit time (+ 1)\n");
                 time++;
                 printf("Add +1 for instruction execute time \n");
                 time++;
             }
             else  //main memory
             {
                 
                 printf(" main memory  \n\n");
                 
                 //place block to top of L1
                 kickout_address=LIcache->cache_out(address,5);
                 if(kickout_address==0)//if not have dirty kickout
                 {
                     kickout_address=0; //do nothing
                 }
                 else //if has dirty kickout
                 {
                     kickout_address=L2cache->cache_out(kickout_address,6);
                     
                 }
                 
                 //place block to top of L2
                 kickout_address=L2cache->cache_out(address,6);
                 
                 
                 flag=1;
                 printf("Add LDcache miss time (+ 1) \n");
                 time++;
                 printf("Add L2 miss time (+ 10) \n");
                 time=time+10;
                 printf("Add memory to L2 transfer time (+ 180) \n");
                 time=time+180;
                 printf("Add L2 to L1 transfer time (+ 20)\n");
                 time=time+20;
                 printf("Add L2 hit replay time (+ 8)\n");
                 time=time+8;
                 printf("Add L1i hit replay time (+ 1) \n");
                 time++;
                 printf("Add +1 for instruction execute time \n");
                 time++;
                 
             }
         
         }

        printf("-------simunation time: %d------- \n",time);
        //SIMULATION DONE
        
    }
    
    //REPORT GENERATION
    float CPI= i_ref*10/((r_ref));
    float iCPI= w_ref*10/((r_ref));
    float eCPI= r_ref*10/((i_ref));
    
    printf("cacheI info:\n ");
    LIcache->print_info();
    printf("LIVC info: \n ");
    LIcache->print_VC_info();
    printf("------------------------------------\n\n");
    printf(" \n cache2 info: \n");
    L2cache->print_info();
    printf("L2VC info: \n");
    L2cache->print_VC_info();
    
    printf("**************************************");
    printf ("Conig INFO : Icache Size: %d Dcache size: %d L2cache size:  %d Icache associativity: %d Dcache associativity: %d L2cache associativity:%d \n",i_size,d_size,l2_size,i_associative, d_associative,l2_associative);
    printf(" exe time : %d\n", time);
    printf(" total reference : %d\n\n", line_count);
    
     printf(" Inst ref:%d \n", i_ref);
     printf(" Read ref:%d \n", r_ref);
    printf(" Write ref:%d \n", w_ref);
    
    printf(" \n Number of reference types: \n", line_count);
    float clc1=(i_ref/line_count)*100;
    float clc2=(r_ref/line_count)*100;
    float clc3=(w_ref/line_count)*100;
    printf(" Instruction:%d  [%f percent]\n", i_ref,clc1);
    printf(" Read :%d [%f percent] \n", r_ref,clc2);
    printf(" Write :%d  [%f percent]\n", w_ref ,clc3);
    

    
    printf(" CPI%f  \n", CPI);
    printf(" Ideal: Exec. Time :%d  \n", iCPI);
    printf(" Ideal mis-aligned:  :%d  \n", eCPI );
    
    
     printf("Memory Level: L1i");
    printf("Hit Count = %d \n",iHi);
    printf("Miss Count = %d \n",(line_count-iHi));
    printf("Requests = %d \n",line_count);
    printf(" Hit Rate = %d  percent\n",float((iHi-line_count)/1000*line_count));
     printf(" Miss Rate = %d  percent\n",float(1-(iHi-line_count)/10*line_count));
     
    

    
    
}
