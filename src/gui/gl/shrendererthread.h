/*
 * shrendererthread.h
 *
 *  Created on: 27.12.2012
 *      Author: Ralph
 */

#ifndef SHRENDERERTHREAD_H_
#define SHRENDERERTHREAD_H_

#include <QtCore/QDebug>
#include <QThread>
#include <QtCore/QVector>

class SHRendererThread : public QThread
{
public:
    SHRendererThread( QVector<ColumnVector>* data, int m_nx, int m_ny, int m_nz, float m_dx, float m_dy, float m_dz, int xi, int yi, int zi, QVector<int> visibleArea, int lod, int order, int orient, int id );
    virtual ~SHRendererThread();

    QVector<float>* getVerts();

private:
    void run();

    QVector<ColumnVector>* m_data;

    int m_nx;
    int m_ny;
    int m_nz;
    float m_dx;
    float m_dy;
    float m_dz;

    int m_xi;
    int m_yi;
    int m_zi;

    QVector<int> m_visibleArea;
    int m_lod;
    int m_order;
    int m_orient;
    int m_id;

    QVector<float>* m_verts;
};

#endif /* SHRENDERERTHREAD_H_ */