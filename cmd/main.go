package main

import (
	"log"
	"os"

	"github.com/imager200/mozjpego"
)

func main() {
	t := mozjpego.Parameters{Quality: 85, Progressive: false}

	b, err := os.ReadFile("jpeg_image.jpeg")
	if err != nil {
		log.Fatal(err.Error())
	}

	res, err := mozjpego.Compress(b, t)
	if err != nil {
		log.Fatal(err.Error())
	}

	if err := os.WriteFile("result.jpeg", res, 0644); err != nil {
		log.Fatal(err.Error())
	}
}
