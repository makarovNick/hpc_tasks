#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#define BYTE uint8_t

BYTE* to_grayscale(BYTE* img, int height, int width, int channels)
{
    size_t img_size = width * height * channels;
    int gray_channels = channels == 4 ? 2 : 1;
    size_t gray_img_size = width * height * gray_channels * sizeof(BYTE);
 
    BYTE *gray_img = (BYTE*) malloc(gray_img_size);
    if(gray_img == NULL) 
    {
        printf("Unable to allocate memory for the gray image.\n");
        exit(1);
    } 
    for(BYTE *p = img, *pg = gray_img; p != img + img_size; p += channels, pg += gray_channels) 
    {
        *pg = (BYTE)((*p * 0.299 + *(p + 1) * 0.587 + *(p + 2) * 0.114));
        if(channels == 4) 
        {
            *(pg + 1) = *(p + 3);
        }
    }
    return gray_img;
}

BYTE*** init_image(int height, int width, int channels)
{
    BYTE*** image = (BYTE***) malloc(sizeof(BYTE**) * height);
    for (int i = 0; i < height; ++i)
    {
        image[i] = (BYTE**) malloc(sizeof(BYTE*) * width);
        for (int j = 0; j < width; ++j)
        {
            image[i][j] = (BYTE*) malloc(sizeof(BYTE) * channels);
            memset(image[i][j], '\0', channels * sizeof(BYTE));
        }
    }
    
    return image;
}

void copy_image(BYTE*** from, BYTE*** to, int height, int width, int channels)
{
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; j++)
        {
            memcpy(to[i][j], from[i][j], channels);
        }
    }
}

void free_image(BYTE*** image, int height, int width, int channels)
{
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; j++)
        {
            free(image[i][j]);
        }
        free(image[i]);
    }
    free(image);
}

void to_image(BYTE* img, BYTE*** image, int height, int width, int channels)
{
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; j++)
        {
            for (int k = 0; k < channels; k++)
            {
                image[i][j][k] = img[i * width * channels + j * channels + k];
            }   
        }
    }
}

void from_image(BYTE* img, BYTE*** image, int height, int width, int channels)
{
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; j++)
        {
            for (int k = 0; k < channels; k++)
            {
            }
        }        
    }
}


BYTE*** read_image(const char * name, int * width, int * height, int * channels)
{
    BYTE *img = stbi_load(name, width, height, channels, 0);
    if(img == NULL) 
    {
        printf("Error in loading the image\n");
        exit(1);
    }
    BYTE*** i = init_image(*height, *width, *channels);
    to_image(img, i, *height, *width, *channels);

    return i;
}

void write_image(const char * name, BYTE*** image, int width, int height, int channels)
{
    BYTE *img = (BYTE*) malloc(sizeof(BYTE) * width * height *channels);
    from_image(img, image, height, width, channels);
    
    if(stbi_write_jpg(name, width, height, channels, img, 100))
    {
        printf("Error in writing the image\n");
        exit(1);
    }
}
