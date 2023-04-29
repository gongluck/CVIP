#!/usr/bin/python3
from bcc import BPF

# 要在内核中执行的代码（字符串）
program = """
int hello_world() {
    bpf_trace_printk("Hello World  !\\n");
    return 0;
}
"""

b = BPF(text=program)

# 使用BCC辅助函数获取execve系统调用的函数名
syscall = b.get_syscall_fnname("execve")

# 挂载内核探针
b.attach_kprobe(event=syscall, fn_name="hello_world")

# 轮询输出
# /sys/kernel/debug/tracing/trace_pipe
b.trace_print()
