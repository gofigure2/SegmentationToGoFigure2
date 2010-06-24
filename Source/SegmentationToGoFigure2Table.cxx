/*=========================================================================
  Author: $Author: krm15 $  // Author of last commit
  Version: $Rev: 1473 $  // Revision of last commit
  Date: $Date: 2010-05-18 17:31:00 -0400 (Tue, 18 May 2010) $  // Date of last commit
=========================================================================*/

/*=========================================================================
 Authors: The GoFigure Dev. Team.
 at Megason Lab, Systems biology, Harvard Medical school, 2009

 Copyright (c) 2009, President and Fellows of Harvard College.
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice,
 this list of conditions and the following disclaimer.
 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 Neither the name of the  President and Fellows of Harvard College
 nor the names of its contributors may be used to endorse or promote
 products derived from this software without specific prior written
 permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkSegmentationToGoFigure2Table.h"
#include "itkCellSegmentationStatistics.h"
#include "itkVector.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "vtkPolyData.h"
#include "vtkPolyDataReader.h"
#include "vtkSmartPointer.h"
#include "vtkPolyDataMySQLTextWriter.h"

int main ( int argc, char* argv[] )
{
  if ( argc < 7 )
  {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " cellImage membraneImage segImage";
    std::cerr << " trackID modelDir outFile " << std::endl;
    return EXIT_FAILURE;
  }

  const unsigned int Dimension = 3;
  typedef itk::Image< unsigned short, Dimension > FeatureImageType;
  typedef itk::Image< float, Dimension >         InputImageType;
  typedef itk::Image< unsigned short, Dimension >  SegmentImageType;

  typedef itk::ImageFileReader< FeatureImageType > FeatureReaderType;
  typedef itk::ImageFileReader< SegmentImageType > SegmentReaderType;

  typedef itk::SegmentationToGoFigure2Table< SegmentImageType > FilterType;

  typedef itk::CellSegmentationStatistics< FeatureImageType,
    InputImageType, SegmentImageType > StatisticsFilterType;

  typedef vnl_vector< unsigned int > VectorType;

  // Read cell image
  FeatureReaderType::Pointer cell = FeatureReaderType::New();
  cell->SetFileName ( argv[1] );
  cell->Update();
  FeatureImageType::Pointer nucleiImg = cell->GetOutput();

  FeatureReaderType::Pointer membrane = FeatureReaderType::New();
  membrane->SetFileName ( argv[2] );
  membrane->Update();
  FeatureImageType::Pointer membraneImg = membrane->GetOutput();

  // Read segmentation image
  SegmentReaderType::Pointer reader = SegmentReaderType::New();
  reader->SetFileName ( argv[3] );
  reader->Update();
  SegmentImageType::Pointer segment = reader->GetOutput();

  // Compute statistics
  StatisticsFilterType::Pointer stat_nuclei = StatisticsFilterType::New();
  stat_nuclei->SetInput ( segment );
  stat_nuclei->SetRawImage ( nucleiImg );
  stat_nuclei->SetLargestCellRadius ( 5.0 );
  stat_nuclei->GenerateData();

  StatisticsFilterType::Pointer stat_membrane = StatisticsFilterType::New();
  stat_membrane->SetInput ( segment );
  stat_membrane->SetRawImage ( membraneImg );
  stat_membrane->SetLargestCellRadius ( 5.0 );
  stat_membrane->GenerateData();

  // Read trackId vector
  unsigned int m_NumberOfCells = stat_nuclei->GetNumOfLabels();
  std::ifstream trackFile( argv[4] );

//   while ( getline( trackFile, s ) )
//     {
//     m_NumberOfCells++;
//     }

  unsigned int r = 0;
  VectorType trackId;
  trackId.set_size( m_NumberOfCells );
  trackFile.seekg (0, std::ios::beg);
  while ( !trackFile.eof() )
    {
    trackFile >> trackId[r++];
    }

  std::string polyDataFile, s;
  vtkSmartPointer<vtkPolyDataMySQLTextWriter> convertPolyDataToString =
    vtkSmartPointer<vtkPolyDataMySQLTextWriter>::New();

  // Update the contents of the filter
  FilterType::Pointer filter = FilterType::New();
  
  filter->m_ImagingSession.Name = "TimePoint 00 to 09";
  filter->m_ImagingSession.CreationDate = "2009-11-05 09:13:35";
  filter->m_ImagingSession.MicroscopeName = "Cyclops";

  // Fill color table
  filter->SetNumberOfColors ( 2 );
  filter->m_Colors[0].Alpha = 255;
  filter->m_Colors[0].Blue = 0;
  filter->m_Colors[0].ColorID = 1;
  filter->m_Colors[0].Green = 0;
  filter->m_Colors[0].Red = 255;
  filter->m_Colors[0].Name = "Red";
  filter->m_Colors[0].Description = "Membrane";

  filter->m_Colors[1].Alpha = 255;
  filter->m_Colors[1].Blue = 0;
  filter->m_Colors[1].ColorID = 2;
  filter->m_Colors[1].Green = 255;
  filter->m_Colors[1].Red = 0;
  filter->m_Colors[1].Name = "Green";
  filter->m_Colors[1].Description = "Nuclei";

  // Fill cell type table
  filter->SetNumberOfCellTypes ( 1 );
  filter->m_CellTypes[0].CellTypeID = 1;
  filter->m_CellTypes[0].Name = "Epithelial";
  filter->m_CellTypes[0].Description = "Ear Development";

  // Fill subcellulartype table
  filter->SetNumberOfSubCellularTypes ( 0 );

  // Fill channel tables
  filter->SetNumberOfChannels ( 2 );
  filter->m_Channels[0].ChannelID = 1;
  filter->m_Channels[0].ChannelNumber = 0;
  filter->m_Channels[0].NumberOfBits = 8;
  filter->m_Channels[0].ColorID = 1;
  filter->m_Channels[0].Name = "RFP";

  filter->m_Channels[1].ChannelID = 2;
  filter->m_Channels[1].ChannelNumber = 1;
  filter->m_Channels[1].NumberOfBits = 8;
  filter->m_Channels[1].ColorID = 2;
  filter->m_Channels[1].Name = "H2B";

  // Fill coordinates table
  filter->SetNumberOfCoordinates ( 2 * m_NumberOfCells );

  // Fill track table
  filter->SetNumberOfTracks ( m_NumberOfCells );

  // Fill intensity tables
  filter->SetNumberOfIntensitys( 2 * m_NumberOfCells );

  // Fill mesh table
  filter->SetNumberOfMeshes ( m_NumberOfCells );
  for( unsigned int i = 0; i < m_NumberOfCells; i++ )
    {
    // first the min of the bounding box
    filter->m_Coordinates[2*i].CoordID = 2*i;
    filter->m_Coordinates[2*i].PCoord = 0;
    filter->m_Coordinates[2*i].RCoord = 0;
    filter->m_Coordinates[2*i].CCoord = 0;
    filter->m_Coordinates[2*i].XTileCoord = 0;
    filter->m_Coordinates[2*i].YTileCoord = 0;
    filter->m_Coordinates[2*i].ZTileCoord = 0;
    filter->m_Coordinates[2*i].XCoord = stat_nuclei->m_Region[i].GetIndex()[0];
    filter->m_Coordinates[2*i].YCoord = stat_nuclei->m_Region[i].GetIndex()[1];
    filter->m_Coordinates[2*i].ZCoord = stat_nuclei->m_Region[i].GetIndex()[2];
    filter->m_Coordinates[2*i].TCoord = 0;
    
    // then the max of the bounding box
    filter->m_Coordinates[2*i + 1].CoordID = 2*i + 1;
    filter->m_Coordinates[2*i + 1].PCoord = 0;
    filter->m_Coordinates[2*i + 1].RCoord = 0;
    filter->m_Coordinates[2*i + 1].CCoord = 0;
    filter->m_Coordinates[2*i + 1].XTileCoord = 0;
    filter->m_Coordinates[2*i + 1].YTileCoord = 0;
    filter->m_Coordinates[2*i + 1].ZTileCoord = 0;
    filter->m_Coordinates[2*i + 1].XCoord = stat_nuclei->m_Region[i].GetIndex()[0] + stat_nuclei->m_Region[i].GetSize()[0] - 1;
    filter->m_Coordinates[2*i + 1].YCoord = stat_nuclei->m_Region[i].GetIndex()[1] + stat_nuclei->m_Region[i].GetSize()[1] - 1;
    filter->m_Coordinates[2*i + 1].ZCoord = stat_nuclei->m_Region[i].GetIndex()[2] + stat_nuclei->m_Region[i].GetSize()[2] - 1;
    filter->m_Coordinates[2*i + 1].TCoord = 0;

    filter->m_Tracks[i].TrackID = i;
    filter->m_Tracks[i].LineageID = 0;
    filter->m_Tracks[i].ColorID = 2;
    filter->m_Tracks[i].CoordIDMax = 2*i;
    filter->m_Tracks[i].CoordIDMin = 2*i + 1;
    filter->m_Tracks[i].TrackFamilyID = 0;

    filter->m_Meshes[i].MeshID = i;
    filter->m_Meshes[i].CellTypeID = 1;
    filter->m_Meshes[i].SubCellularTypeID = 0;
    filter->m_Meshes[i].CoordIDMax = 2*i + 1;
    filter->m_Meshes[i].CoordIDMin = 2*i;
    filter->m_Meshes[i].ColorID = 2;
    filter->m_Meshes[i].TrackID = trackId[i];
    filter->m_Meshes[i].ImagingSessionID = 0;

    // update vtkFilename
    std::ostringstream istream;
    istream << argv[5] << "0_" << stat_nuclei->m_LabelLookup[i] << ".vtk";
    polyDataFile = istream.str();

    // read vtkPolydata
    vtkSmartPointer<vtkPolyDataReader> model = vtkSmartPointer<vtkPolyDataReader>::New();

    model->SetFileName( polyDataFile.c_str() );
    model->Update();
    vtkPolyData* mesh = model->GetOutput();
    s = convertPolyDataToString->GetMySQLText( mesh );

    filter->m_Meshes[i].Points = s;

    filter->m_Intensitys[2*i].IntensityID = 2*i;
    filter->m_Intensitys[2*i].Value = stat_nuclei->m_Mean[i] * stat_nuclei->m_Size[i];
    filter->m_Intensitys[2*i].MeshID = i;
    filter->m_Intensitys[2*i].ChannelID = 2;

    filter->m_Intensitys[2*i + 1].IntensityID = 2*i + 1;
    filter->m_Intensitys[2*i + 1].Value = stat_membrane->m_Mean[i] * stat_membrane->m_Size[i];
    filter->m_Intensitys[2*i + 1].MeshID = i;
    filter->m_Intensitys[2*i + 1].ChannelID = 1;

    }

  std::fstream outFile;
  outFile.open ( argv[6],std::ios::out );

  filter->PrintStart( outFile, 0 );
  filter->PrintColor( outFile, 2 );
  filter->PrintCell( outFile, 2 );
  filter->PrintSubCellular( outFile, 2 );
  filter->PrintCoordinate( outFile, 2 );
  filter->PrintLineage( outFile, 2 );
  filter->PrintTrack( outFile, 2 );
  filter->PrintMesh( outFile, 2 );
  filter->PrintChannel( outFile, 2 );
  filter->PrintIntensity( outFile, 2 );
  filter->PrintEnd( outFile, 0 );

  outFile.close();

  return EXIT_SUCCESS;
  }
