/*
 * fiberselector.h
 *
 *  Created on: Feb 26, 2013
 *      Author: schurade
 */

#ifndef FIBERSELECTOR_H_
#define FIBERSELECTOR_H_

#include "../../algos/kdtree.h"

#include <QtCore/QVector>
#include <QtCore/QObject>
#include <QtCore/QAbstractItemModel>

class FiberSelector : public QObject
{
    Q_OBJECT

public:
    FiberSelector( QAbstractItemModel* roiModel );
    virtual ~FiberSelector();

    void init( QVector< QVector< float > >& data );

    QVector<bool>* getSelection();

private:
    QModelIndex createIndex( int branch, int pos, int column );
    void updatePresentRois();

    QAbstractItemModel* m_roiModel;

    int m_numLines;
    int m_numPoints;

    bool m_isInitialized;

    KdTree* m_kdTree;
    QVector<float>m_kdVerts;
    QVector<int>m_reverseIndexes;

    QVector<bool>m_rootfield;
    QList<QVector<bool> >m_branchfields;
    QList<QList<QVector<bool> > >m_bitfields;

    QVector<float> m_boxMin;
    QVector<float> m_boxMax;

private slots:
    void roiChanged( const QModelIndex &topLeft, const QModelIndex &bottomRight );
    void roiInserted( const QModelIndex &parent, int start, int end );
    void roiDeleted( const QModelIndex &parent, int start, int end );

    void updateBox( int branch, int pos );
    void boxTest( QVector<bool>& workfield, int left, int right, int axis );

    void updateBranch( int branch );
    void updateRoot();
};

#endif /* FIBERSELECTOR_H_ */