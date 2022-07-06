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
.LFB1519:
	.cfi_startproc
	endbr32
	pushl	%ebp													#旧基址进栈保存
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp										#新栈(基址)
	.cfi_def_cfa_register 5
	call	__x86.get_pc_thunk.ax					#把esp的内容赋值给eax (movl (%esp),%eax)
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	8(%ebp), %eax									#根据具体内存偏移获取形参
	addl	%eax, %eax
	popl	%ebp													#恢复旧基址
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret																	#函数返回
	.cfi_endproc
.LFE1519:
	.size	_Z4funci, .-_Z4funci
	.globl	main
	.type	main, @function
main:
.LFB1520:
	.cfi_startproc
	endbr32
	pushl	%ebp 													#旧基址进栈保存
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp										#新栈(基址)
	.cfi_def_cfa_register 5
	call	__x86.get_pc_thunk.ax					#把esp的内容赋值给eax (movl (%esp),%eax)
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	pushl	$5														#函数参数入栈
	call	_Z4funci											#函数调用
	addl	$4, %esp											#函数参数出栈
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret																	#函数返回
	.cfi_endproc
.LFE1520:
	.size	main, .-main
	.type	_Z41__static_initialization_and_destruction_0ii, @function
_Z41__static_initialization_and_destruction_0ii:
.LFB2001:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	subl	$4, %esp
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.bx
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx
	cmpl	$1, 8(%ebp)
	jne	.L7
	cmpl	$65535, 12(%ebp)
	jne	.L7
	subl	$12, %esp
	leal	_ZStL8__ioinit@GOTOFF(%ebx), %eax
	pushl	%eax
	call	_ZNSt8ios_base4InitC1Ev@PLT
	addl	$16, %esp
	subl	$4, %esp
	leal	__dso_handle@GOTOFF(%ebx), %eax
	pushl	%eax
	leal	_ZStL8__ioinit@GOTOFF(%ebx), %eax
	pushl	%eax
	movl	_ZNSt8ios_base4InitD1Ev@GOT(%ebx), %eax
	pushl	%eax
	call	__cxa_atexit@PLT
	addl	$16, %esp
.L7:
	nop
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE2001:
	.size	_Z41__static_initialization_and_destruction_0ii, .-_Z41__static_initialization_and_destruction_0ii
	.type	_GLOBAL__sub_I__Z4funci, @function
_GLOBAL__sub_I__Z4funci:
.LFB2002:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$8, %esp
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	subl	$8, %esp
	pushl	$65535
	pushl	$1
	call	_Z41__static_initialization_and_destruction_0ii
	addl	$16, %esp
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE2002:
	.size	_GLOBAL__sub_I__Z4funci, .-_GLOBAL__sub_I__Z4funci
	.section	.init_array,"aw"
	.align 4
	.long	_GLOBAL__sub_I__Z4funci
	.section	.text.__x86.get_pc_thunk.ax,"axG",@progbits,__x86.get_pc_thunk.ax,comdat
	.globl	__x86.get_pc_thunk.ax
	.hidden	__x86.get_pc_thunk.ax
	.type	__x86.get_pc_thunk.ax, @function
__x86.get_pc_thunk.ax:
.LFB2003:
	.cfi_startproc
	movl	(%esp), %eax
	ret
	.cfi_endproc
.LFE2003:
	.section	.text.__x86.get_pc_thunk.bx,"axG",@progbits,__x86.get_pc_thunk.bx,comdat
	.globl	__x86.get_pc_thunk.bx
	.hidden	__x86.get_pc_thunk.bx
	.type	__x86.get_pc_thunk.bx, @function
__x86.get_pc_thunk.bx:
.LFB2004:
	.cfi_startproc
	movl	(%esp), %ebx
	ret
	.cfi_endproc
.LFE2004:
	.hidden	__dso_handle
	.ident	"GCC: (Ubuntu 9.4.0-1ubuntu1~20.04) 9.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 4
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 4
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 4
4:
