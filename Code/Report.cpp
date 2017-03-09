void output(default_cache* dcache, default_cache* icache, default_cache* l2cache, char *argv[], FILE * report){
	numRefs = instRefs + writeRefs + readRefs;
	numCycles = instCycles + writeCycles + readCycles;
fprintf(report, "-------------------------------------------------------------------------\n %s Simulation Results\n-------------------------------------------------------------------------\n\n\t", argv[1]);
	fprintf(report, "Memory system:\n\t\t");
		fprintf(report, "Dcache size = %d  : ways = %d : block size = 32\n\t\t", d_size, d_associative);
		fprintf(report, "Icache size = %d : ways = %d : block size = 32\n\t\t", i_size, i_associative);
		fprintf(report, "L2-cache size = %d : ways = %d : block size = 64\n\t\t", l2_size, l2_associative);
		fprintf(report, "Memory ready time = 50 : chunksize = 16 : chunktime = 20\n\n\t"); // check if variables are correct
	
	fprintf(report, "Execute time = %llu; Total refs = %llu\n\t", numCycles, numRefs);
	fprintf(report, "Inst refs = %llu; Data refs = %llu\n\n\t", instRefs, writeRefs + readRefs);
	
	//(float) to [float]
	fprintf(report, "Number of reference types: [Percentage]\n\t\t");
		fprintf(report, "Reads = %llu\t[%f]\n\t\t", readRefs, (float)100*readRefs/numRefs);
		fprintf(report, "Writes = %llu\t[%f]\n\t\t", writeRefs, (float)100*writeRefs/numRefs);
		fprintf(report, "Inst. = %llu\t[%f]\n\t\t", instRefs, (float)100*instRefs/numRefs);
		fprintf(report, "Total = %llu\n\n\t", numRefs);
	
	fprintf(report, "Total cycles for activities: [Percentage]\n\t\t");
		fprintf(report, "Reads = %llu\t[%f]\n\t\t", readCycles, (float)100*readCycles/numCycles);
		fprintf(report, "Writes = %llu\t[%f]\n\t\t", writeCycles, (float)100*writeCycles/numCycles);
		fprintf(report, "Inst. = %llu\t[%f]\n\t\t", instCycles, (float)100*instCycles/numCycles);
		fprintf(report, "Total = %llu\n\n\t", numCycles);
	
	//don't need this - need to replace with CPI, Ideal Exec Time
	fprintf(report, "Average cycles per activity:\n\t\t");
	fprintf(report, "Read = %f; Write = %f; Inst = %f\n\t", (float)readCycles/readRefs, (float)writeCycles/writeRefs, (float)numCycles/instRefs);
	fprintf(report, "Ideal: Exec. Time = %llu; CPI = %f\n\t", readRefs+writeRefs+2*instRefs, (float)(readRefs+writeRefs+2*instRefs)/instRefs); // correct variables needed
	fprintf(report, "Ideal misaligned: Exec. Time = %llu; CPI = %f\n\n\t", misalignedTime+readRefs+writeRefs+2*instRefs, (float)(misalignedTime+readRefs+writeRefs+2*instRefs)/instRefs); // correct variables needed
	
	fprintf(report, "Memory Level: L1i\n\t\t");
		fprintf(report, "Hit Count = %llu Miss Count = %llu\n\t\t", hitCountI, missCountI);
		fprintf(report, "Total Requests = %llu\n\t\t", hitCountI + missCountI);
		fprintf(report, "Hit Rate = %f%s  Miss Rate = %f%s \n\t\t", (float)100*hitCountI/(hitCountI + missCountI), "%", (float)100*missCountI/(hitCountI + missCountI), "%");
		fprintf(report, "Kickouts = %llu; Dirty Kickouts = %llu; Transfers = %llu\n\n\t", kickoutCountI, dirtyKickoutCountI, missCountI);
		//Put "VC Hit count here"

	fprintf(report, "Memory Level: L1d\n\t\t");
		fprintf(report, "Hit Count = %llu Miss Count = %llu\n\t\t", hitCountD, missCountD);
		fprintf(report, "Total Requests = %llu\n\t\t", hitCountD + missCountD);
		fprintf(report, "Hit Rate = %f%s Miss Rate = %f%s\n\t\t", (float)100*hitCountD/(hitCountD + missCountD), "%", (float)100*missCountD/(hitCountD + missCountD), "%");
		fprintf(report, "Kickouts = %llu; Dirty Kickouts = %llu; Transfers = %llu\n\n\t", kickoutCountD, dirtyKickoutCountD, missCountD);
		//Put "VC Hit count here"

	fprintf(report, "Memory Level: L2\n\t\t");
		fprintf(report, "Hit Count = %llu Miss Count = %llu\n\t\t", hitCountL2, missCountL2);
		fprintf(report, "Total Requests = %llu\n\t\t", hitCountL2 + missCountL2);
		fprintf(report, "Hit Rate = %f%s Miss Rate = %f%s\n\t\t", (float)100*hitCountL2/(hitCountL2 + missCountL2), "%", (float)100*missCountL2/(hitCountL2 + missCountL2), "%");
		fprintf(report, "Kickouts = %llu; Dirty Kickouts = %llu; Transfers = %llu\n\n\t", kickoutCountL2, dirtyKickoutCountL2, missCountL2);
		//Put "VC Hit count here"

	int counter1 = 1, counter2 = 1, counter3 = 1;
	while(i_associative>1){
		i_associative /= 2;
		counter1++;
	}
	while(d_associative>1){
		d_associative /= 2;
		counter2++;
	}
	while(l2_associative>1){
		l2_associative /= 2;
		counter3++;
	}
	fprintf(report, "L1 cache cost (Icache $%d) + (Dcache $%d) = %d\n\t", i_size*counter1*100/4096, d_size*counter2*100/4096, i_size*counter1*100/4096 + d_size*counter2*100/4096);
	fprintf(report, "L2 cache cost = $%d; Memory cost = $75\n\t", 50*counter3);
	fprintf(report, "Total Cost = $%d\n\t", i_size*counter1*100/4096 + d_size*counter2*100/4096 + 50*counter3 + 75);
