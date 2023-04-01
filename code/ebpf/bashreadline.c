/**
 * @Author: gongluck
 * @Date: 3/31/2023, 10:29:38 PM
 * @LastEditors: gongluck
 * @LastEditTime: 3/31/2023, 10:57:22 PM
 * Description: 
 * Copyright: Copyright (©)}) 2023 gongluck93@gmail.com. All rights reserved.
 */

// usr/src/linux-source-5.15.0/include/
#include <uapi/linux/bpf.h>
#include <uapi/linux/ptrace.h>

// 自定义数据结构，传递给python
struct mydata
{
  u32 uid;          // user id
  char command[64]; // command line
};
// 定义一个 Perf 事件类型的 BPF 映射
BPF_PERF_OUTPUT(myevents);

// uretprobe 处理函数的定义格式应该为 `function_name(struct pt_regs *ctx)`，而返回值可以通过宏 `PT_REGS_RC(ctx)` 来获取
int bash_readline(struct pt_regs *ctx)
{
  struct mydata data = {};
  data.uid = bpf_get_current_uid_gid(); // 保存结果中的低32位uid

  // 从用户空间读取readline的返回值并保存
  bpf_probe_read_user(&data.command, sizeof(data.command), (void *)PT_REGS_RC(ctx));

  // 提交性能事件
  myevents.perf_submit(ctx, &data, sizeof(data));

  return 0;
}
