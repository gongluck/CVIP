# Linux网络

- [Linux网络](#linux网络)
  - [命令配置](#命令配置)

## 命令配置

- ```RingBuffer```

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

- 软中断统计

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

- 网卡队列

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

- 硬中断合并

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

- 接收/发送数据合并```LGO/LRO/TSO/GSO```

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

- 网络状态

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