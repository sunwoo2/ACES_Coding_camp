//-------------------------------------------------------------
//
//  PROGRAM: Blocked Matrix Multiplication kernel
//
//  PURPOSE: Computes an element of the proudct matrix
//
//              C = A * B
//
//           Using the well known blocked algorithm.  
//
//           To derive this algorithm, start with the naive
//           triply nested loop algorithm with a dot product 
//           for each element of C.  Decompose each loop 
//           into blocks of size blcksz.  This gives you 6
//           nested loops with three loops over blocks
//           and three loops over indices inside the blocks.
// 
//           Rearrange the loops to put the 3 loops over blocks 
//           at the outermost loops of the loop nest.  You'll
//           see that the three "inner" loops are just the 
//           regular matrix product between blocks.
//
//           The algorithms is simple.  Keeping all the indices
//           straight is not.  We will use the following 
//           conventions:
//
//             i,j,k            ... indices of full, global matrices 
//             Iblk, Jblk, Kblk ... indices of matrix blocks
//             iloc, jloc, kloc ... indices inside blocks
//                 
//  HISTORY: Written by Tim Mattson, November 2013 
//           Updated by Simon McIntosh-Smith, August 2014 
//
//  LICENSE: This work is licensed under the Creative Commons
//           Attribution 4.0 International License.
//           To view a copy of this license, visit
//           http://creativecommons.org/licenses/by/4.0/
//           or send a letter to:
//              Creative Commons,
//              444 Castro Street, Suite 900,
//              Mountain View, California, 94041, USA.
//
//-------------------------------------------------------------

// It turns out that the compiler generates much better code if
// we "hardwire" this block size.  16 works well for an NVIDIA 
// GPU, 32 works well for a CPU
#define blksz 32

__kernel void mmul4(
                const unsigned int             N,
                __global const float* restrict A,
                __global const float* restrict B,
                __global       float* restrict C,
                __local        float* restrict Awrk,
                __local        float* restrict Bwrk)
{
    int kloc, Kblk;	// priv	
    float Ctmp=0.0f;	// priv

    //  This work-item will compute element C(i,j)
    const int j = get_global_id(0);	// (0) : 현재 work-item의 위치가 왼쪽에서부터 몇번째에 위치해 있는지 반환(열)(1차원)
    const int i = get_global_id(1);	// (1) : 현재 work-item의 위치가 위쪽에서부터 몇번째에 위치해 있는지 반환(행)(2차원)

    // Element C(i,j) is in block C(Iblk,Jblk)
    const int Jblk = get_group_id(0);	// 그룹위치
    const int Iblk = get_group_id(1);

    // C(i,j) is element C(iloc, jloc) of block C(Iblk, Jblk)
    const int jloc = get_local_id(0);	// work-group내 위치
    const int iloc = get_local_id(1);	// 결론 -> 어떤 get_ID 든지 간에 가리키는 절대위치는 동일!!!

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
    for (Kblk = 0;  Kblk<Num_BLK;  Kblk++)	// C(i,j)를 위해 A한블럭*B한블럭 Num_BLK개 더해주는 loop 
    {
       // Load A(Iblk,Kblk) and B(Kblk,Jblk) into local memory.
       // Each work-item loads a single element of the two blocks
       // which are shared with the entire work-group.

       Awrk[iloc*blksz+jloc] = A[Abase+iloc*N+jloc];	// A전체에서 한블럭을 그냥 한블럭으로 맵핑(local 행과 열이 바뀌긴 했지만 어차피 전체 다 병렬실행 하면 다 맵핑되는거니깐)
       Bwrk[iloc*blksz+jloc] = B[Bbase+iloc*N+jloc];

       barrier(CLK_LOCAL_MEM_FENCE);

       // Compute dot products over local blocks to find
       // the contribution to C(i,j) from this block
       #pragma unroll	// 차이 없는데..?
       for (kloc=0; kloc<blksz; kloc++)		// block 내부 연산  루프
          Ctmp += Awrk[iloc*blksz+kloc] * Bwrk[kloc*blksz+jloc];
       

       barrier(CLK_LOCAL_MEM_FENCE);
       Abase += Ainc;
       Bbase += Binc;
    }
 
    // update global C matrix 
    C[i*N+j] = Ctmp;	
}
