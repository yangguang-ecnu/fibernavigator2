#include <QtGui/QtGui>

#include "../../data/datastore.h"
#include "../gl/combinednavrenderer.h"

#include "combinednavglwidget.h"

CombinedNavGLWidget::CombinedNavGLWidget( DataStore* dataStore, QString name, QWidget *parent, const QGLWidget *shareWidget ) :
	QGLWidget( parent, shareWidget )
{
    m_renderer = new CombinedNavRenderer( name );
    m_renderer->setModel( dataStore );
    connect( dataStore, SIGNAL( dataChanged( QModelIndex, QModelIndex ) ), this, SLOT( update() ) );
}

CombinedNavGLWidget::~CombinedNavGLWidget()
{
}

QSize CombinedNavGLWidget::minimumSizeHint() const
{
    return QSize( 50, 50 );
}

QSize CombinedNavGLWidget::sizeHint() const
{
    return QSize( 400, 400 );
}

void CombinedNavGLWidget::initializeGL()
{
	m_renderer->initGL();
}

void CombinedNavGLWidget::paintGL()
{
	m_renderer->draw();
}

void CombinedNavGLWidget::resizeGL( int width, int height )
{
	m_renderer->resizeGL( width, height );
}

void CombinedNavGLWidget::mousePressEvent( QMouseEvent *event )
{
    if ( event->buttons() & Qt::LeftButton )
    {
        //m_sceneRenderer->leftMouseDown( event->x(), event->y() );
    }
    updateGL();
}

void CombinedNavGLWidget::mouseMoveEvent( QMouseEvent *event )
{
    if ( event->buttons() & Qt::LeftButton )
    {
        //m_sceneRenderer->leftMouseDrag( event->x(), event->y() );
    }
    updateGL();
}

void CombinedNavGLWidget::mouseReleaseEvent( QMouseEvent *event )
{
    updateGL();
}

void CombinedNavGLWidget::update()
{
    updateGL();
}