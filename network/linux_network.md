# Linux网络

- [Linux网络](#linux网络)
	- [命令配置](#命令配置)
	- [Linux网络工作图示](#linux网络工作图示)
		- [TCP状态轮转](#tcp状态轮转)
		- [网络收包](#网络收包)
	- [源码](#源码)
		- [用户层接口](#用户层接口)
		- [网络模块初始化和启动](#网络模块初始化和启动)
		- [硬中断处理](#硬中断处理)
		- [软中断处理](#软中断处理)
		- [TCP协议处理函数](#tcp协议处理函数)

## 命令配置

  <details>
  <summary>RingBuffer</summary>

  ```shell
  # ethtool -g ens33
  Ring parameters for ens33:
  Pre-set maximums:
  RX:             4096
  RX Mini:        0
  RX Jumbo:       0
  TX:             4096
  Current hardware settings:
  RX:             256
  RX Mini:        0
  RX Jumbo:       0
  TX:             256
  # ethtool -S ens33
  NIC statistics:
      rx_packets: 53462
      tx_packets: 16941
      rx_bytes: 61247636
      tx_bytes: 2280327
      rx_broadcast: 0
      tx_broadcast: 0
      rx_multicast: 0
      tx_multicast: 0
      rx_errors: 0
      tx_errors: 0
      tx_dropped: 0
      multicast: 0
      collisions: 0
      rx_length_errors: 0
      rx_over_errors: 0
      rx_crc_errors: 0
      rx_frame_errors: 0
      rx_no_buffer_count: 0
      rx_missed_errors: 0
      tx_aborted_errors: 0
      tx_carrier_errors: 0
      tx_fifo_errors: 0
      tx_heartbeat_errors: 0
      tx_window_errors: 0
      tx_abort_late_coll: 0
      tx_deferred_ok: 0
      tx_single_coll_ok: 0
      tx_multi_coll_ok: 0
      tx_timeout_count: 0
      tx_restart_queue: 0
      rx_long_length_errors: 0
      rx_short_length_errors: 0
      rx_align_errors: 0
      tx_tcp_seg_good: 158
      tx_tcp_seg_failed: 0
      rx_flow_control_xon: 0
      rx_flow_control_xoff: 0
      tx_flow_control_xon: 0
      tx_flow_control_xoff: 0
      rx_long_byte_count: 61247636
      rx_csum_offload_good: 49426
      rx_csum_offload_errors: 0
      alloc_rx_buff_failed: 0
      tx_smbus: 0
      rx_smbus: 0
      dropped_smbus: 0
  #修改RingBuffer
  # ethtool -G ens33 rx 64 tx 64
  ```
  </details>

  <details>
  <summary>软中断统计</summary>

  ```shell
  # cat /proc/softirqs
                      CPU0       CPU1
            HI:          0          1
        TIMER:      53948     158070
        NET_TX:        958          2
        NET_RX:      36133        100
        BLOCK:       4371       9380
      IRQ_POLL:          0          0
      TASKLET:       1352        393
        SCHED:      51704     155414
      HRTIMER:          0          0
          RCU:      67739     116630
  ```
  </details>

  <details>
  <summary>网卡队列</summary>

  ```shell
  # ethtool -l eth0
  Channel parameters for eth0:
  Pre-set maximums:
  RX:             0
  TX:             0
  Other:          0
  Combined:       1
  Current hardware settings:
  RX:             0
  TX:             0
  Other:          0
  Combined:       1
  #修改网卡队列数
  # ethtool -L eth0 combined 1
  #查看网卡队列中断号
  # cat /proc/interrupts | grep ens33
    19:      39793         29   IO-APIC   19-fasteoi   ens33
  #查看中断与CPU的亲和性
  # cat /proc/irq/19/smp_affinity
  00000000,00000000,00000000,00000001
  #自动调整中断与CPU的亲和性
  # service irqbalance start
  ```
  </details>

  <details>
  <summary>硬中断合并</summary>

  ```shell
  #查看硬中断合并信息
  # ethtool -c ens33
  Coalesce parameters for ens33:
  Adaptive RX: off  TX: off   #自适应硬中断合并
  stats-block-usecs: 0
  sample-interval: 0
  pkt-rate-low: 0
  pkt-rate-high: 0

  rx-usecs: 3   #超时产生RX中断
  rx-frames: 0  #产生RX中断的触发帧数
  rx-usecs-irq: 0
  rx-frames-irq: 0

  tx-usecs: 0
  tx-frames: 0
  tx-usecs-irq: 0
  tx-frames-irq: 0

  rx-usecs-low: 0
  rx-frame-low: 0
  tx-usecs-low: 0
  tx-frame-low: 0

  rx-usecs-high: 0
  rx-frame-high: 0
  tx-usecs-high: 0
  tx-frame-high: 0
  # 修改硬中断合并配置
  #ethtool -C ens33 adaptive-rx on
  ```
  </details>

  <details>
  <summary>接收/发送数据合并LGO/LRO/TSO/GSO</summary>

  ```shell
  # ethtool -k ens33
  Features for ens33:
  rx-checksumming: off
  tx-checksumming: on
          tx-checksum-ipv4: off [fixed]
          tx-checksum-ip-generic: on
          tx-checksum-ipv6: off [fixed]
          tx-checksum-fcoe-crc: off [fixed]
          tx-checksum-sctp: off [fixed]
  scatter-gather: on
          tx-scatter-gather: on
          tx-scatter-gather-fraglist: off [fixed]
  tcp-segmentation-offload: on        #网卡TSO
          tx-tcp-segmentation: on
          tx-tcp-ecn-segmentation: off [fixed]
          tx-tcp-mangleid-segmentation: off
          tx-tcp6-segmentation: off [fixed]
  udp-fragmentation-offload: off      #网卡USO
  generic-segmentation-offload: on    #内核GSO
  generic-receive-offload: on         #网卡GRO
  large-receive-offload: off [fixed]  #内核LRO
  rx-vlan-offload: on
  tx-vlan-offload: on [fixed]
  ntuple-filters: off [fixed]
  receive-hashing: off [fixed]
  highdma: off [fixed]
  rx-vlan-filter: on [fixed]
  vlan-challenged: off [fixed]
  tx-lockless: off [fixed]
  netns-local: off [fixed]
  tx-gso-robust: off [fixed]
  tx-fcoe-segmentation: off [fixed]
  tx-gre-segmentation: off [fixed]
  tx-gre-csum-segmentation: off [fixed]
  tx-ipxip4-segmentation: off [fixed]
  tx-ipxip6-segmentation: off [fixed]
  tx-udp_tnl-segmentation: off [fixed]
  tx-udp_tnl-csum-segmentation: off [fixed]
  tx-gso-partial: off [fixed]
  tx-sctp-segmentation: off [fixed]
  tx-esp-segmentation: off [fixed]
  fcoe-mtu: off [fixed]
  tx-nocache-copy: off
  loopback: off [fixed]
  rx-fcs: off
  rx-all: off
  tx-vlan-stag-hw-insert: off [fixed]
  rx-vlan-stag-hw-parse: off [fixed]
  rx-vlan-stag-filter: off [fixed]
  l2-fwd-offload: off [fixed]
  hw-tc-offload: off [fixed]
  esp-hw-offload: off [fixed]
  esp-tx-csum-hw-offload: off [fixed]
  rx-udp_tunnel-port-offload: off [fixed]
  #打开GRO/LRO/TSO/USO/GSO
  # ethtool -K ens33 gro on
  # ethtool -K ens33 lro on
  # ethtool -K ens33 tso on
  # ethtool -K ens33 uso on
  # ethtool -K ens33 gso on
  ```
  </details>

  <details>
  <summary>网络状态</summary>

  ```shell
  # netstat -s
  Ip:
      6080495019 total packets received
      0 forwarded
      0 incoming packets discarded
      6080494931 incoming packets delivered
      3716502972 requests sent out
      31 outgoing packets dropped
      33 dropped because of missing route
  Icmp:
      9310453 ICMP messages received
      313208 input ICMP message failed.
      InCsumErrors: 1793
      ICMP input histogram:
          destination unreachable: 302809
          timeout in transit: 10815
          echo requests: 8994768
          echo replies: 55
          timestamp request: 194
          timestamp reply: 1
      8997498 ICMP messages sent
      0 ICMP messages failed
      ICMP output histogram:
          destination unreachable: 2305
          echo request: 231
          echo replies: 8994768
          timestamp replies: 194
  IcmpMsg:
          InType0: 55
          InType3: 302809
          InType8: 8994768
          InType11: 10815
          InType13: 194
          InType14: 1
          InType194: 3
          InType200: 2
          InType201: 1
          InType207: 1
          InType211: 1
          InType215: 2
          InType221: 3
          InType239: 2
          InType250: 3
          OutType0: 8994768
  IcmpMsg:
          OutType3: 2305
          OutType8: 231
          OutType14: 194
  Tcp:
      383132 active connections openings
      3842476 passive connection openings
      11577 failed connection attempts
      148545 connection resets received
      2 connections established
      6070263737 segments received
      13923780696 segments send out
      13485259 segments retransmited
      53 bad segments received.
      430104 resets sent
      InCsumErrors: 4
  Udp:
      937126 packets received
      816 packets to unknown port received.
      0 packet receive errors
      963738 packets sent
      0 receive buffer errors
      0 send buffer errors
  UdpLite:
  TcpExt:
      467 SYN cookies sent
      442 SYN cookies received
      99540 invalid SYN cookies received
      2302 resets received for embryonic SYN_RECV sockets
      199228 packets pruned from receive queue because of socket buffer overrun
      2543 packets pruned from receive queue
      6 packets dropped from out-of-order queue because of socket buffer overrun
      182 ICMP packets dropped because they were out-of-window
      1 ICMP packets dropped because socket was locked
      2569510 TCP sockets finished time wait in fast timer
      147988 packets rejects in established connections because of timestamp
      3892967 delayed acks sent
      16827 delayed acks further delayed because of locked socket
      Quick ack mode was activated 15420549 times
      1307 times the listen queue of a socket overflowed  #全队列溢出 watch 'netstat -s | grep overflowed'
      1774 SYNs to LISTEN sockets dropped
      229845 packets directly queued to recvmsg prequeue.
      11114478 bytes directly in process context from backlog
      1904317231 bytes directly received in process context from prequeue
      4604190546 packet headers predicted
      199426 packets header predicted and directly queued to user
      745823000 acknowledgments not containing data payload received
      51773658 predicted acknowledgments
      943677 times recovered from packet loss by selective acknowledgements
      Detected reordering 6896 times using FACK
      Detected reordering 9355 times using SACK
      Detected reordering 20267 times using time stamp
      1261 congestion windows fully recovered without slow start
      19725 congestion windows partially recovered using Hoe heuristic
      3420 congestion windows recovered without slow start by DSACK
      18754 congestion windows recovered without slow start after partial ack
      TCPLostRetransmit: 552617
      13157 timeouts after SACK recovery
      4697 timeouts in loss state
      12526638 fast retransmits
      152042 forward retransmits
      323419 retransmits in slow start
      307499 other TCP timeouts
      TCPLossProbes: 148267
      TCPLossProbeRecovery: 51046
      44252 SACK retransmits failed
      12 times receiver scheduled too late for direct processing
      34364460 packets collapsed in receive queue due to low socket buffer
      15439143 DSACKs sent for old packets
      8545 DSACKs sent for out of order packets
      82525 DSACKs received
      553 DSACKs for out of order packets received
      160410 connections reset due to unexpected data
      122314 connections reset due to early user close
      1492 connections aborted due to timeout
      TCPDSACKIgnoredOld: 442
      TCPDSACKIgnoredNoUndo: 62055
      TCPSpuriousRTOs: 10619
      TCPSackShifted: 3614009
      TCPSackMerged: 10642494
      TCPSackShiftFallback: 5677308
      TCPBacklogDrop: 2889
      TCPReqQFullDoCookies: 471
      TCPRcvCoalesce: 2838800354
      TCPOFOQueue: 230785818
      TCPOFODrop: 2474
      TCPOFOMerge: 7846
      TCPChallengeACK: 194
      TCPSYNChallenge: 127
      TCPSpuriousRtxHostQueues: 202
      TCPAutoCorking: 5477
      TCPFromZeroWindowAdv: 1146832
      TCPToZeroWindowAdv: 1146841
      TCPWantZeroWindowAdv: 1208834
      TCPSynRetrans: 290545
      TCPOrigDataSent: 10606765627
      TCPHystartTrainDetect: 97229
      TCPHystartTrainCwnd: 2761685
      TCPHystartDelayDetect: 3434
      TCPHystartDelayCwnd: 666590
      TCPACKSkippedSynRecv: 413
      TCPACKSkippedPAWS: 30
      TCPACKSkippedSeq: 87322
      TCPACKSkippedFinWait2: 6
      TCPACKSkippedTimeWait: 7641
      TCPACKSkippedChallenge: 55
  IpExt:
      InNoRoutes: 24
      InMcastPkts: 9
      OutMcastPkts: 9
      InBcastPkts: 90
      OutBcastPkts: 85
      InOctets: 30446247020205
      OutOctets: 15505627659658
      InMcastOctets: 288
      OutMcastOctets: 288
      InBcastOctets: 5600
      OutBcastOctets: 2720
      InNoECTPkts: 6081995067
      InECT1Pkts: 2
      InECT0Pkts: 495
      InCEPkts: 5
  ```
  </details>

## Linux网络工作图示

### TCP状态轮转

![tcp_state](https://github.com/gongluck/images/blob/main/Network/tcp_state.png)

### 网络收包

![network_recvpack](https://github.com/gongluck/images/blob/main/Network/network_recvpack.png)

![ksoftirqd_recvpack](https://github.com/gongluck/images/blob/main/Network/ksoftirqd_recvpack.png)

## 源码

### 用户层接口

<details>
<summary>用户层接口</summary>

```C++
// Server
socket(...,SOCK_STREAM,0);
bind(...,&server_address, ...);
listen(...);
accept(..., &client_address, ...);
recv(..., &clientaddr, ...);
close(...);

//client
socket(...,SOCK_STREAM,0);
connect();
send(...,&server_address,...);
```
</details>

### 网络模块初始化和启动

<details>
<summary>kernel/softirq.c</summary>

```C++
/* PLEASE, avoid to allocate new softirqs, if you need not _really_ high
   frequency threaded job scheduling. For almost all the purposes
   tasklets are more than enough. F.e. all serial device BHs et
   al. should be converted to tasklets, not to softirqs.
 */
enum
{
	HI_SOFTIRQ=0,
	TIMER_SOFTIRQ,
	NET_TX_SOFTIRQ,
	NET_RX_SOFTIRQ,
	BLOCK_SOFTIRQ,
	BLOCK_IOPOLL_SOFTIRQ,
	TASKLET_SOFTIRQ,
	SCHED_SOFTIRQ,
	HRTIMER_SOFTIRQ,
	RCU_SOFTIRQ,    /* Preferable RCU should always be the last softirq */

	NR_SOFTIRQS
};
#define SOFTIRQ_STOP_IDLE_MASK (~(1 << RCU_SOFTIRQ))

//中断向量
static struct softirq_action softirq_vec[NR_SOFTIRQS] __cacheline_aligned_in_smp;
char *softirq_to_name[NR_SOFTIRQS] = {
		"HI", "TIMER", "NET_TX", "NET_RX", "BLOCK", "BLOCK_IOPOLL",
		"TASKLET", "SCHED", "HRTIMER", "RCU"};
//注册软中断处理函数
void open_softirq(int nr, void (*action)(struct softirq_action *))
{
	softirq_vec[nr].action = action;
}

static int ksoftirqd_should_run(unsigned int cpu)
{
	return local_softirq_pending();
}
static void run_ksoftirqd(unsigned int cpu)
{
	local_irq_disable();
	if (local_softirq_pending()) //__softirq_pending
	{
		//处理软中断
		__do_softirq();
		rcu_note_context_switch(cpu);
		local_irq_enable();
		cond_resched();
		return;
	}
	local_irq_enable();
}
//软中断线程相关信息
static struct smp_hotplug_thread softirq_threads = {
		.store = &ksoftirqd,
		.thread_should_run = ksoftirqd_should_run, //判断线程变量__softirq_pending是否置位
		.thread_fn = run_ksoftirqd,
		.thread_comm = "ksoftirqd/%u", // ps -ef | grep ksoftirqd
};
//创建ksoftirqd线程
static __init int spawn_ksoftirqd(void)
{
	register_cpu_notifier(&cpu_nfb);

	//为每个CPU注册关联的线程
	BUG_ON(smpboot_register_percpu_thread(&softirq_threads));

	return 0;
}
early_initcall(spawn_ksoftirqd);
```
</details>

<details>
<summary>net/core/dev.c</summary>

```C++
//协议处理列表
struct list_head ptype_base[PTYPE_HASH_SIZE] __read_mostly;
struct list_head ptype_all __read_mostly; /* Taps */
static struct list_head offload_base __read_mostly;
//获取协议处理列表的表头
static inline struct list_head *ptype_head(const struct packet_type *pt)
{
	if (pt->type == htons(ETH_P_ALL))
		return &ptype_all;
	else
		return &ptype_base[ntohs(pt->type) & PTYPE_HASH_MASK];
}
/**
 *	dev_add_pack - add packet handler
 *	@pt: packet type declaration
 *
 *	Add a protocol handler to the networking stack. The passed &packet_type
 *	is linked into kernel lists and may not be freed until it has been
 *	removed from the kernel lists.
 *
 *	This call does not sleep therefore it can not
 *	guarantee all CPU's that are in middle of receiving packets
 *	will see the new packet type (until the next received packet).
 */
void dev_add_pack(struct packet_type *pt)
{
	//获取协议处理列表的表头
	struct list_head *head = ptype_head(pt);

	spin_lock(&ptype_lock);
	list_add_rcu(&pt->list, head);
	spin_unlock(&ptype_lock);
}
EXPORT_SYMBOL(dev_add_pack);

/*
 *	Initialize the DEV module. At boot time this walks the device list and
 *	unhooks any devices that fail to initialise (normally hardware not
 *	present) and leaves us with a valid list of present and active devices.
 *
 */

/*
 *       This is called single threaded during boot, so no need
 *       to take the rtnl semaphore.
 */
//初始化网络模块
static int __init net_dev_init(void)
{
	int i, rc = -ENOMEM;

	BUG_ON(!dev_boot_phase);

	if (dev_proc_init())
		goto out;

	if (netdev_kobject_init())
		goto out;

	INIT_LIST_HEAD(&ptype_all);
	for (i = 0; i < PTYPE_HASH_SIZE; i++)
		INIT_LIST_HEAD(&ptype_base[i]);

	INIT_LIST_HEAD(&offload_base);

	if (register_pernet_subsys(&netdev_net_ops))
		goto out;

	/*
	 *	Initialise the packet receive queues.
	 */

	for_each_possible_cpu(i)
	{
		//为每个CPU初始化softnet_data
		struct softnet_data *sd = &per_cpu(softnet_data, i);
		memset(sd, 0, sizeof(*sd));
		skb_queue_head_init(&sd->input_pkt_queue);
		skb_queue_head_init(&sd->process_queue);
		sd->completion_queue = NULL;
		INIT_LIST_HEAD(&sd->poll_list); //初始化poll回调函数队列
		sd->output_queue = NULL;
		sd->output_queue_tailp = &sd->output_queue;
#ifdef CONFIG_RPS
		sd->csd.func = rps_trigger_softirq;
		sd->csd.info = sd;
		sd->csd.flags = 0;
		sd->cpu = i;
#endif

		sd->backlog.poll = process_backlog;
		sd->backlog.weight = weight_p;
		sd->backlog.gro_list = NULL;
		sd->backlog.gro_count = 0;
	}

	dev_boot_phase = 0;

	/* The loopback device is special if any other network devices
	 * is present in a network namespace the loopback device must
	 * be present. Since we now dynamically allocate and free the
	 * loopback device ensure this invariant is maintained by
	 * keeping the loopback device as the first device on the
	 * list of network devices.  Ensuring the loopback devices
	 * is the first device that appears and the last network device
	 * that disappears.
	 */
	if (register_pernet_device(&loopback_net_ops))
		goto out;

	if (register_pernet_device(&default_device_ops))
		goto out;

	//设置软中断处理函数
	open_softirq(NET_TX_SOFTIRQ, net_tx_action);
	open_softirq(NET_RX_SOFTIRQ, net_rx_action);

	hotcpu_notifier(dev_cpu_callback, 0);
	dst_init();
	rc = 0;
out:
	return rc;
}

//网络子系统初始化
subsys_initcall(net_dev_init);
```
</details>

<details>
<summary>net/ipv4/protocol.c</summary>

```C++
//网络协议处理函数数组
const struct net_protocol __rcu *inet_protos[MAX_INET_PROTOS] __read_mostly;
const struct net_offload __rcu *inet_offloads[MAX_INET_PROTOS] __read_mostly;

/*
 *	Add a protocol handler to the hash tables
 */
//注册协议处理函数
int inet_add_protocol(const struct net_protocol *prot, unsigned char protocol)
{
	if (!prot->netns_ok)
	{
		pr_err("Protocol %u is not namespace aware, cannot register.\n",
					 protocol);
		return -EINVAL;
	}

	//将协议处理函数注册到inet_protos数组中
	return !cmpxchg((const struct net_protocol **)&inet_protos[protocol],
									NULL, prot)
						 ? 0
						 : -1;
}
EXPORT_SYMBOL(inet_add_protocol);
```
</details>

<details>
<summary>net/ipv4/af_inet.c</summary>

```C++
// IP协议处理函数
static struct packet_type ip_packet_type __read_mostly = {
		.type = cpu_to_be16(ETH_P_IP),
		.func = ip_rcv,
};
// TCP协议处理函数
static const struct net_protocol tcp_protocol = {
		.early_demux = tcp_v4_early_demux,
		.handler = tcp_v4_rcv,
		.err_handler = tcp_v4_err,
		.no_policy = 1,
		.netns_ok = 1,
};
// UDP协议处理函数
static const struct net_protocol udp_protocol = {
		.handler = udp_rcv,
		.err_handler = udp_err,
		.no_policy = 1,
		.netns_ok = 1,
};
//注册网络协议
static int __init inet_init(void)
{
	struct inet_protosw *q;
	struct list_head *r;
	int rc = -EINVAL;

	BUILD_BUG_ON(sizeof(struct inet_skb_parm) > FIELD_SIZEOF(struct sk_buff, cb));

	sysctl_local_reserved_ports = kzalloc(65536 / 8, GFP_KERNEL);
	if (!sysctl_local_reserved_ports)
		goto out;

	rc = proto_register(&tcp_prot, 1);
	if (rc)
		goto out_free_reserved_ports;

	rc = proto_register(&udp_prot, 1);
	if (rc)
		goto out_unregister_tcp_proto;

	rc = proto_register(&raw_prot, 1);
	if (rc)
		goto out_unregister_udp_proto;

	rc = proto_register(&ping_prot, 1);
	if (rc)
		goto out_unregister_raw_proto;

	/*
	 *	Tell SOCKET that we are alive...
	 */

	(void)sock_register(&inet_family_ops);

#ifdef CONFIG_SYSCTL
	ip_static_sysctl_init();
#endif

	tcp_prot.sysctl_mem = init_net.ipv4.sysctl_tcp_mem;

	/*
	 *	Add all the base protocols.
	 */

	if (inet_add_protocol(&icmp_protocol, IPPROTO_ICMP) < 0)
		pr_crit("%s: Cannot add ICMP protocol\n", __func__);
	//注册UDP协议
	if (inet_add_protocol(&udp_protocol, IPPROTO_UDP) < 0)
		pr_crit("%s: Cannot add UDP protocol\n", __func__);
	//注册TCP协议
	if (inet_add_protocol(&tcp_protocol, IPPROTO_TCP) < 0)
		pr_crit("%s: Cannot add TCP protocol\n", __func__);
#ifdef CONFIG_IP_MULTICAST
	if (inet_add_protocol(&igmp_protocol, IPPROTO_IGMP) < 0)
		pr_crit("%s: Cannot add IGMP protocol\n", __func__);
#endif

	/* Register the socket-side information for inet_create. */
	for (r = &inetsw[0]; r < &inetsw[SOCK_MAX]; ++r)
		INIT_LIST_HEAD(r);

	for (q = inetsw_array; q < &inetsw_array[INETSW_ARRAY_LEN]; ++q)
		inet_register_protosw(q);

	/*
	 *	Set the ARP module up
	 */

	arp_init();

	/*
	 *	Set the IP module up
	 */

	ip_init();

	tcp_v4_init();

	/* Setup TCP slab cache for open requests. */
	tcp_init();

	/* Setup UDP memory threshold */
	udp_init();

	/* Add UDP-Lite (RFC 3828) */
	udplite4_register();

	ping_init();

	/*
	 *	Set the ICMP layer up
	 */

	if (icmp_init() < 0)
		panic("Failed to create the ICMP control socket.\n");

		/*
		 *	Initialise the multicast router
		 */
#if defined(CONFIG_IP_MROUTE)
	if (ip_mr_init())
		pr_crit("%s: Cannot init ipv4 mroute\n", __func__);
#endif
	/*
	 *	Initialise per-cpu ipv4 mibs
	 */

	if (init_ipv4_mibs())
		pr_crit("%s: Cannot init ipv4 mibs\n", __func__);

	ipv4_proc_init();

	ipfrag_init();

	//注册IP协议
	dev_add_pack(&ip_packet_type);

	rc = 0;
out:
	return rc;
out_unregister_raw_proto:
	proto_unregister(&raw_prot);
out_unregister_udp_proto:
	proto_unregister(&udp_prot);
out_unregister_tcp_proto:
	proto_unregister(&tcp_prot);
out_free_reserved_ports:
	kfree(sysctl_local_reserved_ports);
	goto out;
}

//注册网络协议
fs_initcall(inet_init);
```
</details>

<details>
<summary>drivers/net/ethernet/intel/igb/igb_main.c</summary>

```C++
// igb网卡驱动实现函数
static const struct net_device_ops igb_netdev_ops = {
		.ndo_open = igb_open, //启用网卡
		.ndo_stop = igb_close,
		.ndo_start_xmit = igb_xmit_frame,
		.ndo_get_stats64 = igb_get_stats64,
		.ndo_set_rx_mode = igb_set_rx_mode,
		.ndo_set_mac_address = igb_set_mac,
		.ndo_change_mtu = igb_change_mtu,
		.ndo_do_ioctl = igb_ioctl,
		.ndo_tx_timeout = igb_tx_timeout,
		.ndo_validate_addr = eth_validate_addr,
		.ndo_vlan_rx_add_vid = igb_vlan_rx_add_vid,
		.ndo_vlan_rx_kill_vid = igb_vlan_rx_kill_vid,
		.ndo_set_vf_mac = igb_ndo_set_vf_mac,
		.ndo_set_vf_vlan = igb_ndo_set_vf_vlan,
		.ndo_set_vf_tx_rate = igb_ndo_set_vf_bw,
		.ndo_set_vf_spoofchk = igb_ndo_set_vf_spoofchk,
		.ndo_get_vf_config = igb_ndo_get_vf_config,
#ifdef CONFIG_NET_POLL_CONTROLLER
		.ndo_poll_controller = igb_netpoll,
#endif
		.ndo_fix_features = igb_fix_features,
		.ndo_set_features = igb_set_features,
};
/**
 *  igb_probe - Device Initialization Routine
 *  @pdev: PCI device information struct
 *  @ent: entry in igb_pci_tbl
 *
 *  Returns 0 on success, negative on failure
 *
 *  igb_probe initializes an adapter identified by a pci_dev structure.
 *  The OS initialization, configuring of the adapter private structure,
 *  and a hardware reset occur.
 **/
static int igb_probe(struct pci_dev *pdev, const struct pci_device_id *ent)
{
	struct net_device *netdev;
	struct igb_adapter *adapter;
	struct e1000_hw *hw;
	u16 eeprom_data = 0;
	s32 ret_val;
	static int global_quad_port_a; /* global quad port a indication */
	const struct e1000_info *ei = igb_info_tbl[ent->driver_data];
	unsigned long mmio_start, mmio_len;
	int err, pci_using_dac;
	u8 part_str[E1000_PBANUM_LENGTH];

	/* Catch broken hardware that put the wrong VF device ID in
	 * the PCIe SR-IOV capability.
	 */
	if (pdev->is_virtfn)
	{
		WARN(1, KERN_ERR "%s (%hx:%hx) should not be a VF!\n",
				 pci_name(pdev), pdev->vendor, pdev->device);
		return -EINVAL;
	}

	err = pci_enable_device_mem(pdev);
	if (err)
		return err;

	pci_using_dac = 0;
	err = dma_set_mask(&pdev->dev, DMA_BIT_MASK(64));
	if (!err)
	{
		err = dma_set_coherent_mask(&pdev->dev, DMA_BIT_MASK(64));
		if (!err)
			pci_using_dac = 1;
	}
	else
	{
		err = dma_set_mask(&pdev->dev, DMA_BIT_MASK(32));
		if (err)
		{
			err = dma_set_coherent_mask(&pdev->dev,
																	DMA_BIT_MASK(32));
			if (err)
			{
				dev_err(&pdev->dev,
								"No usable DMA configuration, aborting\n");
				goto err_dma;
			}
		}
	}

	err = pci_request_selected_regions(pdev, pci_select_bars(pdev, IORESOURCE_MEM),
																		 igb_driver_name);
	if (err)
		goto err_pci_reg;

	pci_enable_pcie_error_reporting(pdev);

	pci_set_master(pdev);
	pci_save_state(pdev);

	err = -ENOMEM;
	netdev = alloc_etherdev_mq(sizeof(struct igb_adapter),
														 IGB_MAX_TX_QUEUES);
	if (!netdev)
		goto err_alloc_etherdev;

	SET_NETDEV_DEV(netdev, &pdev->dev);

	pci_set_drvdata(pdev, netdev);
	adapter = netdev_priv(netdev);
	adapter->netdev = netdev;
	adapter->pdev = pdev;
	hw = &adapter->hw;
	hw->back = adapter;
	adapter->msg_enable = netif_msg_init(debug, DEFAULT_MSG_ENABLE);

	mmio_start = pci_resource_start(pdev, 0);
	mmio_len = pci_resource_len(pdev, 0);

	err = -EIO;
	hw->hw_addr = ioremap(mmio_start, mmio_len);
	if (!hw->hw_addr)
		goto err_ioremap;

	//注册网卡驱动实现函数
	netdev->netdev_ops = &igb_netdev_ops;
	igb_set_ethtool_ops(netdev);
	netdev->watchdog_timeo = 5 * HZ;

	strncpy(netdev->name, pci_name(pdev), sizeof(netdev->name) - 1);

	netdev->mem_start = mmio_start;
	netdev->mem_end = mmio_start + mmio_len;

	/* PCI config space info */
	hw->vendor_id = pdev->vendor;
	hw->device_id = pdev->device;
	hw->revision_id = pdev->revision;
	hw->subsystem_vendor_id = pdev->subsystem_vendor;
	hw->subsystem_device_id = pdev->subsystem_device;

	/* Copy the default MAC, PHY and NVM function pointers */
	memcpy(&hw->mac.ops, ei->mac_ops, sizeof(hw->mac.ops));
	memcpy(&hw->phy.ops, ei->phy_ops, sizeof(hw->phy.ops));
	memcpy(&hw->nvm.ops, ei->nvm_ops, sizeof(hw->nvm.ops));
	/* Initialize skew-specific constants */
	err = ei->get_invariants(hw);
	if (err)
		goto err_sw_init;

	/* setup the private structure */
	err = igb_sw_init(adapter);
	if (err)
		goto err_sw_init;

	igb_get_bus_info_pcie(hw);

	hw->phy.autoneg_wait_to_complete = false;

	/* Copper options */
	if (hw->phy.media_type == e1000_media_type_copper)
	{
		hw->phy.mdix = AUTO_ALL_MODES;
		hw->phy.disable_polarity_correction = false;
		hw->phy.ms_type = e1000_ms_hw_default;
	}

	if (igb_check_reset_block(hw))
		dev_info(&pdev->dev,
						 "PHY reset is blocked due to SOL/IDER session.\n");

	/* features is initialized to 0 in allocation, it might have bits
	 * set by igb_sw_init so we should use an or instead of an
	 * assignment.
	 */
	netdev->features |= NETIF_F_SG |
											NETIF_F_IP_CSUM |
											NETIF_F_IPV6_CSUM |
											NETIF_F_TSO |
											NETIF_F_TSO6 |
											NETIF_F_RXHASH |
											NETIF_F_RXCSUM |
											NETIF_F_HW_VLAN_CTAG_RX |
											NETIF_F_HW_VLAN_CTAG_TX;

	/* copy netdev features into list of user selectable features */
	netdev->hw_features |= netdev->features;
	netdev->hw_features |= NETIF_F_RXALL;

	/* set this bit last since it cannot be part of hw_features */
	netdev->features |= NETIF_F_HW_VLAN_CTAG_FILTER;

	netdev->vlan_features |= NETIF_F_TSO |
													 NETIF_F_TSO6 |
													 NETIF_F_IP_CSUM |
													 NETIF_F_IPV6_CSUM |
													 NETIF_F_SG;

	netdev->priv_flags |= IFF_SUPP_NOFCS;

	if (pci_using_dac)
	{
		netdev->features |= NETIF_F_HIGHDMA;
		netdev->vlan_features |= NETIF_F_HIGHDMA;
	}

	if (hw->mac.type >= e1000_82576)
	{
		netdev->hw_features |= NETIF_F_SCTP_CSUM;
		netdev->features |= NETIF_F_SCTP_CSUM;
	}

	netdev->priv_flags |= IFF_UNICAST_FLT;

	adapter->en_mng_pt = igb_enable_mng_pass_thru(hw);

	/* before reading the NVM, reset the controller to put the device in a
	 * known good starting state
	 */
	hw->mac.ops.reset_hw(hw);

	/* make sure the NVM is good , i211 parts have special NVM that
	 * doesn't contain a checksum
	 */
	if (hw->mac.type != e1000_i211)
	{
		if (hw->nvm.ops.validate(hw) < 0)
		{
			dev_err(&pdev->dev, "The NVM Checksum Is Not Valid\n");
			err = -EIO;
			goto err_eeprom;
		}
	}

	/* copy the MAC address out of the NVM */
	if (hw->mac.ops.read_mac_addr(hw))
		dev_err(&pdev->dev, "NVM Read Error\n");

	memcpy(netdev->dev_addr, hw->mac.addr, netdev->addr_len);

	if (!is_valid_ether_addr(netdev->dev_addr))
	{
		dev_err(&pdev->dev, "Invalid MAC Address\n");
		err = -EIO;
		goto err_eeprom;
	}

	/* get firmware version for ethtool -i */
	igb_set_fw_version(adapter);

	setup_timer(&adapter->watchdog_timer, igb_watchdog,
							(unsigned long)adapter);
	setup_timer(&adapter->phy_info_timer, igb_update_phy_info,
							(unsigned long)adapter);

	INIT_WORK(&adapter->reset_task, igb_reset_task);
	INIT_WORK(&adapter->watchdog_task, igb_watchdog_task);

	/* Initialize link properties that are user-changeable */
	adapter->fc_autoneg = true;
	hw->mac.autoneg = true;
	hw->phy.autoneg_advertised = 0x2f;

	hw->fc.requested_mode = e1000_fc_default;
	hw->fc.current_mode = e1000_fc_default;

	igb_validate_mdi_setting(hw);

	/* By default, support wake on port A */
	if (hw->bus.func == 0)
		adapter->flags |= IGB_FLAG_WOL_SUPPORTED;

	/* Check the NVM for wake support on non-port A ports */
	if (hw->mac.type >= e1000_82580)
		hw->nvm.ops.read(hw, NVM_INIT_CONTROL3_PORT_A + NVM_82580_LAN_FUNC_OFFSET(hw->bus.func), 1,
										 &eeprom_data);
	else if (hw->bus.func == 1)
		hw->nvm.ops.read(hw, NVM_INIT_CONTROL3_PORT_B, 1, &eeprom_data);

	if (eeprom_data & IGB_EEPROM_APME)
		adapter->flags |= IGB_FLAG_WOL_SUPPORTED;

	/* now that we have the eeprom settings, apply the special cases where
	 * the eeprom may be wrong or the board simply won't support wake on
	 * lan on a particular port
	 */
	switch (pdev->device)
	{
	case E1000_DEV_ID_82575GB_QUAD_COPPER:
		adapter->flags &= ~IGB_FLAG_WOL_SUPPORTED;
		break;
	case E1000_DEV_ID_82575EB_FIBER_SERDES:
	case E1000_DEV_ID_82576_FIBER:
	case E1000_DEV_ID_82576_SERDES:
		/* Wake events only supported on port A for dual fiber
		 * regardless of eeprom setting
		 */
		if (rd32(E1000_STATUS) & E1000_STATUS_FUNC_1)
			adapter->flags &= ~IGB_FLAG_WOL_SUPPORTED;
		break;
	case E1000_DEV_ID_82576_QUAD_COPPER:
	case E1000_DEV_ID_82576_QUAD_COPPER_ET2:
		/* if quad port adapter, disable WoL on all but port A */
		if (global_quad_port_a != 0)
			adapter->flags &= ~IGB_FLAG_WOL_SUPPORTED;
		else
			adapter->flags |= IGB_FLAG_QUAD_PORT_A;
		/* Reset for multiple quad port adapters */
		if (++global_quad_port_a == 4)
			global_quad_port_a = 0;
		break;
	default:
		/* If the device can't wake, don't set software support */
		if (!device_can_wakeup(&adapter->pdev->dev))
			adapter->flags &= ~IGB_FLAG_WOL_SUPPORTED;
	}

	/* initialize the wol settings based on the eeprom settings */
	if (adapter->flags & IGB_FLAG_WOL_SUPPORTED)
		adapter->wol |= E1000_WUFC_MAG;

	/* Some vendors want WoL disabled by default, but still supported */
	if ((hw->mac.type == e1000_i350) &&
			(pdev->subsystem_vendor == PCI_VENDOR_ID_HP))
	{
		adapter->flags |= IGB_FLAG_WOL_SUPPORTED;
		adapter->wol = 0;
	}

	device_set_wakeup_enable(&adapter->pdev->dev,
													 adapter->flags & IGB_FLAG_WOL_SUPPORTED);

	/* reset the hardware with the new settings */
	igb_reset(adapter);

	/* Init the I2C interface */
	err = igb_init_i2c(adapter);
	if (err)
	{
		dev_err(&pdev->dev, "failed to init i2c interface\n");
		goto err_eeprom;
	}

	/* let the f/w know that the h/w is now under the control of the
	 * driver. */
	igb_get_hw_control(adapter);

	strcpy(netdev->name, "eth%d");
	err = register_netdev(netdev);
	if (err)
		goto err_register;

	/* carrier off reporting is important to ethtool even BEFORE open */
	netif_carrier_off(netdev);

#ifdef CONFIG_IGB_DCA
	if (dca_add_requester(&pdev->dev) == 0)
	{
		adapter->flags |= IGB_FLAG_DCA_ENABLED;
		dev_info(&pdev->dev, "DCA enabled\n");
		igb_setup_dca(adapter);
	}

#endif
#ifdef CONFIG_IGB_HWMON
	/* Initialize the thermal sensor on i350 devices. */
	if (hw->mac.type == e1000_i350 && hw->bus.func == 0)
	{
		u16 ets_word;

		/* Read the NVM to determine if this i350 device supports an
		 * external thermal sensor.
		 */
		hw->nvm.ops.read(hw, NVM_ETS_CFG, 1, &ets_word);
		if (ets_word != 0x0000 && ets_word != 0xFFFF)
			adapter->ets = true;
		else
			adapter->ets = false;
		if (igb_sysfs_init(adapter))
			dev_err(&pdev->dev,
							"failed to allocate sysfs resources\n");
	}
	else
	{
		adapter->ets = false;
	}
#endif
	/* do hw tstamp init after resetting */
	igb_ptp_init(adapter);

	dev_info(&pdev->dev, "Intel(R) Gigabit Ethernet Network Connection\n");
	/* print bus type/speed/width info, not applicable to i354 */
	if (hw->mac.type != e1000_i354)
	{
		dev_info(&pdev->dev, "%s: (PCIe:%s:%s) %pM\n",
						 netdev->name,
						 ((hw->bus.speed == e1000_bus_speed_2500) ? "2.5Gb/s" : (hw->bus.speed == e1000_bus_speed_5000) ? "5.0Gb/s"
																																																						: "unknown"),
						 ((hw->bus.width == e1000_bus_width_pcie_x4) ? "Width x4" : (hw->bus.width == e1000_bus_width_pcie_x2) ? "Width x2"
																																		: (hw->bus.width == e1000_bus_width_pcie_x1)	 ? "Width x1"
																																																									 : "unknown"),
						 netdev->dev_addr);
	}

	ret_val = igb_read_part_string(hw, part_str, E1000_PBANUM_LENGTH);
	if (ret_val)
		strcpy(part_str, "Unknown");
	dev_info(&pdev->dev, "%s: PBA No: %s\n", netdev->name, part_str);
	dev_info(&pdev->dev,
					 "Using %s interrupts. %d rx queue(s), %d tx queue(s)\n",
					 adapter->msix_entries ? "MSI-X" : (adapter->flags & IGB_FLAG_HAS_MSI) ? "MSI"
																																								 : "legacy",
					 adapter->num_rx_queues, adapter->num_tx_queues);
	switch (hw->mac.type)
	{
	case e1000_i350:
	case e1000_i210:
	case e1000_i211:
		igb_set_eee_i350(hw);
		break;
	case e1000_i354:
		if (hw->phy.media_type == e1000_media_type_copper)
		{
			if ((rd32(E1000_CTRL_EXT) &
					 E1000_CTRL_EXT_LINK_MODE_SGMII))
				igb_set_eee_i354(hw);
		}
		break;
	default:
		break;
	}

	pm_runtime_put_noidle(&pdev->dev);
	return 0;

err_register:
	igb_release_hw_control(adapter);
	memset(&adapter->i2c_adap, 0, sizeof(adapter->i2c_adap));
err_eeprom:
	if (!igb_check_reset_block(hw))
		igb_reset_phy(hw);

	if (hw->flash_address)
		iounmap(hw->flash_address);
err_sw_init:
	igb_clear_interrupt_scheme(adapter);
	iounmap(hw->hw_addr);
err_ioremap:
	free_netdev(netdev);
err_alloc_etherdev:
	pci_release_selected_regions(pdev,
															 pci_select_bars(pdev, IORESOURCE_MEM));
err_pci_reg:
err_dma:
	pci_disable_device(pdev);
	return err;
}
// igb网卡驱动程序
static struct pci_driver igb_driver = {
		.name = igb_driver_name,
		.id_table = igb_pci_tbl,
		.probe = igb_probe, //硬件加载
		.remove = igb_remove,
#ifdef CONFIG_PM
		.driver.pm = &igb_pm_ops,
#endif
		.shutdown = igb_shutdown,
		.sriov_configure = igb_pci_sriov_configure,
		.err_handler = &igb_err_handler};
/**
 *  igb_init_module - Driver Registration Routine
 *
 *  igb_init_module is the first routine called when the driver is
 *  loaded. All it does is register with the PCI subsystem.
 **/
//初始化igb网卡模块
static int __init igb_init_module(void)
{
	int ret;
	pr_info("%s - version %s\n",
					igb_driver_string, igb_driver_version);

	pr_info("%s\n", igb_copyright);

#ifdef CONFIG_IGB_DCA
	dca_register_notify(&dca_notifier);
#endif

	//注册igb网卡驱动程序
	ret = pci_register_driver(&igb_driver);
	return ret;
}
module_init(igb_init_module);

/**
 *  igb_alloc_q_vector - Allocate memory for a single interrupt vector
 *  @adapter: board private structure to initialize
 *  @v_count: q_vectors allocated on adapter, used for ring interleaving
 *  @v_idx: index of vector in adapter struct
 *  @txr_count: total number of Tx rings to allocate
 *  @txr_idx: index of first Tx ring to allocate
 *  @rxr_count: total number of Rx rings to allocate
 *  @rxr_idx: index of first Rx ring to allocate
 *
 *  We allocate one q_vector.  If allocation fails we return -ENOMEM.
 **/
static int igb_alloc_q_vector(struct igb_adapter *adapter,
															int v_count, int v_idx,
															int txr_count, int txr_idx,
															int rxr_count, int rxr_idx)
{
	struct igb_q_vector *q_vector;
	struct igb_ring *ring;
	int ring_count, size;

	/* igb only supports 1 Tx and/or 1 Rx queue per vector */
	if (txr_count > 1 || rxr_count > 1)
		return -ENOMEM;

	ring_count = txr_count + rxr_count;
	size = sizeof(struct igb_q_vector) +
				 (sizeof(struct igb_ring) * ring_count);

	/* allocate q_vector and rings */
	q_vector = kzalloc(size, GFP_KERNEL);
	if (!q_vector)
		return -ENOMEM;

	/* initialize NAPI */
	//注册NAPI的poll函数igb_poll
	netif_napi_add(adapter->netdev, &q_vector->napi,
								 igb_poll, 64);

	/* tie q_vector and adapter together */
	adapter->q_vector[v_idx] = q_vector;
	q_vector->adapter = adapter;

	/* initialize work limits */
	q_vector->tx.work_limit = adapter->tx_work_limit;

	/* initialize ITR configuration */
	q_vector->itr_register = adapter->hw.hw_addr + E1000_EITR(0);
	q_vector->itr_val = IGB_START_ITR;

	/* initialize pointer to rings */
	ring = q_vector->ring;

	/* intialize ITR */
	if (rxr_count)
	{
		/* rx or rx/tx vector */
		if (!adapter->rx_itr_setting || adapter->rx_itr_setting > 3)
			q_vector->itr_val = adapter->rx_itr_setting;
	}
	else
	{
		/* tx only vector */
		if (!adapter->tx_itr_setting || adapter->tx_itr_setting > 3)
			q_vector->itr_val = adapter->tx_itr_setting;
	}

	if (txr_count)
	{
		/* assign generic ring traits */
		ring->dev = &adapter->pdev->dev;
		ring->netdev = adapter->netdev;

		/* configure backlink on ring */
		ring->q_vector = q_vector;

		/* update q_vector Tx values */
		igb_add_ring(ring, &q_vector->tx);

		/* For 82575, context index must be unique per ring. */
		if (adapter->hw.mac.type == e1000_82575)
			set_bit(IGB_RING_FLAG_TX_CTX_IDX, &ring->flags);

		/* apply Tx specific ring traits */
		ring->count = adapter->tx_ring_count;
		ring->queue_index = txr_idx;

		/* assign ring to adapter */
		adapter->tx_ring[txr_idx] = ring;

		/* push pointer to next ring */
		ring++;
	}

	if (rxr_count)
	{
		/* assign generic ring traits */
		ring->dev = &adapter->pdev->dev;
		ring->netdev = adapter->netdev;

		/* configure backlink on ring */
		ring->q_vector = q_vector;

		/* update q_vector Rx values */
		igb_add_ring(ring, &q_vector->rx);

		/* set flag indicating ring supports SCTP checksum offload */
		if (adapter->hw.mac.type >= e1000_82576)
			set_bit(IGB_RING_FLAG_RX_SCTP_CSUM, &ring->flags);

		/*
		 * On i350, i354, i210, and i211, loopback VLAN packets
		 * have the tag byte-swapped.
		 */
		if (adapter->hw.mac.type >= e1000_i350)
			set_bit(IGB_RING_FLAG_RX_LB_VLAN_BSWAP, &ring->flags);

		/* apply Rx specific ring traits */
		ring->count = adapter->rx_ring_count;
		ring->queue_index = rxr_idx;

		/* assign ring to adapter */
		adapter->rx_ring[rxr_idx] = ring;
	}

	return 0;
}
/**
 *  igb_alloc_q_vectors - Allocate memory for interrupt vectors
 *  @adapter: board private structure to initialize
 *
 *  We allocate one q_vector per queue interrupt.  If allocation fails we
 *  return -ENOMEM.
 **/
static int igb_alloc_q_vectors(struct igb_adapter *adapter)
{
	int q_vectors = adapter->num_q_vectors;
	int rxr_remaining = adapter->num_rx_queues;
	int txr_remaining = adapter->num_tx_queues;
	int rxr_idx = 0, txr_idx = 0, v_idx = 0;
	int err;

	if (q_vectors >= (rxr_remaining + txr_remaining))
	{
		for (; rxr_remaining; v_idx++)
		{
			err = igb_alloc_q_vector(adapter, q_vectors, v_idx,
															 0, 0, 1, rxr_idx);

			if (err)
				goto err_out;

			/* update counts and index */
			rxr_remaining--;
			rxr_idx++;
		}
	}

	for (; v_idx < q_vectors; v_idx++)
	{
		int rqpv = DIV_ROUND_UP(rxr_remaining, q_vectors - v_idx);
		int tqpv = DIV_ROUND_UP(txr_remaining, q_vectors - v_idx);
		err = igb_alloc_q_vector(adapter, q_vectors, v_idx,
														 tqpv, txr_idx, rqpv, rxr_idx);

		if (err)
			goto err_out;

		/* update counts and index */
		rxr_remaining -= rqpv;
		txr_remaining -= tqpv;
		rxr_idx++;
		txr_idx++;
	}

	return 0;

err_out:
	adapter->num_tx_queues = 0;
	adapter->num_rx_queues = 0;
	adapter->num_q_vectors = 0;

	while (v_idx--)
		igb_free_q_vector(adapter, v_idx);

	return -ENOMEM;
}
/**
 *  igb_init_interrupt_scheme - initialize interrupts, allocate queues/vectors
 *  @adapter: board private structure to initialize
 *  @msix: boolean value of MSIX capability
 *
 *  This function initializes the interrupts and allocates all of the queues.
 **/
static int igb_init_interrupt_scheme(struct igb_adapter *adapter, bool msix)
{
	struct pci_dev *pdev = adapter->pdev;
	int err;

	igb_set_interrupt_capability(adapter, msix);

	err = igb_alloc_q_vectors(adapter);
	if (err)
	{
		dev_err(&pdev->dev, "Unable to allocate memory for vectors\n");
		goto err_alloc_q_vectors;
	}

	igb_cache_ring_register(adapter);

	return 0;

err_alloc_q_vectors:
	igb_reset_interrupt_capability(adapter);
	return err;
}
/**
 *  igb_request_msix - Initialize MSI-X interrupts
 *  @adapter: board private structure to initialize
 *
 *  igb_request_msix allocates MSI-X vectors and requests interrupts from the
 *  kernel.
 **/
static int igb_request_msix(struct igb_adapter *adapter)
{
	struct net_device *netdev = adapter->netdev;
	struct e1000_hw *hw = &adapter->hw;
	int i, err = 0, vector = 0, free_vector = 0;

	err = request_irq(adapter->msix_entries[vector].vector,
										igb_msix_other, 0, netdev->name, adapter);
	if (err)
		goto err_out;

	for (i = 0; i < adapter->num_q_vectors; i++)
	{
		struct igb_q_vector *q_vector = adapter->q_vector[i];

		vector++;

		q_vector->itr_register = hw->hw_addr + E1000_EITR(vector);

		if (q_vector->rx.ring && q_vector->tx.ring)
			sprintf(q_vector->name, "%s-TxRx-%u", netdev->name,
							q_vector->rx.ring->queue_index);
		else if (q_vector->tx.ring)
			sprintf(q_vector->name, "%s-tx-%u", netdev->name,
							q_vector->tx.ring->queue_index);
		else if (q_vector->rx.ring)
			sprintf(q_vector->name, "%s-rx-%u", netdev->name,
							q_vector->rx.ring->queue_index);
		else
			sprintf(q_vector->name, "%s-unused", netdev->name);

		//注册中断函数
		err = request_irq(adapter->msix_entries[vector].vector,
											igb_msix_ring, 0, q_vector->name,
											q_vector);
		if (err)
			goto err_free;
	}

	igb_configure_msix(adapter);
	return 0;

err_free:
	/* free already assigned IRQs */
	free_irq(adapter->msix_entries[free_vector++].vector, adapter);

	vector--;
	for (i = 0; i < vector; i++)
	{
		free_irq(adapter->msix_entries[free_vector++].vector,
						 adapter->q_vector[i]);
	}
err_out:
	return err;
}
/**
 *  igb_request_irq - initialize interrupts
 *  @adapter: board private structure to initialize
 *
 *  Attempts to configure interrupts using the best available
 *  capabilities of the hardware and kernel.
 **/
static int igb_request_irq(struct igb_adapter *adapter)
{
	struct net_device *netdev = adapter->netdev;
	struct pci_dev *pdev = adapter->pdev;
	int err = 0;

	//多队列网卡
	if (adapter->msix_entries)
	{
		err = igb_request_msix(adapter);
		if (!err)
			goto request_done;
		/* fall back to MSI */
		igb_free_all_tx_resources(adapter);
		igb_free_all_rx_resources(adapter);

		igb_clear_interrupt_scheme(adapter);
		err = igb_init_interrupt_scheme(adapter, false);
		if (err)
			goto request_done;

		igb_setup_all_tx_resources(adapter);
		igb_setup_all_rx_resources(adapter);
		igb_configure(adapter);
	}

	igb_assign_vector(adapter->q_vector[0], 0);

	if (adapter->flags & IGB_FLAG_HAS_MSI)
	{
		err = request_irq(pdev->irq, igb_intr_msi, 0,
											netdev->name, adapter);
		if (!err)
			goto request_done;

		/* fall back to legacy interrupts */
		igb_reset_interrupt_capability(adapter);
		adapter->flags &= ~IGB_FLAG_HAS_MSI;
	}

	err = request_irq(pdev->irq, igb_intr, IRQF_SHARED,
										netdev->name, adapter);

	if (err)
		dev_err(&pdev->dev, "Error %d getting interrupt\n",
						err);

request_done:
	return err;
}
/**
 *  igb_open - Called when a network interface is made active
 *  @netdev: network interface device structure
 *
 *  Returns 0 on success, negative value on failure
 *
 *  The open entry point is called when a network interface is made
 *  active by the system (IFF_UP).  At this point all resources needed
 *  for transmit and receive operations are allocated, the interrupt
 *  handler is registered with the OS, the watchdog timer is started,
 *  and the stack is notified that the interface is ready.
 **/
//启用igb网卡
static int __igb_open(struct net_device *netdev, bool resuming)
{
	struct igb_adapter *adapter = netdev_priv(netdev);
	struct e1000_hw *hw = &adapter->hw;
	struct pci_dev *pdev = adapter->pdev;
	int err;
	int i;

	/* disallow open during test */
	if (test_bit(__IGB_TESTING, &adapter->state))
	{
		WARN_ON(resuming);
		return -EBUSY;
	}

	if (!resuming)
		pm_runtime_get_sync(&pdev->dev);

	netif_carrier_off(netdev);

	/* allocate transmit descriptors */
	//分配传输描述符数组
	err = igb_setup_all_tx_resources(adapter);
	if (err)
		goto err_setup_tx;

	/* allocate receive descriptors */
	//分配接收描述符数组
	err = igb_setup_all_rx_resources(adapter);
	if (err)
		goto err_setup_rx;

	igb_power_up_link(adapter);

	/* before we allocate an interrupt, we must be ready to handle it.
	 * Setting DEBUG_SHIRQ in the kernel makes it fire an interrupt
	 * as soon as we call pci_request_irq, so we have to setup our
	 * clean_rx handler before we do so.
	 */
	igb_configure(adapter);

	//注册中断函数
	err = igb_request_irq(adapter);
	if (err)
		goto err_req_irq;

	/* Notify the stack of the actual queue counts. */
	err = netif_set_real_num_tx_queues(adapter->netdev,
																		 adapter->num_tx_queues);
	if (err)
		goto err_set_queues;

	err = netif_set_real_num_rx_queues(adapter->netdev,
																		 adapter->num_rx_queues);
	if (err)
		goto err_set_queues;

	/* From here on the code is the same as igb_up() */
	clear_bit(__IGB_DOWN, &adapter->state);

	for (i = 0; i < adapter->num_q_vectors; i++)
		//启用NAPI
		napi_enable(&(adapter->q_vector[i]->napi));

	/* Clear any pending interrupts. */
	rd32(E1000_ICR);

	igb_irq_enable(adapter);

	/* notify VFs that reset has been completed */
	if (adapter->vfs_allocated_count)
	{
		u32 reg_data = rd32(E1000_CTRL_EXT);
		reg_data |= E1000_CTRL_EXT_PFRSTD;
		wr32(E1000_CTRL_EXT, reg_data);
	}

	//开启全部队列
	netif_tx_start_all_queues(netdev);

	if (!resuming)
		pm_runtime_put(&pdev->dev);

	/* start the watchdog. */
	hw->mac.get_link_status = 1;
	schedule_work(&adapter->watchdog_task);

	return 0;

err_set_queues:
	igb_free_irq(adapter);
err_req_irq:
	igb_release_hw_control(adapter);
	igb_power_down_link(adapter);
	igb_free_all_rx_resources(adapter);
err_setup_rx:
	igb_free_all_tx_resources(adapter);
err_setup_tx:
	igb_reset(adapter);
	if (!resuming)
		pm_runtime_put(&pdev->dev);

	return err;
}
static int igb_open(struct net_device *netdev)
{
	return __igb_open(netdev, false);
}
```
</details>

### 硬中断处理

<details>
<summary>net/core/dev.c</summary>

```C++
/**
 * __napi_schedule - schedule for receive
 * @n: entry to schedule
 *
 * The entry's receive function will be scheduled to run
 */
void __napi_schedule(struct napi_struct *n)
{
	unsigned long flags;

	local_irq_save(flags);
	____napi_schedule(&__get_cpu_var(softnet_data), n);
	local_irq_restore(flags);
}
EXPORT_SYMBOL(__napi_schedule);

/* Called with irq disabled */
static inline void ____napi_schedule(struct softnet_data *sd,
																		 struct napi_struct *napi)
{
	//插入poll回调函数
	list_add_tail(&napi->poll_list, &sd->poll_list);
	//设置软中断标记
	__raise_softirq_irqoff(NET_RX_SOFTIRQ);
}
```
</details>

<details>
<summary>drivers/net/ethernet/intel/igb/igb_main.c</summary>

```C++
//网卡数据硬中断处理函数
static irqreturn_t igb_msix_ring(int irq, void *data)
{
	struct igb_q_vector *q_vector = data;

	/* Write the ITR value calculated from the previous interrupt. */
	igb_write_itr(q_vector);

	// NAPI调度
	napi_schedule(&q_vector->napi);

	return IRQ_HANDLED;
}
```
</details>

### 软中断处理

<details>
<summary>kernel/softirq.c</summary>

```C++
static void run_ksoftirqd(unsigned int cpu)
{
	local_irq_disable();
	if (local_softirq_pending()) //__softirq_pending
	{
		//处理软中断
		__do_softirq();
		rcu_note_context_switch(cpu);
		local_irq_enable();
		cond_resched();
		return;
	}
	local_irq_enable();
}
asmlinkage void __do_softirq(void)
{
	struct softirq_action *h;
	__u32 pending;
	unsigned long end = jiffies + MAX_SOFTIRQ_TIME;
	int cpu;
	unsigned long old_flags = current->flags;
	int max_restart = MAX_SOFTIRQ_RESTART;

	/*
	 * Mask out PF_MEMALLOC s current task context is borrowed for the
	 * softirq. A softirq handled such as network RX might set PF_MEMALLOC
	 * again if the socket is related to swap
	 */
	current->flags &= ~PF_MEMALLOC;

	pending = local_softirq_pending();
	account_irq_enter_time(current);

	__local_bh_disable((unsigned long)__builtin_return_address(0),
										 SOFTIRQ_OFFSET);
	lockdep_softirq_enter();

	cpu = smp_processor_id();
restart:
	/* Reset the pending bitmask before enabling irqs */
	set_softirq_pending(0);

	local_irq_enable();

	h = softirq_vec;

	do
	{
		if (pending & 1)
		{
			unsigned int vec_nr = h - softirq_vec;
			int prev_count = preempt_count();

			kstat_incr_softirqs_this_cpu(vec_nr);

			trace_softirq_entry(vec_nr);
			//处理数据
			h->action(h);
			trace_softirq_exit(vec_nr);
			if (unlikely(prev_count != preempt_count()))
			{
				printk(KERN_ERR "huh, entered softirq %u %s %p"
												"with preempt_count %08x,"
												" exited with %08x?\n",
							 vec_nr,
							 softirq_to_name[vec_nr], h->action,
							 prev_count, preempt_count());
				preempt_count() = prev_count;
			}

			rcu_bh_qs(cpu);
		}
		h++;
		pending >>= 1;
	} while (pending);

	local_irq_disable();

	pending = local_softirq_pending();
	if (pending)
	{
		if (time_before(jiffies, end) && !need_resched() &&
				--max_restart)
			goto restart;

		wakeup_softirqd();
	}

	lockdep_softirq_exit();

	account_irq_exit_time(current);
	__local_bh_enable(SOFTIRQ_OFFSET);
	tsk_restore_flags(current, old_flags, PF_MEMALLOC);
}
```
</details>

<details>
<summary>net/core/dev.c</summary>

```C++
//接收数据软中断处理函数
static void net_rx_action(struct softirq_action *h)
{
	struct softnet_data *sd = &__get_cpu_var(softnet_data);
	unsigned long time_limit = jiffies + 2;
	int budget = netdev_budget;
	void *have;

	//关闭硬中断
	local_irq_disable();

	while (!list_empty(&sd->poll_list))
	{
		struct napi_struct *n;
		int work, weight;

		/* If softirq window is exhuasted then punt.
		 * Allow this to run for 2 jiffies since which will allow
		 * an average latency of 1.5/HZ.
		 */
		if (unlikely(budget <= 0 || time_after_eq(jiffies, time_limit)))
			goto softnet_break;

		local_irq_enable();

		/* Even though interrupts have been re-enabled, this
		 * access is safe because interrupts can only add new
		 * entries to the tail of this list, and only ->poll()
		 * calls can remove this head entry from the list.
		 */
		n = list_first_entry(&sd->poll_list, struct napi_struct, poll_list);

		have = netpoll_poll_lock(n);

		weight = n->weight;

		/* This NAPI_STATE_SCHED test is for avoiding a race
		 * with netpoll's poll_napi().  Only the entity which
		 * obtains the lock and sees NAPI_STATE_SCHED set will
		 * actually make the ->poll() call.  Therefore we avoid
		 * accidentally calling ->poll() when NAPI is not scheduled.
		 */
		work = 0;
		if (test_bit(NAPI_STATE_SCHED, &n->state))
		{
			//调用poll回调函数(igb_poll)
			work = n->poll(n, weight);
			trace_napi_poll(n);
		}

		WARN_ON_ONCE(work > weight);

		budget -= work;

		local_irq_disable();

		/* Drivers must not modify the NAPI state if they
		 * consume the entire weight.  In such cases this code
		 * still "owns" the NAPI instance and therefore can
		 * move the instance around on the list at-will.
		 */
		if (unlikely(work == weight))
		{
			if (unlikely(napi_disable_pending(n)))
			{
				local_irq_enable();
				napi_complete(n);
				local_irq_disable();
			}
			else
			{
				if (n->gro_list)
				{
					/* flush too old packets
					 * If HZ < 1000, flush all packets.
					 */
					local_irq_enable();
					napi_gro_flush(n, HZ >= 1000);
					local_irq_disable();
				}
				list_move_tail(&n->poll_list, &sd->poll_list);
			}
		}

		netpoll_poll_unlock(have);
	}
out:
	net_rps_action_and_irq_enable(sd);

#ifdef CONFIG_NET_DMA
	/*
	 * There may not be any more sk_buffs coming right now, so push
	 * any pending DMA copies to hardware
	 */
	dma_issue_pending_all();
#endif

	return;

softnet_break:
	sd->time_squeeze++;
	__raise_softirq_irqoff(NET_RX_SOFTIRQ);
	goto out;
}

gro_result_t napi_gro_receive(struct napi_struct *napi, struct sk_buff *skb)
{
	skb_gro_reset_offset(skb);

	return napi_skb_finish(dev_gro_receive(napi, skb), skb);
}
EXPORT_SYMBOL(napi_gro_receive);

static gro_result_t napi_skb_finish(gro_result_t ret, struct sk_buff *skb)
{
	switch (ret)
	{
	case GRO_NORMAL:
		if (netif_receive_skb(skb)) //将数据包送到协议栈
			ret = GRO_DROP;
		break;

	case GRO_DROP:
		kfree_skb(skb);
		break;

	case GRO_MERGED_FREE:
		if (NAPI_GRO_CB(skb)->free == NAPI_GRO_FREE_STOLEN_HEAD)
			kmem_cache_free(skbuff_head_cache, skb);
		else
			__kfree_skb(skb);
		break;

	case GRO_HELD:
	case GRO_MERGED:
		break;
	}

	return ret;
}

/**
 *	netif_receive_skb - process receive buffer from network
 *	@skb: buffer to process
 *
 *	netif_receive_skb() is the main receive data processing function.
 *	It always succeeds. The buffer may be dropped during processing
 *	for congestion control or by the protocol layers.
 *
 *	This function may only be called from softirq context and interrupts
 *	should be enabled.
 *
 *	Return values (usually ignored):
 *	NET_RX_SUCCESS: no congestion
 *	NET_RX_DROP: packet was dropped
 */
int netif_receive_skb(struct sk_buff *skb)
{
	net_timestamp_check(netdev_tstamp_prequeue, skb);

	if (skb_defer_rx_timestamp(skb))
		return NET_RX_SUCCESS;

#ifdef CONFIG_RPS
	if (static_key_false(&rps_needed))
	{
		struct rps_dev_flow voidflow, *rflow = &voidflow;
		int cpu, ret;

		rcu_read_lock();

		cpu = get_rps_cpu(skb->dev, skb, &rflow);

		if (cpu >= 0)
		{
			ret = enqueue_to_backlog(skb, cpu, &rflow->last_qtail);
			rcu_read_unlock();
			return ret;
		}
		rcu_read_unlock();
	}
#endif
	return __netif_receive_skb(skb);
}
EXPORT_SYMBOL(netif_receive_skb);

static int __netif_receive_skb(struct sk_buff *skb)
{
	int ret;

	if (sk_memalloc_socks() && skb_pfmemalloc(skb))
	{
		unsigned long pflags = current->flags;

		/*
		 * PFMEMALLOC skbs are special, they should
		 * - be delivered to SOCK_MEMALLOC sockets only
		 * - stay away from userspace
		 * - have bounded memory usage
		 *
		 * Use PF_MEMALLOC as this saves us from propagating the allocation
		 * context down to all allocation sites.
		 */
		current->flags |= PF_MEMALLOC;
		ret = __netif_receive_skb_core(skb, true);
		tsk_restore_flags(current, pflags, PF_MEMALLOC);
	}
	else
		ret = __netif_receive_skb_core(skb, false);

	return ret;
}

static int __netif_receive_skb_core(struct sk_buff *skb, bool pfmemalloc)
{
	struct packet_type *ptype, *pt_prev;
	rx_handler_func_t *rx_handler;
	struct net_device *orig_dev;
	struct net_device *null_or_dev;
	bool deliver_exact = false;
	int ret = NET_RX_DROP;
	__be16 type;

	net_timestamp_check(!netdev_tstamp_prequeue, skb);

	trace_netif_receive_skb(skb);

	/* if we've gotten here through NAPI, check netpoll */
	if (netpoll_receive_skb(skb))
		goto out;

	orig_dev = skb->dev;

	skb_reset_network_header(skb);
	if (!skb_transport_header_was_set(skb))
		skb_reset_transport_header(skb);
	skb_reset_mac_len(skb);

	pt_prev = NULL;

	rcu_read_lock();

another_round:
	skb->skb_iif = skb->dev->ifindex;

	__this_cpu_inc(softnet_data.processed);

	if (skb->protocol == cpu_to_be16(ETH_P_8021Q) ||
			skb->protocol == cpu_to_be16(ETH_P_8021AD))
	{
		skb = vlan_untag(skb);
		if (unlikely(!skb))
			goto unlock;
	}

#ifdef CONFIG_NET_CLS_ACT
	if (skb->tc_verd & TC_NCLS)
	{
		skb->tc_verd = CLR_TC_NCLS(skb->tc_verd);
		goto ncls;
	}
#endif

	if (pfmemalloc)
		goto skip_taps;

	list_for_each_entry_rcu(ptype, &ptype_all, list)
	{
		if (!ptype->dev || ptype->dev == skb->dev)
		{
			if (pt_prev)
				ret = deliver_skb(skb, pt_prev, orig_dev);
			pt_prev = ptype;
		}
	}

skip_taps:
#ifdef CONFIG_NET_CLS_ACT
	skb = handle_ing(skb, &pt_prev, &ret, orig_dev);
	if (!skb)
		goto unlock;
ncls:
#endif

	if (pfmemalloc && !skb_pfmemalloc_protocol(skb))
		goto drop;

	if (vlan_tx_tag_present(skb))
	{
		if (pt_prev)
		{
			ret = deliver_skb(skb, pt_prev, orig_dev);
			pt_prev = NULL;
		}
		if (vlan_do_receive(&skb))
			goto another_round;
		else if (unlikely(!skb))
			goto unlock;
	}

	rx_handler = rcu_dereference(skb->dev->rx_handler);
	if (rx_handler)
	{
		if (pt_prev)
		{
			ret = deliver_skb(skb, pt_prev, orig_dev);
			pt_prev = NULL;
		}
		switch (rx_handler(&skb))
		{
		case RX_HANDLER_CONSUMED:
			ret = NET_RX_SUCCESS;
			goto unlock;
		case RX_HANDLER_ANOTHER:
			goto another_round;
		case RX_HANDLER_EXACT:
			deliver_exact = true;
		case RX_HANDLER_PASS:
			break;
		default:
			BUG();
		}
	}

	if (vlan_tx_nonzero_tag_present(skb))
		skb->pkt_type = PACKET_OTHERHOST;

	/* deliver only exact match when indicated */
	null_or_dev = deliver_exact ? skb->dev : NULL;

	type = skb->protocol;
	list_for_each_entry_rcu(ptype,
													&ptype_base[ntohs(type) & PTYPE_HASH_MASK], list)
	{
		if (ptype->type == type &&
				(ptype->dev == null_or_dev || ptype->dev == skb->dev ||
				 ptype->dev == orig_dev))
		{
			if (pt_prev)
				ret = deliver_skb(skb, pt_prev, orig_dev);
			pt_prev = ptype;
		}
	}

	if (pt_prev)
	{
		if (unlikely(skb_orphan_frags(skb, GFP_ATOMIC)))
			goto drop;
		else
			ret = pt_prev->func(skb, skb->dev, pt_prev, orig_dev);
	}
	else
	{
	drop:
		atomic_long_inc(&skb->dev->rx_dropped);
		kfree_skb(skb);
		/* Jamal, now you will not able to escape explaining
		 * me how you were going to use this. :-)
		 */
		ret = NET_RX_DROP;
	}

unlock:
	rcu_read_unlock();
out:
	return ret;
}
static inline int deliver_skb(struct sk_buff *skb,
															struct packet_type *pt_prev,
															struct net_device *orig_dev)
{
	if (unlikely(skb_orphan_frags(skb, GFP_ATOMIC)))
		return -ENOMEM;
	atomic_inc(&skb->users);
	//调用注册函数处理协议
	return pt_prev->func(skb, skb->dev, pt_prev, orig_dev);
}
```
</details>

<details>
<summary>drivers/net/ethernet/intel/igb/igb_main.c</summary>

```C++
/**
 *  igb_poll - NAPI Rx polling callback
 *  @napi: napi polling structure
 *  @budget: count of how many packets we should handle
 **/
// poll回调函数
static int igb_poll(struct napi_struct *napi, int budget)
{
	struct igb_q_vector *q_vector = container_of(napi,
																							 struct igb_q_vector,
																							 napi);
	bool clean_complete = true;

#ifdef CONFIG_IGB_DCA
	if (q_vector->adapter->flags & IGB_FLAG_DCA_ENABLED)
		igb_update_dca(q_vector);
#endif
	if (q_vector->tx.ring)
		//处理数据发送中断
		clean_complete = igb_clean_tx_irq(q_vector);

	if (q_vector->rx.ring)
		//处理数据接收中断
		clean_complete &= igb_clean_rx_irq(q_vector, budget);

	/* If all work not completed, return budget and keep polling */
	if (!clean_complete)
		return budget;

	/* If not enough Rx work done, exit the polling mode */
	napi_complete(napi);
	igb_ring_irq_enable(q_vector);

	return 0;
}
static bool igb_clean_rx_irq(struct igb_q_vector *q_vector, const int budget)
{
	struct igb_ring *rx_ring = q_vector->rx.ring;
	struct sk_buff *skb = rx_ring->skb;
	unsigned int total_bytes = 0, total_packets = 0;
	u16 cleaned_count = igb_desc_unused(rx_ring);

	do
	{
		union e1000_adv_rx_desc *rx_desc;

		/* return some buffers to hardware, one at a time is too slow */
		if (cleaned_count >= IGB_RX_BUFFER_WRITE)
		{
			igb_alloc_rx_buffers(rx_ring, cleaned_count);
			cleaned_count = 0;
		}

		rx_desc = IGB_RX_DESC(rx_ring, rx_ring->next_to_clean);

		if (!igb_test_staterr(rx_desc, E1000_RXD_STAT_DD))
			break;

		/* This memory barrier is needed to keep us from reading
		 * any other fields out of the rx_desc until we know the
		 * RXD_STAT_DD bit is set
		 */
		rmb();

		//从RingBuffer上取出数据
		/* retrieve a buffer from the ring */
		skb = igb_fetch_rx_buffer(rx_ring, rx_desc, skb);

		/* exit if we failed to retrieve a buffer */
		if (!skb)
			break;

		cleaned_count++;

		/* fetch next buffer in frame if non-eop */
		if (igb_is_non_eop(rx_ring, rx_desc))
			continue;

		/* verify the packet layout is correct */
		if (igb_cleanup_headers(rx_ring, rx_desc, skb))
		{
			skb = NULL;
			continue;
		}

		/* probably a little skewed due to removing CRC */
		total_bytes += skb->len;

		/* populate checksum, timestamp, VLAN, and protocol */
		igb_process_skb_fields(rx_ring, rx_desc, skb);

		//小包合并
		napi_gro_receive(&q_vector->napi, skb);

		/* reset skb pointer */
		skb = NULL;

		/* update budget accounting */
		total_packets++;
	} while (likely(total_packets < budget));

	/* place incomplete frames back on ring for completion */
	rx_ring->skb = skb;

	u64_stats_update_begin(&rx_ring->rx_syncp);
	rx_ring->rx_stats.packets += total_packets;
	rx_ring->rx_stats.bytes += total_bytes;
	u64_stats_update_end(&rx_ring->rx_syncp);
	q_vector->rx.total_packets += total_packets;
	q_vector->rx.total_bytes += total_bytes;

	if (cleaned_count)
		igb_alloc_rx_buffers(rx_ring, cleaned_count);

	return (total_packets < budget);
}
```
</details>


### TCP协议处理函数

<details>
<summary>TCP协议处理函数</summary>

```C++
// net/ipv4/tcp_ipv4.c
//TCP协议的注册处理函数集合
struct proto tcp_prot = {
		.name = "TCP",
		.owner = THIS_MODULE,
		.close = tcp_close,
		.connect = tcp_v4_connect,
		.disconnect = tcp_disconnect,
		.accept = inet_csk_accept,
		.ioctl = tcp_ioctl,
		.init = tcp_v4_init_sock,
		.destroy = tcp_v4_destroy_sock,
		.shutdown = tcp_shutdown,
		.setsockopt = tcp_setsockopt,
		.getsockopt = tcp_getsockopt,
		.recvmsg = tcp_recvmsg,
		.sendmsg = tcp_sendmsg,
		.sendpage = tcp_sendpage,
		.backlog_rcv = tcp_v4_do_rcv,
		.release_cb = tcp_release_cb,
		.mtu_reduced = tcp_v4_mtu_reduced,
		.hash = inet_hash,
		.unhash = inet_unhash,
		.get_port = inet_csk_get_port,
		.enter_memory_pressure = tcp_enter_memory_pressure,
		.sockets_allocated = &tcp_sockets_allocated,
		.orphan_count = &tcp_orphan_count,
		.memory_allocated = &tcp_memory_allocated,
		.memory_pressure = &tcp_memory_pressure,
		.sysctl_wmem = sysctl_tcp_wmem,
		.sysctl_rmem = sysctl_tcp_rmem,
		.max_header = MAX_TCP_HEADER,
		.obj_size = sizeof(struct tcp_sock),
		.slab_flags = SLAB_DESTROY_BY_RCU,
		.twsk_prot = &tcp_timewait_sock_ops,
		.rsk_prot = &tcp_request_sock_ops,
		.h.hashinfo = &tcp_hashinfo,
		.no_autobind = true,
#ifdef CONFIG_COMPAT
		.compat_setsockopt = compat_tcp_setsockopt,
		.compat_getsockopt = compat_tcp_getsockopt,
#endif
#ifdef CONFIG_MEMCG_KMEM
		.init_cgroup = tcp_init_cgroup,
		.destroy_cgroup = tcp_destroy_cgroup,
		.proto_cgroup = tcp_proto_cgroup,
#endif
};
EXPORT_SYMBOL(tcp_prot);
```
</details>