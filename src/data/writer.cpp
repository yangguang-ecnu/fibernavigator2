/*
 * writer.cpp
 *
 * Created on: Jun 1, 2012
 * @author Ralph Schurade
 */
#include "writer.h"

#include "datasets/dataset3d.h"
#include "datasets/datasetbingham.h"
#include "datasets/datasetnifti.h"
#include "datasets/datasetdwi.h"
#include "datasets/datasetscalar.h"
#include "datasets/datasettensor.h"
#include "datasets/datasetsh.h"
#include "datasets/datasetfibers.h"
#include "datasets/datasetmesh.h"
#include "datasets/datasetisosurface.h"

#include "mesh/trianglemesh2.h"

#include <QDebug>

#include <fstream>
#include <iostream>
#include <stdio.h>

Writer::Writer( Dataset* dataset, QString fileName ) :
        m_dataset( dataset ), m_fileName( fileName )
{
}

Writer::~Writer()
{
}

bool Writer::save()
{
    switch ( (Fn::DatasetType)( m_dataset->properties()->get( Fn::Property::TYPE ).toInt() ) )
    {
        case Fn::DatasetType::NIFTI_SCALAR:
        {
            nifti_image* out = createHeader( 1 );

            QVector<float>* data = dynamic_cast<DatasetScalar*>( m_dataset )->getData();

            out->data = data->data();
            if ( nifti_set_filenames( out, m_fileName.toStdString().c_str(), 0, 1 ) )
            {
                qDebug() << "NIfTI filename Problem" << endl;
            }

            nifti_image_write( out );

            out->data = NULL;
            nifti_image_free( out );
        }
            break;
        case Fn::DatasetType::NIFTI_VECTOR:
        {
            QVector<QVector3D>* data = dynamic_cast<Dataset3D*>( m_dataset )->getData();

            nifti_image* img = dynamic_cast<DatasetNifti*>( m_dataset )->getHeader();

            int dim = 3;
            nifti_image* out = createHeader( dim );
            QVector<float> outData( img->nx * img->ny * img->nz * dim );

            int blockSize = img->nx * img->ny * img->nz;
            setDescrip( out, "fnav2_vec3" );

            for ( int z = 0; z < img->nz; ++z )
            {
                for ( int y = 0; y < img->ny; ++y )
                {
                    for ( int x = 0; x < img->nx; ++x )
                    {
                        QVector3D vData = data->at( x + y * img->nx + z * img->nx * img->ny );

                        outData[( x + y * img->nx + z * img->nx * img->ny )] = vData.x();
                        outData[( x + y * img->nx + z * img->nx * img->ny + blockSize )] = vData.y();
                        outData[( x + y * img->nx + z * img->nx * img->ny + 2 * blockSize )] = vData.z();
                    }
                }
            }
            out->data = &( outData[0] );

            if ( nifti_set_filenames( out, m_fileName.toStdString().c_str(), 0, 1 ) )
            {
                qDebug() << "NIfTI filename Problem" << endl;
            }

            nifti_image_write( out );

            out->data = NULL;
            nifti_image_free( out );
        }
            break;
        case Fn::DatasetType::NIFTI_TENSOR:
        {
            nifti_image* out = createHeader( 6 );

            QVector<Matrix>* data = dynamic_cast<DatasetTensor*>( m_dataset )->getData();

            nifti_image* img = dynamic_cast<DatasetNifti*>( m_dataset )->getHeader();
            QVector<float> outData( img->nx * img->ny * img->nz * 6 );

            int blockSize = img->nx * img->ny * img->nz;
            setDescrip( out, "fnav2_tensor" );

            for ( int z = 0; z < img->nz; ++z )
            {
                for ( int y = 0; y < img->ny; ++y )
                {
                    for ( int x = 0; x < img->nx; ++x )
                    {
                        Matrix tensor = data->at( x + y * img->nx + z * img->nx * img->ny );
                        outData[( x + y * img->nx + z * img->nx * img->ny )] = tensor( 1, 1 );
                        outData[( x + y * img->nx + z * img->nx * img->ny ) + blockSize] = tensor( 1, 2 );
                        outData[( x + y * img->nx + z * img->nx * img->ny ) + blockSize * 2] = tensor( 1, 3 );
                        outData[( x + y * img->nx + z * img->nx * img->ny ) + blockSize * 3] = tensor( 2, 2 );
                        outData[( x + y * img->nx + z * img->nx * img->ny ) + blockSize * 4] = tensor( 2, 3 );
                        outData[( x + y * img->nx + z * img->nx * img->ny ) + blockSize * 5] = tensor( 3, 3 );
                    }
                }
            }
            out->data = &( outData[0] );

            if ( nifti_set_filenames( out, m_fileName.toStdString().c_str(), 0, 1 ) )
            {
                qDebug() << "NIfTI filename Problem" << endl;
            }

            nifti_image_write( out );

            out->data = NULL;
            nifti_image_free( out );
        }
            break;
        case Fn::DatasetType::NIFTI_SH:
        {
            QVector<ColumnVector>* data = dynamic_cast<DatasetSH*>( m_dataset )->getData();

            nifti_image* img = dynamic_cast<DatasetNifti*>( m_dataset )->getHeader();

            int dim = data->at( 0 ).Nrows();
            nifti_image* out = createHeader( dim );
            QVector<float> outData( img->nx * img->ny * img->nz * dim );

            int blockSize = img->nx * img->ny * img->nz;
            setDescrip( out, "fnav2_qball" );

            for ( int z = 0; z < img->nz; ++z )
            {
                for ( int y = 0; y < img->ny; ++y )
                {
                    for ( int x = 0; x < img->nx; ++x )
                    {
                        ColumnVector vData = data->at( x + y * img->nx + z * img->nx * img->ny );

                        for ( int i = 0; i < dim; ++i )
                        {
                            outData[( x + y * img->nx + z * img->nx * img->ny + i * blockSize )] = vData( i + 1 );
                        }
                    }
                }
            }
            out->data = &( outData[0] );

            if ( nifti_set_filenames( out, m_fileName.toStdString().c_str(), 0, 1 ) )
            {
                qDebug() << "NIfTI filename Problem" << endl;
            }

            nifti_image_write( out );

            out->data = NULL;
            nifti_image_free( out );
        }
            break;
        case Fn::DatasetType::NIFTI_BINGHAM:
        {
            QVector<QVector<float> >* data = dynamic_cast<DatasetBingham*>( m_dataset )->getData();

            nifti_image* img = dynamic_cast<DatasetNifti*>( m_dataset )->getHeader();

            int dim = data->at( 0 ).size();
            nifti_image* out = createHeader( dim );
            QVector<float> outData( img->nx * img->ny * img->nz * dim );

            int blockSize = img->nx * img->ny * img->nz;
            setDescrip( out, "fnav2_bingham" );

            for ( int z = 0; z < img->nz; ++z )
            {
                for ( int y = 0; y < img->ny; ++y )
                {
                    for ( int x = 0; x < img->nx; ++x )
                    {
                        QVector<float> vData = data->at( x + y * img->nx + z * img->nx * img->ny );

                        for ( int i = 0; i < dim; ++i )
                        {
                            outData[( x + y * img->nx + z * img->nx * img->ny + i * blockSize )] = vData[i];
                        }
                    }
                }
            }
            out->data = &( outData[0] );

            if ( nifti_set_filenames( out, m_fileName.toStdString().c_str(), 0, 1 ) )
            {
                qDebug() << "NIfTI filename Problem" << endl;
            }

            nifti_image_write( out );

            out->data = NULL;
            nifti_image_free( out );
        }
            break;
        case Fn::DatasetType::NIFTI_DWI:
        {
            QVector<ColumnVector>* data = dynamic_cast<DatasetDWI*>( m_dataset )->getData();
            QVector<float>* b0data = dynamic_cast<DatasetDWI*>( m_dataset )->getB0Data();

            nifti_image* img = dynamic_cast<DatasetNifti*>( m_dataset )->getHeader();

            int dim = data->at( 0 ).Nrows() + 1;
            nifti_image* out = createHeader( dim );
            QVector<float> outData( img->nx * img->ny * img->nz * dim );

            QVector<float> bvals = dynamic_cast<DatasetDWI*>( m_dataset )->getBvals();
            QVector<QVector3D> bvecs = dynamic_cast<DatasetDWI*>( m_dataset )->getBvecs();
            QVector<float> bvalOut;
            for ( int i = 0; i < bvals.size(); ++i )
            {
                bvalOut.push_back( bvals[i] );
            }
            for ( int i = 0; i < bvecs.size(); ++i )
            {
                bvalOut.push_back( bvecs[i].x() );
                bvalOut.push_back( bvecs[i].y() );
                bvalOut.push_back( bvecs[i].z() );
            }
            char* extData = reinterpret_cast<char*>( &bvalOut[0] );
            nifti_add_extension( out, extData, bvalOut.size() * 4, 0 );

            int blockSize = img->nx * img->ny * img->nz;
            setDescrip( out, "fnav2_dwi" );

            for ( int i = 0; i < blockSize; ++i )
            {
                outData[i] = b0data->at( i );
            }

            for ( int z = 0; z < img->nz; ++z )
            {
                for ( int y = 0; y < img->ny; ++y )
                {
                    for ( int x = 0; x < img->nx; ++x )
                    {
                        ColumnVector vData = data->at( x + y * img->nx + z * img->nx * img->ny );
                        for ( int i = 1; i < dim; ++i )
                        {
                            outData[( x + y * img->nx + z * img->nx * img->ny + i * blockSize )] = vData( i );
                        }
                    }
                }
            }
            out->data = &( outData[0] );

            if ( nifti_set_filenames( out, m_fileName.toStdString().c_str(), 0, 1 ) )
            {
                qDebug() << "NIfTI filename Problem" << endl;
            }

            nifti_image_write( out );

            out->data = NULL;
            nifti_image_free( out );
        }
            break;
        case Fn::DatasetType::FIBERS:
        {
            saveFibs( m_fileName );
            break;
        }
        case Fn::DatasetType::MESH_BINARY:
        {
            TriangleMesh2* mesh = dynamic_cast<DatasetMesh*>( m_dataset )->getMesh();
            saveMeshVTK( m_fileName, mesh );
            break;
        }
        case Fn::DatasetType::MESH_ISOSURFACE:
        {
            TriangleMesh2* mesh = dynamic_cast<DatasetIsosurface*>( m_dataset )->getMesh();
            saveMeshVTK( m_fileName, mesh );
            break;
        }
        default:
            break;
    }
    return true;
}

