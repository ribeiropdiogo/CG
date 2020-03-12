//
// Created by ruimendes on 11/03/20.
//

#ifndef GENERATOR_TXML_H
#define GENERATOR_TXML_H

#include <string.h>
#include <cstdlib>
#include <includes/Engine.h>
#include "tinyxml.h"

class Txml {
private:
    char * sampleDir = strdup("../../samples/XML/");
    TiXmlElement * pElemGlobal;
    TiXmlHandle hRootGlobal=0;
    const char * info;
    int x=0;
public:
    void loadFile(char * string)
    {
        sampleDir=(char *) realloc(sampleDir,strlen(sampleDir)+strlen(string)+1);
        strcat(sampleDir,string);
    }

    void loadGroup(Engine * e,TiXmlElement * elem, bool isSub, int * parentGroup,int * currentGroup,int * latestGroup)
    {
        e->newGroup();
        if (isSub)
        {
            e->addSubgroup(*parentGroup);
            printf("idPai: %d idAtual: %d\n",*parentGroup,*currentGroup);
        }
        TiXmlElement * aux;
        float x,y,z,angle;
        int hCurrent;
        x=y=z=angle=0;
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
                if (aux->Attribute("angle"))
                    angle = (float) atof(aux->Attribute("angle"));
                e->newTransform( * (new TransformEvent(1,x,y,z,angle)));
            }
            else if (strcmp(s,"translate")==0){
                if (aux->Attribute("X"))
                    x = (float) atof(aux->Attribute("X"));
                if (aux->Attribute("Y"))
                    y = (float) atof(aux->Attribute("Y"));
                if (aux->Attribute("Z"))
                    z = (float) atof(aux->Attribute("Z"));
                e->newTransform( * (new TransformEvent(2,x,y,z)));
            }
            else if (strcmp(s,"scale")==0)
            {
                if (aux->Attribute("axisX"))
                    x = (float) atof(aux->Attribute("axisX"));
                if (aux->Attribute("axisY"))
                    y = (float) atof(aux->Attribute("axisY"));
                if (aux->Attribute("axisZ"))
                    z = (float) atof(aux->Attribute("axisZ"));
                e->newTransform( * (new TransformEvent(0,x,y,z)));
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
            else if (strcmp(s,"models")==0)
                for (TiXmlElement * models = aux->FirstChildElement();models;models=models->NextSiblingElement())
                    e->newObj(models->Attribute("file"));
        }
    }

    void loadEngine(Engine * e)
    {
        TiXmlDocument doc(sampleDir);
        if (!doc.LoadFile()) return;
        TiXmlHandle hDoc(&doc);
        TiXmlElement *pElem;
        TiXmlHandle hRoot(0);
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
            }
        }
    }
};

#endif //GENERATOR_TXML_H
