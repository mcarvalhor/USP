import java.util.Scanner;



class Main {

	/**
	* Classe que representa uma data genérica, podendo ela ser válida ou não.
	*
	* @author Matheus C.R.
	*/
	private static class Date {

		/**
		 * Variáveis fixas que indicam o número de cada mês.
		 * 
		 */
		public static final int
			MM_JAN = 1,
			MM_FEB = 2,
			MM_MAR = 3,
			MM_APR = 4,
			MM_MAY = 5,
			MM_JUN = 6,
			MM_JUL = 7,
			MM_AUG = 8,
			MM_SEP = 9,
			MM_OCT = 10,
			MM_NOV = 11,
			MM_DEC = 12; // Não foi usado "enum" porque não podemos fazer operações numéricas com enum, como podemos com inteiros.
	
		private int DD, MM, YY; // Valores atuais da data (1-[28, 30, 31], 1-12, 1-inf).
		private boolean valid; // Como ainda não estamos tratando exceções, devemos armazenar uma variável que indica se esta instância é válida ou não.

		/**
		 * Construtor para uma determinada data.
		 * 
		 */
		public Date(int dd, int mm, int yy){
			DD = dd;
			MM = mm;
			YY = yy;
			validateMe();
		}

		/**
		 * Verifica a validade da atual instância.
		 * 
		 */
		private void validateMe(){
			valid = true;

			if(YY < 1 || MM < MM_JAN || DD < 1)	valid = false;

			if(MM > MM_DEC)	valid = false;

			if(MM == MM_FEB) { // Fevereiro é a exceção
				if(DD > 28)	valid = false;
			} else if(MM <= MM_JUL){ // Antes de julho, os meses par possuem até 30 dias.
				if(MM % 2 == 0 && DD > 30)	valid = false;
				else if(MM % 2 != 0 && DD > 31)	valid = false;
			} else { // Depois de julho, os meses par passam a ter 31 dias.
				if(MM % 2 == 0 && DD > 31)	valid = false;
				else if(MM % 2 != 0 && DD > 30)	valid = false;
			}

		}

		/**
		 * Altera o dia da data.
		 * 
		 * @param dd novo dia
		 */
		public void setDD(int dd) {
			this.DD = dd;
			validateMe();
		}

		/**
		 * Obtém o dia da data.
		 * 
		 * @return o dia
		 */
		public int getDD() {
			return DD;
		}

		/**
		 * Altera o mês da data.
		 * 
		 * @param mm novo mês
		 */
		public void setMM(int mm) {
			this.MM = mm;
			validateMe();
		}

		/**
		 * Obtém o mês da data.
		 * 
		 * @return o mês
		 */
		public int getMM() {
			return MM;
		}

		/**
		 * Altera o ano da data.
		 * 
		 * @param yy novo ano
		 */
		public void setYY(int yy) {
			this.YY = yy;
			validateMe();
		}

		/**
		 * Obtém o ano da data.
		 * 
		 * @return o ano
		 */
		public int getYY() {
			return YY;
		}

		/**
		 * Imprime na tela a data da instância atual.
		 * 
		 */
		public void displayDate(){
			System.out.println(this);
		}

		/**
		 * Retorna a data, ou "DATA INVALIDA" caso esta instância não represente uma data válida.
		 * 
		 * @return o valor da instância como String
		 */
		@Override
		public String toString(){
			if(!valid)	return "DATA INVALIDA";
			return DD + "/" + MM + "/" + YY;
		}

	}



	public static void main(String args[]){

		Scanner FS = new Scanner(System.in);
		int i, dd, mm, yy, N = FS.nextInt();
		Date Aux;

		for(i = 0; i < N; i++){

			dd = FS.nextInt();
			mm = FS.nextInt();
			yy = FS.nextInt();

			Aux = new Date(dd, mm, yy);
			Aux.displayDate();

		}

		FS.close();

	}

}



