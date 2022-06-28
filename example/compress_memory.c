#include <stdio.h>
#include <jpeglib.h>
#include <stdlib.h>
#include <setjmp.h>
#include <math.h>

#define INPUT_BUF_SIZE  4096

unsigned char* compress_JPEG_file(char *filename, int quality) {
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE *input_file; 
  cinfo.err = jpeg_std_error(&jerr);
  unsigned long insize = 0;
  unsigned char *inbuffer = NULL;

  jpeg_create_compress(&cinfo);

   cinfo.in_color_space = JCS_RGB;

   jpeg_set_defaults(&cinfo);
  if ((input_file = fopen(filename, "rb")) == NULL) {
    fprintf(stderr, "can't open %s\n", filename);
    exit(1);
  }
   struct jpeg_decompress_struct dinfo;
   struct jpeg_error_mgr jerr2;

   dinfo.err =  jpeg_std_error(&jerr2);

   jpeg_create_decompress(&dinfo);

    /********************** Read file into memory *******************************/  
    //can be done in different ways
    //source: https://github.com/mozilla/mozjpeg/blob/master/djpeg.c#L647
     size_t nbytes;
    do {
      inbuffer = (unsigned char *)realloc(inbuffer, insize + INPUT_BUF_SIZE);
        printf("\nsize inbuffer: %ld %ld\n", sizeof(inbuffer[0]), insize);
      if (inbuffer == NULL) {
        fprintf(stderr, "memory allocation failure\n");
        exit(EXIT_FAILURE);
      }
      nbytes = fread(&inbuffer[insize], 1, INPUT_BUF_SIZE, input_file);
       printf("\nread : %ld\n", nbytes);
      if (nbytes < INPUT_BUF_SIZE && ferror(input_file)) {
          fprintf(stderr, "can't read from %s\n", filename);
      }
      insize += (unsigned long)nbytes;
    } while (nbytes == INPUT_BUF_SIZE);

    /***************************************************************************/ 

   jpeg_mem_src(&dinfo, inbuffer, insize);

   unsigned char *outbuffer = NULL;
   unsigned long outsize = insize;
   jpeg_mem_dest(&cinfo, &outbuffer, &outsize);

    jpeg_save_markers(&dinfo, JPEG_COM, 0xFFFF);
   for (int m = 0; m < 16; m++)
    jpeg_save_markers(&dinfo, JPEG_APP0 + m, 0xFFFF);

  jpeg_read_header(&dinfo, TRUE);

  jpeg_start_decompress(&dinfo);

  cinfo.in_color_space = dinfo.out_color_space;
  cinfo.input_components = dinfo.output_components;
  cinfo.data_precision = dinfo.data_precision;
  cinfo.image_width = dinfo.image_width;
  cinfo.image_height = dinfo.image_height;
  cinfo.raw_data_in = FALSE;
  /* Step 3: set parameters for compression */

  jpeg_set_quality(&cinfo, quality, TRUE);


  jpeg_start_compress(&cinfo, TRUE);

    JSAMPARRAY image_buffer = (*cinfo.mem->alloc_sarray) ((j_common_ptr) &dinfo, JPOOL_IMAGE, (JDIMENSION) (cinfo.image_width * cinfo.input_components), (JDIMENSION) 1);

    while (cinfo.next_scanline < cinfo.image_height) {
        int numscanlines = jpeg_read_scanlines(&dinfo, image_buffer, 1);
        jpeg_write_scanlines(&cinfo, image_buffer, numscanlines);
    }

    /* Step 6: Finish compression */

    jpeg_finish_compress(&cinfo);
    fclose(input_file);

    /* Step 7: release JPEG compression object */

    /* This is an important step since it will release a good deal of memory. */
    jpeg_destroy_compress(&cinfo);

    jpeg_finish_decompress(&dinfo);

    jpeg_destroy_decompress(&dinfo);

    printf("outsize: %ld", outsize);

  return outbuffer;
}

int main(int argc, char **argv) {
    if (argc < 4) {
      fprintf(stderr, "usage: %s [src_file] [dest_file] [quality]\n", argv[0]);
      exit(EXIT_FAILURE);
    }

    char* src_file_name = argv[1];
    char *dest_file_name = argv[2];
    char *quality = argv[3];

    unsigned char* result = compress_JPEG_file(src_file_name, atoi(quality));

    printf("size of: %ld", sizeof(result));

    FILE *output = fopen(dest_file_name, "wb");

    if (output == NULL) {
        exit(1);
    }
    
    for (int i = 0; i < sizeof(result); i++) {
        fwrite(result, sizeof(result), result[i], output);
    }

    fclose(output);
    return 0;
}
