# 排序算法

- [排序算法](#排序算法)
  - [冒泡排序](#冒泡排序)
  - [选择排序](#选择排序)
  - [插入排序](#插入排序)
  - [归并排序](#归并排序)
  - [快速排序](#快速排序)
  - [桶排序](#桶排序)
  - [计数排序](#计数排序)
  - [基数排序](#基数排序)

![排序算法](https://github.com/gongluck/images/blob/main/data_structure_and_algorithm/sort/sort.png)

## 冒泡排序

![冒泡排序](https://github.com/gongluck/images/blob/main/data_structure_and_algorithm/sort/bubble.gif)

- 冒泡排序只会操作相邻的两个数据。每次冒泡操作都会对相邻的两个元素进行比较，看是否满足大小关系要求。如果不满足就让它俩互换。一次冒泡会让至少一个元素移动到它应该在的位置，重复`n`次，就完成了`n`个数据的排序工作。
- 冒泡过程还可以优化。当某次冒泡操作已经没有数据交换时，说明已经达到完全有序，不用再继续执行后续的冒泡操作。

## 选择排序

![选择排序](https://github.com/gongluck/images/blob/main/data_structure_and_algorithm/sort/select.gif)

- 将数组中的数据遍历，先拿第一个进行比较，看后面的有没有比这更小的，有的话交换，没有就第二个进行比较，依次比较，一共需要比数组大小-1 次
- 选择排序是一种不稳定的排序算法。选择排序每次都要找剩余未排序元素中的最小值，并和前面的元素交换位置，这样破坏了稳定性。

## 插入排序

![插入排序](https://github.com/gongluck/images/blob/main/data_structure_and_algorithm/sort/insert.gif)

- 将数组中的数据分为两个区间，已排序区间和未排序区间。初始已排序区间只有一个元素，就是数组的第一个元素。
- 插入算法的核心思想是取未排序区间中的元素，在已排序区间中找到合适的插入位置将其插入，并保证已排序区间数据一直有序。重复这个过程，直到未排序区间中元素为空，算法结束。

## 归并排序

![归并排序](https://github.com/gongluck/images/blob/main/data_structure_and_algorithm/sort/merge.gif)

- 把数组从中间分成前后两部分，然后对前后两部分分别排序，再将排好序的两部分合并在一起，这样整个数组就都有序了。
- 归并排序算法是一种在任何情况下时间复杂度都比较稳定的排序算法，这也使它存在致命的缺点，即归并排序不是原地排序算法，空间复杂度比较高，是`O(n)`。

## 快速排序

![快速排序](https://github.com/gongluck/images/blob/main/data_structure_and_algorithm/sort/quick.gif)

- 如果要排序数组中下标从 p 到 r 之间的一组数据，选择 p 到 r 之间的任意一个数据作为 pivot(分区点)。
- 遍历 p 到 r 之间的数据，将小于 pivot 的放到左边，将大于 pivot 的放到右边，将 pivot 放到中间。经过这一步骤之后，数组 p 到 r 之间的数据就被分成了三个部分，前面 p 到 q-1 之间都是小于 pivot 的，中间是 pivot，后面的 q+1 到 r 之间是大于 pivot 的。
- 快速排序算法虽然最坏情况下的时间复杂度是 O(n<sup>2</sup>)，但是平均情况下时间复杂度都是 O(nlogn)。

## 桶排序

![桶排序](https://github.com/gongluck/images/blob/main/data_structure_and_algorithm/sort/bucket.png)

- 将要排序的数据分到几个有序的桶里，每个桶里的数据再单独进行排序。桶内排完序之后，再把每个桶里的数据按照顺序依次取出，组成的序列就是有序的了。
- 桶排序比较适合用在外部排序中。所谓的外部排序就是数据存储在外部磁盘中，数据量比较大，内存有限，无法将数据全部加载到内存中。

## 计数排序

![计数排序](https://github.com/gongluck/images/blob/main/data_structure_and_algorithm/sort/count.png)

- 计数排序其实是桶排序的一种特殊情况。当要排序的 n 个数据，所处的范围并不大的时候，比如最大值是 k，就可以把数据划分成 k 个桶。每个桶内的数据值都是相同的，省掉了桶内排序的时间。
- 计数排序只能用在数据范围不大的场景中，如果数据范围 k 比要排序的数据 n 大很多，就不适合用计数排序了。而且，计数排序只能给非负整数排序，如果要排序的数据是其他类型的，要将其在不改变相对大小的情况下，转化为非负整数。

## 基数排序

![基数排序](https://github.com/gongluck/images/blob/main/data_structure_and_algorithm/sort/radix.gif)

- 基数排序对要排序的数据是有要求的，需要可以分割出独立的"位"来比较，而且位之间有递进的关系，如果 a 数据的高位比 b 数据大，那剩下的低位就不用比较了。
- 每一位的数据范围不能太大，要可以用线性排序算法来排序，否则，基数排序的时间复杂度就无法做到 O(n)了。
