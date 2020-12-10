#include "image.h"

#define PIXELS_PER_BLOCK 512
#define THREAD_PER_BLOCK 4

#define min(a, b) ((a) < (b) ? (a) : (b))

__global__ void filter(BYTE* img, int filter_size, double* filter, int height, int width, int channels, BYTE* res)
{
    const int x = blockDim.x * blockIdx.x + threadIdx.x;
    const int y = blockDim.y * blockIdx.y + threadIdx.y;

    double s = 0.0;
    //if (x ==0 && y == 0)
    {
        for (int i = 0; i < filter_size; i++)
        {
            for (int j = 0; j < filter_size; j++)
            {
                s += filter[i * filter_size + j];
            }
        }

        for (int i = 0; i < filter_size; i++)
        {
            for (int j = 0; j < filter_size; j++)
            {
                filter[i * filter_size + j] /= s;
            }
        }
        filter_size /= 2;
    }

    //__syncthreads();
    //printf("%d , %lf \n", filter_size, s);    
    for (int i = x * (PIXELS_PER_BLOCK / THREAD_PER_BLOCK); i < min(height, x * (PIXELS_PER_BLOCK / THREAD_PER_BLOCK) + PIXELS_PER_BLOCK / THREAD_PER_BLOCK); i++)
    {
        for (int j = y * (PIXELS_PER_BLOCK / THREAD_PER_BLOCK); j < min(width, y * (PIXELS_PER_BLOCK / THREAD_PER_BLOCK) + PIXELS_PER_BLOCK / THREAD_PER_BLOCK); j++)
        {
            for (int c = 0; c < channels; c++)
            {
            	res[i * width * channels + j * channels + c] = 0;
                for (int v = -filter_size; v <= filter_size; v++)
                {
                    for (int h = -filter_size; h <= filter_size; h++)
                    {
                        if (i + v >= 0 && j + h >= 0 && i + v <= height - 1 && j + h <= width - 1)
                        {
                            res[i * width * channels + j * channels + c] += filter[(v + filter_size) * (filter_size * 2 + 1) + h + filter_size] * img[(i + v) * width * channels + (j + h) * channels + c];
                        }
                    }
                }

            }
        }
    }
}


int main(int argc, char** argv) 
{
    
    if (argc < 3)
    {
	printf("usage : %s <input_image_path> <output_image_path>", argv[0]);
	exit(1);
    }
    int width, height, channels;
    BYTE* h_img = stbi_load(argv[1], &width, &height, &channels, 0);
    BYTE* d_img;
    BYTE* d_res_img;
    if(h_img == NULL) 
    {
        printf("Error in loading the image\n");
        exit(1);
    }

    cudaMalloc(&d_img, sizeof(BYTE) * width * height * channels);
    cudaMalloc(&d_res_img, sizeof(BYTE) * width * height * channels);

    cudaMemcpy(d_img, h_img, sizeof(BYTE) * width * height *channels, cudaMemcpyHostToDevice);

    int filter_size = 3;
    printf("Size of filter : \n");
    scanf("%d", &filter_size);
    double* h_filter = (double*) malloc(sizeof(double) * filter_size * filter_size);
    if (filter_size & 1 == 0)
    {
	printf("filter size must be odd\n");
        exit(1);
    }
    printf("Filter : \n");
    for (int i = 0; i < filter_size; ++i)
    {
        for (int j = 0; j < filter_size; ++j)
	{
	    printf("filter[i][j] : \n");
	    scanf("%d", &h_filter[i * filter_size + j]);
	}
    }
    double* d_filter;

    // h_filter[0] = 1;     h_filter[1] = 1;   h_filter[2] = 1;
    // h_filter[3] = 1;     h_filter[4] = 1;   h_filter[5] = 1;
    // h_filter[6] = 1;     h_filter[7] = 1;   h_filter[8] = 1;

    cudaMalloc(&d_filter, sizeof(double) * filter_size * filter_size);
    cudaMemcpy(d_filter, h_filter, sizeof(double) * filter_size * filter_size, cudaMemcpyHostToDevice);

    int blck_x = (height + PIXELS_PER_BLOCK - 1) / PIXELS_PER_BLOCK;
    int blck_y = (width + PIXELS_PER_BLOCK - 1) / PIXELS_PER_BLOCK;
    filter<<<dim3(blck_x, blck_y), dim3(THREAD_PER_BLOCK, THREAD_PER_BLOCK)>>>(d_img, filter_size, d_filter, height, width, channels, d_res_img);

    cudaMemcpy(h_img, d_res_img, sizeof(BYTE) * height * width * channels, cudaMemcpyDeviceToHost);

    if(!stbi_write_jpg(argv[2], width, height, channels, h_img, 100))
    {
        printf("Error in writing the image\n");
        exit(1);
    }

    free(h_filter);
    cudaFree(d_filter);
    free(h_img);
    cudaFree(d_img);
    cudaFree(d_res_img);
   

    return 0;
}
