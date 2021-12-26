package score;

import java.io.Serializable;

/**
 * Essa classe representa um score de jogador.
 *
 */
@SuppressWarnings("serial")
public class Score implements Serializable {

	private long score, duration;
	private String player;

	public Score(String player) {
		score = duration = 0;
		this.player = player;
	}

	public long getScore() {
		return score;
	}

	public void setScore(long score) {
		this.score = score;
	}

	public long getDuration() {
		return duration;
	}

	public void setDuration(long duration) {
		this.duration = duration;
	}

	public String getPlayer() {
		return player;
	}

	public void setPlayer(String player) {
		this.player = player;
	}

}
