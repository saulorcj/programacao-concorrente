import java.util.ArrayList;

class Monitor {
    private int qt_par;

    public Monitor() {
        this.qt_par = 0;
    }

    public synchronized void IncSePar(int numero){
        if (numero % 2 == 0)
            this.qt_par++;
    }

    public synchronized  int getQtPar(){
        return this.qt_par;
    }
}

class T1 extends Thread{
    private int id;
    private int qt_threads;
    Monitor monitor;

    ArrayList<Integer> lista_numeros;

    public T1(int tid, Monitor monitor, int qt_threads, ArrayList<Integer> lista_numeros){
        this.id = tid;
        this.monitor = monitor;
        this.qt_threads = qt_threads;
        this.lista_numeros = lista_numeros;
    }

    public void run(){
        System.out.println("Thread " + this.id + " iniciou!");
        for(int i = this.id; i < this.lista_numeros.size(); i += this.qt_threads){
            this.monitor.IncSePar(lista_numeros.get(i));
        }
        System.out.println("Thread " + this.id + " terminou!");
    }
}

class QuantidadePar{
    // Quantidade de Threads
    static final int N = 6;
    // Quantidade de números no vetor
    static final int Q = 200;

    public static void main(String[] args){
        // vetor de threads
        Thread[] threads = new Thread[N];
        // vetor de números
        ArrayList<Integer> lista_numeros = new ArrayList<>();
        // monitor
        Monitor monitor = new Monitor();

        // Preenchendo a lista de números
        for(int i = 0; i < Q; i++){
            lista_numeros.add(i);
        }

        // Instanciando as threads
        for(int i = 0; i < threads.length; i++){
            threads[i] = new T1(i, monitor, N, lista_numeros);
        }

        // Iniciando as threads
        for(int i = 0; i < threads.length; i++){
            threads[i].start();
        }

        // Encerrando as threads
        for(int i = 0; i < threads.length; i++){
            try {
                threads[i].join();
            } catch (InterruptedException e) { return; }
        }

        // A quantidade de pares deve ser igual a metade da quantidade de números na lista
        System.out.println("Quantidade de pares = " + monitor.getQtPar());
    }
}
