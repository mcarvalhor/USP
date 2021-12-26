import java.util.Scanner;
import java.util.Arrays;
import java.io.*;

class UsuarioNaoAchadoException extends Exception {

	@Override
	public String getMessage() {
		return "Usuário não existe no banco de dados.";
	}

}


class UsuarioExistenteException extends Exception {

	@Override
	public String getMessage() {
		return "Usuário já existe no banco de dados.";
	}

}


class SenhaIncorretaException extends Exception {

	@Override
	public String getMessage() {
		return "Senha do usuário está incorreta.";
	}

}


/**
 * Classe que representa um usuário do sistema.
 * 
 * @author Matheus (github.com/mcarvalhor)
 */
class Usuario implements Serializable {

	/**
	 * Dados do usuário.
	 * 
	 */
	public String
		nomeUsuario,
		senha,
		nomeCompleto;

	/**
	 * Construtor padrão.
	 * 
	 * @param nomeCompleto nome completo do usuário
	 * @param nomeUsuario nome de usuário único (login)
	 * @param senha senha do usuário
	 */
	public Usuario(String nomeCompleto, String nomeUsuario, String senha){
		this.nomeCompleto = nomeCompleto;
		this.nomeUsuario = nomeUsuario;
		this.senha = senha;
	}

	/**
	 * Construtor que busca o usuário de um arquivo binário (de objetos).
	 * 
	 * @param nomeUsuario nome de usuário (login)
	 * @param senha senha para tentar login
	 * @param arquivoObjetos arquivo onde estão os usuários
	 */
	public Usuario(String nomeUsuario, String senha, ObjectInputStream arquivoObjetos) throws SenhaIncorretaException, UsuarioNaoAchadoException, IOException, ClassNotFoundException {
		Usuario[] usuarioAux;
		int i;
		usuarioAux = (Usuario[]) arquivoObjetos.readObject();
		for(i = 0; i < usuarioAux.length; i++) {
			if(usuarioAux[i].nomeUsuario.equals(nomeUsuario)) {
				if(!usuarioAux[i].credenciar(senha))
					throw new SenhaIncorretaException();
				this.nomeCompleto = usuarioAux[i].nomeCompleto;
				this.nomeUsuario = usuarioAux[i].nomeUsuario;
				this.senha = usuarioAux[i].senha;
				return;
			}
		}
		throw new UsuarioNaoAchadoException();
	}

	/**
	 * Construtor que busca o usuário de um arquivo de texto.
	 * 
	 * @param nomeUsuario nome de usuário (login)
	 * @param senha senha para tentar login
	 * @param arquivoTexto arquivo onde estão os usuários
	 */
	public Usuario(String nomeUsuario, String senha, BufferedReader arquivoTexto) throws SenhaIncorretaException, UsuarioNaoAchadoException, IOException {
		String usuarioAux, nomeAux;
		Usuario aux;
		int i;
		while((usuarioAux = arquivoTexto.readLine()) != null){
			if(usuarioAux.split(" ")[0].equals(nomeUsuario)) {
				nomeAux = usuarioAux.split(" ")[2];
				for(i = 3; i < usuarioAux.split(" ").length; i++){
					nomeAux += " " + usuarioAux.split(" ")[i];
				}
				aux = new Usuario(nomeAux, usuarioAux.split(" ")[0], usuarioAux.split(" ")[1]);
				if(!aux.credenciar(senha))
					throw new SenhaIncorretaException();
				this.nomeCompleto = aux.nomeCompleto;
				this.nomeUsuario = aux.nomeUsuario;
				this.senha = aux.senha;
				return;
			}
		}
		throw new UsuarioNaoAchadoException();
	}

	/**
	 * Tenta credenciar um usuário através de uma senha.
	 * 
	 * @param senha senha para credenciar usuário
	 * @return true se a senha estiver correta ou false caso contrário
	 */
	public boolean credenciar(String senha) {
		//senha = senha;
		if(this.senha.equals(senha))
			return true;
		return false;
	}

	/**
	 * Salva o usuário (this) em um arquivo binário (de objetos).
	 * 
	 * @param arquivoEntrada arquivo onde já possuem usuários salvos (leitura)
	 * @param arquivoSaida arquivo em que os usuários já salvos e o usuário atual serão salvos (escrita)
	 */
	public void cadastrar(ObjectInputStream arquivoEntrada, ObjectOutputStream arquivoSaida) throws UsuarioExistenteException, IOException, ClassNotFoundException {
		Usuario[] usuarioAux;
		int i;
		if(arquivoEntrada != null){
			usuarioAux = (Usuario[]) arquivoEntrada.readObject();
			for(i = 0; i < usuarioAux.length; i++)
				if(usuarioAux[i].nomeUsuario.equals(this.nomeUsuario))
					throw new UsuarioExistenteException();
		}else
			usuarioAux = new Usuario[0];
		usuarioAux = Arrays.copyOf(usuarioAux, usuarioAux.length + 1);
		usuarioAux[usuarioAux.length - 1] = this;
		arquivoSaida.writeObject(usuarioAux);
	}

