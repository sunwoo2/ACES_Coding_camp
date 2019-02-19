//------------------------------------------------------------------------------
//
//  PROGRAM: Matrix Multiplication driver
//
//  PURPOSE: This is a driver program to test various ways of computing
//           the product:
//
//                C  = A * B
//
//           A and B are set to constant matrices so we
//           can make a quick test of the multiplication.
//
//  USAGE:   The matrices are constant matrices, square and the order is
//           set as a constant, ORDER (see mult.h).
//
//  HISTORY: Written by Tim Mattson, August 2010 
//           Modified by Simon McIntosh-Smith, September 2011
//           Modified by Tom Deakin and Simon McIntosh-Smith, October 2012
//           Ported to C by Tom Deakin, July 2013
//           Modified to assume square matrices by Simon McIntosh-Smith, Sep 2014
//
//------------------------------------------------------------------------------

#include "matmul.h"
#include "matrix_lib.h"
#include "err_code.h"
#include "device_picker.h"

char* getKernelSource(char* filename);

char * kernelsource = "__kernel void mmul(                              \n" \
"   const int N,                                                        \n" \
"   __global float* A,                                                  \n" \
"   __global float* B,                                                  \n" \
"   __global float* C)                                                  \n" \
"{                                                                      \n" \
"	int k;                                                          \n" \
"       int i = get_global_id(0);                                       \n" \
"       int j = get_global_id(1);                                       \n" \
"       float tmp = 0.0f;                                               \n" \
"       for(k=0; k<N; k++)                                              \n" \
"          tmp += A[i*N+k]*B[k*N+j];                                    \n" \
"          C[i*N+j] = tmp;                                             \n" \
"}                                                                      \n" \
"\n";

char * kernelsource1 = "__kernel void mmul1(                            \n" \
"   const int N,                                                        \n" \
"   __global float* A,                                                  \n" \
"   __global float* B,                                                  \n" \
"   __global float* C)                                                  \n" \
"{                                                                      \n" \
"	int j, k;                                                       \n" \
"       int i = get_global_id(0);                                       \n" \
"       float tmp;		                                        \n" \
"       for(j=0; j<N; j++){                                             \n" \
"      	   tmp = 0.0f;                       	                        \n" \
"             for(k=0; k<N; k++)                                        \n" \
"      	         tmp += A[i*N+k]*B[k*N+j];                              \n" \
"          C[i*N+j] = tmp;                                              \n" \
"       }                                        		        \n" \
"}                                                                      \n" \
"\n";

char * kernelsource2 = "__kernel void mmul2(                            \n" \
"   const int N,                                                        \n" \
"   __global float* A,                                                  \n" \
"   __global float* B,                                                  \n" \
"   __global float* C)                                                  \n" \
"{                                                                      \n" \
"	int j, k;                                                       \n" \
"       int i = get_global_id(0);                                       \n" \
"       float tmp;		                                        \n" \
"       float Awrk[1024];	                                        \n" \
"       			                                        \n" \
"       for(k=0; k<N; k++)	                                        \n" \
"       	Awrk[k] = A[i*N+k];                                     \n" \
"       			                                        \n" \
"       for(j=0; j<N; j++){                                             \n" \
"      	   tmp = 0.0f;                       	                        \n" \
"             for(k=0; k<N; k++)                                        \n" \
"      	         tmp += Awrk[k]*B[k*N+j];                               \n" \
"          C[i*N+j] = tmp;                                              \n" \
"       }                                        		        \n" \
"}                                                                      \n" \
"\n";

