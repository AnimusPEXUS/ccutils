package main

import (
	"fmt"
	"net"
	"os"
)

func wkr(c *net.UnixConn) {
	defer func() { c.Close() }()

	var err error
	var scanned_line string

	for {
		scanned_line = ""
		_, err = fmt.Scanln(&scanned_line)
		if err != nil {
			fmt.Println("C err: new line read error: ", err)
			return
		}

		fmt.Println("sending line: ", scanned_line)

		_, err := c.Write(([]byte)(scanned_line + "\n"))
		if err != nil {
			fmt.Println("C err: line sending error: ", err)
			return
		}

	}
}

func main() {

	uaddr := &net.UnixAddr{
		Net:  "unix",
		Name: "./test_unix_socket",
	}

	c, err := net.DialUnix("unix", nil, uaddr)
	if err != nil {
		os.Exit(1)
	}

	wkr(c)

	os.Exit(0)
}
