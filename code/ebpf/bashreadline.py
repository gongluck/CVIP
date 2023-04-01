'''
:@Author: gongluck
:@Date: 3/31/2023, 10:29:21 PM
:@LastEditors: gongluck
:@LastEditTime: 3/31/2023, 10:57:46 PM
:Description: 
:Copyright: Copyright (©)}) 2023 gongluck93@gmail.com. All rights reserved.
'''

# 引入BCC
from bcc import BPF

# 加载eBPF程序
bpf = BPF(src_file="bashreadline.c")

# 挂载性能事件，监控bash的readline函数，调用自定义的bash_readline函数
bpf.attach_uretprobe(name="/usr/bin/bash", sym="readline",
                     fn_name="bash_readline")


# 自定义性能事件回调函数
def print_event(cpu, mydata, size):
    event = bpf["myevents"].event(mydata)
    print("%-6d %s" % (event.uid, event.command.decode("utf-8")))


print("%-6s %s" % ("UID", "COMMAND"))

# 绑定性能事件映射和输出函数，并从映射中循环读取数据
bpf["myevents"].open_perf_buffer(print_event)
while True:
    try:
        bpf.perf_buffer_poll()
    except KeyboardInterrupt:
        exit()
