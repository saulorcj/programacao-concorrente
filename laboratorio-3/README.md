# Laboratório 3

> Saulo Roberto do Carmo Junger

| :exclamation: O número de threads usado foi 4 |
|-----------------------------------------------|

1. O programa foi testado através de uma página web externa e seu arquivo de saída corresponde a uma multiplicação das duas matrizes.
2. Tempo de execução médio de cada etapa para cada tamanho das matrizes:
   1. $500x500$
      1. Inicialização:  0,003093508 segundos
      2. Processamento:  0,141914167 segundos
      3. Finalização:    0,003927023 segundos
   2. $1000x1000$
      1. Inicialização:  0,008786080 segundos
      2. Processamento:  0,961216633 segundos
      3. Finalização:    0,003014069 segundos
   3. $2000x2000$
      1. Inicialização:  0,027136087 segundos
      2. Processamento:  8,470230000 segundos
      3. Finalização:    0,038849097 segundos
3. Ganho de desempenho para cada tamanho de matriz:
   1. $500x500 : \frac{0,390401519}{0,148934698} = 2,621293253$
   2. $1000x1000 : \frac{3,112957925}{0,973016782} = 3,199284928$ 
   3. $2000x2000 : \frac{25,497607343}{8,536215184} = 2,986992103$
4. É possível observar que em nenhum caso houve um ganho tão bom quanto o ganho teórico visto no laboratório anterior, que era muito próximo de 4. Entretanto, o ganho de performance, ainda sim, é muito notável e mais expressivo para matrizes com tamanhos maiores, como no caso das que tinham dimensões $1000$ e $2000$.