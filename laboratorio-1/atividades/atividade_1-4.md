# Respostas das Atividades 1 até 4

> Saulo Roberto do Carmo Junger

---

1. O programa ```hello.c``` cria 10 threads para executarem a função ```PrintHello``` e, ao final, tmabém usa o ```printf``` para informar o término da função principal. Olhando o código, é possível observar que, embora a ordem de criação das threads siga um único padrão (1,2,3,4,5...), a execução de suas funções não necessariamente ocorrem logo após as threads serem criadas. Isso ocorre, pois o tempo de execução das threads não depende da ordem em que elas forem criadas no código, sendo mais influenciado pela manipulação do sistema operacional e características do hardware.

2. O programa ```hello_arg.c``` é muito semelhando ao ```hello.c```, entretanto a função ```PrintHello``` recebe como parâmetro um inteiro informando qual thread está sendo executada e o exibe. Com esse programa, é possível ver melhor como que o tempo de execucão de uma thread é independente das demais, podendo a primeira thread a ser criada executar sua função após a terceira, mesmo que essa tenha sido criada após ela. Como dito anteriormente, isso ocorre pois a ordem de execução das funções não é dito no código e sim influenciado pelo sistema operacional e hardware.

3. O programa ```hello_args.c``` se difere do anterior, pois ele passa para a função mais de um argumento e ele faz isso através de uma nova estrutura de dados que o programa criou através do struct. Nessa nova estrutura, chamada de ```t_Args```, há dois atributos que farão o papel das duas entradas da função. Portanto, a função não irá receber diretamente duas entradas, mas sim uma estrutura de dados única que contém dois atributos.

4. O programa ```hello_join.c``` usa a função ```pthread_join``` antes do término da função principal para garantir que as threads secundárias sejam executadas todas antes do ```printf``` final da função principal. Para isso, ele percorre o vetor contendo os identificadores de cada thread e chama a função ```pthread_join```.