nifti_image* Writer::createHeader( int dim )
{
    nifti_image* img = dynamic_cast<DatasetNifti*>( m_dataset )->getHeader();

    nifti_image* out = nifti_simple_init_nim();

    out->nx = img->nx;
    out->ny = img->ny;
    out->nz = img->nz;

    out->nvox = img->nx * img->ny * img->nz * dim;

    out->dx = img->dx;
    out->dy = img->dy;
    out->dz = img->dz;

    out->nifti_type = 1; // 1==NIFTI-1 (1 file)

    out->freq_dim = 1;
    out->phase_dim = 2;
    out->slice_dim = 3;

    out->qform_code = 1;
    out->sform_code = 1;

    out->nt = dim;
    out->nv = 4;
    out->ndim = 4;

    for ( size_t i = 0; i < 4; ++i )
    {
        for ( size_t j = 0; j < 4; ++j )
        {
            out->qto_xyz.m[i][j] = img->qto_xyz.m[i][j];
            out->sto_xyz.m[i][j] = img->sto_xyz.m[i][j];
        }
    }

    {
        float dx, dy, dz;
        nifti_mat44_to_quatern( out->qto_xyz, &( out->quatern_b ), &( out->quatern_c ), &( out->quatern_d ), &( out->qoffset_x ), &( out->qoffset_y ),
                &( out->qoffset_z ), &dx, &dy, &dz, &( out->qfac ) );
    }

    out->qto_ijk = nifti_mat44_inverse( out->qto_xyz );
    out->sto_ijk = nifti_mat44_inverse( out->sto_xyz );

    out->nbyper = 4;
    out->datatype = DT_FLOAT;

    return out;
}

