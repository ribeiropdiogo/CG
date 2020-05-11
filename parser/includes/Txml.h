//
// Created by ruimendes on 11/03/20.
//

#ifndef GENERATOR_TXML_H
#define GENERATOR_TXML_H

#include <string.h>
#include <cstdlib>
#include <includes/Engine.h>
#include "tinyxml.h"

string sea_cubemap[6] = {
        "sea/right.jpg",
        "sea/left.jpg",
        "sea/top.jpg",
        "sea/bottom.jpg",
        "sea/front.jpg",
        "sea/back.jpg"
};

string stars_cubemap[6] = {
        "stars/px.png",
        "stars/nx.png",
        "stars/py.png",
        "stars/ny.png",
        "stars/pz.png",
        "stars/nz.png"
};

class Txml {
private:
    char * sampleDir = strdup("../../samples/XML/");
    TiXmlElement * pElemGlobal;
    TiXmlHandle hRootGlobal=0;
    const char * info;
    int x=0;

    void preLoadCubemap(Engine *e, string * cubemap) {
        for(int i = 0; i < 6; i++) {
            e->appendCubeMapFace(cubemap[i]);
        }
    }

public:

    void loadFile(char * string)
    {
        sampleDir=(char *) realloc(sampleDir,strlen(sampleDir)+strlen(string)+1);
        strcat(sampleDir,string);
    }

    void backgroundSpec(Engine *e, TiXmlElement* elem) {
        float r, g, b;
        TiXmlElement * aux;
        r = g = b = 0.0f;

        for (aux=elem->FirstChildElement();aux;aux=aux->NextSiblingElement()) {

            const char* s = aux->Value();

            if(!strcmp(s,"color")) {
                if(aux->Attribute("r"))
                    r = atof(aux->Attribute("r"));
                if(aux->Attribute("g"))
                    g = atof(aux->Attribute("g"));
                if(aux->Attribute("b"))
                    b = atof(aux->Attribute("b"));
            }

            e->setBackgroundColor(r, g, b);

        }
    }

    void loadGroup(Engine * e,TiXmlElement * elem, bool isSub, int * parentGroup,int * currentGroup,int * latestGroup)
    {
        e->newGroup();
        if (isSub)
        {
            e->addSubgroup(*parentGroup);
            e->addUpgroup(*parentGroup,*currentGroup);
        }
        TiXmlElement * aux;
        string texture;
        float x,y,z,angle, time;
        float diffR, diffG, diffB,
                aR, aG, aB,
                sR, sG, sB, shine;
        diffR = diffG = diffB = aR = aG = aB = shine = 1.0f;
        sR = sG = sB = 1.0f;

        bool timeDep = false;
        int hCurrent;
        x=y=z=angle=0;
        int count = 0;
        for (aux=elem->FirstChildElement();aux;aux=aux->NextSiblingElement())
        {
            const char * s = aux->Value();
            x = y = z = 0.0;
            if (strcmp(s,"rotate")==0)
            {
                if (aux->Attribute("axisX"))
                    x = (float) atof(aux->Attribute("axisX"));
                if (aux->Attribute("axisY"))
                    y = (float) atof(aux->Attribute("axisY"));
                if (aux->Attribute("axisZ"))
                    z = (float) atof(aux->Attribute("axisZ"));
                if (aux->Attribute("time")) {
                    time = (float) atof(aux->Attribute("time"));
                    e->newTransform( * (new TransformEvent(x,y,z,(int)(time*1000))));
                }
                else if (aux->Attribute("angle")) {
                    angle = (float) atof(aux->Attribute("angle"));
                    e->newTransform( * (new TransformEvent(x,y,z,angle)));
                }
            }
            else if (strcmp(s,"translate")==0){
                time = 0.0f;
                timeDep = false;
                vector<glm::vec3> points;

                if (aux->Attribute("time")) {
                    time = (float) atof(aux->Attribute("time"));
                    timeDep = true;
                }

                for (TiXmlElement * models = aux->FirstChildElement();models;models=models->NextSiblingElement()) {
                    x = (float) atof(models->Attribute("X"));
                    y = (float) atof(models->Attribute("Y"));
                    z = (float) atof(models->Attribute("Z"));
                    points.push_back(glm::vec3(x, y, z));
                }

                if(points.size() >= 4) {
                    e->newTransform(*new TransformEvent(points, (int)(time*1000), true));
                    continue;
                }

                if (aux->Attribute("X"))
                    x = (float) atof(aux->Attribute("X"));
                if (aux->Attribute("Y"))
                    y = (float) atof(aux->Attribute("Y"));
                if (aux->Attribute("Z"))
                    z = (float) atof(aux->Attribute("Z"));

                if (timeDep) {
                    e->newTransform( * (new TransformEvent(TRANSLATE,x,y,z,(int)(time*1000))));
                }
                else
                    e->newTransform( * (new TransformEvent(TRANSLATE,x,y,z)));
            }
            else if (strcmp(s,"scale")==0)
            {
                if (aux->Attribute("axisX"))
                    x = (float) atof(aux->Attribute("axisX"));
                if (aux->Attribute("axisY"))
                    y = (float) atof(aux->Attribute("axisY"));
                if (aux->Attribute("axisZ"))
                    z = (float) atof(aux->Attribute("axisZ"));
                if (aux->Attribute("time")) {
                    time = (float) atof(aux->Attribute("time"));
                    e->newTransform( * (new TransformEvent(SCALE,x,y,z,(int)(time*1000))));
                }
                else
                    e->newTransform( * (new TransformEvent(SCALE,x,y,z)));
            }
            else if (strcmp(s,"group")==0)
            {
                hCurrent=*currentGroup;
                *parentGroup=*currentGroup;
                *latestGroup=*latestGroup+1;
                *currentGroup=*latestGroup;
                loadGroup(e,aux,true,parentGroup,currentGroup,latestGroup);
                *currentGroup=hCurrent;
            }
            else if (strcmp(s,"models")==0){

                for (TiXmlElement * models = aux->FirstChildElement();models;models = models -> NextSiblingElement()){
                    int r = 0, g = 0 ,b = 255;
                    diffR = diffG = diffB = 0.0f;

                    if(models->Attribute("texture")) {
                        texture = * new string(models->Attribute("texture"));
                    }

                    // Permite definir luzes difusas.
                    if(models->Attribute("diffR")) {
                        diffR = (float) atof(models->Attribute("diffR"));
                    }
                    if(models->Attribute("diffG")) {
                        diffG = (float) atof(models->Attribute("diffG"));
                    }
                    if(models->Attribute("diffB")) {
                        diffB = (float) atof(models->Attribute("diffB"));
                    }

                    // Permite definir luzes ambiente.
                    if(models->Attribute("aR")) {
                        aR = (float) atof(models->Attribute("aR"));
                    }
                    if(models->Attribute("aG")) {
                        aG = (float) atof(models->Attribute("aG"));
                    }
                    if(models->Attribute("aB")) {
                        aB = (float) atof(models->Attribute("aB"));
                    }

                    // Permite definir luzes especulares
                    if(models->Attribute("sR")) {
                        sR = (float) atof(models->Attribute("sR"));
                    }
                    if(models->Attribute("sG")) {
                        sG = (float) atof(models->Attribute("sG"));
                    }
                    if(models->Attribute("sB")) {
                        sB = (float) atof(models->Attribute("sB"));
                    }

                    // representa o shine
                    if(models->Attribute("shine")) {
                        shine = (float) atof(models->Attribute("shine"));
                    }

                    float props[10] = {diffR,diffG,diffB,
                                       aR,   aG,   aB,
                                       sR,   sG,   sB,
                                       shine};

                    if(models->Attribute("file"))
                        e->newObj(models->Attribute("file"),props, texture);
                    texture.erase();

                }
            }

        }
    }

