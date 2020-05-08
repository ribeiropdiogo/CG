//
// Created by syrayse on 10/03/20.
//

#ifndef GENERATOR_ENGINE_H
#define GENERATOR_ENGINE_H

#include <unordered_map>
#include <vector>
#include "DrawEvent.h"
#include "Group.h"
#include "EngineMotion.h"
#include "IL/il.h"

class Engine {
private:
    /**
     * Nome da janela associada ao Engine.
     */
    const string WIN_NAME = "CG@DI-UM";

    /**
     * Motor de processamento de movimentos
     * que dá suport básico ao atual Engine.
     */
    static EngineMotion motion;

    vector<DrawEvent> loadedEvents;

    /**
     * Armazena pares <Nome_Ficheiro, Objeto> de forma
     * a conseguir eficientemente evitar leituras de ficheiros
     * anteriormente já processados.
     */
    unordered_map<string,Object3d> loadedObj;

    /**
     * Fila de espera constituida por todos os grupos
     * que foram encontrados no ficheiro XML, os grupos
     * são posteriormente processados por ordem de chegada.
     */
    static vector<Group*> groups;

    static GLuint* VAOs;
    static GLuint* VBOs;

    /**
     * Numero de objetos distintos já encontrados.
     */
    unsigned int numObjs = 0;

    /**
     * Buffer dos vértices, que permite um desenho eficiente.
     */
    static GLuint * buffers;

    /**
     * Indexes dos vértices, que um desenho eficiente.
     */
    static GLuint * indexes;

    /**
     * Buffer das texturas.
     */
    static GLuint * normals;

    /**
     * Buffer das coordenadas das texturas.
     */
    static GLuint * texCoords;

    /**
     * Calcula o grupo mais recente a ser encontrado,
     * correspondente àquele que de momento está a ser
     * processado.
     * @return Grupo mais recente.
     */
    Group * latestGroup();

    /**
     * Permite a criação de um novo DrawEvent, associado
     * a um determinado ficheiro.
     *
     * Se o ficheiro já tiver sido anteriormente processado,
     * então o DrawEvent originado terá sido anteriormente
     * criado. Caso contrário, a função cria o respectivo
     * DrawEvent.
     *
     * @param file Ficheiro que se pretende ler.
     * @return Evento criado.
     */
    DrawEvent newDrawing(const string& file);

    /**
     * Guarda as texturas fornecidasdna estrutura devida.
     */
    void loadTexture(int idx, string texture, GLuint *textures);

    /**
     * Efetua o binding de todos os objetos, fazendo
     * a respectiva geração dos  buffers de indexs
     * e vertices, armazenando-os desta forma em memória de buffer.
     */
    void bindAllObjects();

    /**
     * Função que dá inicio ao processamento hierarquico
     * dos grupos.
     *
     * Numa primeira iteração indice deve ser igual a 0,
     * correspondente a indica que o engine vai começar
     * o processamento dos grupos, hierarquicamente, partindo
     * do ponto idx.
     *
     * @param idx Ponto de partida.
     */
    static int runGroups(int idx, int milis);

    /**
     * Desenha um eixo xyz, de forma a conseguir tornar o
     * engine mais intuitivo e para facilitação em termos
     * de debugging.
     */
    static void drawAxes();

    /**
     * Efetuar a renderização do Engine, processando
     * todas os eventos e transformações existentes.
     *
     * Os grupos são desenhados de forma a que a integridade
     * das transformações seja só restrita ao seu grupo, de tal
     * modo, antes de começar um novo desenho de um grupo é
     * feito um 'pushMatrix', assim que o desenho deste grupo
     * termina é efetuado um 'popMatrix', permitindo desta
     * forma compor as diferentes transformações.
     */
    static void renderScene();

    static void idleFunc();

    static void wrap_proj(int w, int h);
    static void wrap_ascii(unsigned char key, int x, int y);
    static void wrap_special(int key, int x, int y);
    static void wrap_up_ascii(unsigned char key, int x, int y);
    static void wrap_up_special(int key, int x, int y);
    static void processMouseButtons(int button, int state, int xx, int yy);

public:
    /**
     * Inicia o Engine, neste momento o Engine está
     * pronto a receber novos inputs, como grupos,
     * objetos, etc...
     */
    void start(int *eargc, char **argv);

    /**
     * Coloca o glut em main loop, não permite adicionar
     * entidades ao subsistema do engine.
     */
    void loop();

    /**
     * Termina o Engine.
     */
    void close();

    /**
     * Permite comunicar ao Engine que um novo grupo acaba de
     * ser criado e está de momento a ser processado.
     */
    void newGroup();

    /**
     * Adiciona uma nova transformação, associando esta ao grupo
     * mas recente.
     * @param te Transformação a ser associada.
     */
    void newTransform(TransformEvent te);

    void addSubgroup(int group);

    vector<int> addUpgroup(int upGroup,int group);

    void initialCamera(float x, float y, float z);

    void newObj(const string &file, int r, int g, int b,
            float diffR, float diffG, float diffB, string texture);

    DrawEvent newDrawing(const string &file, int r, int g, int b,
            float diffR, float diffG, float diffB, const string& texture);
};

#endif //GENERATOR_ENGINE_H
