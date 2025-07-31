package main

import (
	"bufio"
	"fmt"
	"net"
	"os"
	"strings"
)

func main() {
	conn, err := net.Dial("tcp", "localhost:9000")
	if err != nil {
		fmt.Println("❌ Could not connect to server:", err)
		return
	}
	defer conn.Close()

	reader := bufio.NewReader(conn)
	input := bufio.NewReader(os.Stdin)

	go func() {
		// Background goroutine to print all server messages
		for {
			msg, err := reader.ReadString('\n')
			if err != nil {
				fmt.Println("❌ Disconnected from server.")
				os.Exit(0)
			}
			fmt.Print(msg)
		}
	}()

	for {
		text, _ := input.ReadString('\n')
		text = strings.TrimSpace(text)
		if text == "" {
			continue
		}

		// Send input to server
		_, err := fmt.Fprintf(conn, "%s\n", text)
		if err != nil {
			fmt.Println("❌ Failed to send data:", err)
			break
		}
	}
}
