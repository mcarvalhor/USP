package resources;

import java.sql.Time;
import java.util.Random;
import java.util.concurrent.Semaphore;

/**
 * Essa classe implementa métodos úteis para uso no programa.
 *
 */
public class Utils {

	private static Random randomGen = null;
	/**
	 * Esse método encerra o programa devido a um erro fatal.
	 *
	 */
	public static void fatalError() {
		System.err.println("== FATAL ERROR: EXITING ==");
		System.exit(-1);
	}

	/**
	 * Este método dorme a thread que o chamou.
	 * @param ms milisegundos
	 */
	public static void sleep(long ms) {
		try {
			Thread.sleep(ms);
		} catch(Exception exc) {
			System.err.printf("Could not sleep thread: %s.",
					exc.getMessage());
			System.err.println();
			fatalError();
		}
	}

	public static void acquireLock(Semaphore lock) {
		try {
			lock.acquire();
		} catch(Exception exc) {
			System.err.printf("Could not acquire '%s' semaphore lock: %s.",
					lock.toString(), exc.getMessage());
			System.err.println();
			fatalError();
		}
	}

	public static void joinThread(Thread thread) {
		try {
			thread.join();
		} catch(Exception exc) {
			System.err.printf("Could not join threads '%s' and %s.",
					Thread.currentThread().toString(), thread.toString());
			System.err.println();
			fatalError();
		}
	}

	public static Random getRandom() {
		if(randomGen == null) {
			randomGen = new Random();
			randomGen.setSeed(System.currentTimeMillis());
		}
		return randomGen;
	}

}
