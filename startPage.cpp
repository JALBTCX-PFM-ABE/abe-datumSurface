
/*********************************************************************************************

    This is public domain software that was developed by or for the U.S. Naval Oceanographic
    Office and/or the U.S. Army Corps of Engineers.

    This is a work of the U.S. Government. In accordance with 17 USC 105, copyright protection
    is not available for any work of the U.S. Government.

    Neither the United States Government, nor any employees of the United States Government,
    nor the author, makes any warranty, express or implied, without even the implied warranty
    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE, or assumes any liability or
    responsibility for the accuracy, completeness, or usefulness of any information,
    apparatus, product, or process disclosed, or represents that its use would not infringe
    privately-owned rights. Reference herein to any specific commercial products, process,
    or service by trade name, trademark, manufacturer, or otherwise, does not necessarily
    constitute or imply its endorsement, recommendation, or favoring by the United States
    Government. The views and opinions of authors expressed herein do not necessarily state
    or reflect those of the United States Government, and shall not be used for advertising
    or product endorsement purposes.
*********************************************************************************************/


/****************************************  IMPORTANT NOTE  **********************************

    Comments in this file that start with / * ! or / / ! are being used by Doxygen to
    document the software.  Dashes in these comment blocks are used to create bullet lists.
    The lack of blank lines after a block of dash preceeded comments means that the next
    block of dash preceeded comments is a new, indented bullet list.  I've tried to keep the
    Doxygen formatting to a minimum but there are some other items (like <br> and <pre>)
    that need to be left alone.  If you see a comment that starts with / * ! or / / ! and
    there is something that looks a bit weird it is probably due to some arcane Doxygen
    syntax.  Be very careful modifying blocks of Doxygen comments.

*****************************************  IMPORTANT NOTE  **********************************/



#include "startPage.hpp"
#include "startPageHelp.hpp"

