/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkVectorResampleImageFilterTest.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#include "itkImage.h"
#include "itkVectorResampleImageFilter.h"
#include "itkIdentityTransform.h"
#include "itkVectorLinearInterpolateImageFunction.h"
#include "itkRGBPixel.h"
#include "itkImageRegionIteratorWithIndex.h"


int itkVectorResampleImageFilterTest( int argc, char * argv[] )
{

  const     unsigned int   Dimension = 2;
  typedef   unsigned char  PixelComponentType;
  typedef   itk::RGBPixel< PixelComponentType > PixelType;

  typedef itk::Image< PixelType,  Dimension >   ImageType;

  typedef itk::VectorResampleImageFilter<
                            ImageType, ImageType >  FilterType;

  FilterType::Pointer filter = FilterType::New();

  typedef itk::VectorLinearInterpolateImageFunction< 
                       ImageType, double >  InterpolatorType;

  InterpolatorType::Pointer interpolator = InterpolatorType::New();

  filter->SetInterpolator( interpolator );


  typedef itk::IdentityTransform< double, Dimension >  TransformType;
  TransformType::Pointer transform = TransformType::New();

  filter->SetTransform( transform );


  ImageType::SpacingType spacing;
  spacing.Fill( 1.0 );

  ImageType::PointType origin;
  origin.Fill( 0.0 );

  ImageType::RegionType     region;
  ImageType::SizeType       size;
  ImageType::IndexType      start;

  size[0] = 128;
  size[1] = 128;

  start[0] = 0;
  start[1] = 0;

  region.SetSize( size );
  region.SetIndex( start );

  ImageType::Pointer image = ImageType::New();

  image->SetOrigin( origin );
  image->SetSpacing( spacing );
  image->SetRegions( region );
  image->Allocate();

  PixelType pixelValue;

  itk::ImageRegionIteratorWithIndex< ImageType > it( image, region );

  // Fill the image with some color pattern
  it.GoToBegin();
  while( !it.IsAtEnd() )
    {
    ImageType::IndexType index = it.GetIndex();
    pixelValue.SetRed( index[0] * 2 );
    pixelValue.SetGreen( index[0] + index[1] );
    pixelValue.SetBlue( index[1] * 2 );
    it.Set( pixelValue );
    ++it;
    }


  PixelType blackValue;
  blackValue.Fill( 0 );

  filter->SetDefaultPixelValue( blackValue );

  // Set the spacing for the resampling
  spacing[0] *= 2.0;
  spacing[1] *= 2.0;

  filter->SetOutputSpacing( spacing );


  // keep the origin
  filter->SetOutputOrigin( origin );

  // set the size
  size[0] /= 2;
  size[1] /= 2;

  filter->SetSize( size );


  filter->SetInput( image ); 


  try
    {
    filter->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << "Exception thrown " << std::endl;
    std::cerr << excp << std::endl;
    }

  return EXIT_SUCCESS;

}

