#include <stdio.h>
#include <stdlib.h>

#include <CL/cl.h>


#define MEM_SIZE (128)
#define MAX_SOURCE_SIZE (0x100000)

int main()
{
	cl_device_id device_id = NULL;
	cl_context context = NULL;
	cl_command_queue command_queue = NULL;
	cl_mem memobj = NULL;
	cl_program program = NULL;
	cl_kernel kernel = NULL;
	cl_platform_id platform_id = NULL;
	cl_uint ret_num_device;
	cl_uint ret_num_platforms;
	cl_int ret;

	char string[MEM_SIZE];

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


	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
	if(ret != CL_SUCCESS || command_queue==NULL )
	{
		printf("Failed to create command Queue\n");
		return 0;
	}


	memobj = clCreateBuffer(context, CL_MEM_READ_WRITE, MEM_SIZE*sizeof(char), NULL, &ret);
	if(ret != CL_SUCCESS)
	{
		printf("Failed to create buffer\n");
		return 0;
	}

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

	kernel = clCreateKernel(program, "hello_world_opencl", &ret);
	if(kernel == NULL)
	{
		printf("Failed to create kernel\n");
		return 0;
	}

	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&memobj);

	ret = clEnqueueTask(command_queue, kernel, 0, NULL, NULL);

	ret = clEnqueueReadBuffer(command_queue, memobj, CL_TRUE, 0,
			MEM_SIZE * sizeof(char), string, 0, NULL, NULL);

	puts(string);

	ret = clFlush(command_queue);
	ret = clFinish(command_queue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(memobj);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);

	free(source_str);

	return 0;
}
