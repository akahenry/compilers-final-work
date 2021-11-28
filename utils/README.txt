/*
Grupo D
  Henry Bernardo Kochenborger de Avila
  Marcos Samuel Winkel Landi
*/
O grupo adotou a otimização por janela (nesse caso específico com janela de tamanho 3) para LOADAIs desnecessários entre dois STOREAIs.

Essa otimização realiza a seguinte transformação de:
```
STOREAI A => B, C
LOADAI B, C => D
STOREAI D => E, F
```

Para:
```
STOREAI A => B, C
STOREAI A => E, F
```

Como foi realizada sobre a linguagem intermediária ILOC e a tradução da linguagem ILOC para Assembly é feita de tal modo que qualquer instrução ILOC gera ao menos 1 instrução Assembly, conclui-se que essa otimização (redução de código) no resultado intermediário também será refletido como uma otimização (redução de código) no resultado em Assembly.

Para exemplificar, é necessário identificar situações em que isso ocorre. Os casos mais simples são relacionados ao uso de uma atribuição seguida de um `return` que faz o uso da variável atribuída e, portanto, serão utilizados como exemplo:
* Os arquivos ILOC de cada exemplo são dados por:
    - ex-N.iloc: código ILOC gerado sem otimização do exemplo N;
    - ex-N-optimized.iloc: código ILOC gerado com otimização do exemplo N;
* Os arquivos Assembly de cada exemplo são dados por:
    - ex-N.s: código Assembly gerado sem otimização do exemplo N;
    - ex-N-optimized.s: código Assembly gerado com otimização do exemplo N;

Exemplo 1:
```
int main()
{
  int a <= 0;
  int b <= 1;
  int c <= 6;

  if (a > 0)
  {
    a = b + c;
    return a;
  }
  else
  {
    a = c - b;
    return a;
  };
}
```

Neste caso, é possível verificar na linha 45 do arquivo ILOC que há uma economia de 1 instrução (essa que é dada por um acesso a memória para escrita em um registrador).
Já no arquivo Assembly, é possível verificar que na linha 204 do arquivo ASM há uma economia de 2 instruções.

Exemplo 2:
```
int a;

int recursive(int v)
{
  int t;
  if (a > 0)
  {
    a = a-1;
    t = v + recursive(v+1);
    return t;
  }
  else
  {
    t = v + 1;
    return t;
  };
}

int main()
{
  int b;
  a = 1000;

  b = recursive(1);
  return b;
}
```

Neste caso, é possível verificar na linha 60 do arquivo ILOC que há uma economia de 1 instrução.
Já no arquivo Assembly, é possível verificar que na linha 304 do arquivo ASM há uma economia de 2 instruções.

Em ambos os exemplos, há mais economia de código do que as apontadas. Entretanto, elas se comportam da mesma maneira que as mencionadas e não influenciam a questão da transformação gerar uma otimização visto que elas não adicionam novas instruções a serem executadas (apenas removem instruções).
