package mozjpego

/*
 #cgo CFLAGS: -I/opt/libjpeg-turbo/include/
 #cgo LDFLAGS: -L/opt/libjpeg-turbo/lib64 -lturbojpeg -ljpeg -Wl,-rpath=/opt/mozjpeg/lib64/
 #include <stdio.h>
#include <jpeglib.h>
#include <stdlib.h>
#include <setjmp.h>
#include <math.h>

#define INPUT_BUF_SIZE  4096

char* compress_JPEG_file(char *inbuffer, unsigned long insize, int quality) {
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
  cinfo.err = jpeg_std_error(&jerr);

  jpeg_create_compress(&cinfo);
   cinfo.in_color_space = JCS_RGB;

   jpeg_set_defaults(&cinfo);

   struct jpeg_decompress_struct dinfo;
   struct jpeg_error_mgr jerr2;

   dinfo.err =  jpeg_std_error(&jerr2);

   jpeg_create_decompress(&dinfo);

   jpeg_mem_src(&dinfo, inbuffer, insize);

   unsigned char *outbuffer = NULL;
   unsigned long outsize = 0;
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

  jpeg_set_quality(&cinfo, quality, TRUE);

  jpeg_start_compress(&cinfo, TRUE);

    JSAMPARRAY image_buffer = (*cinfo.mem->alloc_sarray) ((j_common_ptr) &dinfo, JPOOL_IMAGE, (JDIMENSION) (cinfo.image_width * cinfo.input_components), (JDIMENSION) 1);

    while (cinfo.next_scanline < cinfo.image_height) {
        int numscanlines = jpeg_read_scanlines(&dinfo, image_buffer, 1);
        jpeg_write_scanlines(&cinfo, image_buffer, numscanlines);
    }

    jpeg_finish_compress(&cinfo);

    jpeg_destroy_compress(&cinfo);

    jpeg_finish_decompress(&dinfo);

    jpeg_destroy_decompress(&dinfo);

  return outbuffer;
}
*/
import "C"
import (
	"fmt"
	"os"
	"unsafe"
)

//cjpeg
type Parameters struct {
	Quality     uint8
	Progressive bool
	BaseLine    bool
}

func Compress(imageBytes []byte, params Parameters) ([]byte, error) {

	//image.Decode(r io.Reader)

	fmt.Println("input: ", len(imageBytes))

	result := C.compress_JPEG_file(C.CString(string(imageBytes)), C.ulong(len(imageBytes)), C.int(params.Quality))

	//up := unsafe.Pointer(result)

	fmt.Println("size1: ", unsafe.Sizeof(result))

	//i := 0

	/* 	for i < int(unsafe.Sizeof(result)) {
		d := (*C.int)(unsafe.Pointer(result)) + unsafe.Sizeof(i)


		i++
	} */

	res := []byte(C.GoString(result))

	fmt.Println("size2: ", len(res))

	os.TempDir()

	for i, _ := range res {
		//current := (*[]byte)(unsafe.Pointer(uintptr(res[i])))
		fmt.Printf("%d\n", res[i])
		fmt.Printf("%d\n", unsafe.Sizeof(&res[i]))
		fmt.Printf("%d\n", unsafe.Alignof(&res[i]))
		//fmt.Printf("%d\n", current)
		//unsafe.Pointer(&res[.i])
	}

	return []byte(C.GoString(result)), nil
}