char * kernelsource3 = "__kernel void mmul3(                            \n" \
"   const int N,                                                        \n" \
"   __global float* A,                                                  \n" \
"   __global float* B,                                                  \n" \
"   __global float* C,                                                  \n" \
"   __local float* Bwrk)                                                \n" \
"{                                                                      \n" \
"	int j, k;                                                       \n" \
"       int i = get_global_id(0);                                       \n" \
"       int iloc = get_local_id(0);                                     \n" \ 
"       int nloc = get_local_size(0);                                   \n" \ 
"       			                                        \n" \
"       float tmp;		                                        \n" \
"       float Awrk[1024];	                                        \n" \ 
"       			                                        \n" \
"       for(k=0; k<N; k++)	                                        \n" \
"       	Awrk[k] = A[i*N+k];                                     \n" \
"       			                                        \n" \
"       for(j=0; j<N; j++){                                             \n" \
"          for(k=iloc; k<N; k+=nloc)                                    \n" \
"      		Bwrk[k] = B[k*N+j];                                     \n" \
"      				                                        \n" \
"      		barrier(CLK_LOCAL_MEM_FENCE);                           \n" \
"      				                                        \n" \
"      	   tmp = 0.0f;                       	                        \n" \
"          for(k=0; k<N; k++)                                           \n" \
"      	        tmp += Awrk[k]*Bwrk[k];                                 \n" \
"      	        			                                \n" \
"          C[i*N+j] = tmp;                                              \n" \
"      	        			                                \n" \
"      	   barrier(CLK_LOCAL_MEM_FENCE);                                \n" \
"       }                                        		        \n" \
"}                                                                      \n" \
"\n";


int main(int argc, char *argv[])
{
    float *h_A;             // A matrix
    float *h_B;             // B matrix
    float *h_C;             // C = A*B matrix
    float *h_D;             
    int N;                  // A[N][N], B[N][N], C[N][N]
    int size;               // number of elements in each matrix

    cl_mem d_a, d_b, d_c;   // Matrices in device memory

    double start_time;      // Starting time
    double run_time;        // timing data

    cl_int err;             // error code returned from OpenCL calls
    cl_device_id     device;     // compute device id 
    cl_context       context;       // compute context
    cl_command_queue commands;      // compute command queue
    cl_program       program;       // compute program
    cl_kernel        kernel;        // compute kernel

    N    = ORDER;
    size = N * N;	

    h_A = (float *)malloc(size * sizeof(float));	// 대략 4메가
    h_B = (float *)malloc(size * sizeof(float));
    h_C = (float *)malloc(size * sizeof(float));
    h_D = (float *)malloc(size * sizeof(float));



//--------------------------------------------------------------------------------
// Create a context, queue and device.
//--------------------------------------------------------------------------------

    cl_uint deviceIndex = 0;	// 1 - GPU(NVIDIA GeForce GTX), 1 - CPU(Intel Xeon)
    parseArguments(argc, argv, &deviceIndex);

    // Get list of devices
    cl_device_id devices[MAX_DEVICES];
    unsigned numDevices = getDeviceList(devices);

    // Check device index in range
    if (deviceIndex >= numDevices)
    {
      printf("Invalid device index (try '--list')\n");
      return EXIT_FAILURE;
    }

    device = devices[deviceIndex];

    char name[MAX_INFO_STRING];
    getDeviceName(device, name);
    printf("\nUsing OpenCL device: %s\n", name);

    // Create a compute context
    context = clCreateContext(0, 1, &device, NULL, NULL, &err);
    checkError(err, "Creating context");

   // Create a command queue
    commands = clCreateCommandQueue(context, device, 0, &err);
    checkError(err, "Creating command queue");

//--------------------------------------------------------------------------------
// Run sequential version on the host
//--------------------------------------------------------------------------------
/*
    initmat(N, h_A, h_B, h_C);

    printf("\n===== Sequential, matrix mult (dot prod), order %d on host CPU ======\n",ORDER);
    for(int i = 0; i < COUNT; i++)
    {
        zero_mat(N, h_C);
        start_time = wtime();

        seq_mat_mul_sdot(N, h_A, h_B, h_C);

        run_time  = wtime() - start_time;
        results(N, h_C, run_time);
    }
*/ 
    // 행렬 값 저장
//    for(int i=0; i<size; i++)
//	    h_D[i] = h_C[i];


//--------------------------------------------------------------------------------
// Setup the buffers, initialize matrices, and write them into global memory
//--------------------------------------------------------------------------------

    //  Reset A, B and C matrices (just to play it safe)
    initmat(N, h_A, h_B, h_C);

    d_a = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                            sizeof(float) * size, h_A, &err);
    checkError(err, "Creating buffer d_a");

    d_b = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                            sizeof(float) * size, h_B, &err);
    checkError(err, "Creating buffer d_b");

    d_c = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                            sizeof(float) * size, NULL, &err);
    checkError(err, "Creating buffer d_c");


