/*=========================================================================
  Author: $Author: krm15 $  // Author of last commit
  Version: $Rev: 1658 $  // Revision of last commit
  Date: $Date: 2010-06-14 15:49:25 -0400 (Mon, 14 Jun 2010) $  // Date of last commit
=========================================================================*/

/*=========================================================================
 Authors: The GoFigure Dev. Team.
 at Megason Lab, Systems biology, Harvard Medical school, 2009-10

 Copyright (c) 2009-10, President and Fellows of Harvard College.
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

#ifndef __itkSegmentationToGoFigure2Table_txx
#define __itkSegmentationToGoFigure2Table_txx

#include "itkSegmentationToGoFigure2Table.h"

namespace itk
{
template < class TSegmentImage >
SegmentationToGoFigure2Table< TSegmentImage >
::SegmentationToGoFigure2Table()
{
  m_NumberOfCellTypes = 0;
  m_NumberOfSubCellularTypes = 0;
  m_NumberOfColors = 0;
  m_NumberOfChannels = 0;
  m_NumberOfMeshes = 0;
  m_NumberOfTracks = 0;
}

template < class TSegmentImage >
SegmentationToGoFigure2Table< TSegmentImage >
::~SegmentationToGoFigure2Table()
{}

template < class TSegmentImage >
void
SegmentationToGoFigure2Table< TSegmentImage >::
PrintStart ( std::ostream& os, Indent indent ) const
{
  os << indent << "<?xml version=\"1.0\" ?>" << std::endl;
  os << indent << "<ExportTraces version=\"1\"" << std::endl;
  os << indent << "  <imagingsession>" << std::endl;
  os << indent << "    <Name>" <<this->m_ImagingSession.Name <<"</Name>" << std::endl;
  os << indent << "    <CreationDate>" <<this->m_ImagingSession.CreationDate <<"</CreationDate>" << std::endl;
  os << indent << "    <MicroscopeName>" <<this->m_ImagingSession.MicroscopeName <<"</MicroscopeName>" << std::endl;
//  os << indent << "    <Name>TimePoint 00 to 09</Name>" << std::endl;
//  os << indent << "    <CreationDate>2009-11-05 09:13:35</CreationDate>" << std::endl;
//  os << indent << "    <MicroscopeName>Cyclops</MicroscopeName>" << std::endl;
  os << indent << "  </imagingsession>" << std::endl;
}

template < class TSegmentImage >
void
SegmentationToGoFigure2Table< TSegmentImage >::
PrintEnd ( std::ostream& os, Indent indent ) const
{
  os << indent << "</ExportTraces>" << std::endl;
}

template < class TSegmentImage >
void
SegmentationToGoFigure2Table< TSegmentImage >::
PrintCell ( std::ostream& os, Indent indent ) const
{
  os << indent << "<NumberOfcelltype>" << this->m_NumberOfCellTypes << "</NumberOfcelltype>" << std::endl;
  for( unsigned int i = 0; i < this->m_CellTypes.size(); i++ )
    {
    os << indent << "<celltype>" << std::endl;
    os << indent << "  <Name>" <<  this->m_CellTypes[i].Name << "</Name>" << std::endl;
    os << indent << "  <CellTypeID>" <<  this->m_CellTypes[i].CellTypeID << "</CellTypeID>" << std::endl;
    os << indent << "  <Description>" <<  this->m_CellTypes[i].Description << "</Description>" << std::endl;
    os << indent << "</celltype>" << std::endl;
    }
}


template < class TSegmentImage >
void
SegmentationToGoFigure2Table< TSegmentImage >::
PrintSubCellular( std::ostream& os, Indent indent ) const
{
  os << indent << "<NumberOfsubcellulartype>" << this->m_NumberOfSubCellularTypes << "</NumberOfsubcellulartype>" << std::endl;
  for( unsigned int i = 0; i < this->m_SubCellularTypes.size(); i++ )
    {
    os << indent << "<subcellulartype>" << std::endl;
    os << indent << "  <Description>" <<  this->m_SubCellularTypes[i].Description << "</Description>" << std::endl;
    os << indent << "  <SubCellularID>" <<  this->m_SubCellularTypes[i].SubCellularID<< "</SubCellularID>" << std::endl;
    os << indent << "  <Name>" <<  this->m_SubCellularTypes[i].Name << "</Name>" << std::endl;
    os << indent << "</subcellulartype>" << std::endl;
    }
}


template < class TSegmentImage >
void
SegmentationToGoFigure2Table< TSegmentImage >::
PrintColor ( std::ostream& os, Indent indent ) const
{
  os << indent << "<NumberOfcolor>" << this->m_NumberOfColors << "</NumberOfcolor>" << std::endl;
  for( unsigned int i = 0; i < this->m_Colors.size(); i++ )
    {
    os << indent << "<color>" << std::endl;
    os << indent << "  <Alpha>" << m_Colors[i].Alpha << "</Alpha>" << std::endl;
    os << indent << "  <Blue>" << m_Colors[i].Blue << "</Blue>" << std::endl;
    os << indent << "  <ColorID>" <<  m_Colors[i].ColorID << "</ColorID>" << std::endl;
    os << indent << "  <Description>" <<  m_Colors[i].Description << "</Description>" << std::endl;
    os << indent << "  <Green>" << m_Colors[i].Green << "</Green>" << std::endl;
    os << indent << "  <Name>" <<  m_Colors[i].ColorID << "</Name>" << std::endl;
    os << indent << "  <Red>" << m_Colors[i].Red << "</Red>" << std::endl;
    os << indent << "</color>" << std::endl;
    }
}


template < class TSegmentImage >
void
SegmentationToGoFigure2Table< TSegmentImage >::
PrintChannel ( std::ostream& os, Indent indent ) const
{
  os << indent << "<NumberOfchannel>" << this->m_NumberOfChannels << "</NumberOfchannel>" << std::endl;
  for( unsigned int i = 0; i < this->m_Channels.size(); i++ )
    {
    os << indent << "<channel>" << std::endl;
    os << indent << "  <ChannelID>" << m_Channels[i].ChannelID << "</ChannelID>" << std::endl;
    os << indent << "  <ChannelNumber>" << m_Channels[i].ChannelNumber << "</ChannelNumber>" << std::endl;
    os << indent << "  <ColorID>" << m_Channels[i].ColorID << "</ColorID>" << std::endl;
    os << indent << "  <ImagingSessionID>" << m_Channels[i].ImagingSessionID << "</ImagingSessionID>" << std::endl;
    os << indent << "  <Name>" <<  this->m_Channels[i].Name << "</Name>" << std::endl;
    os << indent << "  <NumberOfBits>" <<  m_Channels[i].NumberOfBits << "</NumberOfBits>" << std::endl;
    os << indent << "</channel>" << std::endl;
    }
}


template < class TSegmentImage >
void
SegmentationToGoFigure2Table< TSegmentImage >::
PrintIntensity ( std::ostream& os, Indent indent ) const
{
  os << indent << "<NumberOfintensity>" << this->m_NumberOfIntensitys << "</NumberOfintensity>" << std::endl;
  for( unsigned int i = 0; i < this->m_Intensitys.size(); i++ )
    {
    os << indent << "<intensity>" << std::endl;
    os << indent << "  <ChannelID>" << m_Intensitys[i].ChannelID << "</ChannelID>" << std::endl;
    os << indent << "  <IntensityID>" << m_Intensitys[i].IntensityID << "</IntensityID>" << std::endl;
    os << indent << "  <MeshID>" << m_Intensitys[i].MeshID << "</MeshID>" << std::endl;
    os << indent << "  <Value>" << m_Intensitys[i].Value << "</Value>" << std::endl;
    os << indent << "</intensity>" << std::endl;
    }
}


template < class TSegmentImage >
void
SegmentationToGoFigure2Table< TSegmentImage >::
PrintCoordinate ( std::ostream& os, Indent indent ) const
{
  os << indent << "<NumberOfcoordinate>" << this->m_NumberOfCoordinates << "</NumberOfcoordinate>" << std::endl;
  for( unsigned int i = 0; i < this->m_Coordinates.size(); i++ )
    {
    os << indent << "<coordinate>" << std::endl;
    os << indent << "  <CCoord>" << m_Coordinates[i].CCoord << "</CCoord>" << std::endl;
    os << indent << "  <CoordID>" << m_Coordinates[i].CoordID << "</CoordID>" << std::endl;
    os << indent << "  <RCoord>" << m_Coordinates[i].RCoord << "</RCoord>" << std::endl;
    os << indent << "  <TCoord>" << m_Coordinates[i].TCoord << "</TCoord>" << std::endl;
    os << indent << "  <XCoord>" << m_Coordinates[i].XCoord << "</XCoord>" << std::endl;
    os << indent << "  <YCoord>" << m_Coordinates[i].YCoord << "</YCoord>" << std::endl;
    os << indent << "  <YTileCoord>" << m_Coordinates[i].YTileCoord << "</YTileCoord>" << std::endl;
    os << indent << "  <ZCoord>" << m_Coordinates[i].ZCoord << "</ZCoord>" << std::endl;
    os << indent << "</coordinate>" << std::endl;
    }
}


template < class TSegmentImage >
void
SegmentationToGoFigure2Table< TSegmentImage >::
PrintLineage( std::ostream& os, Indent indent ) const
{
  os << indent << "<NumberOflineage>" << 0 << "</NumberOflineage>" << std::endl;
}


template < class TSegmentImage >
void
SegmentationToGoFigure2Table< TSegmentImage >::
PrintTrack ( std::ostream& os, Indent indent ) const
{
  os << indent << "<NumberOftrack>" << this->m_NumberOfTracks << "</NumberOftrack>" << std::endl;
  for( unsigned int i = 0; i < this->m_Tracks.size(); i++ )
    {
    os << indent << "<track>" << std::endl;
    os << indent << "  <ColorID>" << m_Tracks[i].ColorID << "</ColorID>" << std::endl;
    os << indent << "  <CoordIDMax>" << m_Tracks[i].CoordIDMax << "</CoordIDMax>" << std::endl;
    os << indent << "  <CoordIDMin>" << m_Tracks[i].CoordIDMin << "</CoordIDMin>" << std::endl;
    os << indent << "  <ImagingSessionID>" << m_Tracks[i].ImagingSessionID << "</ImagingSessionID>" << std::endl;
    os << indent << "  <LineageID>" << m_Tracks[i].LineageID << "</LineageID>" << std::endl;
//     os << indent << "  <Points>" << m_Tracks[i].Points << "</Points>" << std::endl;
    os << indent << "  <TrackFamilyID>" << m_Tracks[i].TrackFamilyID << "</TrackFamilyID>" << std::endl;
    os << indent << "  <TrackID>" << m_Tracks[i].TrackID << "</TrackID>" << std::endl;
    os << indent << "</track>" << std::endl;
    }
}


template < class TSegmentImage >
void
SegmentationToGoFigure2Table< TSegmentImage >::
PrintMesh ( std::ostream& os, Indent indent ) const
{
  os << indent << "<NumberOfmesh>" << this->m_NumberOfMeshes << "</NumberOfmesh>" << std::endl;
  for( unsigned int i = 0; i < this->m_Meshes.size(); i++ )
    {
    os << indent << "<mesh>" << std::endl;
    os << indent << "  <CellTypeID>" << m_Meshes[i].CellTypeID << "</CellTypeID>" << std::endl;
    os << indent << "  <ColorID>" << m_Meshes[i].ColorID << "</ColorID>" << std::endl;
    os << indent << "  <CoordIDMax>" << m_Meshes[i].CoordIDMax << "</CoordIDMax>" << std::endl;
    os << indent << "  <CoordIDMin>" << m_Meshes[i].CoordIDMin << "</CoordIDMin>" << std::endl;
    os << indent << "  <ImagingSessionID>" << m_Meshes[i].ImagingSessionID << "</ImagingSessionID>" << std::endl;
    os << indent << "  <MeshID>" << m_Meshes[i].MeshID << "</MeshID>" << std::endl;
    os << indent << "  <Points>" << m_Meshes[i].Points << "</Points>" << std::endl;
    os << indent << "  <SubCellularID>" << m_Meshes[i].SubCellularTypeID << "</SubCellularID>" << std::endl;
    os << indent << "  <TrackID>" << m_Meshes[i].TrackID << "</TrackID>" << std::endl;
    os << indent << "</mesh>" << std::endl;
    }
}

} /* end namespace itk */

#endif
