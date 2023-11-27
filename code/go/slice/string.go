/*
 * @Author: gongluck
 * @Date: 2023-11-21 17:09:10
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-11-21 17:46:02
 */

package main

import "fmt"

func main() {
	str := "Hello, World!"

	slice := []byte(str)

	slice[0] = 'h'

	fmt.Println("string:", str)
	fmt.Println("slice:", string(slice))
}
