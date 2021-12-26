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
	 * Padr�o de projeto Singleton
	 * @return a inst�ncia da classe Console
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
	 * Printa a mensagem solicitada pelo usu�rio
	 * @param message informada pelo usu�rio
	 */
	public void printMessage(String message) {
		if (!message.equals("")) {
			System.out.println(message.trim() + " ");
			System.out.flush();
		}
	}

	/**
	 * Printa uma data
	 * @param date data informada pelo usu�rio
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
	 * L� uma linha
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
	 * Printa uma mensagem e l� uma linha
	 * @param message mensagem a ser impressa
	 * @return linha lida
	 */
	public String readLine(String message) {
		printMessage(message);
		return readLine();
	}

	/**
	 * Imprime uma mensagem e l� uma string
	 * @param message mensagem a ser impressa
	 * @return linha lida
	 */
	public String readString(String message) {
		return readLine(message);
	}

	/**
	 * L� uma string
	 * @return string lida
	 */
	public String readString() {
		return readLine();
	}

	/**
	 * L� um caracter
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
	 * Imprime uma mensagem e l� um caracter
	 * @param message mensagem a ser impressa
	 * @return caracter lido
	 */
	public char readChar(String message) {
		printMessage(message);
		return readChar();
	}

	/**
	 * L� um double
	 * @return double lido
	 */
	public double readDouble() {
		return readDouble("");
	}

	/**
	 * Imprime uma mensagem e l� um double
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
	 * L� um float
	 * @return float lido
	 */
	public float readFloat() {
		return readFloat("");
	}

	/**
	 * Imprime uma mensagem e l� um float
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
	 * L� um long
	 * @return long lido
	 */
	public long readLong() {
		return readLong("");
	}

	/**
	 * Imprime uma mensagem e l� um long
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
	 * L� um int
	 * @return
	 */
	public int readInt() {
		return readInt("");
	}

	/**
	 * Imprime uma mensagem e l� um int
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
	 * L� um short
	 * @return short lido
	 */
	public short readShort() {
		return readShort("");
	}

	/**
	 * Imprime uma mensagem e l� um short
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
	 * L� um byte
	 * @return byte lido
	 */
	public byte readByte() {
		return readByte("");
	}

	/**
	 * Imprime uma mensagem e l� um byte
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
	 * L� uma data no formato dd/MM/yyyy
	 * @return data lida
	 */
	public Date readDate() {
		return this.readDate("", "dd/MM/yyyy");
	}

	/**
	 * Imprime uma mensagem e l� uma data no formato dd/MM/yyyy
	 * @param message mensagem a ser impressa
	 * @return data lida
	 */
	public Date readDate(String message) {
		return this.readDate(message, "dd/MM/yyyy");
	}

	/**
	 * Imprime uma mensagem e l� uma data com certo formato
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
				System.out.println("Data inv�lida. Tente novamente!");
			}
		}
	}

	/**
	 * L� um boolean
	 * @return boolean lido
	 */
	public boolean readBoolean() {
		String s = this.readString();
		try {
			int read = Integer.parseInt(s);
			if (read == 0) 
				return false;
			else if (read == 1)
				return true;
		} catch (Exception e) {
			
		}
		return Boolean.parseBoolean(s);
	}

	/**
	 * Imprime uma mensagem e l� um boolean
	 * @param message mensagem a ser impressa
	 * @return boolean lido
	 */
	public boolean readBoolean(String message) {
		String s = this.readString(message);
		try {
			int read = Integer.parseInt(s);
			if (read == 0) 
				return false;
			else if (read == 1)
				return true;
		} catch (Exception e) {
			
		}
		return Boolean.parseBoolean(s);
	}
	
	/**
	 * L� um BigDecimal
	 * @return BigDecimal lido
	 */
	public BigDecimal readBigDecimal()
	{
		return readBigDecimal("");
	}

	/**
	 * Imprime uma mensagem e l� um BigDecimal
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