//--------------------------------------------------------------------------------
// OpenCL matrix multiplication ... Naive
//--------------------------------------------------------------------------------

    // Create the comput program from the source buffer
    program = clCreateProgramWithSource(context, 1, (const char **) & kernelsource, NULL, &err);
    checkError(err, "Creating program");

   // Build the program
    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        size_t len;
        char buffer[2048];

        printf("Error: Failed to build program executable!\n%s\n", err_code(err));
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        printf("%s\n", buffer);
        return EXIT_FAILURE;
    }

    // Create the compute kernel from the program
    kernel = clCreateKernel(program, "mmul", &err);
    checkError(err, "Creating kernel");

    printf("\n===== OpenCL, matrix mult, C(i,j) per work item, order %d ======\n",N);

    // Do the multiplication COUNT times
    for (int i = 0; i < COUNT; i++)
    {
        zero_mat(N, h_C);

        err =  clSetKernelArg(kernel, 0, sizeof(int),    &N);
        err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_a);
        err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &d_b);
        err |= clSetKernelArg(kernel, 3, sizeof(cl_mem), &d_c);
        checkError(err, "Setting kernel arguments");

        start_time = wtime();

        // Execute the kernel over the entire range of C matrix elements ... computing
        // a dot product for each element of the product matrix.  The local work
        // group size is set to NULL ... so I'm telling the OpenCL runtime to
        // figure out a local work group size for me.
        const size_t global[2] = {N, N};	// 2차원배열 지정(global ID 0, 1 이렇게)
        err = clEnqueueNDRangeKernel(		// Highright this line!
            commands,
            kernel,
            2, NULL,		// 2: 2차원(행렬이니깐)(reference card 참조)
            global, NULL,
            0, NULL, NULL);

        checkError(err, "Enqueuing kernel");

        err = clFinish(commands);
        checkError(err, "Waiting for commands to finish");

        run_time = wtime() - start_time;

        err = clEnqueueReadBuffer(
            commands, d_c, CL_TRUE, 0,
            sizeof(float) * size, h_C,
            0, NULL, NULL);
        checkError(err, "Reading back buffer d_c");

        results(N, h_C, run_time);

    } // end for loop


    // 행렬 값 저장
    for(int i=0; i<size; i++){
	    h_D[i] = h_C[i];
	    if(i<50)
	    	printf("%.2f ", h_D[i]);
    } 
    printf("\n");




//--------------------------------------------------------------------------------
// Setup the buffers, initialize matrices, and write them into global memory
//--------------------------------------------------------------------------------

    //  Reset A, B and C matrices (just to play it safe)
    initmat(N, h_A, h_B, h_C);

    d_a = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                            sizeof(float) * size, h_A, &err);
    checkError(err, "Creating buffer d_a");

    d_b = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                            sizeof(float) * size, h_B, &err);
    checkError(err, "Creating buffer d_b");

    d_c = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                            sizeof(float) * size, NULL, &err);
    checkError(err, "Creating buffer d_c");


