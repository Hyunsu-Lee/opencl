#include "common.h"
#include "image.h"

#include <iostream>
#include <CL/cl.h>

using namespace std;

int main(int argc, char **argv)
{
    cl_context context = 0;
    cl_command_queue commandQueue = 0;
    cl_program program = 0;
    cl_device_id device = 0;
    cl_kernel kernel = 0;
    cl_mem memoryObjects[3] = {0, 0, 0};
    cl_int errorNumber;
    int numberOfMemoryObjects = 3;

    if (!createContext(&context))
    {
        cleanUpOpenCL(context, commandQueue, program, kernel, memoryObjects, numberOfMemoryObjects);
        cerr << "Failed to create an OpenCL context. " << __FILE__ << ":"<< __LINE__ << endl;
        return 1;
    }

    if (!createCommandQueue(context, &commandQueue, &device))
    {
        cleanUpOpenCL(context, commandQueue, program, kernel, memoryObjects, numberOfMemoryObjects);
        cerr << "Failed to create the OpenCL command queue. " << __FILE__ << ":"<< __LINE__ << endl;
        return 1;
    }

    if (!createProgram(context, device, "assets/hello_world_opencl.cl", &program))
    {
        cleanUpOpenCL(context, commandQueue, program, kernel, memoryObjects, numberOfMemoryObjects);
        cerr << "Failed to create OpenCL program." << __FILE__ << ":"<< __LINE__ << endl;
        return 1;
    }
    
    kernel = clCreateKernel(program, "hello_world_opencl", &errorNumber);
    if (!checkSuccess(errorNumber))
    {
        cleanUpOpenCL(context, commandQueue, program, kernel, memoryObjects, numberOfMemoryObjects);
        cerr << "Failed to create OpenCL kernel. " << __FILE__ << ":"<< __LINE__ << endl;
        return 1;
    }
}
