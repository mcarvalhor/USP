package console;

import java.math.BigDecimal;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Classe de leitura de dados
 * @author Carolina Aarenas Okawa
 *
 */
public class Console {
	
	private static Console console;
	private SimpleDateFormat dateFormatter;
	
	/**
	 * Padrão de projeto Singleton
	 * @return a instância da classe Console
	 */
	public static Console getInstance() {
		if (console == null)
			console = new Console();
		
		return console;
	}
	
	private Console() {
		dateFormatter = new SimpleDateFormat();
	}

	/**
	 * Printa a mensagem solicitada pelo usuário
	 * @param message informada pelo usuário
	 */
	public void printMessage(String message) {
		if (!message.equals("")) {
			System.out.println(message.trim() + " ");
			System.out.flush();
		}
	}

	/**
	 * Printa uma data
	 * @param date data informada pelo usuário
	 */
	public void printDate(Date date) {
		if (date != null) {
			System.out.println(dateFormatter.format(date));
			System.out.flush();
		}
	}

	/**
	 * Printa uma data formatada
	 * @param date data a ser impressa
	 * @param format formato da data
	 */
	public void printDate(Date date, String format) {
		if (date != null) {
			dateFormatter.applyPattern(format);
			System.out.println(dateFormatter.format(date));
			System.out.flush();
		}
	}

	/**
	 * Lê uma linha
	 * @return a linha lida
	 */
	public String readLine() {
		int tCh;
		String tResult = "";
		boolean tDone = false;

		while (!tDone) {
			try {
				tCh = System.in.read();
				if (tCh < 0 || (char) tCh == '\n')
					tDone = true;
				else if ((char) tCh != '\r') // weird--it used to do \r\n
					// translation
					tResult = tResult + (char) tCh;
			} catch (java.io.IOException tExcept) {
				tDone = true;
			}
		}
		return tResult;
	}

	/**
	 * Printa uma mensagem e lê uma linha
	 * @param message mensagem a ser impressa
	 * @return linha lida
	 */
	public String readLine(String message) {
		printMessage(message);
		return readLine();
	}

	/**
	 * Imprime uma mensagem e lê uma string
	 * @param message mensagem a ser impressa
	 * @return linha lida
	 */
	public String readString(String message) {
		return readLine(message);
	}

	/**
	 * Lê uma string
	 * @return string lida
	 */
	public String readString() {
		return readLine();
	}

	/**
	 * Lê um caracter
	 * @return caracter lido
	 */
	public char readChar() {
		char tResult = '\0';
		int tCh;
		boolean tDone = false;
		boolean tRead = false;

		while (!tDone) {
			try {
				tCh = System.in.read();
				if (tCh < 0 || (char) tCh == '\n')
					tDone = true;
				else if (!tRead && (char) tCh != '\r') {
					tResult = (char) tCh;
					tRead = true;
				}
			} catch (java.io.IOException tExcept) {
				tDone = true;
			}
		}
		return tResult;
	}

	/**
	 * Imprime uma mensagem e lê um caracter
	 * @param message mensagem a ser impressa
	 * @return caracter lido
	 */
	public char readChar(String message) {
		printMessage(message);
		return readChar();
	}

	/**
	 * Lê um double
	 * @return double lido
	 */
	public double readDouble() {
		return readDouble("");
	}

	/**
	 * Imprime uma mensagem e lê um double
	 * @param message mensagem a ser impressa
	 * @return double lido
	 */
	public double readDouble(String message) {
		String tLinha;

		while (true) {
			try {
				printMessage(message);
				tLinha = readLine().trim();
				if (tLinha.equals(""))
					return 0.0;
				return Double.parseDouble(tLinha);
			} catch (NumberFormatException e) {
				System.out.println("Not an double. Please try again!");
			}
		}
	}

	/**
	 * Lê um float
	 * @return float lido
	 */
	public float readFloat() {
		return readFloat("");
	}

	/**
	 * Imprime uma mensagem e lê um float
	 * @param message mensagem a ser impressa
	 * @return float lido
	 */
	public float readFloat(String message) {
		String tLinha;

		while (true) {
			try {
				printMessage(message);
				tLinha = readLine().trim();
				if (tLinha.equals(""))
					return 0.0f;
				return Float.parseFloat(tLinha);
			} catch (NumberFormatException e) {
				System.out.println("Not an float. Please try again!");
			}
		}
	}

	/**
	 * Lê um long
	 * @return long lido
	 */
	public long readLong() {
		return readLong("");
	}

