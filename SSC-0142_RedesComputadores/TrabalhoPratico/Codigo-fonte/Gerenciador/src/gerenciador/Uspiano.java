package gerenciador;

public class Uspiano implements Comparable<Uspiano> {

	private int numeroUSP;
	private String nome;

	public Uspiano(int nUsp, String nome) {
		if(nome == null) {
			throw new NullPointerException();
		}
		this.numeroUSP = nUsp;
		this.nome = nome;
	}

	public Uspiano(String nUsp, String nome) {
		this(Integer.parseInt(nUsp), nome);
	}

	public int getNumeroUSP() {
		return this.numeroUSP;
	}

	public String getName() {
		return this.nome;
	}

	@Override
	public String toString() {
		return this.nome + " (NUSP " + this.numeroUSP + ")";
	}

	@Override
	public int compareTo(Uspiano a) {
		return this.getName().compareTo(a.getName());
	}

}
