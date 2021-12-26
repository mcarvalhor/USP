package main;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import leitor.Leitor;
import console.Console;

public class Main {

	private static final String ADDR_REGEX = "^\\s*([^:]+):([0-9]+)\\s*$";
	private static final Pattern ADDR_PATTERN = Pattern.compile(ADDR_REGEX);

	public static void main(String[] args) {
		Leitor l;
		Console c;
		Matcher m;
		String s;

		c = Console.getInstance();

		System.out.println("Bem-vinde ao Leitor de Cartao.");
		System.out.println();

		do {
			s = c.readLine("E qual o endereco do servidor do gerenciador? (formato = endereco:porta)");
			m = ADDR_PATTERN.matcher(s);
			if (s == null || !m.matches()) {
				System.out
						.println("Endereco invalido! Deve ser no formato endereco:porta. Exemplos: dominio.com:9000 ou 192.168.0.110:8500.");
			}
		} while (s == null || !m.matches());

		try {
			l = new Leitor(m.group(1).trim(), Integer.parseInt(m.group(2)));
			System.out.println("Conectado ao servidor do gerenciador.");
			System.out.println();
			l.execute();
		} catch (Exception ex) {
			System.err.println("Nao foi possivel conectar ao servidor. :(");
			System.err.println(ex);
			ex.printStackTrace();
		}

	}
}
