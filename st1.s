	.text
	.file	"st1.c"
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# BB#0:
	pushq	%rbp
.Ltmp0:
	.cfi_def_cfa_offset 16
.Ltmp1:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
.Ltmp2:
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movabsq	$.L.str, %rdi
	movl	$3, -4(%rbp)
	movl	$4, -8(%rbp)
	movl	-4(%rbp), %eax
	addl	-8(%rbp), %eax
	movl	%eax, -12(%rbp)
	movl	-12(%rbp), %esi
	movb	$0, %al
	callq	printf
	xorl	%esi, %esi
	movl	%eax, -16(%rbp)         # 4-byte Spill
	movl	%esi, %eax
	addq	$16, %rsp
	popq	%rbp
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc

	.type	.L.str,@object          # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"%d\n"
	.size	.L.str, 4


	.ident	"clang version 3.9.1 (tags/RELEASE_391/final)"
	.section	".note.GNU-stack","",@progbits
