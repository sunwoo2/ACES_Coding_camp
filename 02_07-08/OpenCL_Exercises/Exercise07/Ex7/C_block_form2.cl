
#define blksz 16

__kernel void mmul4(
                const unsigned int             N,
                __global const float* restrict A,
                __global const float* restrict B,
                __global       float* restrict C,
                __local        float* restrict Awrk,
                __local        float* restrict Bwrk)
{
    int kloc, Kblk;	
    float Ctmp=0.0f;	

    //  This work-item will compute element C(i,j)
    const int j = get_global_id(0);	// 1D	
    const int i = get_global_id(1);	// 2D

    // Element C(i,j) is in block C(Iblk,Jblk)
    const int Jblk = get_group_id(0);	
    const int Iblk = get_group_id(1);

    // C(i,j) is element C(iloc, jloc) of block C(Iblk, Jblk)
    const int jloc = get_local_id(0);	
    const int iloc = get_local_id(1);	

    // The number of blocks are the same in each dimension
    const int Num_BLK = N/blksz;

    // Setup the upper-left-corner (base address) for the A and
    // B blocks plus the increments to advance base addresses as
    // we loop over blocks
          int Abase = Iblk*N*blksz;  
    const int Ainc  = blksz;

          int Bbase = Jblk*blksz;
    const int Binc  = blksz*N;


    // C(Iblk,Jblk) = (sum over Kblk) A(Iblk,Kblk)*B(Kblk,Jblk)
    for (Kblk = 0;  Kblk<Num_BLK;  Kblk++)
    {
       // Load A(Iblk,Kblk) and B(Kblk,Jblk) into local memory.
       // Each work-item loads a single element of the two blocks
       // which are shared with the entire work-group.

       Awrk[iloc*blksz+jloc] = A[Abase+iloc*N+jloc];	
       Bwrk[iloc*blksz+jloc] = B[Bbase+iloc*N+jloc];

       barrier(CLK_LOCAL_MEM_FENCE);

       // Compute dot products over local blocks to find
       // the contribution to C(i,j) from this block
//       #pragma unroll	// OpenCL 2.0 부터 지원됨!
//       for (kloc=0; kloc<blksz; kloc++)	
       kloc=0;
          Ctmp += Awrk[iloc*blksz+kloc] * Bwrk[kloc*blksz+jloc];
	  kloc++;
          Ctmp += Awrk[iloc*blksz+kloc] * Bwrk[kloc*blksz+jloc];
	  kloc++;
          Ctmp += Awrk[iloc*blksz+kloc] * Bwrk[kloc*blksz+jloc];
	  kloc++;
          Ctmp += Awrk[iloc*blksz+kloc] * Bwrk[kloc*blksz+jloc];
	  kloc++;
          Ctmp += Awrk[iloc*blksz+kloc] * Bwrk[kloc*blksz+jloc];
	  kloc++;
          Ctmp += Awrk[iloc*blksz+kloc] * Bwrk[kloc*blksz+jloc];
	  kloc++;
          Ctmp += Awrk[iloc*blksz+kloc] * Bwrk[kloc*blksz+jloc];
	  kloc++;
          Ctmp += Awrk[iloc*blksz+kloc] * Bwrk[kloc*blksz+jloc];
	  kloc++;
          Ctmp += Awrk[iloc*blksz+kloc] * Bwrk[kloc*blksz+jloc];
	  kloc++;
          Ctmp += Awrk[iloc*blksz+kloc] * Bwrk[kloc*blksz+jloc];
	  kloc++;
          Ctmp += Awrk[iloc*blksz+kloc] * Bwrk[kloc*blksz+jloc];
	  kloc++;
          Ctmp += Awrk[iloc*blksz+kloc] * Bwrk[kloc*blksz+jloc];
	  kloc++;
          Ctmp += Awrk[iloc*blksz+kloc] * Bwrk[kloc*blksz+jloc];
	  kloc++;
          Ctmp += Awrk[iloc*blksz+kloc] * Bwrk[kloc*blksz+jloc];
	  kloc++;
          Ctmp += Awrk[iloc*blksz+kloc] * Bwrk[kloc*blksz+jloc];
	  kloc++;
          Ctmp += Awrk[iloc*blksz+kloc] * Bwrk[kloc*blksz+jloc];

       barrier(CLK_LOCAL_MEM_FENCE);
       Abase += Ainc;
       Bbase += Binc;
    }
 
    // update global C matrix 
    C[i*N+j] = Ctmp;	
}
