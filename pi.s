	.file	"pi.c"
	.text
	.p2align 2,,3
	.globl	_random
	.def	_random;	.scl	2;	.type	32;	.endef
_random:
	subl	$12, %esp
	call	_rand
	flds	LC0
	pushl	%eax
	fidivrl	(%esp)
	fmul	%st(0), %st
	addl	$16, %esp
	ret
	.def	___main;	.scl	2;	.type	32;	.endef
	.section .rdata,"dr"
LC6:
	.ascii "%.15f\12\0"
	.section	.text.startup,"x"
	.p2align 2,,3
	.globl	_main
	.def	_main;	.scl	2;	.type	32;	.endef
_main:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%edi
	pushl	%esi
	andl	$-16, %esp
	subl	$80, %esp
	call	___main
	movl	$-1, %esi
	movl	$-1, %edi
	movl	$0, 56(%esp)
	movl	$0, 60(%esp)
	jmp	L6
	.p2align 2,,3
L4:
	addl	$-1, %esi
	adcl	$-1, %edi
	movl	%edi, %eax
	orl	%esi, %eax
	je	L12
L6:
	call	_rand
	flds	LC0
	fld	%st(0)
	pushl	%eax
	fidivrl	(%esp)
	popl	%edx
	fmul	%st(0), %st
	fxch	%st(1)
	fstpl	16(%esp)
	fstpl	32(%esp)
	call	_rand
	fldl	16(%esp)
	pushl	%eax
	fidivrl	(%esp)
	popl	%ecx
	fmul	%st(0), %st
	fldl	32(%esp)
	faddp	%st, %st(1)
	fld1
	fucompp
	fnstsw	%ax
	testb	$69, %ah
	jne	L4
	addl	$1, 56(%esp)
	adcl	$0, 60(%esp)
	addl	$-1, %esi
	adcl	$-1, %edi
	movl	%edi, %eax
	orl	%esi, %eax
	jne	L6
L12:
	fildq	56(%esp)
	movl	60(%esp), %eax
	testl	%eax, %eax
	js	L13
L7:
	fstpl	72(%esp)
	fldl	72(%esp)
	fmuls	LC4
	fmuls	LC5
	fstpl	4(%esp)
	movl	$LC6, (%esp)
	call	_printf
	leal	-8(%ebp), %esp
	popl	%esi
	popl	%edi
	popl	%ebp
	ret
L13:
	fadds	LC3
	jmp	L7
	.section .rdata,"dr"
	.align 4
LC0:
	.long	1191181824
	.align 4
LC3:
	.long	1602224128
	.align 4
LC4:
	.long	528482304
	.align 4
LC5:
	.long	1082130432
	.def	_rand;	.scl	2;	.type	32;	.endef
	.def	_printf;	.scl	2;	.type	32;	.endef
