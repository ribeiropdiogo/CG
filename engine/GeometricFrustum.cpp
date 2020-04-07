//
// Created by Diogo Ribeiro on 02/04/2020.
//

#include "GeometricFrustum.h"
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#define ANG2RAD 3.14159265358979323846/180.0



GeometricFrustum::GeometricFrustum() {}

GeometricFrustum::~GeometricFrustum() {}

void GeometricFrustum::setCamInternals(float angle, float ratio, float nearD, float farD) {

    this->ratio = ratio;
    this->angle = angle;
    this->nearD = nearD;
    this->farD = farD;

    tang = (float)tan(angle* ANG2RAD * 0.5) ;
    nh = nearD * tang;
    nw = nh * ratio;
    fh = farD  * tang;
    fw = fh * ratio;


}


void GeometricFrustum::setCamDef(Vec3 &p, Vec3 &l, Vec3 &u) {

    Vec3 dir,nc,fc,X,Y,Z;

    Z = p - l;
    Z.normalize();

    X = u.crossprod(Z);
    X.normalize();

    Y = Z.crossprod(X);

    nc = p - Z * nearD;
    fc = p - Z * farD;

    ntl = nc + Y * nh - X * nw;
    ntr = nc + Y * nh + X * nw;
    nbl = nc - Y * nh - X * nw;
    nbr = nc - Y * nh + X * nw;

    ftl = fc + Y * fh - X * fw;
    ftr = fc + Y * fh + X * fw;
    fbl = fc - Y * fh - X * fw;
    fbr = fc - Y * fh + X * fw;

    pl[TOP].set3Points(ntr,ntl,ftl);
    pl[BOTTOM].set3Points(nbl,nbr,fbr);
    pl[LEFT].set3Points(ntl,nbl,fbl);
    pl[RIGHT].set3Points(nbr,ntr,fbr);
    pl[NEARP].set3Points(ntl,ntr,nbr);
    pl[FARP].set3Points(ftr,ftl,fbl);
}


int GeometricFrustum::pointInFrustum(Vec3 &p) {

    int result = INSIDE;
    for(int i=0; i < 6; i++) {

        if (pl[i].distance(p) < 0)
            return OUTSIDE;
    }
    return(result);

}

int GeometricFrustum::triangleInFrustum(Vec3 &p1,Vec3 &p2,Vec3 &p3) {

    int result = INSIDE;
    if (pointInFrustum(p1) == OUTSIDE &&
        pointInFrustum(p2) == OUTSIDE &&
        pointInFrustum(p3) == OUTSIDE){
        result = OUTSIDE;
    }
    return(result);

}


int GeometricFrustum::sphereInFrustum(Vec3 &p, float raio) {

    int result = INSIDE;
    float distance;

    for(int i=0; i < 6; i++) {
        distance = pl[i].distance(p);
        if (distance < -raio)
            return OUTSIDE;
        else if (distance < raio)
            result =  INTERSECT;
    }
    return(result);

}


int GeometricFrustum::boxInFrustum(AxisAlignBoxes &b) {

    int result = INSIDE;
    for(int i=0; i < 6; i++) {
        if (pl[i].distance(b.getVertexP(pl[i].normal)) < 0)
            return OUTSIDE;
        else if (pl[i].distance(b.getVertexN(pl[i].normal)) < 0)
            result =  INTERSECT;
    }
    return(result);

}





void GeometricFrustum::drawPoints() {


    glBegin(GL_POINTS);

    glVertex3f(ntl.getX(),ntl.getY(),ntl.getZ());
    glVertex3f(ntr.getX(),ntr.getY(),ntr.getZ());
    glVertex3f(nbl.getX(),nbl.getY(),nbl.getZ());
    glVertex3f(nbr.getX(),nbr.getY(),nbr.getZ());

    glVertex3f(ftl.getX(),ftl.getY(),ftl.getZ());
    glVertex3f(ftr.getX(),ftr.getY(),ftr.getZ());
    glVertex3f(fbl.getX(),fbl.getY(),fbl.getZ());
    glVertex3f(fbr.getX(),fbr.getY(),fbr.getZ());

    glEnd();
}


