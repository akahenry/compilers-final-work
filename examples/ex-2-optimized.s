.data
.globl a
.type a, @object
.size a, 4
a:
.long 0
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
.globl r22
.type r22, @object
.size r22, 8
r22:
.long 0
.globl r23
.type r23, @object
.size r23, 8
r23:
.long 0
.globl r24
.type r24, @object
.size r24, 8
r24:
.long 0
.globl r25
.type r25, @object
.size r25, 8
r25:
.long 0
.globl r26
.type r26, @object
.size r26, 8
r26:
.long 0
.globl r27
.type r27, @object
.size r27, 8
r27:
.long 0
.globl r28
.type r28, @object
.size r28, 8
r28:
.long 0
.globl r29
.type r29, @object
.size r29, 8
r29:
.long 0
.globl r30
.type r30, @object
.size r30, 8
r30:
.long 0
.globl r31
.type r31, @object
.size r31, 8
r31:
.long 0
.globl r32
.type r32, @object
.size r32, 8
r32:
.long 0
.globl r33
.type r33, @object
.size r33, 8
r33:
.long 0
.globl r34
.type r34, @object
.size r34, 8
r34:
.long 0
.globl r35
.type r35, @object
.size r35, 8
r35:
.long 0
.text
.globl L1
.type L1, @function
.globl main
.type main, @function
call main
L0: 
mov 16(%rsp), %rax
mov %rbp, %rsp
ret
hlt
L1: 
pop 0(%rsp)
mov %rsp, %rbp
add $-528, %rsp
mov r26(%rip), %rax
mov %rax, -496(%rbp)
mov r25(%rip), %rax
mov %rax, -480(%rbp)
mov r24(%rip), %rax
mov %rax, -464(%rbp)
mov r23(%rip), %rax
mov %rax, -448(%rbp)
mov r22(%rip), %rax
mov %rax, -432(%rbp)
mov r21(%rip), %rax
mov %rax, -416(%rbp)
mov r20(%rip), %rax
mov %rax, -400(%rbp)
mov r19(%rip), %rax
mov %rax, -384(%rbp)
mov r18(%rip), %rax
mov %rax, -368(%rbp)
mov r17(%rip), %rax
mov %rax, -352(%rbp)
mov r16(%rip), %rax
mov %rax, -336(%rbp)
mov r15(%rip), %rax
mov %rax, -320(%rbp)
mov r14(%rip), %rax
mov %rax, -304(%rbp)
mov r13(%rip), %rax
mov %rax, -288(%rbp)
mov r12(%rip), %rax
mov %rax, -272(%rbp)
mov r11(%rip), %rax
mov %rax, -256(%rbp)
mov r10(%rip), %rax
mov %rax, -240(%rbp)
mov r9(%rip), %rax
mov %rax, -224(%rbp)
mov r8(%rip), %rax
mov %rax, -208(%rbp)
mov r7(%rip), %rax
mov %rax, -192(%rbp)
mov r6(%rip), %rax
mov %rax, -176(%rbp)
mov r5(%rip), %rax
mov %rax, -160(%rbp)
mov r4(%rip), %rax
mov %rax, -144(%rbp)
mov r3(%rip), %rax
mov %rax, -128(%rbp)
mov r2(%rip), %rax
mov %rax, -112(%rbp)
mov r1(%rip), %rax
mov %rax, -96(%rbp)
mov r0(%rip), %rax
mov %rax, -80(%rbp)
mov a(%rip), %rax
mov %rax, r0(%rip)
movq $0, r1(%rip)
mov r0(%rip), %rbx
mov r1(%rip), %rax
cmpl %ebx, %eax
jl LCMP1
movq $0, r2(%rip)
jmp LCMP2
LCMP1: 
movq $1, r2(%rip)
LCMP2: 
cmpl $0, r2(%rip)
je L3
jne L2
L2: 
mov a(%rip), %rax
mov %rax, r3(%rip)
mov a(%rip), %rax
mov %rax, r4(%rip)
movq $1, r5(%rip)
mov r4(%rip), %rax
sub r5(%rip), %rax
mov %rax, r6(%rip)
mov r6(%rip), %rax
mov %rax, a(%rip)
mov -64(%rbp), %rax
mov %rax, r8(%rip)
mov -48(%rbp), %rax
mov %rax, r9(%rip)
mov -48(%rbp), %rax
mov %rax, r10(%rip)
movq $1, r11(%rip)
mov r10(%rip), %rax
add r11(%rip), %rax
mov %rax, r12(%rip)
mov %rsp, -16(%rsp)
mov %rbp, -32(%rsp)
mov r12(%rip), %rax
mov %rax, -48(%rsp)
call L1
mov -32(%rbp), %rbp
mov %rax, r13(%rip)
mov r9(%rip), %rax
add r13(%rip), %rax
mov %rax, r15(%rip)
mov r15(%rip), %rax
mov %rax, -64(%rbp)
mov r15(%rip), %rax
mov %rax, 16(%rsp)
mov 0(%rbp), %rax
mov %rax, r18(%rip)
mov -352(%rbp), %rax
mov %rax, r17(%rip)
mov -336(%rbp), %rax
mov %rax, r16(%rip)
mov -320(%rbp), %rax
mov %rax, r15(%rip)
mov -304(%rbp), %rax
mov %rax, r14(%rip)
mov -288(%rbp), %rax
mov %rax, r13(%rip)
mov -272(%rbp), %rax
mov %rax, r12(%rip)
mov -256(%rbp), %rax
mov %rax, r11(%rip)
mov -240(%rbp), %rax
mov %rax, r10(%rip)
mov -224(%rbp), %rax
mov %rax, r9(%rip)
mov -208(%rbp), %rax
mov %rax, r8(%rip)
mov -192(%rbp), %rax
mov %rax, r7(%rip)
mov -176(%rbp), %rax
mov %rax, r6(%rip)
mov -160(%rbp), %rax
mov %rax, r5(%rip)
mov -144(%rbp), %rax
mov %rax, r4(%rip)
mov -128(%rbp), %rax
mov %rax, r3(%rip)
mov -112(%rbp), %rax
mov %rax, r2(%rip)
mov -96(%rbp), %rax
mov %rax, r1(%rip)
mov -80(%rbp), %rax
mov %rax, r0(%rip)
mov 16(%rsp), %rax
mov %rbp, %rsp
push 0(%rbp)
ret
jmp L4
L3: 
mov -64(%rbp), %rax
mov %rax, r19(%rip)
mov -48(%rbp), %rax
mov %rax, r20(%rip)
movq $1, r21(%rip)
mov r20(%rip), %rax
add r21(%rip), %rax
mov %rax, r22(%rip)
mov r22(%rip), %rax
mov %rax, -64(%rbp)
mov r22(%rip), %rax
mov %rax, 16(%rsp)
mov 0(%rbp), %rax
mov %rax, r25(%rip)
mov -464(%rbp), %rax
mov %rax, r24(%rip)
mov -448(%rbp), %rax
mov %rax, r23(%rip)
mov -432(%rbp), %rax
mov %rax, r22(%rip)
mov -416(%rbp), %rax
mov %rax, r21(%rip)
mov -400(%rbp), %rax
mov %rax, r20(%rip)
mov -384(%rbp), %rax
mov %rax, r19(%rip)
mov -368(%rbp), %rax
mov %rax, r18(%rip)
mov -352(%rbp), %rax
mov %rax, r17(%rip)
mov -336(%rbp), %rax
mov %rax, r16(%rip)
mov -320(%rbp), %rax
mov %rax, r15(%rip)
mov -304(%rbp), %rax
mov %rax, r14(%rip)
mov -288(%rbp), %rax
mov %rax, r13(%rip)
mov -272(%rbp), %rax
mov %rax, r12(%rip)
mov -256(%rbp), %rax
mov %rax, r11(%rip)
mov -240(%rbp), %rax
mov %rax, r10(%rip)
mov -224(%rbp), %rax
mov %rax, r9(%rip)
mov -208(%rbp), %rax
mov %rax, r8(%rip)
mov -192(%rbp), %rax
mov %rax, r7(%rip)
mov -176(%rbp), %rax
mov %rax, r6(%rip)
mov -160(%rbp), %rax
mov %rax, r5(%rip)
mov -144(%rbp), %rax
mov %rax, r4(%rip)
mov -128(%rbp), %rax
mov %rax, r3(%rip)
mov -112(%rbp), %rax
mov %rax, r2(%rip)
mov -96(%rbp), %rax
mov %rax, r1(%rip)
mov -80(%rbp), %rax
mov %rax, r0(%rip)
mov 16(%rsp), %rax
mov %rbp, %rsp
push 0(%rbp)
ret
L4: 
nop
main:
mov %rsp, %rbp
add $-656, %rsp
mov r35(%rip), %rax
mov %rax, -624(%rbp)
mov r34(%rip), %rax
mov %rax, -608(%rbp)
mov r33(%rip), %rax
mov %rax, -592(%rbp)
mov r32(%rip), %rax
mov %rax, -576(%rbp)
mov r31(%rip), %rax
mov %rax, -560(%rbp)
mov r30(%rip), %rax
mov %rax, -544(%rbp)
mov r29(%rip), %rax
mov %rax, -528(%rbp)
mov r28(%rip), %rax
mov %rax, -512(%rbp)
mov r27(%rip), %rax
mov %rax, -496(%rbp)
mov r26(%rip), %rax
mov %rax, -480(%rbp)
mov r25(%rip), %rax
mov %rax, -464(%rbp)
mov r24(%rip), %rax
mov %rax, -448(%rbp)
mov r23(%rip), %rax
mov %rax, -432(%rbp)
mov r22(%rip), %rax
mov %rax, -416(%rbp)
mov r21(%rip), %rax
mov %rax, -400(%rbp)
mov r20(%rip), %rax
mov %rax, -384(%rbp)
mov r19(%rip), %rax
mov %rax, -368(%rbp)
mov r18(%rip), %rax
mov %rax, -352(%rbp)
mov r17(%rip), %rax
mov %rax, -336(%rbp)
mov r16(%rip), %rax
mov %rax, -320(%rbp)
mov r15(%rip), %rax
mov %rax, -304(%rbp)
mov r14(%rip), %rax
mov %rax, -288(%rbp)
mov r13(%rip), %rax
mov %rax, -272(%rbp)
mov r12(%rip), %rax
mov %rax, -256(%rbp)
mov r11(%rip), %rax
mov %rax, -240(%rbp)
mov r10(%rip), %rax
mov %rax, -224(%rbp)
mov r9(%rip), %rax
mov %rax, -208(%rbp)
mov r8(%rip), %rax
mov %rax, -192(%rbp)
mov r7(%rip), %rax
mov %rax, -176(%rbp)
mov r6(%rip), %rax
mov %rax, -160(%rbp)
mov r5(%rip), %rax
mov %rax, -144(%rbp)
mov r4(%rip), %rax
mov %rax, -128(%rbp)
mov r3(%rip), %rax
mov %rax, -112(%rbp)
mov r2(%rip), %rax
mov %rax, -96(%rbp)
mov r1(%rip), %rax
mov %rax, -80(%rbp)
mov r0(%rip), %rax
mov %rax, -64(%rbp)
mov a(%rip), %rax
mov %rax, r27(%rip)
movq $1000, r28(%rip)
mov r28(%rip), %rax
mov %rax, a(%rip)
mov -48(%rbp), %rax
mov %rax, r30(%rip)
movq $1, r31(%rip)
mov %rsp, -16(%rsp)
mov %rbp, -32(%rsp)
mov r31(%rip), %rax
mov %rax, -48(%rsp)
call L1
mov -32(%rbp), %rbp
mov %rax, r32(%rip)
mov r32(%rip), %rax
mov %rax, -48(%rbp)
mov r32(%rip), %rax
mov %rax, 16(%rsp)
mov -608(%rbp), %rax
mov %rax, r34(%rip)
mov -592(%rbp), %rax
mov %rax, r33(%rip)
mov -576(%rbp), %rax
mov %rax, r32(%rip)
mov -560(%rbp), %rax
mov %rax, r31(%rip)
mov -544(%rbp), %rax
mov %rax, r30(%rip)
mov -528(%rbp), %rax
mov %rax, r29(%rip)
mov -512(%rbp), %rax
mov %rax, r28(%rip)
mov -496(%rbp), %rax
mov %rax, r27(%rip)
mov -480(%rbp), %rax
mov %rax, r26(%rip)
mov -464(%rbp), %rax
mov %rax, r25(%rip)
mov -448(%rbp), %rax
mov %rax, r24(%rip)
mov -432(%rbp), %rax
mov %rax, r23(%rip)
mov -416(%rbp), %rax
mov %rax, r22(%rip)
mov -400(%rbp), %rax
mov %rax, r21(%rip)
mov -384(%rbp), %rax
mov %rax, r20(%rip)
mov -368(%rbp), %rax
mov %rax, r19(%rip)
mov -352(%rbp), %rax
mov %rax, r18(%rip)
mov -336(%rbp), %rax
mov %rax, r17(%rip)
mov -320(%rbp), %rax
mov %rax, r16(%rip)
mov -304(%rbp), %rax
mov %rax, r15(%rip)
mov -288(%rbp), %rax
mov %rax, r14(%rip)
mov -272(%rbp), %rax
mov %rax, r13(%rip)
mov -256(%rbp), %rax
mov %rax, r12(%rip)
mov -240(%rbp), %rax
mov %rax, r11(%rip)
mov -224(%rbp), %rax
mov %rax, r10(%rip)
mov -208(%rbp), %rax
mov %rax, r9(%rip)
mov -192(%rbp), %rax
mov %rax, r8(%rip)
mov -176(%rbp), %rax
mov %rax, r7(%rip)
mov -160(%rbp), %rax
mov %rax, r6(%rip)
mov -144(%rbp), %rax
mov %rax, r5(%rip)
mov -128(%rbp), %rax
mov %rax, r4(%rip)
mov -112(%rbp), %rax
mov %rax, r3(%rip)
mov -96(%rbp), %rax
mov %rax, r2(%rip)
mov -80(%rbp), %rax
mov %rax, r1(%rip)
mov -64(%rbp), %rax
mov %rax, r0(%rip)
mov 16(%rsp), %rax
mov %rbp, %rsp
ret
