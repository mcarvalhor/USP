package gerenciador;

import java.sql.Timestamp;
import java.util.Date;

public class TimerThread extends Thread {

	private Gerenciador main;

	public TimerThread(Gerenciador main) {
		this.main = main;
	}

	@Override
	public void run() {
		long diff;

		while(true) {
			try {
				Thread.sleep(1000);
				main.mutex.acquire();
			} catch(InterruptedException exc) {
				System.err.println("Erro de thread!");
				System.err.println(exc);
				exc.printStackTrace();
			}

			if(main.ultimaPresenca == null) {
				diff = 0;
			} else {
				diff = new Timestamp(new Date().getTime()).getTime() - main.ultimaPresenca.getTime();
			}

			main.mutex.release();

			if(diff < 0) {
				diff *= -1;
			}

			if(diff > 60*15*100 || new Timestamp(new Date().getTime()).getTime() % 82800000 == 0) {
				// Desligar projetor, ar e luz porque não há ninguém na sala por mais de 15min OU já deu 23h.
				try {
					if(!new Message(new Message("GERENCIADOR", "OFF", "").send(this.main.alimentadorProjetor.getAddress(), this.main.alimentadorProjetor.getPort())).getBody().equals("1")) {
						throw new Exception("Projector not off.");
					}
				} catch(Exception exc) {
					System.err.println("Falha ao estabelecer comunicacao com o alimentador do projetor. :O");
				}
				try {
					if(!new Message(new Message("GERENCIADOR", "OFF", "0").send(this.main.alimentadorLuzes.getAddress(), this.main.alimentadorLuzes.getPort())).getBody().equals("1")) {
						throw new Exception("Lights not off.");
					}
				} catch(Exception exc) {
					System.err.println("Falha ao estabelecer comunicacao com o alimentador das luzes. :O");
				}
				try {
					if(!new Message(new Message("GERENCIADOR", "OFF", "").send(this.main.alimentadorAr.getAddress(), this.main.alimentadorAr.getPort())).getBody().equals("1")) {
						throw new Exception("Air not off.");
					}
				} catch(Exception exc) {
					System.err.println("Falha ao estabelecer comunicacao com o alimentador do ar-condicionado. :O");
				}
			}
		}
	}
}
