/*
 * @Author: gongluck
 * @Date: 2023-11-27 15:09:41
 * @Last Modified by: gongluck
 * @Last Modified time: 2023-11-27 15:11:51
 */

package main

import "fmt"

func main() {
	done := make(chan int, 0) // 带缓存的管道

	go func() {
		fmt.Println("你好, 世界")
		fmt.Println("开始发送")
		done <- 1
		fmt.Println("完成发送")
	}()

	fmt.Println("开始接收")
	<-done
	fmt.Println("完成接收")
}
