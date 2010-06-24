/*=========================================================================
  Author: $Author: krm15 $  // Author of last commit
  Version: $Rev: 807 $  // Revision of last commit
  Date: $Date: 2009-11-03 22:35:10 -0500 (Tue, 03 Nov 2009) $  // Date of last commit
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
//
// GenerateModelsFromLabels
//   Usage: GenerateModelsFromLabels InputVolume Startlabel Endlabel
//          where
//          InputVolume is a meta file containing a 3 volume of
//            discrete labels.
//          StartLabel is the first label to be processed
//          EndLabel is the last label to be processed
//          NOTE: There can be gaps in the labeling. If a label does
//          not exist in the volume, it will be skipped.
//
//
#include "vtkDiscreteMarchingCubes.h"
#include "vtkWindowedSincPolyDataFilter.h"
#include "vtkMaskFields.h"
#include "vtkThreshold.h"
#include "vtkGeometryFilter.h"
#include <vtkPolyDataConnectivityFilter.h>
#include "vtkPolyDataWriter.h"
#include "vtkSmartPointer.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkUnstructuredGrid.h"
#include <vtksys/ios/sstream>
// #include "vtkQuadricDecimation.h"
#include "itkVTKImageExport.h"
#include "vtkImageImport.h"

#include "itkNumericSeriesFileNames.h"
#include "itkImageFileReader.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkRegionOfInterestImageFilter.h"
#include "itkShapeLabelObject.h"
#include "itkLabelMap.h"
#include "itkLabelImageToShapeLabelMapFilter.h"

int main (int argc, char *argv[])
{
  if (argc < 3)
    {
    std::cout << "Usage: " << argv[0] << " InputVolume OutputFormat" << std::endl;
    return EXIT_FAILURE;
    }

  const unsigned int Dimension = 3;
  typedef itk::Image< unsigned int, Dimension > SegmentImageType;
  typedef SegmentImageType::RegionType SegmentRegionType;
  typedef SegmentImageType::IndexType SegmentIndexType;
  typedef SegmentIndexType::IndexValueType IndexValueType;
  typedef SegmentImageType::SizeType  SegmentSizeType;
  typedef SegmentSizeType::SizeValueType SizeValueType;
  typedef itk::NumericSeriesFileNames NameGeneratorType;
  typedef itk::ImageFileReader< SegmentImageType > ReaderType;
  typedef unsigned int ShapeLabelType;
  typedef itk::ShapeLabelObject< ShapeLabelType, Dimension > ShapeLabelObjectType;
  typedef itk::LabelMap< ShapeLabelObjectType > ShapeLabelMapType;
  typedef itk::LabelImageToShapeLabelMapFilter< SegmentImageType, ShapeLabelMapType >
    ShapeConverterType;
  typedef itk::RegionOfInterestImageFilter< SegmentImageType, SegmentImageType >
    ROIFilterType;
  typedef itk::ImageRegionIteratorWithIndex< SegmentImageType > IndexIteratorType;
  typedef itk::VTKImageExport< SegmentImageType > ExportType;

  // Create all of the classes we will need
  vtkSmartPointer<vtkDiscreteMarchingCubes> discreteCubes =
    vtkSmartPointer<vtkDiscreteMarchingCubes>::New();
//   vtkSmartPointer<vtkQuadricDecimation> decimate =
//     vtkSmartPointer<vtkQuadricDecimation>::New();
  vtkSmartPointer<vtkWindowedSincPolyDataFilter> smoother =
    vtkSmartPointer<vtkWindowedSincPolyDataFilter>::New();
  vtkSmartPointer<vtkThreshold> selector =
    vtkSmartPointer<vtkThreshold>::New();
  vtkSmartPointer<vtkMaskFields> scalarsOff =
    vtkSmartPointer<vtkMaskFields>::New();
  vtkSmartPointer<vtkGeometryFilter> geometry =
    vtkSmartPointer<vtkGeometryFilter>::New();
  vtkSmartPointer<vtkPolyDataConnectivityFilter> comp =
    vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
  vtkSmartPointer<vtkPolyDataWriter> writer =
    vtkSmartPointer<vtkPolyDataWriter>::New();

  // Define all of the variables
  unsigned int smoothingIterations = 15;
  double passBand = 0.001;
  double featureAngle = 120.0;

  // Generate models from labels
  // 1) Read the meta file in ITK
  // 2) Compute bounding boxes
  // 3) Pass the bounding boxes to ROI filter
  // 4) Send ROI output to ITK2VTK converter
  // 5) Send to discreteCubes
  // 6) Generate a histogram of the labels
  // 7) Generate models from the labeled volume
  // 8) Smooth the models
  // 9) Output each model into a separate file

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName ( argv[1] );
  reader->Update();
  SegmentImageType::Pointer output = reader->GetOutput();

  typedef SegmentImageType::IndexType IndexType;
  typedef IndexType::IndexValueType IndexValueType;
  bool flag;
  SegmentImageType::SizeType size = output->GetLargestPossibleRegion().GetSize();
  SegmentImageType::IndexType index;
  IndexIteratorType It( output, output->GetLargestPossibleRegion() );
  It.GoToBegin();
  while( !It.IsAtEnd() )
    {
    index = It.GetIndex();
    flag = false;
    for( unsigned int i = 0; i < Dimension; i++ )
      {
      if ( ( index[i] == 0 ) || ( index[i] == static_cast<IndexValueType>( size[i] - 1 ) ) )
        {
        flag = true;
        }
      }
    if ( flag )
      {
      It.Set( 0 );
      }
    ++It;
    }

  ShapeConverterType::Pointer shapeConverter =
    ShapeConverterType::New();
  shapeConverter->SetInput ( reader->GetOutput() );
  shapeConverter->SetBackgroundValue ( 0 );
  shapeConverter->Update();
  ShapeLabelMapType::Pointer shapeLabelMap =
    shapeConverter->GetOutput();

  unsigned int N = shapeLabelMap->GetNumberOfLabelObjects();

  ROIFilterType::Pointer ROIfilter = ROIFilterType::New();
  ROIfilter->SetInput ( reader->GetOutput() );

  NameGeneratorType::Pointer nameGeneratorOutput =
    NameGeneratorType::New();
  nameGeneratorOutput->SetSeriesFormat( argv[2] );
  nameGeneratorOutput->SetStartIndex( 1 );
  nameGeneratorOutput->SetEndIndex( N );
  nameGeneratorOutput->SetIncrementIndex( 1 );

  SegmentRegionType region;
  SegmentSizeType inputSize = reader->GetOutput()->GetLargestPossibleRegion().GetSize();

  for (unsigned int i = 1; i <= N; i++)
    {
    region = shapeLabelMap->GetLabelObject ( i )->GetRegion();
    index = region.GetIndex();
    size = region.GetSize();
    for( unsigned int j = 0; j < Dimension; j++ )
      {
      if ( static_cast<IndexValueType>( index[j] ) > 0 )
        {
        index[j] = static_cast<IndexValueType>( index[j] - 1 );
        size[j] = static_cast<SizeValueType>( size[j] + 1 );
        }
      if ( static_cast<SizeValueType>( index[j] + size[j] ) < inputSize[j] )
        size[j] = static_cast<SizeValueType>( size[j] + 1 );
      }
    region.SetIndex( index );
    region.SetSize( size );

    std::cout << i << ' ' << region << std::endl;
    // see if the label exists, if not skip it

    ROIfilter->SetRegionOfInterest ( region );
    ROIfilter->Update();

    // Convert ITK2VTK
    ExportType::Pointer exporter = ExportType::New();
    exporter->SetInput ( ROIfilter->GetOutput() );

    vtkSmartPointer<vtkImageImport> importer =
      vtkSmartPointer<vtkImageImport>::New();

    exporter->Update();
    importer->SetUpdateInformationCallback (
      exporter->GetUpdateInformationCallback() );
    importer->SetPipelineModifiedCallback (
      exporter->GetPipelineModifiedCallback() );
    importer->SetWholeExtentCallback (
      exporter->GetWholeExtentCallback() );
    importer->SetSpacingCallback (
      exporter->GetSpacingCallback() );
    importer->SetOriginCallback (
      exporter->GetOriginCallback() );
    importer->SetScalarTypeCallback (
      exporter->GetScalarTypeCallback() );
    importer->SetNumberOfComponentsCallback (
      exporter->GetNumberOfComponentsCallback() );
    importer->SetPropagateUpdateExtentCallback (
      exporter->GetPropagateUpdateExtentCallback() );
    importer->SetUpdateDataCallback (
      exporter->GetUpdateDataCallback() );
    importer->SetDataExtentCallback (
      exporter->GetDataExtentCallback() );
    importer->SetBufferPointerCallback (
      exporter->GetBufferPointerCallback() );
    importer->SetCallbackUserData (
      exporter->GetCallbackUserData() );
    importer->Update();

    discreteCubes->SetInput( importer->GetOutput() );
    discreteCubes->GenerateValues( N, 1, N );

    smoother->SetInput(discreteCubes->GetOutput());
    smoother->SetNumberOfIterations(smoothingIterations);
    smoother->BoundarySmoothingOff();
    smoother->FeatureEdgeSmoothingOff();
    smoother->SetFeatureAngle(featureAngle);
    smoother->SetPassBand(passBand);
    smoother->NonManifoldSmoothingOn();
    smoother->NormalizeCoordinatesOn();

    // select the cells for a given label
    selector->SetInput(smoother->GetOutput());
    selector->SetInputArrayToProcess(0, 0, 0,
      vtkDataObject::FIELD_ASSOCIATION_CELLS,
      vtkDataSetAttributes::SCALARS);
    selector->ThresholdBetween(i, i);

    // Strip the scalars from the output
    scalarsOff->SetInput(selector->GetOutput());
    scalarsOff->CopyAttributeOff(vtkMaskFields::POINT_DATA,
                               vtkDataSetAttributes::SCALARS);
    scalarsOff->CopyAttributeOff(vtkMaskFields::CELL_DATA,
                               vtkDataSetAttributes::SCALARS);

    geometry->SetInput(scalarsOff->GetOutput());

    comp->SetInput( geometry->GetOutput() );
    comp->ScalarConnectivityOff();
    comp->SetExtractionModeToLargestRegion();
    comp->ColorRegionsOff();


//     decimate->SetInput( geometry->GetOutput() );
//     decimate->SetTargetReduction( 0.10 );
//     decimate->AttributeErrorMetricOff();

    // output the polydata
    vtksys_stl::stringstream ss;
    ss << nameGeneratorOutput->GetFileNames()[i-1];
    std::cout << argv[0] << " writing " << ss.str() << std::endl;

    writer->SetInput(comp->GetOutput());
    writer->SetFileName(ss.str().c_str());
    writer->Write();

    }

  return EXIT_SUCCESS;
}
