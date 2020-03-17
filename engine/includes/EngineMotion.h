//
// Created by syrayse on 10/03/20.
//

#ifndef GENERATOR_ENGINEMOTION_H
#define GENERATOR_ENGINEMOTION_H

class EngineMotion {
public:
    /**
     * Responsavel pelo processamento de eventos associados
     * a *ASCII key stokes*, por parte do *Engine*.
     *
     * De momento, encapsula o seguinte comportamento:
     *      1) Rotação da camâra em torno da origem.
     *
     * @param key Código ASCII associado à tecla pressionada.
     * @param x Posição do rato no eixo X, quando a tecla é pressionada.
     * @param y Posição do rato no eixo Y, quando a tecla é pressionada.
     */
    virtual void handle_ascii(unsigned char key, int x, int y);

    /**
     * Responsavel pelo processamento de eventos associados
     * a *non-ASCII key stokes*, por parte do *Engine*.
     *
     * De momento, encapsula o seguinte comportamento:
     *      1)
     *
     * @param key Código associado à tecla pressionada.
     * @param x Posição do rato no eixo X, quando a tecla é pressionada.
     * @param y Posição do rato no eixo Y, quando a tecla é pressionada.
     */
    virtual void handle_special(int key, int x, int y);

    /**
     * Responsavel pela colocação da camâra ao renderizar
     * a cena.
     *
     * Incluis os seguintes comportamentos:
     *      1) Load da Matriz identidade.
     *      2) Posicionamento da camâra relativo à origem.
     *      3) Polygon Mode considerado atualmente.
     */
    virtual void place_camera();

    /**
     * Permite definr a tela de projeção, acedendo à
     * matriz de projeção.
     *
     * @param w Largura da tela.
     * @param h Altura da tela.
     */
    virtual void projection_size(int w, int h);

    void moveCenario();

    void updateAspect();

    void updateAspectR();

    void updateAspectL();

    void rotateLeft();

    void rotateRight();

    void rotate();
};

#endif //GENERATOR_ENGINEMOTION_H
