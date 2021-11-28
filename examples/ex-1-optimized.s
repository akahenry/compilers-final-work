.data
.globl r0
.type r0, @object
.size r0, 8
r0:
.long 0
.globl r1
.type r1, @object
.size r1, 8
r1:
.long 0
.globl r2
.type r2, @object
.size r2, 8
r2:
.long 0
.globl r3
.type r3, @object
.size r3, 8
r3:
.long 0
.globl r4
.type r4, @object
.size r4, 8
r4:
.long 0
.globl r5
.type r5, @object
.size r5, 8
r5:
.long 0
.globl r6
.type r6, @object
.size r6, 8
r6:
.long 0
.globl r7
.type r7, @object
.size r7, 8
r7:
.long 0
.globl r8
.type r8, @object
.size r8, 8
r8:
.long 0
.globl r9
.type r9, @object
.size r9, 8
r9:
.long 0
.globl r10
.type r10, @object
.size r10, 8
r10:
.long 0
.globl r11
.type r11, @object
.size r11, 8
r11:
.long 0
.globl r12
.type r12, @object
.size r12, 8
r12:
.long 0
.globl r13
.type r13, @object
.size r13, 8
r13:
.long 0
.globl r14
.type r14, @object
.size r14, 8
r14:
.long 0
.globl r15
.type r15, @object
.size r15, 8
r15:
.long 0
.globl r16
.type r16, @object
.size r16, 8
r16:
.long 0
.globl r17
.type r17, @object
.size r17, 8
r17:
.long 0
.globl r18
.type r18, @object
.size r18, 8
r18:
.long 0
.globl r19
.type r19, @object
.size r19, 8
r19:
.long 0
.globl r20
.type r20, @object
.size r20, 8
r20:
.long 0
.globl r21
.type r21, @object
.size r21, 8
r21:
.long 0
.text
.globl main
.type main, @function
call main
L0: 
mov 16(%rsp), %rax
mov %rbp, %rsp
ret
hlt
main:
mov %rsp, %rbp
add $-464, %rsp
mov r21(%rip), %rax
mov %rax, -432(%rbp)
mov r20(%rip), %rax
mov %rax, -416(%rbp)
mov r19(%rip), %rax
mov %rax, -400(%rbp)
mov r18(%rip), %rax
mov %rax, -384(%rbp)
mov r17(%rip), %rax
mov %rax, -368(%rbp)
mov r16(%rip), %rax
mov %rax, -352(%rbp)
mov r15(%rip), %rax
mov %rax, -336(%rbp)
mov r14(%rip), %rax
mov %rax, -320(%rbp)
mov r13(%rip), %rax
mov %rax, -304(%rbp)
mov r12(%rip), %rax
mov %rax, -288(%rbp)
mov r11(%rip), %rax
mov %rax, -272(%rbp)
mov r10(%rip), %rax
mov %rax, -256(%rbp)
mov r9(%rip), %rax
mov %rax, -240(%rbp)
mov r8(%rip), %rax
mov %rax, -224(%rbp)
mov r7(%rip), %rax
mov %rax, -208(%rbp)
mov r6(%rip), %rax
mov %rax, -192(%rbp)
mov r5(%rip), %rax
mov %rax, -176(%rbp)
mov r4(%rip), %rax
mov %rax, -160(%rbp)
mov r3(%rip), %rax
mov %rax, -144(%rbp)
mov r2(%rip), %rax
mov %rax, -128(%rbp)
mov r1(%rip), %rax
mov %rax, -112(%rbp)
mov r0(%rip), %rax
mov %rax, -96(%rbp)
movq $0, r0(%rip)
mov r0(%rip), %rax
mov %rax, -48(%rbp)
movq $1, r2(%rip)
mov r2(%rip), %rax
mov %rax, -64(%rbp)
movq $6, r4(%rip)
mov r4(%rip), %rax
mov %rax, -80(%rbp)
mov -48(%rbp), %rax
mov %rax, r6(%rip)
movq $0, r7(%rip)
mov r6(%rip), %rbx
mov r7(%rip), %rax
cmpl %ebx, %eax
jl LCMP1
movq $0, r8(%rip)
jmp LCMP2
LCMP1: 
movq $1, r8(%rip)
LCMP2: 
cmpl $0, r8(%rip)
je L3
jne L2
L2: 
mov -48(%rbp), %rax
mov %rax, r9(%rip)
mov -64(%rbp), %rax
mov %rax, r10(%rip)
mov -80(%rbp), %rax
mov %rax, r11(%rip)
mov r10(%rip), %rax
add r11(%rip), %rax
mov %rax, r12(%rip)
mov r12(%rip), %rax
mov %rax, -48(%rbp)
mov r12(%rip), %rax
mov %rax, 16(%rsp)
mov -304(%rbp), %rax
mov %rax, r13(%rip)
mov -288(%rbp), %rax
mov %rax, r12(%rip)
mov -272(%rbp), %rax
mov %rax, r11(%rip)
mov -256(%rbp), %rax
mov %rax, r10(%rip)
mov -240(%rbp), %rax
mov %rax, r9(%rip)
mov -224(%rbp), %rax
mov %rax, r8(%rip)
mov -208(%rbp), %rax
mov %rax, r7(%rip)
mov -192(%rbp), %rax
mov %rax, r6(%rip)
mov -176(%rbp), %rax
mov %rax, r5(%rip)
mov -160(%rbp), %rax
mov %rax, r4(%rip)
mov -144(%rbp), %rax
mov %rax, r3(%rip)
mov -128(%rbp), %rax
mov %rax, r2(%rip)
mov -112(%rbp), %rax
mov %rax, r1(%rip)
mov -96(%rbp), %rax
mov %rax, r0(%rip)
mov 16(%rsp), %rax
mov %rbp, %rsp
ret
jmp L4
L3: 
mov -48(%rbp), %rax
mov %rax, r15(%rip)
mov -80(%rbp), %rax
mov %rax, r16(%rip)
mov -64(%rbp), %rax
mov %rax, r17(%rip)
mov r16(%rip), %rax
sub r17(%rip), %rax
mov %rax, r18(%rip)
mov r18(%rip), %rax
mov %rax, -48(%rbp)
mov r18(%rip), %rax
mov %rax, 16(%rsp)
mov -400(%rbp), %rax
mov %rax, r19(%rip)
mov -384(%rbp), %rax
mov %rax, r18(%rip)
mov -368(%rbp), %rax
mov %rax, r17(%rip)
mov -352(%rbp), %rax
mov %rax, r16(%rip)
mov -336(%rbp), %rax
mov %rax, r15(%rip)
mov -320(%rbp), %rax
mov %rax, r14(%rip)
mov -304(%rbp), %rax
mov %rax, r13(%rip)
mov -288(%rbp), %rax
mov %rax, r12(%rip)
mov -272(%rbp), %rax
mov %rax, r11(%rip)
mov -256(%rbp), %rax
mov %rax, r10(%rip)
mov -240(%rbp), %rax
mov %rax, r9(%rip)
mov -224(%rbp), %rax
mov %rax, r8(%rip)
mov -208(%rbp), %rax
mov %rax, r7(%rip)
mov -192(%rbp), %rax
mov %rax, r6(%rip)
mov -176(%rbp), %rax
mov %rax, r5(%rip)
mov -160(%rbp), %rax
mov %rax, r4(%rip)
mov -144(%rbp), %rax
mov %rax, r3(%rip)
mov -128(%rbp), %rax
mov %rax, r2(%rip)
mov -112(%rbp), %rax
mov %rax, r1(%rip)
mov -96(%rbp), %rax
mov %rax, r0(%rip)
mov 16(%rsp), %rax
mov %rbp, %rsp
ret
L4: 
nop
