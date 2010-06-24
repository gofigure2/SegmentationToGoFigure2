/*=========================================================================
  Author: $Author: arnaudgelas $  // Author of last commit
  Version: $Rev: 1362 $  // Revision of last commit
  Date: $Date: 2010-04-29 12:15:50 -0400 (Thu, 29 Apr 2010) $  // Date of last commit
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
/*=========================================================================
  Author: $Author: arnaudgelas $  // Author of last commit
  Version: $Rev: 1362 $  // Revision of last commit
  Date: $Date: 2010-04-29 12:15:50 -0400 (Thu, 29 Apr 2010) $  // Date of last commit
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
/*=========================================================================

Program:   vtkINRIA3D
Module:    $Id: vtkFillImageWithPolyData.h 1080 2009-02-18 13:27:02Z acanale $
Language:  C++
Author:    $Author: arnaudgelas $
Date:      $Date: 2010-04-29 12:15:50 -0400 (Thu, 29 Apr 2010) $
Version:   $Revision: 1362 $

Copyright (c) 2007 INRIA - Asclepios Project. All rights reserved.
See Copyright.txt for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
// version vtkRenderingAddOn
#ifndef _vtk_FillImageWithPolyData_h_
#define _vtk_FillImageWithPolyData_h_

//#include "vtkINRIA3DConfigure.h"

#include "vtkThreadedImageAlgorithm.h"
#include <vtkPolyData.h>

class /*VTK_RENDERINGADDON_EXPORT*/ vtkFillImageWithPolyData : public vtkThreadedImageAlgorithm
{

 public:
  static vtkFillImageWithPolyData *New();
  vtkTypeRevisionMacro (vtkFillImageWithPolyData, vtkThreadedImageAlgorithm);
  void PrintSelf (ostream &os, vtkIndent indent);


  // Set/Get the polyData:
  vtkSetObjectMacro (PolyData, vtkPolyData);
  vtkGetObjectMacro (PolyData, vtkPolyData);

  vtkSetMacro (InsidePixelValue, double);
  vtkGetMacro (InsidePixelValue, double);


  vtkSetMacro (ExtractionDirection, int);//0: X, 1:Y, 2:Z
  vtkGetMacro (ExtractionDirection, int);


  double Angle2D (const double dp1[2], const double dp2[2]);


 protected:
  vtkFillImageWithPolyData();
  ~vtkFillImageWithPolyData();


  virtual int RequestInformation (vtkInformation *,
                                  vtkInformationVector **,
                                  vtkInformationVector *);


  virtual void ThreadedRequestData(vtkInformation *request,
                                   vtkInformationVector **inputVector,
                                   vtkInformationVector *outputVector,
                                   vtkImageData ***inData,
                                   vtkImageData **outData,
                                   int extent[6], int threadId);

 private:
  vtkFillImageWithPolyData (const vtkFillImageWithPolyData&);
  void operator=(const vtkFillImageWithPolyData&);

  vtkPolyData* PolyData;
  double       InsidePixelValue;
  int          ExtractionDirection;
  double       BBox[6];


};

#include <vtkFillImageWithPolyData.cxx>

#endif
