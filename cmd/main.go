package main

import (
	"encoding/binary"
	"log"
	"os"

	"github.com/imager200/mozjpego"
)

func main() {
	t := mozjpego.Parameters{Quality: 10}

	b, err := os.ReadFile("building.jpeg")
	if err != nil {
		log.Fatal(err.Error())
	}

	res, err := mozjpego.Compress(b, t)
	if err != nil {
		log.Fatal(err.Error())
	}

	/* 	if err := os.WriteFile("result.jpeg", res, 0644); err != nil {
		log.Fatal(err.Error())
	} */

	file, err := os.OpenFile("result.jpeg", os.O_CREATE|os.O_RDWR|os.O_APPEND, 0644)
	if err != nil {
		log.Fatal(err.Error())
	}

	/* 	var data = []struct {
	   		n1 uint16
	   		n2 uint8
	   		n3 uint8
	   	}{
	   		{2, 5, 5},
	   		{2, 1, 6},
	   		{2, 5, 5},
	   		{2, 2, 4},
	   	}

	   	for _, chunk := range data {
	   		binary.Write(file, binary.LittleEndian, chunk)
	   	} */

	//buff := &bytes.Buffer{}

	binary.Write(file, binary.LittleEndian, res)

	//fmt.Println("www --- ", buff.Bytes)
}