//--------------------------------------------------------------------------------
// OpenCL matrix multiplication ... One work item per row of C
//--------------------------------------------------------------------------------

    // Create the comput program from the source buffer
    program = clCreateProgramWithSource(context, 1, (const char **) & kernelsource1, NULL, &err);
    checkError(err, "Creating program");

   // Build the program
    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        size_t len;
        char buffer[2048];

        printf("Error: Failed to build program executable!\n%s\n", err_code(err));
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        printf("%s\n", buffer);
        return EXIT_FAILURE;
    }

    // Create the compute kernel from the program
    kernel = clCreateKernel(program, "mmul1", &err);
    checkError(err, "Creating kernel");

    printf("\n===== OpenCL, matrix mult, C row per work item, order %d ======\n",N);

    // Do the multiplication COUNT times
    for (int i = 0; i < COUNT; i++)
    {
        zero_mat(N, h_C);

        err =  clSetKernelArg(kernel, 0, sizeof(int),    &N);
        err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_a);
        err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &d_b);
        err |= clSetKernelArg(kernel, 3, sizeof(cl_mem), &d_c);
        checkError(err, "Setting kernel arguments");

        start_time = wtime();

        // Execute the kernel over the entire range of C matrix elements ... computing
        // a dot product for each element of the product matrix.  The local work
        // group size is set to NULL ... so I'm telling the OpenCL runtime to
        // figure out a local work group size for me.
        const size_t global = N;
        err = clEnqueueNDRangeKernel(		// Highright this line!
            commands,
            kernel,
            1, NULL,		
            &global, NULL,
            0, NULL, NULL);

        checkError(err, "Enqueuing kernel");

        err = clFinish(commands);
        checkError(err, "Waiting for commands to finish");

        run_time = wtime() - start_time;

        err = clEnqueueReadBuffer(
            commands, d_c, CL_TRUE, 0,
            sizeof(float) * size, h_C,
            0, NULL, NULL);
        checkError(err, "Reading back buffer d_c");

        results(N, h_C, run_time);

    } // end for loop


    // 실제 행렬값(C)와 값 동일한지 비교
    for(int i=0; i<50; i++){
	    printf("%.2f ", h_C[i]);
	    if(h_D[i] != h_C[i]){
		printf("\n행렬값 불일치!\n");
		break;
	    }
	    if(i==49)
    		printf("\n행렬값 일치\n");
    }



//--------------------------------------------------------------------------------
// Setup the buffers, initialize matrices, and write them into global memory
//--------------------------------------------------------------------------------

    //  Reset A, B and C matrices (just to play it safe)
    initmat(N, h_A, h_B, h_C);

    d_a = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                            sizeof(float) * size, h_A, &err);
    checkError(err, "Creating buffer d_a");

    d_b = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                            sizeof(float) * size, h_B, &err);
    checkError(err, "Creating buffer d_b");

    d_c = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                            sizeof(float) * size, NULL, &err);
    checkError(err, "Creating buffer d_c");


//--------------------------------------------------------------------------------
// OpenCL matrix multiplication ... Row of A in private memory
//--------------------------------------------------------------------------------

    // Create the comput program from the source buffer
    program = clCreateProgramWithSource(context, 1, (const char **) & kernelsource2, NULL, &err);
    checkError(err, "Creating program");

   // Build the program
    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        size_t len;
        char buffer[2048];

        printf("Error: Failed to build program executable!\n%s\n", err_code(err));
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        printf("%s\n", buffer);
        return EXIT_FAILURE;
    }

    // Create the compute kernel from the program
    kernel = clCreateKernel(program, "mmul2", &err);
    checkError(err, "Creating kernel");

    printf("\n===== OpenCL, matrix mult, C row per work item, A row private, order %d ======\n",N);

    // Do the multiplication COUNT times
    for (int i = 0; i < COUNT; i++)
    {

        err =  clSetKernelArg(kernel, 0, sizeof(int),    &N);
        err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_a);
        err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &d_b);
        err |= clSetKernelArg(kernel, 3, sizeof(cl_mem), &d_c);
        checkError(err, "Setting kernel arguments");

        start_time = wtime();

        // Execute the kernel over the entire range of C matrix elements ... computing
        // a dot product for each element of the product matrix.  The local work
        // group size is set to NULL ... so I'm telling the OpenCL runtime to
        // figure out a local work group size for me.
        const size_t global = N;
        err = clEnqueueNDRangeKernel(		// Highright this line!
            commands,
            kernel,
            1, NULL,		
            &global, NULL,
            0, NULL, NULL);

        checkError(err, "Enqueuing kernel");

        err = clFinish(commands);
        checkError(err, "Waiting for commands to finish");

        run_time = wtime() - start_time;

        err = clEnqueueReadBuffer(
            commands, d_c, CL_TRUE, 0,
            sizeof(float) * size, h_C,
            0, NULL, NULL);
        checkError(err, "Reading back buffer d_c");

        results(N, h_C, run_time);

    } // end for loop


    // 실제 행렬값(C)와 값 동일한지 비교
    for(int i=0; i<50; i++){
	    printf("%.2f ", h_C[i]);
	    if(h_D[i] != h_C[i]){
		printf("\n행렬값 불일치!\n");
		break;
	    }
	    if(i==49)
    		printf("\n행렬값 일치\n");
    }


