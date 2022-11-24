import com.sun.jdi.FloatValue;

import java.util.ArrayList;

import static java.lang.Thread.sleep;

class Buffer {
    ArrayList<Float> buffer; // buffer
    int in, out; // índices de inserção e retirada
    int tam_preenchido; // quantidade de itens presentes no buffer
    int tam_buffer; // espaço total do buffer


    public Buffer(int tam_buffer){ // construtor
        this.tam_buffer = tam_buffer;
        this.buffer = new ArrayList<>(this.tam_buffer);
        for (int i = 0; i < tam_buffer; i++){ // inicializa o array com zeros
            this.buffer.add((float) 0);
        }
        this.in = 0;
        this.out = 0;
        this.tam_preenchido = 0;
    }

    public synchronized void Insere(int id, Float item){
        try {
            while (this.tam_preenchido >= this.tam_buffer) { // enquanto buffer estiver cheio espera
                System.out.println("T[" + id + "] produtora está em espera");
                wait();
            }

            // insere o item
            this.buffer.set(this.in, item);
            // atualiza o índice de inserção
            this.in = (this.in + 1) % this.tam_buffer;
            // incrementa a quantidade de itens dentro do buffer
            this.tam_preenchido++;
            // notifica que a thread consumidora pode executar
            this.notify();
            // imprime o item inserido
            System.out.println("T[" + id + "] produtora inseriu item = " + item);
            // imprime o tamanho utilizado no buffer
            System.out.println("tam_buffer_preenchido = " + this.tam_preenchido);
            // imprime os elementos presentes no buffer
            //this.ImprimirBuffer();
        } catch (InterruptedException e) {
            System.out.println("T[" + id +"] produtora sofreu exceção na função Insere");
        }
    }

    public synchronized void Retira(int id) {
        try {
            while (this.tam_preenchido == 0) { // enquanto buffer estiver vazio espera
                System.out.println("T[" + id + "] consumidora está em espera");
                wait();
            }

            // pega item do buffer
            Float item = this.buffer.get(this.out);
            // atualiza índice de retirada
            this.out = (this.out + 1) % this.tam_buffer;
            // atualiza quantidade de itens no buffer
            this.tam_preenchido--;
            // notifica thread produtora
            this.notify();
            // imprime item retirado
            System.out.println("T[" + id + "] consumidora retirou item = " + item);
            // imprime tamanho utilizado no buffer
            System.out.println("tam_buffer_preenchido = " + this.tam_preenchido);
            //this.ImprimirBuffer();
        } catch (InterruptedException e) {
            System.out.println("T[" + id + "] consumidora sofreu exceção na função Retira");
        }
    }

    public synchronized void ImprimirBuffer(){ // imprime itens do buffer
        for(Float item: this.buffer){
            System.out.println(item);
        }
    }
}

class Produtor extends Thread{
    int id;
    int delayTime;
    Buffer buffer;
    ArrayList<Float> listaInsercao; // lista de itens que serão inseridos

    public Produtor(int id, int delayTime, int tam_processamento, Buffer buffer){
        this.id = id;
        this.delayTime = delayTime;
        this.buffer = buffer;
        this.listaInsercao = new ArrayList<>(tam_processamento);
        for(int i = 0; i < tam_processamento; i++) {
            this.listaInsercao.add((float) i + this.id * 10);
        }
    }

    public void run(){
        try {
            for (Float valor: this.listaInsercao) { // insere os itens presentes na lista
                this.buffer.Insere(this.id, valor);
                sleep(this.delayTime);
            }
        } catch (InterruptedException e) {
            System.out.println("T[" + this.id + "] produtora sofreu exceção na execução");
        }
    }
}

class Consumidor extends Thread{
    int id;
    int delayTime;
    int tam_processamento; // quantidade de itens a serem consumidos
    Buffer buffer;

    public Consumidor(int id, int delayTime, int tam_processamento, Buffer buffer){
        this.id = id;
        this.delayTime = delayTime;
        this.buffer = buffer;
        this.tam_processamento = tam_processamento;
    }

    public void run(){
        try {
            for(int i = 0; i < this.tam_processamento; i++){ // consome todos os itens
                this.buffer.Retira(this.id);
                sleep(this.delayTime);
            }
        } catch (InterruptedException e) {
            System.out.println("T[" + this.id + "] consumidora sofreu exceção na execução");
        }
    }
}

public class ThreadSafe{
    static final int PROD = 4; // quantidade de produtores
    static final int CONS = 4; // quantidade de consumidores
    static final int TAM_BUFFER = 5; // tamanho total do buffer
    static final int TAM_PROCESSAMENTO = 10; // quantidade de itens a serem processados

    public static void main(String[] args){
        Buffer buffer = new Buffer(TAM_BUFFER);
        Produtor[] p = new Produtor[PROD];
        Consumidor[] c = new Consumidor[CONS];

        for(int i = 0; i < PROD; i++){
            p[i] = new Produtor(i, 1, TAM_PROCESSAMENTO, buffer);
            p[i].start();
        }

        for(int i = 0; i < CONS; i++){
            c[i] = new Consumidor(i, 1, TAM_PROCESSAMENTO, buffer);
            c[i].start();
        }

        try { // espera as threads terminarem
            for(int i = 0; i < PROD; i++){
                p[i].join();
            }
            for(int i = 0; i < CONS; i++){
                c[i].join();
            }
        } catch (InterruptedException e) {
            System.out.println("Função main teve exceção");
        }
    }
    /* Resposta da Atividade 2
    * (a) No caso de 1 produtor e 1 consumidor foi possível notar que
    * dificilmente alguma thread entrava em modo de espera, ambas
    * executaram sem interrupção
    * (b) No caso de 1 produtor e 4 consumidores foi possível notar
    * que muitos consumidores ficavam em modo espera recorrentemente,
    * enquanto o produtor nunca entrava nesse estado, visto que os
    * itens eram retirados do buffer constantemente. Por fim, a única
    * thread produtora não foi capaz de liberar todas as threads
    * consumidoras, portanto o programa foi incapaz de encerrar sua
    * execução, permanecendo algumas threads consumidoras ainda
    * executando.
    * (c) No caso de 4 produtores e 1 consumidor a situação foi
    * exatamente como no caso anterior, porém com o produtor e
    * consumidor em situações inversas.
    * (d) no caso de 4 produtores e 4 consumidores a situação foi bem
    * equilibrada, com algumas threads entrando em modo espera algumas
    * vezes, porém todas conseguiram terminar sua execução normalmente.
    * */
}
