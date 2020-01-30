#include "itkGDCMImageIO.h"
#include "itkImageFileWriter.h"
#include "itkNumericSeriesFileNames.h"
#include "itkPNGImageIO.h"
#include "rtkAmsterdamShroudImageFilter.h"
#include <iostream>
#include <itkImage.h>
#include <itkImageSeriesReader.h>
#include <tclap/CmdLine.h>

using namespace std;
int main(int argc, char *argv[]) {
    try {
      TCLAP::CmdLine cmd("srwamsterdamshroud");

      TCLAP::ValueArg<string> outputArg("o", "output", "Output filename", true, "",
                                        "FILENAME", cmd);
      TCLAP::UnlabeledMultiArg<string> filesArg("files", "Files to parse", true,
                                                "FILENAME", cmd);

      cmd.parse(argc, argv);

      auto filenames = filesArg.getValue();
      // TODO: sort the files by id
      auto output = outputArg.getValue();

      cout << "Converting " << filenames.size() << " files to " << output << endl;

      using InputImageType = itk::Image<double, 3>;

      // Reader and writers
      auto reader = itk::ImageSeriesReader<InputImageType>::New();
      reader->SetFileNames(filenames);
      using ImageIOType = itk::GDCMImageIO;
      ImageIOType::Pointer dicomIO = ImageIOType::New();
      reader->SetImageIO(dicomIO);

      using OutputImageType = itk::Image<double, 3>;

      /* // Amsterdam shroud */
      using ShroudFilterType = rtk::AmsterdamShroudImageFilter<OutputImageType>;
      ShroudFilterType::Pointer shroudFilter = ShroudFilterType::New();
      shroudFilter->SetInput(reader->GetOutput());
      shroudFilter->SetUnsharpMaskSize(17);
      shroudFilter->UpdateOutputInformation();

      auto writer = itk::ImageFileWriter<ShroudFilterType::OutputImageType>::New();
      writer->SetFileName(output);

      writer->SetInput(shroudFilter->GetOutput());
      writer->SetNumberOfStreamDivisions(
          shroudFilter->GetOutput()->GetLargestPossibleRegion().GetSize(2));

      writer->Update();

      return EXIT_SUCCESS;
    } catch (exception &e) {
        cerr << e.what() << endl;
        return 1;
    }
}