void Writer::setDescrip( nifti_image* hdr, QString descrip )
{
    if ( descrip.length() > 80 )
    {
        qDebug() << "Writer: descrip too long ";
        return;
    }
    for ( int i = 0; i < 80; ++i )
    {
        hdr->descrip[i] = 0;
    }
    for ( int i = 0; i < descrip.length(); ++i )
    {
        hdr->descrip[i] = descrip.at( i ).toLatin1();
    }
}

void Writer::saveFibs( QString filename )
{
    QVector< QVector<float> >fibs = dynamic_cast<DatasetFibers*>( m_dataset )->getFibs();
    QVector< QVector< QVector<float> > >data = dynamic_cast<DatasetFibers*>( m_dataset )->getData();

    using std::fstream;
    fstream out( filename.toStdString().c_str(), fstream::out | fstream::in | fstream::trunc );
    if( !out || out.bad() )
    {
        // error
    }
    // We use '\n' as line delimiter so also files written under windows (having '\r\n' as delimtier) may be read anywhere
    char lineDelimiter = '\n';

    out << "# vtk DataFile Version 3.0" << lineDelimiter;
    out << "Fibers" << lineDelimiter;
    out << "BINARY" << lineDelimiter;
    out << "DATASET POLYDATA" << lineDelimiter;

    unsigned int numPoints = 0;
    unsigned int numLines = fibs.size();
    for( int i = 0; i < fibs.size(); ++i )
    {
        numPoints += fibs[i].size()/3;
    }
    out << "POINTS " << numPoints << " float" << lineDelimiter;
    unsigned int *rawLineData = new unsigned int[numPoints + numLines];
    float *rawPointData = new float[numPoints * 3];

    unsigned int pntPosOffset = 0;
    unsigned int lnsPosOffset = 0;

    for( int i = 0; i < fibs.size(); ++i )
    {
        QVector<float>fib = fibs[i];
        rawLineData[lnsPosOffset++] = static_cast< unsigned int >( fib.size()/3 );
        for( int j = 0; j < fib.size()/3; ++j )
        {
            rawLineData[lnsPosOffset++] = static_cast< unsigned int >( pntPosOffset / 3 );
            rawPointData[pntPosOffset++] = static_cast< float >( fib[j*3] );
            rawPointData[pntPosOffset++] = static_cast< float >( fib[j*3+1] );
            rawPointData[pntPosOffset++] = static_cast< float >( fib[j*3+2] );
        }
    }

    switchByteOrderOfArray< float >( rawPointData, numPoints * 3 );
    switchByteOrderOfArray< unsigned int >( rawLineData, numLines + numPoints );
    out.write( reinterpret_cast< char* >( rawPointData ), sizeof( float ) * numPoints * 3 );
    out << lineDelimiter;
    out << "LINES " << numLines << " " << numPoints + numLines << lineDelimiter;
    out.write( reinterpret_cast< char* >( rawLineData ), sizeof( unsigned int ) * ( numPoints + numLines ) );
    out << lineDelimiter;
    out.close();
}

