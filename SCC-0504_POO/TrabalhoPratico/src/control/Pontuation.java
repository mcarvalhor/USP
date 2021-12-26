package control;

import java.io.Serializable;

public class Pontuation implements Serializable  {
    private int points;
    private int base;

    /**
     * Construtor que recebe a base multiplicada pelo multiFactor nos ganhos de pontos
     * @param base
     */
    Pontuation(int base) {
        points = 0;
        this.base = base;
    }

    /**
     * Função auxiliar para calcular a pontuação
     * @param n numero de linhas de uma vez
     * @return pontuação obtida
     */
    private int calc(int n) {
        if (n == 1) return base;
        base *= 4;
        return calc(n-1);
    }

    /**
     * Método que incrementa os pontos a partir de um fator multiplicativo que será
     * multiplicado pelo valor "base" e somados a "points".
     * @param multFactor fator multiplicativo
     */
    public void gain(int multFactor) {
        points += calc(multFactor);
        base = 50;
    }

    /**
     * Retorna a quantidade de pontos atual
     * @return points
     */
    public int getPoints() {
        return points;
    }
}