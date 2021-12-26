package resources;

import java.awt.image.BufferedImage;
import javax.imageio.ImageIO;
import javax.sound.sampled.Clip;
import javax.sound.sampled.AudioSystem;
import java.io.File;
import java.nio.file.Files;
import java.nio.file.Paths;


/**
 * Esta classe gerencia todos os recursos (imagens, áudio, ...) disponíveis para uso no jogo.
 */
public class Resources {

	private static boolean loaded = false;

	public static class Image {

		public static final int
				BLANK = 0,
				NEW_GAME_BUTTON = 1,
				LOAD_GAME_BUTTON = 2,
				REMOVE_SCORE_BUTTON = 3,
				PAUSE_ICON = 4,
				PLAY_ICON = 5,
				TOWER = 6,
				SQUARY_START = 7,
				SQUARY_END = 7,
				HORIZONTAL_START = 8,
				HORIZONTAL_END = 11,
				VERTICAL_START = 12,
				VERTICAL_END = 13,
				WEAPON_START = 15,
				DIED_ENEMY = 20;


		/**
		 * Caminho para imagens que serao carregadas.
		 */
		private static final String[] paths = new String[] {
				null,
				null,
				null,
				"closeIcon.png",
				"pauseIcon.png",
				"playIcon.png",
				"tower.png",
				"graphic1.png",
				"integral2.png",
				"integral3.png",
				"sum.png",
				"limit1.png",
				"limit2.jpg",
				"derivative1.png",
				"integral1.png",
				"lapis.png",
				"guidorizzi.jpg",
				"possani.jpeg",
				"mesalva.png",
				"wolfram.png",
				"pow.png",
		};

		/**
		 * Espaco de memoria usado como buffer para as imagens.
		 */
		private static final BufferedImage[] cache = new BufferedImage[paths.length];

		/**
		 * Este metodo retorna uma imagem para uso no jogo.
		 * @param id o id da imagem, sendo este um dos acima
		 * @return a imagem, ou null em caso de erros
		 */
		public static BufferedImage get(int id) {
			if(id < 0 || id >= paths.length)
				return null;

			return cache[id];
		}

	}

	public static class Audio {

		public static final int
				PENCIL_START = 0,
				PENCIL_END = 1,
				BOOK_START = 2,
				BOOK_END = 2,
				POSSANI_START = 3,
				POSSANI_END = 6,
				MESALVA_START = 7,
				MESALVA_END = 7,
				WOLFRAM_START = 8,
				WOLFRAM_END = 8,
				BACKGROUND_START = 9,
				BACKGROUND_END = 13;

		/**
		 * Caminho para os áudios que serão carregados.
		 */
		private static final String[] paths = new String[] {
				"pencil1.wav",
				"pencil2.wav",
				"guido.wav",
				"possani1.wav",
				"possani2.wav",
				"possani3.wav",
				"possani4.wav",
				"mesalva.wav",
				"wolfram.wav",
				"aria-math.wav",
				"pvz-treasure.wav",
				"t-run.wav",
				"defend-bombsite.wav",
				"gta-wanted8.wav"
		};

		/**
		 * Espaço de memória usado como buffer para os áudios.
		 */
		private static final Clip[] cache = new Clip[paths.length];

		/**
		 * Este método retorna um áudio para uso no jogo.
		 * @param id o id do áudio, sendo este um dos acima
		 * @return o áudio, ou null em caso de erros
		 */
		public static Clip get(int id) {
			if(id < 0 || id >= paths.length)
				return null;

			return cache[id];
		}

	}

	/**
	 * Este método carrega para a memória todos os recursos necessários para execução do jogo (imagens, áudios, ...).
	 * @return true se carregou com sucesso, false caso contrário
	 */
	public static boolean load(Object cl) {
		int i;

		if (loaded)
			return true;

		for (i = 0; i < Image.paths.length; i++) {
			if (Image.paths[i] == null) {
				Image.cache[i] = null;
				continue;
			}
			try {
				Image.cache[i] = ImageIO.read(new File("src/resources/img/" + Image.paths[i]));
			} catch (Exception exc) {
				System.err.printf("Could not load image <%s>: %s.", "src/resources/img/" + Image.paths[i], exc.getMessage());
				System.err.println();
				return false;
			}
		}

		for (i = 0; i < Audio.paths.length; i++) {
			if (Audio.paths[i] == null) {
				Audio.cache[i] = null;
				continue;
			}
			try {
				Audio.cache[i] = AudioSystem.getClip();
				Audio.cache[i].open(AudioSystem.getAudioInputStream(new File("src/resources/aud/" + Audio.paths[i])));
			} catch (Exception exc) {
				System.err.printf("Could not load audio <%s>: %s.", "src/resources/aud/" + Audio.paths[i], exc.getMessage());
				System.err.println();
				return false;
			}
		}

		loaded = true;
		return true;
	}

}
