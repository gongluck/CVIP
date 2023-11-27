/*
 * @Author: gongluck
 * @Date: 2023-11-21 17:35:05
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-11-21 17:37:54
 */

package main

import "fmt"

func main() {
	array := [5]int{1, 2, 3, 4, 5}

	slice1 := array[1:3]
	slice2 := array[2:4]
	slice1[1] = 10

	// 底层共享同一个数组
	fmt.Println("array: ", array)
	fmt.Println("slice1: ", slice1)
	fmt.Println("slice2: ", slice2)
}
