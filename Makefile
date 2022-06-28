


compile-example:
	@gcc -Wall -o testcompress_memory -g -I/opt/mozjpeg/include/ -L/opt/libjpeg-turbo/lib64 example/compress_memory.c -lturbojpeg -ljpeg -Wl,-rpath=/opt/mozjpeg/lib64/