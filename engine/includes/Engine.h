//
// Created by syrayse on 10/03/20.
//

#ifndef GENERATOR_ENGINE_H
#define GENERATOR_ENGINE_H

#include <unordered_map>
#include <vector>
#include "Group.h"
#include "EngineMotion.h"
#include "IL/il.h"
#include "Shader.h"

#define MAX_NUM_LIGHTS 20

class Engine {
private:
    /**
     * Nome da janela associada ao Engine.
     */
    const string WIN_NAME = "CG@DI-UM";

    vector<Object3d> loadedEvents;

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
     * Permite comunicar ao Engine que um novo grupo acaba de
     * ser criado e está de momento a ser processado.
     */
    static void newGroup();

    /**
     * Adiciona uma nova transformação, associando esta ao grupo
     * mas recente.
     * @param te Transformação a ser associada.
     */
    static void newTransform(TransformEvent te);

    static void addSubgroup(int group);

    static vector<int> addUpgroup(int upGroup,int group);

    static void appendCubeMapFace(string face);

    static void initialCamera(float x, float y, float z);

    void newObj(const string& file, string texture, Material material);

    Object3d* newDrawing(const string& file, const string& texture, Material material);

    static void setBackgroundColor(float r, float g, float b);
};

#endif //GENERATOR_ENGINE_H
