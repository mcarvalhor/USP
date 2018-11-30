import java.util.Scanner;
import java.lang.Integer;



/**
 * Classe de exceções
 */
class StackException extends Exception {

	@Override
	public String getMessage() {
		return "Erro!";
	}

}

class EmptyStackException extends StackException {
}

class FullStackException extends StackException {
}



/**
 * Classe Stack (Pilha)
 * 
 * @author Matheus (github.com/mcarvalhor)
 */
class Stack<E> {

	/**
	 * Tamanho fixo da pilha.
	 * 
	 */
	private static final int SIZE = 5;

	/**
	 * Array que representa a pilha.
	 * 
	 */
	private E[] elements;

	/**
	 * Número de elementos presentes na pilha.
	 * 
	 */
	private int n;

	/**
	 * Construtor padrão da pilha.
	 * 
	 */
	@SuppressWarnings("unchecked")
	public Stack() {
		this.elements = (E[]) new Object[SIZE];
		this.n = 0;
	}

	/**
	 * Esta função adiciona um elemento no topo da pilha (mais acima).
	 * 
	 * @param Elem o valor inteiro que será adicionado
	 * @throws FullStackException se a pilha estiver cheia e não tiver espaço para adicionar
	 */
	public void push(E Elem) throws FullStackException {
		if(this.n >= SIZE)
			throw new FullStackException();
		this.elements[this.n++] = Elem;
	}

	/**
	 * Esta função retorna o último elemento da pilha (mais acima).
	 * 
	 * @return o valor inteiro desse elemento
	 * @throws EmptyStackException se a pilha estiver vazia e não tiver o que retornar
	 */
	public E top() throws EmptyStackException {
		if(this.n < 1)
			throw new EmptyStackException();
		return this.elements[this.n - 1];
	}

	/**
	 * Esta função remove o último elemento da pilha (mais acima).
	 * 
	 * @throws EmptyStackException se a pilha estiver vazia e não tiver o que remover
	 */
	public void pop() throws EmptyStackException {
		if(this.n < 1)
			throw new EmptyStackException();
		n--;
	}

}



/**
 * Classe principal.
 * 
 * @author Matheus (github.com/mcarvalhor)
 */
class Main { /* Classe Teste: usado Main para poder ser executado no run.codes */

	/**
	 * Esta é a função principal do programa.
	 * 
	 * @param args os argumentos de linha de comando do programa
	 */
	public static void main(String[] args){
		Stack<Integer> elements;
		Scanner reader;
		String op;
		int i, n;

		reader = new Scanner(System.in);
		elements = new Stack<>();

		n = reader.nextInt();
		for(i = 0; i < n; i++){
			op = reader.next();
			try {
				switch(op) {
					case "I":
						elements.push(reader.nextInt());
					break;
					case "R":
						elements.pop();
					break;
					case "P":
						System.out.println(elements.top());
					break;
					default:
						System.out.println("Erro!");
				}
			} catch(EmptyStackException ex) {
				System.out.println(ex.getMessage());
				if(op.equals("R")) {
					System.out.println("Remocao Invalida!");
					System.exit(0);
				}
				System.out.println("Operacao Invalida!");
			} catch(StackException ex){
				System.out.println(ex.getMessage());
				System.out.println("Pilha cheia!");
				System.exit(0);
			}
		}

		reader.close();

	}

}


