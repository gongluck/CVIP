	.file	"test.cpp"
	.text
	.section	.rodata
	.type	_ZStL19piecewise_construct, @object
	.size	_ZStL19piecewise_construct, 1
_ZStL19piecewise_construct:
	.zero	1
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.text
	.globl	_Z4funci
	.type	_Z4funci, @function
_Z4funci:
.LFB1522:
	.cfi_startproc
	endbr64
	pushq	%rbp							#旧基址进栈保存
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp				#新栈(基址)
	.cfi_def_cfa_register 6
													#是否冗余了？movl %edi, %eax	addl %eax, %eax
	movl	%edi, -4(%rbp)		# edi->rbp[0-3]
	movl	-4(%rbp), %eax		# rbp[0-3]->eax
	addl	%eax, %eax				# eax+=eax
	popq	%rbp							#恢复旧基址
	.cfi_def_cfa 7, 8
	ret											#函数返回
	.cfi_endproc
.LFE1522:
	.size	_Z4funci, .-_Z4funci
	.globl	main
	.type	main, @function
main:
.LFB1523:
	.cfi_startproc
	endbr64
	pushq	%rbp								#旧基址进栈保存
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp					#新栈(基址)
	.cfi_def_cfa_register 6
	movl	$5, %edi						#函数参数
	call	_Z4funci						#函数调用
	nop
	popq	%rbp								#恢复旧基址
	.cfi_def_cfa 7, 8
	ret												#函数返回
	.cfi_endproc
.LFE1523:
	.size	main, .-main
	.type	_Z41__static_initialization_and_destruction_0ii, @function
_Z41__static_initialization_and_destruction_0ii:
.LFB2004:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	cmpl	$1, -4(%rbp)
	jne	.L7
	cmpl	$65535, -8(%rbp)
	jne	.L7
	leaq	_ZStL8__ioinit(%rip), %rdi
	call	_ZNSt8ios_base4InitC1Ev@PLT
	leaq	__dso_handle(%rip), %rdx
	leaq	_ZStL8__ioinit(%rip), %rsi
	movq	_ZNSt8ios_base4InitD1Ev@GOTPCREL(%rip), %rax
	movq	%rax, %rdi
	call	__cxa_atexit@PLT
.L7:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2004:
	.size	_Z41__static_initialization_and_destruction_0ii, .-_Z41__static_initialization_and_destruction_0ii
	.type	_GLOBAL__sub_I__Z4funci, @function
_GLOBAL__sub_I__Z4funci:
.LFB2005:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$65535, %esi
	movl	$1, %edi
	call	_Z41__static_initialization_and_destruction_0ii
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2005:
	.size	_GLOBAL__sub_I__Z4funci, .-_GLOBAL__sub_I__Z4funci
	.section	.init_array,"aw"
	.align 8
	.quad	_GLOBAL__sub_I__Z4funci
	.hidden	__dso_handle
	.ident	"GCC: (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
