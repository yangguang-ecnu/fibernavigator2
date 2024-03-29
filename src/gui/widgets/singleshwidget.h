/*
 * singleshwidget.h
 *
 * Created on: 12.07.2012
 * @author Ralph Schurade
 */

#ifndef SINGLESHWIDGET_H_
#define SINGLESHWIDGET_H_

#include "GL/glew.h"
#include <QtOpenGL/QGLWidget>

class SingleSHRenderer;

class SingleSHWidget : public QGLWidget
{
    Q_OBJECT

public:
    SingleSHWidget( QString name, QWidget *parent = 0, const QGLWidget *shareWidget = 0 );
    virtual ~SingleSHWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

public slots:
    void update();

signals:

protected:
    void initializeGL();
    void paintGL();
    void resizeGL( int width, int height );
    void mousePressEvent( QMouseEvent *event );
    void mouseReleaseEvent( QMouseEvent *event );
    void mouseMoveEvent( QMouseEvent *event );

private:
    SingleSHRenderer* m_renderer;
};

#endif /* SINGLESHWIDGET_H_ */