//--------------------------------------------------------------------------------
// Setup the buffers, initialize matrices, and write them into global memory
//--------------------------------------------------------------------------------

    //  Reset A, B and C matrices (just to play it safe)
    initmat(N, h_A, h_B, h_C);

    d_a = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                            sizeof(float) * size, h_A, &err);
    checkError(err, "Creating buffer d_a");

    d_b = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                            sizeof(float) * size, h_B, &err);
    checkError(err, "Creating buffer d_b");

    d_c = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                            sizeof(float) * size, NULL, &err);
    checkError(err, "Creating buffer d_c");


//--------------------------------------------------------------------------------
// OpenCL matrix multiplication ... B column shared between work-items
//--------------------------------------------------------------------------------

    // Create the comput program from the source buffer
    program = clCreateProgramWithSource(context, 1, (const char **) & kernelsource3, NULL, &err);
    checkError(err, "Creating program");

   // Build the program
    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        size_t len;
        char buffer[2048];

        printf("Error: Failed to build program executable!\n%s\n", err_code(err));
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        printf("%s\n", buffer);
        return EXIT_FAILURE;
    }

    // Create the compute kernel from the program
    kernel = clCreateKernel(program, "mmul3", &err);
    checkError(err, "Creating kernel");

    printf("\n===== OpenCL, matrix mult, C row, A priv, B local, order %d ======\n",N);

    // Do the multiplication COUNT times
    for (int i = 0; i < COUNT; i++)
    {

        err =  clSetKernelArg(kernel, 0, sizeof(int),    &N);
        err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_a);
        err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &d_b);
        err |= clSetKernelArg(kernel, 3, sizeof(cl_mem), &d_c);
        err |= clSetKernelArg(kernel, 4, sizeof(float)*N, NULL);
        checkError(err, "Setting kernel arguments");

        start_time = wtime();


        const size_t global = N;
        const size_t local = global/16;		// 16개의 work-group 만들겠다. work-size=64. (주의! work-size가 정수로 떨어지는 work-group을 지정해야함!)
        err = clEnqueueNDRangeKernel(		// Highright this line!
            commands,
            kernel,
            1, NULL,		
            &global, &local,
            0, NULL, NULL);

        checkError(err, "Enqueuing kernel");

        err = clFinish(commands);
        checkError(err, "Waiting for commands to finish");

        run_time = wtime() - start_time;

        err = clEnqueueReadBuffer(
            commands, d_c, CL_TRUE, 0,
            sizeof(float) * size, h_C,
            0, NULL, NULL);
        checkError(err, "Reading back buffer d_c");

        results(N, h_C, run_time);

    } // end for loop


    // 실제 행렬값(C)와 값 동일한지 비교
    for(int i=0; i<50; i++){
	    printf("%.2f ", h_C[i]);
	    if(h_D[i] != h_C[i]){
		printf("\n행렬값 불일치!\n");
		break;
	    }
	    if(i==49)
    		printf("\n행렬값 일치\n");
    }


