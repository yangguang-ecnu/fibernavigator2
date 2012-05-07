#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtOpenGL/QGLWidget>

class QtLogo;

class GLWidget: public QGLWidget
{
    Q_OBJECT

public:
    GLWidget( QWidget *parent = 0 );
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

public slots:

    signals:

protected:
    void initializeGL();
    void paintGL();
    void resizeGL( int width, int height );
    void mousePressEvent( QMouseEvent *event );
    void mouseMoveEvent( QMouseEvent *event );

private:
};

#endif