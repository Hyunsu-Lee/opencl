__kernel void taskpAdd(__global float* A, __global float* B, __global float* C)
{
    int base = 0;
    C[base+0] = A[base+0] + C[base+0];
    C[base+4] = A[base+4] + C[base+4];
    C[base+8] = A[base+8] + C[base+8];
    C[base+12] = A[base+12] + C[base+12];
}
__kernel void taskpSub(__global float* A, __global float* B, __global float* C)
{
    int base = 1;
    C[base+0] = A[base+0] - C[base+0];
    C[base+4] = A[base+4] - C[base+4];
    C[base+8] = A[base+8] - C[base+8];
    C[base+12] = A[base+12] - C[base+12];
}
__kernel void taskpMul(__global float* A, __global float* B, __global float* C)
{
    int base = 2;
    C[base+0] = A[base+0] * C[base+0];
    C[base+4] = A[base+4] * C[base+4];
    C[base+8] = A[base+8] * C[base+8];
    C[base+12] = A[base+12] * C[base+12];
}
__kernel void taskpDiv(__global float* A, __global float* B, __global float* C)
{
    int base = 3;
    C[base+0] = A[base+0] / C[base+0];
    C[base+4] = A[base+4] / C[base+4];
    C[base+8] = A[base+8] / C[base+8];
    C[base+12] = A[base+12] / C[base+12];
}