//--------------------------------------------------------------------------------
// OpenCL matrix multiplication ... blocked
//--------------------------------------------------------------------------------

    kernelsource = getKernelSource("./C_block_form2.cl");
    // Create the comput program from the source buffer
    program = clCreateProgramWithSource(context, 1, (const char **) & kernelsource, NULL, &err);
    checkError(err, "Creating program");

   // Build the program
    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        size_t len;
        char buffer[2048];

        printf("Error: Failed to build program executable!\n%s\n", err_code(err));
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        printf("%s\n", buffer);
        return EXIT_FAILURE;
    }

    // Create the compute kernel from the program
    kernel = clCreateKernel(program, "mmul4", &err);
    checkError(err, "Creating kernel");

    printf("\n===== OpenCL, matrix mult, Blocked, order %d ======\n",N);

    // Do the multiplication COUNT times
    for (int i = 0; i < COUNT; i++)
    {
	zero_mat(N, h_C);

	const unsigned blocksize = 16;

        err =  clSetKernelArg(kernel, 0, sizeof(int),    &N);
        err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_a);
        err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &d_b);
        err |= clSetKernelArg(kernel, 3, sizeof(cl_mem), &d_c);
        err |= clSetKernelArg(kernel, 4, sizeof(float)*blocksize*blocksize, NULL);
        err |= clSetKernelArg(kernel, 5, sizeof(float)*blocksize*blocksize, NULL);
        checkError(err, "Setting kernel arguments");

        start_time = wtime();

        // Execute the kernel over the entire range of C matrix elements ... computing
        // a dot product for each element of the product matrix.  The local work
        // group size is set to NULL ... so I'm telling the OpenCL runtime to
        // figure out a local work group size for me.
        const size_t global[2] = {N, N};
        const size_t local[2] = {blocksize, blocksize};
        err = clEnqueueNDRangeKernel(		// Highright this line!
            commands,
            kernel,
            2, NULL,		
            global, local,
            0, NULL, NULL);

        checkError(err, "Enqueuing kernel");

        err = clFinish(commands);
        checkError(err, "Waiting for commands to finish");

        run_time = wtime() - start_time;

        err = clEnqueueReadBuffer(
            commands, d_c, CL_TRUE, 0,
            sizeof(float) * size, h_C,
            0, NULL, NULL);
        checkError(err, "Reading back buffer d_c");

        results(N, h_C, run_time);

    } // end for loop



    // 실제 행렬값(C)와 값 동일한지 비교
    for(int i=0; i<50; i++){
	    printf("%.2f ", h_C[i]);
	    if(h_D[i] != h_C[i]){
		printf("\n행렬값 불일치!\n");
		break;
	    }
	    if(i==49)
    		printf("\n행렬값 일치\n");
    }




//--------------------------------------------------------------------------------
// Clean up!
//--------------------------------------------------------------------------------

    free(h_A);
    free(h_B);
    free(h_C);
    clReleaseMemObject(d_a);
    clReleaseMemObject(d_b);
    clReleaseMemObject(d_c);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(commands);
    clReleaseContext(context);

    return EXIT_SUCCESS;
}

char* getKernelSource(char* filename){
	FILE *file = fopen(filename, "r");
	if(!file){
		fprintf(stderr, "Error: Could not open kernel source file\n");
		exit(EXIT_FAILURE);
	}
	fseek(file, 0, SEEK_END);
	int len = ftell(file) + 1;
	rewind(file);

	char* source = (char*)calloc(sizeof(char), len);
	if(!source){
		fprintf(stderr, "Error: Could not allocate memory for source string\n");
		exit(EXIT_FAILURE);
	}
	fread(source, sizeof(char), len, file);
	fclose(file);
	return source;
}