startPage::startPage (int32_t *argc, char **argv, QWidget *parent, OPTIONS *op):
  QWizardPage (parent)
{
  options = op;


  setPixmap (QWizard::WatermarkPixmap, QPixmap(":/icons/datumSurfaceWatermark.png"));


  setTitle (tr ("Introduction"));

  setWhatsThis (tr ("datumSurface is a Qt wizard program the builds a datum surface using the EGM08 model and one or more shift "
                    "points that are made up of position, MSL offset from the ellipsoid and MLLW (or other datum[i.e. z0]) offset "
                    "from MSL.<br><br>"
                    "The input data points, which consist of a position, a measured separation value between MSL and the ellipsoid, "
                    "and a measured separation value between MSL and the desired datum (usually MLLW), are used to create a cubic spline "
                    "interpolated surface that will be used to determine the datum shift value for the sonar and/or LIDAR data files.  This "
                    "is done by subtracting the EGM08 ellipsoid/geoid separation model value at the location of the measured input data "
                    "points from the measured MSL/ellipsoid separation at that position.  The MLLW separation value is then subtracted from "
                    "the result.<br><br>"
                    "Each of these points is input to a cubic spline interpolation function to create a surface that represents "
                    "the difference between the EGM08 model of the ellipsoid/geoid separation and the measured ellipsoid/MLLW separation.  "
                    "This surface, and the final surface, will be created at a user defined grid size.  Due to the low frequency nature of "
                    "these surfaces, and the fact that we are using the one-minute EGM08 model, the grid size can be fairly large (we use "
                    "100 meters usually).<br><br>"
                    "Once the difference surface is created we then query the EGM08 model at the specified grid "
                    "spacing.  The results of the query are added to the difference surface value at each grid node and then output to a "
                    "CHRTR2 grid file.  This grid file is the final datum shift surface to be applied to the sonar and/or LIDAR data files.<br><br>"
                    "What we have actually done here is to warp the shape of the EGM08 ellipsoid/geoid separation model to our known measured "
                    "points.  The assumption being that the EGM08 model is a better representation of the shape of the separation surface in "
                    "areas where we do not have measured separation values than an interpolated surface would be."));

  QLabel *label = new QLabel (tr ("datumSurface is a Qt wizard program that reads an ASCII file containing one or more ellipsoid/MSL and MSL/datum "
                                  "offsets at geographic locations and outputs a CHRTR2 grid file that is made up of the EGM08 model warped to the "
                                  "input points.  It can also, optionally, output a CHRTR2 grid file that contains just the EGM08 surface.  In the "
                                  "input ASCII file each entry must consist of latitude, longitude, MSL ellipsoid offset, MLLW (or other datum) "
                                  "offset from MSL.  MSL ellipsoid offset is the distance from MSL to the ellipsoid.  Positive values indicate "
                                  "that MSL is above the ellipsoid.  MLLW offset is the distance from MLLW to MSL.  All fields must be comma "
                                  "separated.  Select an input file below.  You may then select an area file, change the default output file name, "
                                  "and set the bin size for the output file.  Note that the bin size can be fairly large since the final field will "
                                  "not have much high frequency content.  Normally 100 meters is considered to be reasonable.<br><br>"
                                  "Help is available by clicking on the Help button and then clicking on the item for which you need information.  For "
                                  "a more involved explanation of this program click the Help button and then click on this text.  Click <b>Next</b> "
                                  "to continue or <b>Cancel</b> to exit."));
  label->setWordWrap (true);


  QVBoxLayout *vbox = new QVBoxLayout (this);
  vbox->addWidget (label);
  vbox->addStretch (10);


  QHBoxLayout *input_file_box = new QHBoxLayout (0);
  input_file_box->setSpacing (8);

  vbox->addLayout (input_file_box);


  QLabel *input_file_label = new QLabel (tr ("ASCII Input File"), this);
  input_file_box->addWidget (input_file_label, 1);

  input_file_edit = new QLineEdit (this);
  input_file_edit->setReadOnly (true);
  input_file_box->addWidget (input_file_edit, 10);

  QPushButton *input_file_browse = new QPushButton (tr ("Browse..."), this);
  input_file_box->addWidget (input_file_browse, 1);

  input_file_label->setWhatsThis (input_fileText);
  input_file_edit->setWhatsThis (input_fileText);
  input_file_browse->setWhatsThis (input_fileBrowseText);

  connect (input_file_browse, SIGNAL (clicked ()), this, SLOT (slotInputFileBrowse ()));


  QHBoxLayout *area_box = new QHBoxLayout (0);
  area_box->setSpacing (8);

  vbox->addLayout (area_box);


  QLabel *area_file_label = new QLabel (tr ("Area File"), this);
  area_box->addWidget (area_file_label, 1);

  area_file_edit = new QLineEdit (this);
  area_file_edit->setReadOnly (true);
  area_box->addWidget (area_file_edit, 10);

  QPushButton *area_file_browse = new QPushButton (tr ("Browse..."), this);
  area_box->addWidget (area_file_browse, 1);


  area_file_label->setWhatsThis (area_fileText);
  area_file_edit->setWhatsThis (area_fileText);
  area_file_browse->setWhatsThis (area_fileBrowseText);

  connect (area_file_browse, SIGNAL (clicked ()), this, SLOT (slotAreaFileBrowse ()));


  QHBoxLayout *output_box = new QHBoxLayout (0);
  output_box->setSpacing (8);

  vbox->addLayout (output_box);


  QLabel *output_file_label = new QLabel (tr ("Output CHRTR2 File"), this);
  output_box->addWidget (output_file_label, 1);

  output_file_edit = new QLineEdit (this);
  output_box->addWidget (output_file_edit, 10);

  QPushButton *output_file_browse = new QPushButton (tr ("Browse..."), this);
  output_box->addWidget (output_file_browse, 1);

  output_file_label->setWhatsThis (output_fileText);
  output_file_edit->setWhatsThis (output_fileText);
  output_file_browse->setWhatsThis (output_fileBrowseText);

  connect (output_file_browse, SIGNAL (clicked ()), this, SLOT (slotOutputFileBrowse ()));


  QGroupBox *gridBox = new QGroupBox (tr ("Grid size (meters)"), this);
  QHBoxLayout *gridBoxLayout = new QHBoxLayout;
  gridBox->setLayout (gridBoxLayout);
  QSpinBox *grid = new QSpinBox (gridBox);
  grid->setRange (2, 1850);
  grid->setSingleStep (100);
  grid->setToolTip (tr ("Change the grid size"));
  grid->setWhatsThis (gridText);
  grid->setValue (NINT (options->grid_size));
  gridBoxLayout->addWidget (grid);


  vbox->addWidget (gridBox);


  if (*argc == 2)
    {
      char input_name[512];

      QString input_file_name = QString (argv[1]);

      strcpy (input_name, argv[1]);

      input_file_edit->setText (input_file_name);
    }


  registerField ("input_file_edit", input_file_edit);
  registerField ("output_file_edit", output_file_edit);
  registerField ("area_file_edit*", area_file_edit);
  registerField ("grid", grid);
}



void startPage::slotInputFileBrowse ()
{
  QStringList         files, filters;
  QString             file;
  char                input_name[512];


  QFileDialog *fd = new QFileDialog (this, tr ("datumSurface Open Input File"));
  fd->setViewMode (QFileDialog::List);


  //  Always add the current working directory and the last used directory to the sidebar URLs in case we're running from the command line.
  //  This function is in the nvutility library.

  setSidebarUrls (fd, options->input_dir);


  filters << tr ("All files (*)");

  fd->setNameFilters (filters);
  fd->setFileMode (QFileDialog::ExistingFile);
  fd->selectNameFilter (tr ("All files (*)"));

  if (fd->exec () == QDialog::Accepted)
    {
      files = fd->selectedFiles ();

      QString input_file_name = files.at (0);


      if (!input_file_name.isEmpty())
        {
          strcpy (input_name, input_file_name.toLatin1 ());

          input_file_edit->setText (input_file_name);
        }

      options->input_dir = fd->directory ().absolutePath ();
    }
}