	/**
	 * Imprime uma mensagem e lê um long
	 * @param message mensagem a ser impressa
	 * @return long lido
	 */
	public long readLong(String message) {
		String tLinha;

		while (true) {
			try {
				printMessage(message);
				tLinha = readLine().trim();
				if (tLinha.equals(""))
					return 0L;
				return Long.parseLong(tLinha);
			} catch (NumberFormatException e) {
				System.out.println("Not an long. Please try again!");
			}
		}
	}

	/**
	 * Lê um int
	 * @return
	 */
	public int readInt() {
		return readInt("");
	}

	/**
	 * Imprime uma mensagem e lê um int
	 * @param message mensagem a ser impressa
	 * @return int lido
	 */
	public int readInt(String message) {
		String tLinha;

		while (true) {
			try {
				printMessage(message);
				tLinha = readLine().trim();
				if (tLinha.equals(""))
					return 0;
				return Integer.parseInt(tLinha);
			} catch (NumberFormatException e) {
				System.out.println("Not an int. Please try again!");
			}
		}
	}

	/**
	 * Lê um short
	 * @return short lido
	 */
	public short readShort() {
		return readShort("");
	}

	/**
	 * Imprime uma mensagem e lê um short
	 * @param message mensagem a ser impressa
	 * @return short lido
	 */
	public short readShort(String message) {
		String tLinha;

		while (true) {
			try {
				printMessage(message);
				tLinha = readLine().trim();
				if (tLinha.equals(""))
					return (short) 0;
				return Short.parseShort(tLinha);
			} catch (NumberFormatException e) {
				System.out.println("Not an short. Please try again!");
			}
		}
	}

	/**
	 * Lê um byte
	 * @return byte lido
	 */
	public byte readByte() {
		return readByte("");
	}

	/**
	 * Imprime uma mensagem e lê um byte
	 * @param message mensagem a ser impressa
	 * @return byte lido
	 */
	public byte readByte(String message) {
		String tLinha;

		while (true) {
			try {
				printMessage(message);
				tLinha = readLine().trim();
				if (tLinha.equals(""))
					return (byte) 0;
				return Byte.parseByte(tLinha);
			} catch (NumberFormatException e) {
				System.out.println("Not an byte. Please try again!");
			}
		}
	}

	/**
	 * Lê uma data no formato dd/MM/yyyy
	 * @return data lida
	 */
	public Date readDate() {
		return this.readDate("", "dd/MM/yyyy");
	}

	/**
	 * Imprime uma mensagem e lê uma data no formato dd/MM/yyyy
	 * @param message mensagem a ser impressa
	 * @return data lida
	 */
	public Date readDate(String message) {
		return this.readDate(message, "dd/MM/yyyy");
	}

	/**
	 * Imprime uma mensagem e lê uma data com certo formato
	 * @param message mensagem a ser impressa
	 * @param format formato da data a ser lida
	 * @return data lida
	 */
	public Date readDate(String message, String format) {
		dateFormatter.applyPattern(format);
		String tLinha;

		while (true) {
			try {
				printMessage(message);
				tLinha = readLine().trim();
				return dateFormatter.parse(tLinha);
			} catch (ParseException e) {
				System.out.println("Data inválida. Tente novamente!");
			}
		}
	}

	/**
	 * Lê um boolean
	 * @return boolean lido
	 */
	public boolean readBoolean() {
		return Boolean.parseBoolean(this.readString());
	}

	/**
	 * Imprime uma mensagem e lê um boolean
	 * @param message mensagem a ser impressa
	 * @return boolean lido
	 */
	public boolean readBoolean(String message) {
		return Boolean.parseBoolean(this.readString(message));
	}
	
	/**
	 * Lê um BigDecimal
	 * @return BigDecimal lido
	 */
	public BigDecimal readBigDecimal()
	{
		return readBigDecimal("");
	}

	/**
	 * Imprime uma mensagem e lê um BigDecimal
	 * @param message mensagem a ser impressa
	 * @return BigDecimal lido
	 */
	public BigDecimal readBigDecimal(String message)
	{
		String tLinha;

		while (true)
		{
			try
			{
				printMessage(message);
				tLinha = readLine().trim();
				if (tLinha.equals(""))
					return new BigDecimal(0);
				return new BigDecimal(tLinha);
			}
			catch (NumberFormatException e)
			{
				System.out.println("Not a BigDecimal. Please try again!");
			}
		}
	}
}