    void loadEngine(Engine * e)
    {
        TiXmlDocument doc(sampleDir);
        if (!doc.LoadFile()) return;
        TiXmlHandle hDoc(&doc);
        TiXmlElement *pElem;
        TiXmlHandle hRoot(0);
        TiXmlElement * aux;
        pElem = hDoc.FirstChildElement().Element();
        if (!pElem) return;
        const char *line = pElem->Value();
        hRoot = TiXmlHandle(pElem);
        int currentGroup=0;
        int parentGroup=0;
        int latestGroup=0;

        if (strcmp(line, "scene") == 0) {
            pElem = hRoot.FirstChildElement().Element();
            for (pElem; pElem; pElem = pElem->NextSiblingElement()) {

                if (strcmp(pElem->Value(),"group")==0)
                    loadGroup(e,pElem,false,&parentGroup,&currentGroup,&latestGroup);
                else if(strcmp(pElem->Value(),"camera") == 0){
                    float posX = 0, posY = 0, posZ = 0;
                    if(pElem->Attribute("posX"))
                        posX = (float) atof(pElem->Attribute("posX"));
                    if(pElem->Attribute("posY"))
                        posY = (float) atof(pElem->Attribute("posY"));
                    if(pElem->Attribute("posZ"))
                        posZ = (float) atof(pElem->Attribute("posZ"));
                    e->initialCamera(posX,posY,posZ);
                }
                else if(strcmp(pElem->Value(),"cubemap") == 0) {
                    if(pElem->Attribute("map")) {
                        const char * prdef = pElem->Attribute("map");
                        if(!strcmp( prdef, "SEA" ))
                            preLoadCubemap(e, sea_cubemap);
                        else if(!strcmp( prdef,"STARS" ))
                            preLoadCubemap(e, stars_cubemap);
                    }
                    else {
                        for (aux = pElem->FirstChildElement(); aux; aux = aux->NextSiblingElement()) {
                            if (strcmp(aux->Value(), "face") == 0 && aux->Attribute("file")) {
                                e->appendCubeMapFace(aux->Attribute("file"));
                            }
                        }
                    }
                }
                else if(strcmp(pElem->Value(),"background") == 0) {
                    backgroundSpec(e, pElem);
                }
            }
        }
    }
};

#endif //GENERATOR_TXML_H
