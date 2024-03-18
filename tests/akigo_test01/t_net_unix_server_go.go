package main

import (
	"fmt"
	"net"
	"os"
	// "time"
)

func TryReadLine(buff []byte) (ret []byte, ok bool) {
	fmt.Println("searching new line")
	defer func() {
		if !ok {
			fmt.Println("not found")
		} else {
			fmt.Println("found")
		}
	}()
	for i, v := range buff {
		if v == '\n' {
			return buff[:i], true
		}
	}
	return buff[0:0], false
}

func ProcessLine(line []byte) {
	text := string(line)

	fmt.Println("S>: ", text)
}

func wkr(c *net.UnixConn) {
	defer func() {
		fmt.Println("wkr exit")
		c.Close()
	}()

	buff := []byte{}
	for {
		tb := make([]byte, 1024)

		// c.SetDeadline(time.Now().Add(time.Duration(time.Second)))

		actually_read, err := c.Read(tb)
		if err != nil {
			fmt.Println("conn read error:", err)
			break
		}

		for i := 0; i != actually_read; i++ {
			fmt.Printf("%d: %x ", i, tb[i])
		}

		fmt.Println()

		buff = append(buff, tb[:actually_read]...)

		for {
			line, ok := TryReadLine(buff)
			if !ok {
				fmt.Println("no ready to use line found")
				break
			}

			buff = buff[len(line):]

			for len(buff) != 0 && (buff[0] == 13 || buff[0] == 10) {
				buff = buff[1:]
			}

			ProcessLine(line)
		}
	}
}

func main() {

	uaddr := new(net.UnixAddr)

	uaddr.Net = "unix"
	uaddr.Name = "./test_unix_socket"

	os.Remove("./test_unix_socket")

	s, err := net.ListenUnix("unix", uaddr)
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}

	for {
		c, err := s.AcceptUnix()

		if err != nil {
			fmt.Println(err)
			os.Exit(1)
		}

		go wkr(c)

	}

	os.Exit(0)
}