void Writer::saveMeshVTK( QString filename, TriangleMesh2* mesh )
{
    int bufferSize = mesh->bufferSize();
    float* points = mesh->getVertices();
    int* indexes = mesh->getIndexes();
    float* colors = mesh->getVertexColors();

    int numPoints = mesh->numVerts();
    int numTris = mesh->numTris();

    float* pointsOut = new float[ numPoints * 3 ];
    float* colorsOut = new float[ numPoints * 3 ];
    int* trisOut = new int[numTris*4];

    for ( int i = 0; i < numTris; ++i )
    {
        trisOut[ 4 * i    ] = 3;
        trisOut[4 * i + 1 ] = indexes[ 3 * i ];
        trisOut[4 * i + 2 ] = indexes[ 3 * i + 1];
        trisOut[4 * i + 3 ] = indexes[ 3 * i + 2];
    }

    for ( int i = 0; i < numPoints; ++i )
    {
        pointsOut[3*i  ] = points[bufferSize * i];
        pointsOut[3*i+1] = points[bufferSize * i+1];
        pointsOut[3*i+2] = points[bufferSize * i+2];
        colorsOut[3*i  ] = colors[4*i];
        colorsOut[3*i+1] = colors[4*i+1];
        colorsOut[3*i+2] = colors[4*i+2];
    }

    switchByteOrderOfArray< float >( pointsOut, numPoints * 3 );
    switchByteOrderOfArray< float >( colorsOut, numPoints * 3 );
    switchByteOrderOfArray< int >( trisOut, numTris * 4 );


    using std::fstream;
    fstream out( filename.toStdString().c_str(), fstream::out | fstream::in | fstream::trunc );
    if( !out || out.bad() )
    {
        // error
    }
    // We use '\n' as line delimiter so also files written under windows (having '\r\n' as delimtier) may be read anywhere
    char lineDelimiter = '\n';

    out << "# vtk DataFile Version 3.0" << lineDelimiter;
    out << "Fibernavigator 2 Mesh" << lineDelimiter;
    out << "BINARY" << lineDelimiter;
    out << "DATASET POLYDATA" << lineDelimiter;

    out << "POINTS " << numPoints << " float" << lineDelimiter;
    out.write( reinterpret_cast< char* >( pointsOut ), sizeof( float ) * numPoints * 3 );
    out << lineDelimiter;

    out << "POLYGONS " << numTris << " " << numTris * 4 << lineDelimiter;
    out.write( reinterpret_cast< char* >( trisOut ), sizeof( int ) * ( numTris * 4 ) );
    out << lineDelimiter;

    out << "POINT_DATA " << numPoints << lineDelimiter;
    out << "COLOR_SCALARS Colors 3" << lineDelimiter;
    out.write( reinterpret_cast< char* >( colorsOut ), sizeof( float ) * numPoints * 3 );
    out << lineDelimiter;

    out.close();

    delete[] pointsOut;
    delete[] colorsOut;
    delete[] trisOut;
}
