This is a Cache Simulator written in C++. It process the instruction sets in the format of trace file that includes instruction, adress and the size of data ( example see "tr5" "tr6"). The simulator will manage the data in between L1 cache,L2 cache and memory to let the system have the best performance by re-arranging the order data sets based on the incoming instruction. 


Cache Simulator will be able to process different trace file ( a set of instruction) and generate the report that represents the performance of the system. It will count each cache's hit count, miss count, and the total processing time. In addition, inside L1 and L2 cahce there is an victim cache.

In the code, the "Simulator.cpp" is the simulation program. It processes the trace file and calls cache simulation function in "cache.cpp". "cahce.cpp" define the cache and has function built in to process the instruction. It consists of different cache blocks, which is defined in "block.cpp". Block can be defined depending on the cache structure. It is the lowest level of data storage in this simulator. The address of the memory is represented in the block file. 



