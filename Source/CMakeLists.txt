ADD_EXECUTABLE( SegmentationToGoFigure2Table 
  SegmentationToGoFigure2Table.cxx
  vtkPolyDataMySQLTextReader.cxx
  vtkPolyDataMySQLTextWriter.cxx
)

TARGET_LINK_LIBRARIES( SegmentationToGoFigure2Table 
  ITKCommon 
  ITKIO 
  ITKStatistics 
  vtkIO )
