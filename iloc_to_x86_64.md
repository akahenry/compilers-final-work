# Tradução ILOC -> x86_64

x86_64 usa o registrador RAX como acumulador pra operações aritméticas e 8 registradores general purpose: R8-R15

## GAS

tem que ter cuidado quando for pesquisar instruções x86_64 pq tem duas sintaxes, e a sintaxe usada pelo gcc é a GAS (também chamado de AT&T), onde:
- precisa prefixar registradores com %
- precisa prefixar constantes com $
- a ordem dos parametros é invertida em relação ao intel
- as instruções podem ser sufixadas com letras pra indicar o tamanho do operando, mas isso não é necessário pq na ausência, o GAS infere a partir do operando de destino (ou, se tiver, operando de memória). Para referencia essas são as letras:
    - b (byte, 8-bit).
    - s (single, float 32-bit).
    - w (word, 16-bit).
    - l (long, int 32-bit ou float 64-bit)
    - q (quad, 64-bit)

## Comentarios importantes

- x86_64 usa um registrador específico pra acesso indireto à memória (pointer): BX. Então provavelmente em todos os lugares (nos mov) que tiver (r1) (acesso ao valor do endereço apontado por r1), r1 deve ser %EBX ou %RBX (EBX é 32-bit, RBX é 64-bit)
- constantes usadas no cálculo de endereço não devem ser prefixadas por $. por exemplo, em `mov 2(%EBX), %EAX`, 2 não é $2
- como tudo ocupa 4 bytes, supus que o acumulador usado é %EAX sempre (32-bit)
- em cada instrução x86 eu mencionei os registradores e labels da instrução ILOC, mas esses ainda tem que ser gerados, com base nos do ILOC
- nas comparações e jumps eu suponho que que false = 0 e true é qualquer valor diferente de 0

## Instruções ILOC que são usadas no nosso código intermediário com as respectivas instruções x86_64 análogas

### Acesso à memória

ILOC_INS_LOAD
    load r1 => r2
    `mov (r1), r2`

ILOC_INS_LOADI
    loadi c1 => r2
    `mov $1, r2`

ILOC_INS_LOADAI
    loadAI r1, c2 => r3
    `mov c2(r1), r3`

ILOC_INS_STOREAI
    storeAI r1 => r2, c3
    `mov r1, c3(r2)`

### Aritméticos

ILOC_INS_ADD
    add r1, r2 => r3
    `mov r1, %EAX`
    `add r2, %EAX`
    `mov %EAX, r3`

ILOC_INS_SUB
    sub r1, r2 => r3
    `mov r1, %EAX`
    `sub r2, %EAX`
    `mov %EAX, r3`

ILOC_INS_MULT
    mult r1, r2 => r3
    `mov r1, %EAX`
    `imul r2`
    `mov %EAX, r3`

ILOC_INS_DIV
    div r1, r2 => r3
    `mov r1, %EAX`
    `cltd` (chamado de CDQ em intel, extende o sinal de EAX pra EDX, necessário pra divisão com sinal)
    `idiv r2`
    `mov %EAX, r3`

ILOC_INS_ADDI
    addI r1, c2 => r3
    `mov r1, %EAX`
    `add c2, %EAX`
    `mov %EAX, r3`

ILOC_INS_RSUBI
    rsubI r1, c2 => r3
    `mov c2, %EAX`
    `sub r1, %EAX`
    `mov %EAX, r3`

ILOC_INS_I2I
    i2i r1 => r2
    `mov r1, r2`

### Branches

ILOC_INS_JUMPI
    jumpI -> l1
    `jmp l1`

ILOC_INS_JUMP
    jump -> r1
    `jmp *r1` (se não funcionar, tem um hack: `pushl %EAX` e depois `ret` hehe)


ILOC_INS_CBR
    cbr r1 -> l2, l3
    `cmp $0, r1`
    `je l3`
    `jne l2`

### Lógicos

ILOC_INS_XORI
    xorI r1, c2 => r3

ILOC_INS_CMP_EQ
    carregando as flags no AH, fazendo mask na AH e guardando AH em r3:
    `cmp r1, r2`
    `lahf` (carrega flags no AH)
    `and $0x40 %AH` (faz mask no AH pra deixar só o bit de zero)
    `mov %AH, r3` (talvez r3 precise ser um registrador 8 bits)

ILOC_INS_CMP_NE
    cmp_NE r1, r2 -> r3
    `mov r1, %EAX`
    `sub r2, %EAX`
    `mov %EAX, r3`

ILOC_INS_CMP_GE
    cmp_GE r1, r2 -> r3

ILOC_INS_CMP_LE
    cmp_LE r1, r2 -> r3

ILOC_INS_CMP_GT
    cmp_GT r1, r2 -> r3

ILOC_INS_CMP_LT
    cmp_LT r1, r2 -> r3


### Sem parametros

ILOC_INS_HALT
    halt
    `hlt`
ILOC_INS_NOP
    nop
    `nop`