void GeometricFrustum::drawLines() {

    glBegin(GL_LINE_LOOP);
    //near plane
    glVertex3f(ntl.getX(),ntl.getY(),ntl.getZ());
    glVertex3f(ntr.getX(),ntr.getY(),ntr.getZ());
    glVertex3f(nbr.getX(),nbr.getY(),nbr.getZ());
    glVertex3f(nbl.getX(),nbl.getY(),nbl.getZ());
    glEnd();

    glBegin(GL_LINE_LOOP);
    //far plane
    glVertex3f(ftr.getX(),ftr.getY(),ftr.getZ());
    glVertex3f(ftl.getX(),ftl.getY(),ftl.getZ());
    glVertex3f(fbl.getX(),fbl.getY(),fbl.getZ());
    glVertex3f(fbr.getX(),fbr.getY(),fbr.getZ());
    glEnd();

    glBegin(GL_LINE_LOOP);
    //bottom plane
    glVertex3f(nbl.getX(),nbl.getY(),nbl.getZ());
    glVertex3f(nbr.getX(),nbr.getY(),nbr.getZ());
    glVertex3f(fbr.getX(),fbr.getY(),fbr.getZ());
    glVertex3f(fbl.getX(),fbl.getY(),fbl.getZ());
    glEnd();

    glBegin(GL_LINE_LOOP);
    //top plane
    glVertex3f(ntr.getX(),ntr.getY(),ntr.getZ());
    glVertex3f(ntl.getX(),ntl.getY(),ntl.getZ());
    glVertex3f(ftl.getX(),ftl.getY(),ftl.getZ());
    glVertex3f(ftr.getX(),ftr.getY(),ftr.getZ());
    glEnd();

    glBegin(GL_LINE_LOOP);
    //left plane
    glVertex3f(ntl.getX(),ntl.getY(),ntl.getZ());
    glVertex3f(nbl.getX(),nbl.getY(),nbl.getZ());
    glVertex3f(fbl.getX(),fbl.getY(),fbl.getZ());
    glVertex3f(ftl.getX(),ftl.getY(),ftl.getZ());
    glEnd();

    glBegin(GL_LINE_LOOP);
    // right plane
    glVertex3f(nbr.getX(),nbr.getY(),nbr.getZ());
    glVertex3f(ntr.getX(),ntr.getY(),ntr.getZ());
    glVertex3f(ftr.getX(),ftr.getY(),ftr.getZ());
    glVertex3f(fbr.getX(),fbr.getY(),fbr.getZ());

    glEnd();
}


void GeometricFrustum::drawPlanes() {

    glBegin(GL_QUADS);

    //near plane
    glVertex3f(ntl.getX(),ntl.getY(),ntl.getZ());
    glVertex3f(ntr.getX(),ntr.getY(),ntr.getZ());
    glVertex3f(nbr.getX(),nbr.getY(),nbr.getZ());
    glVertex3f(nbl.getX(),nbl.getY(),nbl.getZ());

    //far plane
    glVertex3f(ftr.getX(),ftr.getY(),ftr.getZ());
    glVertex3f(ftl.getX(),ftl.getY(),ftl.getZ());
    glVertex3f(fbl.getX(),fbl.getY(),fbl.getZ());
    glVertex3f(fbr.getX(),fbr.getY(),fbr.getZ());

    //bottom plane
    glVertex3f(nbl.getX(),nbl.getY(),nbl.getZ());
    glVertex3f(nbr.getX(),nbr.getY(),nbr.getZ());
    glVertex3f(fbr.getX(),fbr.getY(),fbr.getZ());
    glVertex3f(fbl.getX(),fbl.getY(),fbl.getZ());

    //top plane
    glVertex3f(ntr.getX(),ntr.getY(),ntr.getZ());
    glVertex3f(ntl.getX(),ntl.getY(),ntl.getZ());
    glVertex3f(ftl.getX(),ftl.getY(),ftl.getZ());
    glVertex3f(ftr.getX(),ftr.getY(),ftr.getZ());

    //left plane

    glVertex3f(ntl.getX(),ntl.getY(),ntl.getZ());
    glVertex3f(nbl.getX(),nbl.getY(),nbl.getZ());
    glVertex3f(fbl.getX(),fbl.getY(),fbl.getZ());
    glVertex3f(ftl.getX(),ftl.getY(),ftl.getZ());

    // right plane
    glVertex3f(nbr.getX(),nbr.getY(),nbr.getZ());
    glVertex3f(ntr.getX(),ntr.getY(),ntr.getZ());
    glVertex3f(ftr.getX(),ftr.getY(),ftr.getZ());
    glVertex3f(fbr.getX(),fbr.getY(),fbr.getZ());

    glEnd();

}

void GeometricFrustum::drawNormals() {

    Vec3 a,b;

    glBegin(GL_LINES);

    // near
    a = (ntr + ntl + nbr + nbl) * 0.25;
    b = a + pl[NEARP].normal;
    glVertex3f(a.getX(),a.getY(),a.getZ());
    glVertex3f(b.getX(),b.getY(),b.getZ());

    // far
    a = (ftr + ftl + fbr + fbl) * 0.25;
    b = a + pl[FARP].normal;
    glVertex3f(a.getX(),a.getY(),a.getZ());
    glVertex3f(b.getX(),b.getY(),b.getZ());

    // left
    a = (ftl + fbl + nbl + ntl) * 0.25;
    b = a + pl[LEFT].normal;
    glVertex3f(a.getX(),a.getY(),a.getZ());
    glVertex3f(b.getX(),b.getY(),b.getZ());

    // right
    a = (ftr + nbr + fbr + ntr) * 0.25;
    b = a + pl[RIGHT].normal;
    glVertex3f(a.getX(),a.getY(),a.getZ());
    glVertex3f(b.getX(),b.getY(),b.getZ());

    // top
    a = (ftr + ftl + ntr + ntl) * 0.25;
    b = a + pl[TOP].normal;
    glVertex3f(a.getX(),a.getY(),a.getZ());
    glVertex3f(b.getX(),b.getY(),b.getZ());

    // bottom
    a = (fbr + fbl + nbr + nbl) * 0.25;
    b = a + pl[BOTTOM].normal;
    glVertex3f(a.getX(),a.getY(),a.getZ());
    glVertex3f(b.getX(),b.getY(),b.getZ());

    glEnd();


}