void startPage::slotOutputFileBrowse ()
{
  QFileDialog *fd = new QFileDialog (this, tr ("datumSurface Output File"));
  fd->setViewMode (QFileDialog::List);


  //  Always add the current working directory and the last used directory to the sidebar URLs in case we're running from the command line.
  //  This function is in the nvutility library.

  setSidebarUrls (fd, options->output_dir);


  QStringList filters;
  filters << QString ("CHRTR2 (*.ch2)");

  fd->setNameFilters (filters);
  fd->setFileMode (QFileDialog::AnyFile);
  fd->selectNameFilter (QString ("CHRTR2 (*.ch2)"));

  QStringList files;
  if (fd->exec () == QDialog::Accepted)
    {
      files = fd->selectedFiles ();

      QString output_file_name = files.at (0);


      if (!output_file_name.isEmpty())
        {
          output_file_edit->setText (output_file_name);
        }

      options->output_dir = fd->directory ().absolutePath ();
    }
}


void startPage::slotAreaFileBrowse ()
{
  SHPHandle shpHandle;
  SHPObject *shape = NULL;
  int32_t type, numShapes;
  double minBounds[4], maxBounds[4];


  QFileDialog *fd = new QFileDialog (this, tr ("datumSurface Area File"));
  fd->setViewMode (QFileDialog::List);


  //  Always add the current working directory and the last used directory to the sidebar URLs in case we're running from the command line.
  //  This function is in the nvutility library.

  setSidebarUrls (fd, options->area_dir);


  QStringList filters;
  filters << tr ("Area file (*.ARE *.are *.afs *.shp *.SHP)");

  fd->setNameFilters (filters);
  fd->setFileMode (QFileDialog::ExistingFile);
  fd->selectNameFilter (tr ("Area file (*.ARE *.are *.afs *.shp *.SHP)"));


  QStringList files;
  if (fd->exec () == QDialog::Accepted)
    {
      files = fd->selectedFiles ();

      QString area_file_name = files.at (0);


      if (!area_file_name.isEmpty())
        {
          if (area_file_name.endsWith (".shp", Qt::CaseInsensitive))
            {
              char shpname[1024];
              strcpy (shpname, area_file_name.toLatin1 ());


              //  Open shape file

              shpHandle = SHPOpen (shpname, "rb");

              if (shpHandle == NULL)
                {
                  QMessageBox::warning (this, "datumSurface", tr ("Cannot open shape file %1!").arg (area_file_name));
                  return;
                }
              else
                {
                  //  Get shape file header info

                  SHPGetInfo (shpHandle, &numShapes, &type, minBounds, maxBounds);


                  if (type != SHPT_POLYGON && type != SHPT_POLYGONZ && type != SHPT_POLYGONM &&
                      type != SHPT_ARC && type != SHPT_ARCZ && type != SHPT_ARCM)
                    {
                      QMessageBox::warning (this, "datumSurface", tr ("Shape file %1 is not a polygon or polyline file!").arg (area_file_name));
                      return;
                    }
                  else
                    {
                      //  Read only the first shape.

                      shape = SHPReadObject (shpHandle, 0);


                      //  Check the number of vertices.

                      if (shape->nVertices < 3)
                        {
                          SHPClose (shpHandle);
                          QMessageBox::warning (this, "datumSurface", tr ("Number of vertices (%1) of shape file %2 is too few for a polygon!").arg
                                                (shape->nVertices).arg (area_file_name));
                          return;
                        }


                      //  Read the vertices to take a shot at determining that this is a geographic polygon.

                      for (int32_t j = 0 ; j < shape->nVertices ; j++)
                        {
                          if (shape->padfX[j] < -360.0 || shape->padfX[j] > 360.0 || shape->padfY[j] < -90.0 || shape->padfY[j] > 90.0)
                            {
                              SHPDestroyObject (shape);
                              SHPClose (shpHandle);
                              QMessageBox::warning (this, "datumSurface", tr ("Shape file %1 does not appear to be geographic!").arg (area_file_name));
                              return;
                            }
                        }


                      SHPDestroyObject (shape);
                      SHPClose (shpHandle);
                    }
                }
            }                      

          area_file_edit->setText (area_file_name);


          //  If one hasn't been set, set the output CHRTR2 filename.

          if (output_file_edit->text ().isEmpty ())
            {
              QString output_file_name;

              if (area_file_name.endsWith (".ARE"))
                {
                  output_file_name = area_file_name.replace (".ARE", "_sep.ch2");
                }
              else if (area_file_name.endsWith (".are"))
                {
                  output_file_name = area_file_name.replace (".are", "_sep.ch2");
                }
              else if (area_file_name.endsWith (".shp"))
                {
                  output_file_name = area_file_name.replace (".shp", "_sep.ch2");
                }
              else if (area_file_name.endsWith (".SHP"))
                {
                  output_file_name = area_file_name.replace (".SHP", "_sep.ch2");
                }
              else
                {
                  output_file_name = area_file_name.replace (".afs", "_sep.ch2");
                }
              output_file_edit->setText (output_file_name);
            }
        }

      options->area_dir = fd->directory ().absolutePath ();
    }
}
