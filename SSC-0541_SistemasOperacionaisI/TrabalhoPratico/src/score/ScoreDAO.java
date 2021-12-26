package score;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

import javax.swing.JOptionPane;

public class ScoreDAO {

	@SuppressWarnings("resource")
	public static List<Score> getAll() {

		List<Score> scores = new ArrayList<Score>();

		try {

			String path = new File("score.csv").getAbsolutePath();
			BufferedReader br = null;
			String line = "";

			br = new BufferedReader(new FileReader(path));

			while ((line = br.readLine()) != null) {

				String[] s = line.split(",");

				Score score = new Score(s[0]);
				score.setScore(Long.parseLong(s[1].trim()));
				score.setDuration(Long.parseLong(s[2].trim()));
				
				scores.add(score);
			}

		} catch (FileNotFoundException e) {
			//e.printStackTrace();
			System.out.println("No scores file");
		} catch (Exception e) {
			e.printStackTrace();
			JOptionPane.showMessageDialog(null, "Could not get scores");
		}
		
		Collections.sort(scores, new Comparator<Score>() {

			@Override
			public int compare(Score s0, Score s1) {
				if (s0.getScore() > s1.getScore()) return -1;
				if (s0.getScore() < s1.getScore()) return 1;
				if (s0.getDuration() < s1.getDuration()) return -1;
				if (s0.getDuration() > s1.getDuration()) return 1;
				return 0;
			}
		});

		return scores;
	}

	public static void add(Score score) {

		File file = new File("score.csv");

		try {
			FileWriter fw = new FileWriter(file, true);

			fw.append(score.getPlayer() + ", " + score.getScore() + ", "
					+ score.getDuration() + "\n");
			fw.flush();
			fw.close();
		} catch (IOException e) {
			JOptionPane.showMessageDialog(null, "Could not save score");
			e.printStackTrace();
		}
	}
}
