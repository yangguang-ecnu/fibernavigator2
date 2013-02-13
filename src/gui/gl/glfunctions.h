/*
 * glfunctions.h
 *
 *  Created on: 15.05.2012
 *      Author: Ralph Schurade
 */

#ifndef GLFUNCTIONS_H_
#define GLFUNCTIONS_H_

#include "../../thirdparty/glew/include/glew.h"

#include "../../algos/colormapbase.h"

#include <QtCore/QList>
#include <QtGui/QVector3D>
#include <QtGui/QImage>

class QAbstractItemModel;
class QGLShaderProgram;

struct VertexData
{
    QVector3D position;
    QVector3D texCoord;
};


class GLFunctions
{
public:
    static void setupTextures( QAbstractItemModel* model );
    static void setTexInterpolation( QAbstractItemModel* model, int row );

    static QGLShaderProgram* initShader( QString name );
    static void loadShaders();
    static void reloadShaders();

    static void setShaderVars( QString name, QAbstractItemModel* model = 0 );
    static void setShaderVarsTensor( QGLShaderProgram* program, QAbstractItemModel* model );
    static void setShaderVarsTensorEV( QGLShaderProgram* program, QAbstractItemModel* model );
    static void setShaderVarsEV( QGLShaderProgram* program, QAbstractItemModel* model );
    static void setShaderVarsSlice( QGLShaderProgram* program, QAbstractItemModel* model );
    static void setShaderVarsQBall( QGLShaderProgram* program, QAbstractItemModel* model );
    static void setShaderVarsMesh( QGLShaderProgram* program, QAbstractItemModel* model );
    static void setShaderVarsFiber( QGLShaderProgram* program, QAbstractItemModel* model );
    static void setShaderVarsBox( QGLShaderProgram* program );

    static QList<int> getTextureIndexes( QAbstractItemModel* model );

    static QGLShaderProgram* getShader( QString name );

    static QVector<QString> getShaderNames();
    static QString getShaderCode( QString name );
    static void setShaderCode( QString name,  QString source );

    static int getPickIndex();

    static void generate_frame_buffer_texture( const int screen_width, const int screen_height );
    static void generate_pixel_buffer_objects();
    static uint get_object_id( int x, int y );

    static void beginPicking();
    static void endPicking();
    static bool isPicking();

    static void beginOffscreen();
    static void endOffscreen();

    static QImage* getOffscreenTexture();

    static void updateColormapShader();

    static void addColormap( ColormapBase colormap );
    static void addColormap2( ColormapBase colormap );
    static ColormapBase getColormap( int id );

private:
    GLFunctions();
    virtual ~GLFunctions();

    static void copyShaderToString( QString name, QString ext );

    static QHash< QString, QString >m_shaderSources;
    static QHash< QString, QGLShaderProgram* >m_shaders;
    static QVector<QString>m_shaderNames;
    static QVector<ColormapBase>m_colormaps;

    static bool shadersLoaded;
    static unsigned int pickIndex;
    static bool picking;

    static GLuint tex;
    static GLuint rbo;
    static GLuint fbo;
    static GLuint pbo_a;
    static GLuint pbo_b;
    static int screenWidth;
    static int screenHeight;
};

#endif /* GLFUNCTIONS_H_ */
