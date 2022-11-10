# Respostas das atividades

> Saulo Roberto do Carmo Junger

1. HelloThread.java
   1. Pelo que entendi, o programa consiste em ançar várias threads, na qual cada uma é responsável porimprimir uma mensagem contendo seu id.
   2. Feito.
   3. Sim, a ordem de execução das threads se altera. Isso ocorre, pois a execução de cada thread depende do funcionamento da cpu e do seu gerenciamento pelo sistema operacional, não respeitando a ordem presente no código.
   4. Feito.
   5. Com a alteração, a thread principal, da função main, passou a se encerrar somente quando todas as outras threads encerram. Isso é devido ao uso do método join, que espera o encerramento da thread.
2. OlaThread.java
   1. Nesse novo código a classe que implementa o método executado pela thread não mais usa a interface Runnable, mas sim é uma classe filha da classe Thread.
   2. Não houve mudança significativa na impressão das threads.
3. TIncrementoBase.java
   1. O programa tem como objetivo incrementar a variável r em 1 100.000 vezes por thread. Então, se há duas threads, esse incremento será feito pelas duas na mesma quantidade. A seção crítica são os momentos onde escrevemos na variável r e lemos dela. A saída esperada é 200.000, visto que á duas threads e cada uma incrementará 1 na variável 100.000 vezes.
   2. Os valores impressos não condizem com o esperado, visto que a função que executa a seção crítica do código, de escrita, não está 'protegida' com um comando que não permita sua execução ao mesmo tempo por mais de uma thread.
4. TIncrementoBase.java
   1. Feito.
   2. Feito.
   3. Os valores impressos foram sempre os esperados, pois agora as funções que executam as seções críticas do código estão com o comando synchronized, que evita que elas sejam executadas ao mesmo tempo por mais de uma thread.
5. O programa com a solução é o ContadorPar.java