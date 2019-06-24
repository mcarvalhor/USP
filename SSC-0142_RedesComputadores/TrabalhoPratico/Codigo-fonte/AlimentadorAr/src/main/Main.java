package main;

import alimentador.Alimentador;
import java.util.regex.*;
import console.Console;

public class Main {

	private static final String ADDR_REGEX = "^\\s*([^:]+):([0-9]+)\\s*$";
	private static final Pattern ADDR_PATTERN = Pattern.compile(ADDR_REGEX);

	public static void main(String[] args) {
		Alimentador a;
		Console c;
		Matcher m;
		String s;
		int p;

		c = Console.getInstance();

		System.out.println("Bem-vinde ao Alimentador do Ar-condicionado.");
		System.out.println();

		do {
			p = c.readInt("Em qual porta o servidor do alimentador deve iniciar? (recomendado = 9001-9999)");

			if (p < 1 || p > 65535) {
				System.out
						.println("Porta invalida! Deve ser um valor entre 1-65535.");
			}
		} while (p < 1 || p > 65535);

		do {
			s = c.readLine("E qual o endereco do servidor do gerenciador? (formato = endereco:porta)");
			m = ADDR_PATTERN.matcher(s);
			if (s == null || !m.matches()) {
				System.out
						.println("Endereco invalido! Deve ser no formato endereco:porta. Exemplos: dominio.com:9000 ou 192.168.0.110:8500.");
			}
		} while (s == null || !m.matches());

		try {
			a = new Alimentador(m.group(1).trim(),
					Integer.parseInt(m.group(2)), p);
			System.out.println("O servidor esta hospedado em: "
					+ a.getHostAddress() + ":" + a.getPort());
			a.execute();
		} catch (Exception ex) {
			System.err.println("Nao foi possivel iniciar o servidor. :(");
			System.err.println(ex);
			ex.printStackTrace();
		}

	}

}