	/**
	 * Salva o usuário (this) em um arquivo de texto.
	 * 
	 * @param arquivoEntrada arquivo onde já possuem usuários salvos (leitura)
	 * @param arquivoSaida arquivo em que os usuários já salvos e o usuário atual serão salvos (escrita)
	 */
	public void cadastrar(BufferedReader arquivoEntrada, BufferedWriter arquivoSaida) throws UsuarioExistenteException, IOException {
		String usuarioAux;
		if(arquivoEntrada != null){
			while((usuarioAux = arquivoEntrada.readLine()) != null)
				if(usuarioAux.split(" ")[0].equals(this.nomeUsuario))
					throw new UsuarioExistenteException();
		}
		usuarioAux = this.nomeUsuario + " " + this.senha + " " + this.nomeCompleto;
		arquivoSaida.write(usuarioAux);
	}

}



/**
 * Classe principal do programa.
 * 
 * @author Matheus (github.com/Matheus)
 */
class Main {

	String nomeArquivoObjetos = "usuarios.obj.dat", nomeArquivoTexto = "usuarios.txt.dat";

	/**
	 * Função principal do programa.
	 * 
	 * @param args argumentos de linha de comando
	 */
	public static void main(String[] args) throws IOException, ClassNotFoundException {
		String nomeCompleto, nomeUsuario, senha;
		ObjectOutputStream escritorObjetos;
		ObjectInputStream leitorObjetos;
		Usuario usuarioAux;
		Console console;
		Scanner reader;
		int op;

		if(args.length != 2) {
			System.out.println("Uso do programa:\n\t" + Main.class.getName() + " (TIPO_DE_ARQUIVO) (NOME_ARQUIVO)\nOnde:\n\t(TIPO_DE_ARQUIVO) -> 0 para arquivo de objetos ou 1 para arquivo de texto\n\t(NOME_ARQUIVO) -> nome do arquivo a ser usado");
			System.exit(-1);
		}

		console = System.console();
		if(console == null) {
			System.out.println("Erro ao carregar o console!");
			System.exit(-1);
		}

		reader = new Scanner(System.in);

		System.out.println("Bem-vind@ ao programa de login.\n");
		System.out.println("== Principal ==");
		System.out.println("  1. Cadastrar usuário");
		System.out.println("  2. Logar usuário");
		System.out.println(" -1. Sair");
		System.out.print(" Entre com a operação > ");
		op = reader.nextInt();

		while(op != -1){
			switch(op){
				case 1:
					System.out.println("\n\t== Cadastro ==");
					System.out.print("\tEntre com um nome de usuário > ");
					nomeUsuario = reader.next();
					System.out.print("\tEntre com o seu nome completo > ");
					nomeCompleto = reader.next();
					nomeCompleto += reader.nextLine();
					System.out.print("\tEntre com uma senha > ");
					senha = new String(console.readPassword());
					usuarioAux = new Usuario(nomeCompleto, nomeUsuario, senha);
					leitorObjetos = null;
					escritorObjetos = null;
					try {
						try {
							leitorObjetos = new ObjectInputStream(new BufferedInputStream(new FileInputStream(nomeArquivoObjetos)));
						} catch(Exception e){
							leitorObjetos = null;
						}
						escritorObjetos = new ObjectOutputStream(new BufferedOutputStream(new FileOutputStream(nomeArquivoObjetos)));
						usuarioAux.cadastrar(leitorObjetos, escritorObjetos);
					} catch(UsuarioExistenteException e){
						System.out.println("Usuário já existe!");
					} finally {
						if(leitorObjetos != null){
							leitorObjetos.close();
						}
						if(escritorObjetos != null){
							escritorObjetos.close();
						}
					}
				break;
				case 2:
					System.out.println("\n\t== Login ==");
					System.out.print("\tEntre com seu nome de usuário > ");
					nomeUsuario = reader.next();
					System.out.print("\tEntre com sua senha > ");
					senha = new String(console.readPassword());
					leitorObjetos = null;
					try {
						leitorObjetos = new ObjectInputStream(new BufferedInputStream(new FileInputStream(nomeArquivoObjetos)));
						usuarioAux = new Usuario(nomeUsuario, senha, leitorObjetos);
						System.out.println("\tBem-vind@ ao sistema, " + usuarioAux.nomeCompleto + "! Fazendo logoff...");
					} catch(UsuarioNaoAchadoException|FileNotFoundException e) {
						System.out.println("\tUsuário não cadastrado!");
					} catch(SenhaIncorretaException e) {
						System.out.println("\tSenha incorreta!");
					} finally {
						if(leitorObjetos != null){
							leitorObjetos.close();
						}
					}
				break;
				default:
					System.out.println("Operação inválida!");
			}
			System.out.println("\n== Principal ==");
			System.out.println("  1. Cadastrar usuário");
			System.out.println("  2. Logar usuário");
			System.out.println(" -1. Sair");
			System.out.print(" Entre com a operação > ");
			op = reader.nextInt();
		}

		System.out.println("Até logo!");
		reader.close();
	}

}


