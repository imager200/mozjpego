This is a tiny cgo wrapper around the MozJPEG API. It only supports the `quality` and the `progressive` parameters, as it is still a work in progress.

# Requirements

* mozjpego assumes that MozJPEG C header files as well as the shared librairies, resulting from running `make && make install` in the official MozJPEG [project](https://github.com/mozilla/mozjpeg), are already present in the host system. mozjpego makes the assumption that the host system is Linux, so it expects that:

 - The header files are present under `/opt/mozjpeg/include/`
 - The shared libraries files are present under `/opt/mozjpeg/lib64/`

If you are using a different operation system or your MozJPEG installation happens to be in a different location you can use the `CGO_CFLAGS` and `CGO_LDFLAGS` environment variables to override the `#cgo` directive [here](https://github.com/imager200/mozjpego/blob/master/cjpeg.go#L4)

This is the solution that I found to avoid bloating the repo with the many `.h` and `.so` files required. If you have a better suggestion, please let me know.

# Usage Example:

```
package main

import (
	"log"
	"os"

	"github.com/imager200/mozjpego"
)

func main() {
	p := mozjpego.Parameters{Quality: 85, Progressive: false}

	b, err := os.ReadFile("jpeg_image.jpeg")
	if err != nil {
		log.Fatal(err.Error())
	}

	res, err := mozjpego.Compress(b, p)
	if err != nil {
		log.Fatal(err.Error())
	}

	if err := os.WriteFile("result.jpeg", res, 0644); err != nil {
		log.Fatal(err.Error())
	}
}

```

# Similar projects:

- [https://github.com/pixiv/go-libjpeg](https://github.com/pixiv/go-libjpeg)



