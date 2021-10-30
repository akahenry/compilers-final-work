# Translation

## Programa Geral

"inicializa RFP, RSP e RBSS" (talvez opcional com a opção --restrict do simulador)

"pula para o endereço da main"
`jumpI <label da função main>`

## main

"reserva espaço para a variavel local x"
`rsp <= rsp + 4`

## Chamada de função

"calcula e salva o endereço de retorno"
`r1 <= rpc + 5 + 2*parametros`
`rsp[0] <= r1`

"salva rsp e rfp"
`rsp[4] <= rsp`
`rsp[8] <= rfp`

para cada parametro i in range(numero de parametros):
    "carrega valor do parametro em r0"
    `r0 <= <lugar com parametro>`
    "empilha o parametro"
    `rsp[12 + i*4] <= r0`

"salta para o inicio da função"
`jumpI <label da função>`

## Dentro da função

`<label da função>:`

"atualiza o RFP e o RSP"
`rsp <= rsp + 12 + i*4 + 4` onde i é o numero de parametros

para cada parametro i in range(numero de parametros):
    "obtém o parâmetro"
    `r<i> <= rfp + 12 + i*4`

`<executa as coisa de dentro da função>`

"registra o valor de retorno"
`rfp[12 + n*4] <= <lugar com resultado>` onde n é o numero de parametros

"obtém endereço de retorno"
`r0 <= rfp[0]`

"obtém e atualiza RSP e RFP salvos"
`r1 <= rfp[4]`
`r2 <= rfp[8]`
`rsp <= r1`
`rfp <= r2`

"salta para o endereço de retorno"
`jump r0`