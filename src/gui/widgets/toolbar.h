/*
 * toolbar.h
 *
 * Created on: 14.06.2012
 * @author Ralph Schurade
 */

#ifndef TOOLBAR_H_
#define TOOLBAR_H_

#include "controls/fnaction.h"

#include "../../data/enums.h"

#include <QtGui>

class ToolBarView;
class TensorTrackWidget;
class CrossingTrackWidget;

class ToolBar : public QToolBar
{
    Q_OBJECT

public:
    ToolBar( const QString &title, QWidget* parent=0 );
    virtual ~ToolBar();

    void setSelectionModel( QItemSelectionModel* selectionModel );

private:
    ToolBarView* m_toolBarView;

    FNAction* m_isosurfaceAct;
    FNAction* m_distanceMapAct;
    FNAction* m_testAct;
    FNAction* m_vectorAction1;
    FNAction* m_qball4Act;
    FNAction* m_qball6Act;
    FNAction* m_qball8Act;
    FNAction* m_tensorAct;
    FNAction* m_faAct;
    FNAction* m_evAct;
    FNAction* m_meshAction1;
    FNAction* m_fiberTrackingAct;
    FNAction* m_crossingTrackingAct;
    FNAction* m_fiberThinningAct;
    FNAction* m_fiberTractDensityAct;
    FNAction* m_fiberTractColorAct;
    FNAction* m_binghamAction;
    FNAction* m_bingham2tensorAction;
    FNAction* m_cutSelectedFibersAction;

    TensorTrackWidget* m_ttw;
    CrossingTrackWidget* m_ctw;

    void createActions();

private slots:
    void slot( Fn::Algo algo );
    void slotSelectionChanged( int type );

    void tensorTrackFinished();
    void crossingTrackFinished();
};

#endif /* TOOLBAR_H_ */
