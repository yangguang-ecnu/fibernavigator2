/*
 * dataset3d.cpp
 *
 *  Created on: May 9, 2012
 *      Author: schurade
 */
#include <QtCore/QDebug>

#include "datasetdwi.h"

DatasetDWI::DatasetDWI( QString filename, QVector<ColumnVector>* data, QVector<float> b0Data, QVector<int>bvals, QVector<QVector3D>bvecs ) :
        DatasetNifti( filename, FNDT_NIFTI_DWI ),
        m_data( data ),
        m_b0Data( b0Data ),
        m_bvals( bvals ),
        m_bvecs( bvecs )
{
    m_properties["active"] = true;
    m_properties["colormap"] = 0;
    m_properties["interpolation"] = false;
    m_properties["alpha"] = 1.0;
}

DatasetDWI::~DatasetDWI()
{
    m_data->clear();
    m_b0Data.clear();
    m_bvals.clear();
    m_bvecs.clear();
}

QVector<ColumnVector>* DatasetDWI::getData()
{
    return m_data;
}

QVector<float> DatasetDWI::getB0Data()
{
    return m_b0Data;
}

QVector<int> DatasetDWI::getBvals()
{
    return m_bvals;
}

QVector<QVector3D> DatasetDWI::getBvecs()
{
    return m_bvecs;
}

void DatasetDWI::examineDataset()
{
    int type = getProperty( "datatype" ).toInt();
    int nx = getProperty( "nx" ).toInt();
    int ny = getProperty( "ny" ).toInt();
    int nz = getProperty( "nz" ).toInt();
    int dim = getProperty( "nt" ).toInt();
    int size = nx * ny * nz * dim;

    if ( type == DT_UNSIGNED_CHAR )
    {
        m_properties["size"] = static_cast<int>( size * sizeof( unsigned char ) );

        m_properties["min"] = 0;
        m_properties["max"] = 255;
    }


    if ( type == DT_SIGNED_SHORT )
    {
        m_properties["size"] = static_cast<int>( size * sizeof( unsigned char ) );

        float max = -32767;
        float min = 32768;

        for ( size_t i = 0; i < m_b0Data.size(); ++i )
        {
            min = qMin( min, m_b0Data[ i ] );
            max = qMax( max, m_b0Data[ i ] );
        }
        m_properties["min"] = min;
        m_properties["max"] = max;

    }


    if ( type == DT_FLOAT )
    {
        m_properties["size"] = static_cast<int>( size * sizeof( float ) );

        m_properties["min"] = -1.0;
        m_properties["max"] = 1.0;
    }
    m_properties["lowerThreshold"] = m_properties["min"].toFloat();
    m_properties["upperThreshold"] = m_properties["max"].toFloat();
}

void DatasetDWI::createTexture()
{
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

    glGenTextures( 1, &m_textureGLuint );

    glBindTexture( GL_TEXTURE_3D, m_textureGLuint );

    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP );
    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP );


    int nx = getProperty( "nx" ).toInt();
    int ny = getProperty( "ny" ).toInt();
    int nz = getProperty( "nz" ).toInt();
    int type = getProperty( "datatype" ).toInt();

    int mult = 65535 / m_properties["max"].toFloat();

    float* tmpData = new float[nx*ny*nz];
    float max = m_properties["max"].toFloat();
    for ( int i = 0; i < nx*ny*nz; ++i )
    {
        tmpData[i] = m_b0Data[i] / max;
    }

    glTexImage3D( GL_TEXTURE_3D, 0, GL_LUMINANCE_ALPHA, nx, ny, nz, 0, GL_LUMINANCE, GL_FLOAT, tmpData );
    delete[] tmpData;

}
