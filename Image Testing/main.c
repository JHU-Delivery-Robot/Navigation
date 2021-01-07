#include <stdlib.h>
#include <stdint.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

enum allocation_type {
    NO_ALLOCATION, SELF_ALLOCATED, STB_ALLOCATED
};

/* Structure for image input */
typedef struct {
	size_t size;
	int channels;
	uint8_t *pix;             /* bitmap data */
	int width;            /* width of bitmap array (map width in pixels) */
	int height;           /* height of bitmap array (map height in pixels) */
	enum allocation_type allocation_;
} Image;

void copy_image(const char *pathname) {
	Image *im = malloc(sizeof(Image));

	if((im->pix = stbi_load(pathname, &im->width, &im->height, &im->channels, 0)) != NULL) {
		im->size = im->width * im->height * im->channels;
		im->allocation_ = STB_ALLOCATED;
	} else {
		free(im);
		return;
	}

	Image *im2 = malloc(sizeof(Image));

	im2->width = im->width;

	im2->height = im->height;

	im2->size = im2->width * im2->height;

	im2->channels = 1;

	im2->pix = malloc(im->width * im->height * sizeof(uint8_t));

	if (im->channels > 1) {
		for(unsigned im_count = 0, im2_count = 0; im_count < im->size && im2_count < im2->size; im_count += im->channels, im2_count += im2->channels) {
			*(im2->pix + im2_count) = (uint8_t)(*(im->pix + im_count) == 255 && *(im->pix + im_count + 1) == 255 && *(im->pix + im_count + 2) == 255 ? 255 : 0);
		}
	} else if (im->channels == 1) {
		for(unsigned im_count = 0, im2_count = 0; im_count < im->size && im2_count < im2->size; im_count += im->channels, im2_count += im2->channels) {
			*(im2->pix + im2_count) = (uint8_t)(*(im->pix + im_count) == 255 ? 255 : 0);
		}
	} else {
		free(im2->pix);
		free(im2);
		free(im->pix);
		free(im);
		return;
	}

	free(im->pix);
	free(im);

	stbi_write_png("output.png", im2->width, im2->height, im2->channels, im2->pix, im2->width * im2->channels);

	free(im2->pix);
	free(im2);
}

int main () {
	char str[100];
	printf("Enter Image file name: ");
	scanf("%s", str);
	copy_image(str);
	return 0;
}