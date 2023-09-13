# 异常

- [异常](#异常)
  - [Go 异常定义](#go-异常定义)
  - [panic](#panic)

## Go 异常定义

Go异常是一种非预期的结果，发生异常可能表示程序中存在 BUG 或发生了其它不可控的问题。Go 语言推荐使用 recover 函数将内部异常转为错误处理，这使得用户可以真正的关心业务相关的错误处理。

Go 语言库的实现习惯: 即使在包内部使用了 panic，但是在导出函数时会被转化为明确的错误值。

## panic

panic 支持抛出任意类型的异常（而不仅仅是 error 类型的错误），recover 函数调用的返回值和 panic 函数的输入参数类型一致，它们的函数签名如下：

```go
func panic(interface{})
func recover() interface{}
```

Go 语言函数调用的正常流程是函数执行返回语句返回结果，在这个流程中是没有异常的，因此在这个流程中执行 recover 异常捕获函数始终是返回 nil。另一种是异常流程: 当函数调用 panic 抛出异常，函数将停止执行后续的普通语句，但是之前注册的 defer 函数调用仍然保证会被正常执行，然后再返回到调用者。对于当前函数的调用者，因为处理异常状态还没有被捕获，和直接调用 panic 函数的行为类似。在异常发生时，如果在 defer 中执行 recover 调用，它可以捕获触发 panic 时的参数，并且恢复到正常的执行流程。

在非 defer 语句中执行 recover 调用是初学者常犯的错误:

```go
func main() {
    if r := recover(); r != nil {
        log.Fatal(r)
    }

    panic(123)

    if r := recover(); r != nil {
        log.Fatal(r)
    }
}
```

上面程序中两个 recover 调用都不能捕获任何异常。在第一个 recover 调用执行时，函数必然是在正常的非异常执行流程中，这时候 recover 调用将返回 nil。发生异常时，第二个 recover 调用将没有机会被执行到，因为 panic 调用会导致函数马上执行已经注册 defer 的函数后返回。

其实 recover 函数调用有着更严格的要求：我们必须在 defer 函数中直接调用 recover。如果 defer 中调用的是 recover 函数的包装函数的话，异常的捕获工作将失败！比如，有时候我们可能希望包装自己的 MyRecover 函数，在内部增加必要的日志信息然后再调用 recover，这是错误的做法：

```go
func main() {
    defer func() {
        // 无法捕获异常
        if r := MyRecover(); r != nil {
            fmt.Println(r)
        }
    }()
    panic(1)
}

func MyRecover() interface{} {
    log.Println("trace...")
    return recover()
}
```

同样，如果是在嵌套的 defer 函数中调用 recover 也将导致无法捕获异常：

```go
func main() {
    defer func() {
        defer func() {
            // 无法捕获异常
            if r := recover(); r != nil {
                fmt.Println(r)
            }
        }()
    }()
    panic(1)
}
```

2 层嵌套的 defer 函数中直接调用 recover 和 1 层 defer 函数中调用包装的 MyRecover 函数一样，都是经过了 2 个函数帧才到达真正的 recover 函数，这个时候 Goroutine 的对应上一级栈帧中已经没有异常信息。

必须要和有异常的栈帧只隔一个栈帧，recover 函数才能正常捕获异常。换言之，recover 函数捕获的是祖父一级调用函数栈帧的异常（刚好可以跨越一层 defer 函数）！
