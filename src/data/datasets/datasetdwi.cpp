/*
 * dataset3d.cpp
 *
 *  Created on: May 9, 2012
 *      Author: schurade
 */
#include <QtCore/QDebug>

#include "datasetdwi.h"

DatasetDWI::DatasetDWI( QString filename, QVector<ColumnVector>* data, QVector<float> b0Data, QVector<int>bvals, QVector<QVector3D>bvecs, nifti_image* header ) :
        DatasetNifti( filename, FNDT_NIFTI_DWI, header ),
        m_data( data ),
        m_b0Data( b0Data ),
        m_bvals( bvals ),
        m_bvecs( bvecs )
{
    m_properties["active"] = true;
    m_properties["colormap"] = 0;
    m_properties["interpolation"] = false;
    m_properties["alpha"] = 1.0;
    m_properties["order"] = 4;

    examineDataset();
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

        for ( int i = 0; i < m_b0Data.size(); ++i )
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

    if ( m_qform( 1, 1 ) < 0 || m_sform( 1, 1 ) < 0 )
    {
        qDebug() << m_properties["name"].toString() << ": RADIOLOGICAL orientation detected. Flipping voxels on X-Axis";
        flipX();
    }
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

    float* tmpData = new float[nx*ny*nz];
    float max = m_properties["max"].toFloat();
    for ( int i = 0; i < nx*ny*nz; ++i )
    {
        tmpData[i] = m_b0Data[i] / max;
    }

    glTexImage3D( GL_TEXTURE_3D, 0, GL_LUMINANCE_ALPHA, nx, ny, nz, 0, GL_LUMINANCE, GL_FLOAT, tmpData );
    delete[] tmpData;

}

void DatasetDWI::flipX()
{
    int xDim = m_properties["nx"].toInt();
    int yDim = m_properties["ny"].toInt();
    int zDim = m_properties["nz"].toInt();

    QVector<ColumnVector>* newData = new QVector<ColumnVector>();
    QVector<float>newB0Data;

    for( int z = 0; z < zDim; ++z )
    {
        for( int y = 0; y < yDim; ++y )
        {
            for( int x = xDim -1; x >= 0; --x )
            {
                newData->push_back( m_data->at( x + y * xDim + z * xDim * yDim ) );
                newB0Data.push_back( m_b0Data[x + y * xDim + z * xDim * yDim]);
            }
        }
    }

    for( int i = 0; i < m_bvecs.size(); ++i )
    {
        m_bvecs[i].setX( m_bvecs[i].x() * -1.0 );
    }

    m_header->qto_xyz.m[0][0] = qMax( m_header->qto_xyz.m[0][0], m_header->qto_xyz.m[0][0] * -1.0f );
    m_header->sto_xyz.m[0][0] = qMax( m_header->sto_xyz.m[0][0], m_header->sto_xyz.m[0][0] * -1.0f );
    m_header->qto_xyz.m[0][3] = 0.0;
    m_header->sto_xyz.m[0][3] = 0.0;

    m_data->clear();
    m_data = newData;
    m_b0Data = newB0Data;
}
