# Respostas das Atividades

> Saulo Roberto do Carmo Junger

---

1.  ### __Tarefa 1__
    1.  Os possíveis finais para a variável ```y``` são 0, 1 e 2, pois esses são os valores que podem estar armazenados em ```x``` quando a thread 3 for executada e alocar o valor dele na variável ```y```. Não é possível que a variável ```y``` termine com o valor 3, visto que nenhuma operação aritmética é feita sobre o valor de ```x``` ou ```y```.
    2.  Na maioria das vezes a variável ```y``` termina com o valor 0 e às vezes com o valor 2.
    3.  Alterei o código para que a thread 2 ocorra após a thread 1. Com essa mudança, a variàvel ```y``` passou a terminar às vezes com o valor 1.
    4.  O programa apresenta um problema de condição de corrida ruim, visto que a ordem em que as threads são executadas influenciam no resultado final obtido.
2.  ### __Tarefa 2__
    1.  A saída esperada é o valor 200.000.
    2.  Os resultados empressos variam muito. Não há um valor que seja mais recorrente.
    3.  Executei inúmeras vezes e em nenhum momento o resultado foi o esperado. Isso pode estar ocorrendo devido ao fato de serem muitas operações de leitura e escrita de memória ocorrendo simultaneamente entre as 2 threads. Isso acaba aumentando muito as chances de haver erros e o resultado quse sempre é incorreto.
    4.  O problema de condição de corrida é ruim e não é tolerável, visto que dificilmente obtemos o resultado correto.
3.  ### __Tarefa 3__
    1.  sa
    2.  sa
    3.  sa