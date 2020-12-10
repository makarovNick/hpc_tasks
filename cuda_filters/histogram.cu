#include "image.h"

#define PIXELS_PER_BLOCK 512
#define THREAD_PER_BLOCK 4

#define min(a, b) ((a) < (b) ? (a) : (b))

__global__ void histogram(BYTE* img, int height, int width, int channels, int * histogram)
{

    int x = threadIdx.x + blockIdx.x * blockDim.x; 
    int y = threadIdx.y + blockIdx.y * blockDim.y; 
    
    for (int i = x * (PIXELS_PER_BLOCK / THREAD_PER_BLOCK); i < min(height, x * (PIXELS_PER_BLOCK / THREAD_PER_BLOCK) + PIXELS_PER_BLOCK / THREAD_PER_BLOCK); i++)
    {
        for (int j = y * (PIXELS_PER_BLOCK / THREAD_PER_BLOCK); j < min(width, y * (PIXELS_PER_BLOCK / THREAD_PER_BLOCK) + PIXELS_PER_BLOCK / THREAD_PER_BLOCK); j++)
        {
            if (channels < 3)
            {
		
                atomicAdd(histogram + img[i * width * channels + j * channels + 0], 1);
            }
            else
            {
                atomicAdd(histogram + (int)(img[i * width * channels + j * channels + 0] * 0.299
                                          + img[i * width * channels + j * channels + 1] * 0.587
                                          + img[i * width * channels + j * channels + 2] * 0.114), 1);
            } 
        }
    }
}

void save_histogram(const char* name, int* hist, int height)
{    
    BYTE* img = (BYTE*) malloc(sizeof(BYTE) * height * 256);
    int max = 0;
    for (int i = 0; i < 256; i++)
    {
        max = (max > hist[i]) ? max : hist[i];
    }

    double coef = 1.0 * height / max;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < 256; j++)
        {
            if ((int)(hist[j] * coef) <= i)
            {
                img[(height - 1 - i) * 256 + j + 0] = 0;
            }
            else
            {
                img[(height - 1 - i) * 256 + j + 0] = 255;
            }
        }
    }

    stbi_write_jpg(name, 256, height, 1, img, 100);

    free(img);
}

int main(int argc, char** argv) 
{
    int width, height, channels;
    BYTE* h_image =  stbi_load(argv[1], &width, &height, &channels, 0);
    printf("H = %d, W = %d, C = %d\n", height, width, channels);
    BYTE* d_image;
    int* d_out;
    int* h_hist =(int*) malloc(sizeof(int) * 256);

    cudaMalloc(&d_image, height * width * height * sizeof(BYTE));
    cudaMalloc(&d_out, 256 * sizeof(int));

    cudaMemcpy(d_image, h_image, sizeof(BYTE) * channels * width * height, cudaMemcpyHostToDevice);

    int blck_x = (height + PIXELS_PER_BLOCK - 1) / PIXELS_PER_BLOCK;
    int blck_y = (width + PIXELS_PER_BLOCK - 1) / PIXELS_PER_BLOCK;
    
    memset(h_hist, '\0', sizeof(int) * 256);
    cudaMemcpy(d_out, h_hist, sizeof(int) * 256, cudaMemcpyHostToDevice);
    histogram<<<dim3(blck_x, blck_y), dim3(THREAD_PER_BLOCK, THREAD_PER_BLOCK)>>>(d_image, height, width, channels, d_out);
    cudaMemcpy(h_hist, d_out, sizeof(int) * 256 , cudaMemcpyDeviceToHost);
    for (int i = 0; i < 256; ++i)
    {
	printf("%d ", h_hist[i]);
    }
 
    save_histogram("hist.jpg", h_hist, 100);

    free(h_image);
    free(h_hist);
    cudaFree(d_out);
    cudaFree(d_image);

    return 0;
}
