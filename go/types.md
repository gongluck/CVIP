# 数据类型

- [数据类型](#数据类型)
	- [数组](#数组)
	- [切片](#切片)

## 数组

```go
//创建数组
// NewArray returns a new fixed-length array Type.
func NewArray(elem *Type, bound int64) *Type {
	if bound < 0 { //判断数组长度合法性
		Fatalf("NewArray: invalid bound %v", bound)
	}
	//创建内部Type类型
	t := New(TARRAY)
	//额外字段指向数组实体
	t.Extra = &Array{Elem: elem, Bound: bound}
	t.SetNotInHeap(elem.NotInHeap())
	return t
}

//数组数据的实体
// Array contains Type fields specific to array types.
type Array struct {
	Elem  *Type // element type
	Bound int64 // number of elements; <0 if unknown yet
}
```

## 切片

```go
//创建切片
// NewSlice returns the slice Type with element type elem.
func NewSlice(elem *Type) *Type {
	//缓存
	if t := elem.Cache.slice; t != nil {
		if t.Elem() != elem {
			Fatalf("elem mismatch")
		}
		return t
	}

	//创建内部Type类型
	t := New(TSLICE)
	//额外字段指向切片实体
	t.Extra = Slice{Elem: elem}
	//缓存
	elem.Cache.slice = t
	return t
}

//切片数据实体
// Slice contains Type fields specific to slice types.
type Slice struct {
	Elem *Type // element type
}

//运行时切片可以转换成这个结构
// SliceHeader is the runtime representation of a slice.
// It cannot be used safely or portably and its representation may
// change in a later release.
// Moreover, the Data field is not sufficient to guarantee the data
// it references will not be garbage collected, so programs must keep
// a separate, correctly typed pointer to the underlying data.
type SliceHeader struct {
	Data uintptr //数组
	Len  int     //切片长度
	Cap  int     //数组容量
}
```