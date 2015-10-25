__kernel void taskpAdd(__global float* A, __global float* B, __global float* C)
{
    int base = 0;
    C[base+0] = A[base+0] + B[base+0];
    C[base+4] = A[base+4] + B[base+4];
    C[base+8] = A[base+8] + B[base+8];
    C[base+12] = A[base+12] + B[base+12];
    
}
__kernel void taskpSub(__global float* A, __global float* B, __global float* C,  __global int* D)
{
    int kk, tt;
//    while(*D){}
    for(tt=0;tt<10;tt++){
    for(kk=0;kk<600000;kk++){
        
    int base = 1;
    C[base+0] = A[base+0] - B[base+0] +1;
    C[base+4] = A[base+4] - B[base+4] +1;
    C[base+8] = A[base+8] - B[base+8] +1;
    C[base+12] = A[base+12] - B[base+12] +1;
    }}
}
__kernel void taskpMul(__global float* A, __global float* B, __global float* C)
{
    int base = 2;
    C[base+0] = A[base+0] * B[base+0];
    C[base+4] = A[base+4] * B[base+4];
    C[base+8] = A[base+8] * B[base+8];
    C[base+12] = A[base+12] * B[base+12];
}
__kernel void taskpDiv(__global float* A, __global float* B, __global float* C)
{
    int base = 3;
    C[base+0] = A[base+0] / B[base+0];
    C[base+4] = A[base+4] / B[base+4];
    C[base+8] = A[base+8] / B[base+8];
    C[base+12] = A[base+12] / B[base+12];
}
