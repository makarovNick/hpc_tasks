#include "image.h"

#define PIXELS_PER_BLOCK 750
#define THREAD_PER_BLOCK 25

#define min(a, b) ((a) < (b) ? (a) : (b))

__global__ void median_filter(BYTE* img, int filter_size, int height, int width, int channels, BYTE* res)
{
    const int x = blockDim.x * blockIdx.x + threadIdx.x;
    const int y = blockDim.y * blockIdx.y + threadIdx.y;
    
    BYTE* medians_arrays = (BYTE*) malloc(sizeof(BYTE) * filter_size * filter_size * channels);

    filter_size /= 2;

    for (int i = x * (PIXELS_PER_BLOCK / THREAD_PER_BLOCK);
         i < min(height,
                 x * (PIXELS_PER_BLOCK / THREAD_PER_BLOCK) + PIXELS_PER_BLOCK / THREAD_PER_BLOCK);
         i++)
    {
        for (int j = y * (PIXELS_PER_BLOCK / THREAD_PER_BLOCK);
             j < min(width, y * (PIXELS_PER_BLOCK / THREAD_PER_BLOCK)
                         + PIXELS_PER_BLOCK / THREAD_PER_BLOCK);
             j++)
        {
            for (int c = 0; c < channels; c++)
            {
                for (int v = -filter_size; v <= filter_size; v++)
                {
                    for (int h = -filter_size; h <= filter_size; h++)
                    {
                        if (i + v >= 0 && j + h >= 0 && i + v <= height - 1 && j + h <= width - 1)
                            medians_arrays[c * ((filter_size * 2 + 1) * (filter_size * 2 + 1))
                                + (filter_size + v) * (filter_size * 2 + 1) + (h + filter_size)]
                                = img[(i + v) * width * channels + (j + h) * channels + c];
                        else
                            medians_arrays[c * ((filter_size * 2 + 1) * (filter_size * 2 + 1))
                                + (filter_size + v) * (filter_size * 2 + 1) + (h + filter_size)]
                                = 0;
                    }
                }
		
                int x, z, m, aux;
                BYTE * array = medians_arrays + c * ((filter_size * 2 + 1) * (filter_size * 2 + 1));
                int size = (filter_size * 2 + 1) * (filter_size * 2 + 1);

                for (x = 0; x < (size - 1); x++)
                {
                    m = x;
                    for (z = (x + 1); z < size; z++)
                    {
                        if (array[z] < array[m])
                        {
                            m = z;
                        }
                    }
                    if (x != m)
                    {
                        aux = array[x];
                        array[x] = array[m];
                        array[m] = aux;
                    }
                }
	
                res[i * width * channels + j * channels + c]
                    = medians_arrays[c * ((filter_size * 2 + 1) * (filter_size * 2 + 1))
                        + filter_size * (filter_size * 2 + 1) + filter_size];
            }
        }
    }

    free(medians_arrays);
}


int main(int argc, char** argv)
{
    if (argc != 4)
    {
        printf("usage : %s <input_image_path> <filter_size> <output_image_path\n", argv[0]);
	exit(1);
    }

    int filter_size = atoi(argv[2]);
    const char * input = argv[1];
    const char * output = argv[3];
    int width, height, channels;
    BYTE* h_img = stbi_load(input, &width, &height, &channels, 0);
    
    BYTE* d_img;
    BYTE* d_res_img;
    if (h_img == NULL)
    {
        printf("Error in loading the image\n");
        exit(1);
    }

    printf("THREAD_PER_BLOCK : %d \nPIXELSE_PER_BLOCK : %d\n", THREAD_PER_BLOCK, PIXELS_PER_BLOCK);
    printf("Loaded image with \nheight : %d \nwidth : %d \nchannels : %d\n", height, width, channels);
    
    cudaMalloc(&d_img, sizeof(BYTE) * width * height * channels);
    cudaMalloc(&d_res_img, sizeof(BYTE) * width * height * channels);

    cudaMemcpy(d_img, h_img, sizeof(BYTE) * width * height * channels, cudaMemcpyHostToDevice);


    int blck_x = (height + PIXELS_PER_BLOCK - 1) / PIXELS_PER_BLOCK;
    int blck_y = (width + PIXELS_PER_BLOCK - 1) / PIXELS_PER_BLOCK;
    printf("blck x    %d    blck y    %d  \n", blck_x, blck_y);
    median_filter<<<dim3(blck_x, blck_y), dim3(THREAD_PER_BLOCK, THREAD_PER_BLOCK)>>>(d_img, filter_size, height, width, channels, d_res_img);

    cudaMemcpy(h_img, d_res_img, sizeof(BYTE) * height * width * channels, cudaMemcpyDeviceToHost);

    if (!stbi_write_jpg(output, width, height, channels, h_img, 100))
    {
        printf("Error in writing the image\n");
        exit(1);
    }

    free(h_img);
    cudaFree(d_img);
    cudaFree(d_res_img);

    return 0;
}
