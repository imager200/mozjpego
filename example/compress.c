#include <stdio.h>
#include <jpeglib.h>
#include <stdlib.h>
#include <setjmp.h>
#include <math.h>


/*
 * Sample routine for JPEG compression.  We assume that the target file name
 * and a compression quality factor are passed in.
 */

void compress_JPEG_file(char *src_file_name, char* dst_file_name, int quality) {
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
  /* More stuff */
  FILE *input_file;               
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);

   cinfo.in_color_space = JCS_RGB;
   
   jpeg_set_defaults(&cinfo);

  if ((input_file = fopen(src_file_name, "rb")) == NULL) {
    fprintf(stderr, "can't open %s\n", src_file_name);
    exit(1);
  }

   struct jpeg_decompress_struct dinfo;
   struct jpeg_error_mgr jerr2;
   dinfo.err =  jpeg_std_error(&jerr2);
   jpeg_create_decompress(&dinfo);

   jpeg_stdio_src(&dinfo, input_file);;
   FILE *output = fopen(dst_file_name, "wb");
   jpeg_stdio_dest(&cinfo, output);

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

  /* First we supply a description of the input image.
   * Four fields of the cinfo struct must be filled in:
   */
  /* Now use the library's routine to set default compression parameters.
   * (You must set at least cinfo.in_color_space before calling this,
   * since the defaults depend on the source color space.)
   */

  /* Now you can set any non-default parameters you wish to.
   * Here we just illustrate the use of quality (quantization table) scaling:
   */
  jpeg_set_quality(&cinfo, quality, TRUE);

  /* Step 4: Start compressor */

  /* TRUE ensures that we will write a complete interchange-JPEG file.
   * Pass TRUE unless you are very sure of what you're doing.
   */
  jpeg_start_compress(&cinfo, TRUE);
  /* Step 5: while (scan lines remain to be written) */
  /*           jpeg_write_scanlines(...); */


 JSAMPARRAY image_buffer = (*cinfo.mem->alloc_sarray) ((j_common_ptr) &cinfo, JPOOL_IMAGE, (JDIMENSION) (cinfo.image_width * cinfo.input_components), (JDIMENSION) 1);

   /* Here we use the library's state variable cinfo.next_scanline as the
   * loop counter, so that we don't have to keep track ourselves.
   * To keep things simple, we pass one scanline per call; you can pass
   * more if you wish, though.
   */

  while (cinfo.next_scanline < cinfo.image_height) {
    /* jpeg_write_scanlines expects an array of pointers to scanlines.
     * Here the array is only one element long, but you could pass
     * more than one scanline at a time if that's more convenient.
     */
    int numscanlines = jpeg_read_scanlines(&dinfo, image_buffer, 1);
    jpeg_write_scanlines(&cinfo, image_buffer, numscanlines);
  }

  /* Step 6: Finish compression */
  jpeg_finish_compress(&cinfo);

  /* Step 7: release JPEG compression object */

  /* This is an important step since it will release a good deal of memory. */
  jpeg_destroy_compress(&cinfo);

  jpeg_finish_decompress(&dinfo);

  jpeg_destroy_decompress(&dinfo);

  /* After finish_compress, we can close the output file. */
  fclose(input_file);
  fclose(output);
}

int main(int argc, char **argv) {
  if (argc < 4) {
      fprintf(stderr, "usage: %s [src_file] [dest_file] [quality]\n", argv[0]);
      exit(EXIT_FAILURE);
    }

    char* src_file_name = argv[1];
    char *dest_file_name = argv[2];
    char *quality = argv[3];

    compress_JPEG_file(src_file_name, dest_file_name, atoi(quality));

    return 0;
}
