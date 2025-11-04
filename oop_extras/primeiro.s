	.file	"primeiro.cpp"
	.text
	.section .rdata,"dr"
_ZStL6ignore:
	.space 1
_ZStL19piecewise_construct:
	.space 1
.LC0:
	.ascii "Ol\303\241! \12\0"
	.section	.text$_ZN10propaganda9inscreverEv,"x"
	.linkonce discard
	.align 2
	.globl	_ZN10propaganda9inscreverEv
	.def	_ZN10propaganda9inscreverEv;	.scl	2;	.type	32;	.endef
	.seh_proc	_ZN10propaganda9inscreverEv
_ZN10propaganda9inscreverEv:
.LFB1756:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$32, %rsp
	.seh_stackalloc	32
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	leaq	.LC0(%rip), %rdx
	movq	.refptr._ZSt4cout(%rip), %rax
	movq	%rax, %rcx
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	nop
	addq	$32, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.section .rdata,"dr"
.LC1:
	.ascii "Ol\303\241 novamente! \12\0"
	.section	.text$_ZN10propaganda5curtaEv,"x"
	.linkonce discard
	.align 2
	.globl	_ZN10propaganda5curtaEv
	.def	_ZN10propaganda5curtaEv;	.scl	2;	.type	32;	.endef
	.seh_proc	_ZN10propaganda5curtaEv
_ZN10propaganda5curtaEv:
.LFB1757:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$32, %rsp
	.seh_stackalloc	32
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	leaq	.LC1(%rip), %rdx
	movq	.refptr._ZSt4cout(%rip), %rax
	movq	%rax, %rcx
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	nop
	addq	$32, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.text
	.globl	main
	.def	main;	.scl	2;	.type	32;	.endef
	.seh_proc	main
main:
.LFB1758:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$48, %rsp
	.seh_stackalloc	48
	.seh_endprologue
	call	__main
	leaq	-1(%rbp), %rax
	movq	%rax, %rcx
	call	_ZN10propaganda9inscreverEv
	leaq	-1(%rbp), %rax
	movq	%rax, %rcx
	call	_ZN10propaganda5curtaEv
	movl	$0, %eax
	addq	$48, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.def	__main;	.scl	2;	.type	32;	.endef
	.ident	"GCC: (MinGW-W64 x86_64-ucrt-posix-seh, built by Brecht Sanders, r1) 15.2.0"
	.def	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc;	.scl	2;	.type	32;	.endef
	.section	.rdata$.refptr._ZSt4cout, "dr"
	.p2align	3, 0
	.globl	.refptr._ZSt4cout
	.linkonce	discard
.refptr._ZSt4cout:
	.quad	_ZSt4cout
