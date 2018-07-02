import java.security.SecureRandom;
import java.math.BigInteger;


/**
 * Classe para criptografar e/ou descriptografar uma informação com uma chave RSA.
 *
 */
class Main{

	public static void main(String[] args){
		String msg, msgEncrypted, msgDecrypted;
		BigInteger p, q, n, m, d, e, exp;
		SecureRandom r;
		int bitLen;

		msg = "Esta é uma mensagem de teste, e o programa deve funcionar adequadamente... :)";
		bitLen = 2048;

		// Escolha de forma aleatória dois números primos grandes p e q
		r = new SecureRandom();
		p = new BigInteger(bitLen / 2, 100, r);
		q = new BigInteger(bitLen / 2, 100, r);

		// Compute n = p * q
		n = p.multiply(q);

		// Compute a função totiente phi(n) = (p -1) (q -1)
		m = (p.subtract(BigInteger.ONE)).multiply(q.subtract(BigInteger.ONE));

		// Escolha um inteiro "e", 1 < "e" < phi(n), "e" e phi(n) sejam primos entre si.
		e = new BigInteger("3");
		while(m.gcd(e).intValue() > 1)
			e = e.add(new BigInteger("2"));

		// d seja inverso multiplicativo de "e"
		d = e.modInverse(m);

		System.out.print("p: " + p);
		System.out.print(" | q: " + q);
		System.out.print(" | n: " + n);
		System.out.print(" | e: " + e);
		System.out.println(" | d: " + d);
		System.out.println();

		// Mensagem criptografada - RSA_encrypt():
		msgEncrypted = new BigInteger(msg.getBytes()).modPow(e, n).toString();
		System.out.println("Mensagem criptografada: " + msgEncrypted);

		// Mensagem decriptografada - RSA_decrypt():
		msgDecrypted = new String(new BigInteger(msgEncrypted).modPow(d, n).toByteArray());
		System.out.println("Mensagem decriptografada: " + msgDecrypted);
	}

}



