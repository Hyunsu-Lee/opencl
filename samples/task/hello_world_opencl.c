#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <CL/cl.h>


#define MEM_SIZE (128)
#define MAX_SOURCE_SIZE (0x100000)

int main()
{
	cl_device_id device_id = NULL;
	cl_context context = NULL;
	cl_command_queue command_queue = NULL;
	cl_program program = NULL;
	cl_kernel kernel[4] = {NULL, NULL, NULL, NULL};
	cl_platform_id platform_id = NULL;
	cl_uint ret_num_device;
	cl_uint ret_num_platforms;
	cl_int ret;

	int i, j;
	float *A;
	float *B;
	float *C;

	cl_mem Amobj;
	cl_mem Bmobj;
	cl_mem Cmobj;

	A = (float *)malloc(4*4*sizeof(float));
	B = (float *)malloc(4*4*sizeof(float));
	C = (float *)malloc(4*4*sizeof(float));

	for(i = 0; i<4;i++){
		for(j = 0; j<4; j++){
			A[i*4+j]=i*4+j+1;
			B[i*4+j]=i*4+j+1;
		}
	}
//	memset(C, 0, 4*4*sizeof(float));
//	C[0]=1;


	FILE *fp;
	char fileName[] = "./assets/hello_world_opencl.cl";
	char *source_str;
	size_t source_size;

	fp = fopen(fileName, "r");
	if(!fp){
		fprintf(stderr, "Failed to load kernel. \n");
		exit(1);
	}

	source_str = (char*)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);

	ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);

	if(ret != CL_SUCCESS || ret_num_platforms<=0)
	{
		printf("Failed to find any OpenCL platforms.\n");
		return 0;
	}

	//??
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_device);
	if(ret != CL_SUCCESS)
	{
		printf("Failed to get device ID\n");
		return 0;
	}

	//??
	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
	if(ret != CL_SUCCESS)
	{
		printf("Failed to create context\n");
		return 0;
	}


	//Queue
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
	if(ret != CL_SUCCESS || command_queue==NULL )
	{
		printf("Failed to create command Queue\n");
		return 0;
	}

	Amobj = clCreateBuffer(context, CL_MEM_READ_WRITE, 4*4*sizeof(float), NULL, &ret);
	Bmobj = clCreateBuffer(context, CL_MEM_READ_WRITE, 4*4*sizeof(float), NULL, &ret);
	Cmobj = clCreateBuffer(context, CL_MEM_READ_WRITE, 4*4*sizeof(float), NULL, &ret);

	ret = clEnqueueWriteBuffer(command_queue, Amobj, CL_TRUE, 0, 4*4*sizeof(float), A, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, Bmobj, CL_TRUE, 0, 4*4*sizeof(float), B, 0, NULL, NULL);
//	ret = clEnqueueWriteBuffer(command_queue, Bmobj, CL_TRUE, 0, 4*4*sizeof(float), C, 0, NULL, NULL);



	program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);

	if(program == NULL)
	{
		printf("Failed to create program\n");
		return 0;
	}

	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

	if(ret != CL_SUCCESS)
	{
		printf("Failed to build program\n");
	}

	kernel[0] = clCreateKernel(program, "taskpAdd",&ret);
	kernel[1] = clCreateKernel(program, "taskpSub",&ret);
	kernel[2] = clCreateKernel(program, "taskpMul",&ret);
	kernel[3] = clCreateKernel(program, "taskpDiv",&ret);


	for(i = 0;i<4;i++){
		ret = clSetKernelArg(kernel[i], 0, sizeof(cl_mem), (void*)&Amobj);
		ret = clSetKernelArg(kernel[i], 1, sizeof(cl_mem), (void*)&Bmobj);
		ret = clSetKernelArg(kernel[i], 2, sizeof(cl_mem), (void*)&Cmobj);
	}

	//커널 실행 부분 - 데이터 병렬 실행
	for(i=0;i<4;i++){
		ret = clEnqueueTask(command_queue, kernel[i],0, NULL,NULL);
	}




	ret = clEnqueueReadBuffer(command_queue, Cmobj, CL_TRUE, 0, 4*4*sizeof(float), C, 0, NULL, NULL);




	for(i = 0 ; i<4 ; i++){
		for(j = 0; j <4;j++){
			printf("%7.2f ",C[i*4+j]);
		}
		printf("\n");
	}

	ret = clFlush(command_queue);
	ret = clFinish(command_queue);
	ret = clReleaseKernel(kernel[0]);
	ret = clReleaseKernel(kernel[1]);
	ret = clReleaseKernel(kernel[2]);
	ret = clReleaseKernel(kernel[3]);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(Amobj);
	ret = clReleaseMemObject(Bmobj);
	ret = clReleaseMemObject(Cmobj);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);

	free(source_str);

	free(A);
	free(B);
	free(C);

	return 0;
}
