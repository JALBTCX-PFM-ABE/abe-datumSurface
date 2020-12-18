
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



#include "datumSurface.hpp"
#include "datumSurfaceHelp.hpp"


void set_defaults (OPTIONS *options);
void envin (OPTIONS *options);
void envout (OPTIONS *options);


datumSurface::datumSurface (int32_t *argc, char **argv, QWidget *parent)
  : QWizard (parent, 0)
{
  QResource::registerResource ("/icons.rcc");


  //  Set the main icon

  setWindowIcon (QIcon (":/icons/datumSurfaceWatermark.png"));


  //  Set the normal defaults

  set_defaults (&options);


  //  Get the user's defaults if available

  envin (&options);


  // Set the application font

  QApplication::setFont (options.font);


  setWizardStyle (QWizard::ClassicStyle);


  setOption (HaveHelpButton, true);
  setOption (ExtendedWatermarkPixmap, false);

  connect (this, SIGNAL (helpRequested ()), this, SLOT (slotHelpClicked ()));


  //  Set the window size and location from the defaults

  this->resize (options.window_width, options.window_height);
  this->move (options.window_x, options.window_y);


  setPage (0, new startPage (argc, argv, this, &options));

  setPage (1, new runPage (this, &progress, &checkList));


  setButtonText (QWizard::CustomButton1, tr ("&Run"));
  setOption (QWizard::HaveCustomButton1, true);
  button (QWizard::CustomButton1)->setToolTip (tr ("Start generating the separation surface"));
  button (QWizard::CustomButton1)->setWhatsThis (runText);
  connect (this, SIGNAL (customButtonClicked (int)), this, SLOT (slotCustomButtonClicked (int)));


  setStartId (0);
}


datumSurface::~datumSurface ()
{
}



void datumSurface::initializePage (int id)
{
  QRect tmp;
  QString string;


  button (QWizard::HelpButton)->setIcon (QIcon (":/icons/contextHelp.png"));
  button (QWizard::CustomButton1)->setEnabled (false);


  switch (id)
    {
    case 0:
      break;

    case 1:
      button (QWizard::CustomButton1)->setEnabled (true);

      options.grid_size = (double) field ("grid").toInt ();
      input_file_name = field ("input_file_edit").toString ();
      area_file_name = field ("area_file_edit").toString ();
      output_file_name = field ("output_file_edit").toString ();

      if (!output_file_name.endsWith (".ch2")) output_file_name.append (".ch2");


      //  Save the output directory.  It might have been input manually instead of browsed.

      options.output_dir = QFileInfo (output_file_name).absoluteDir ().absolutePath ();


      //  Use frame geometry to get the absolute x and y.

      tmp = this->frameGeometry ();
      options.window_x = tmp.x ();
      options.window_y = tmp.y ();


      //  Use geometry to get the width and height.

      tmp = this->geometry ();
      options.window_width = tmp.width ();
      options.window_height = tmp.height ();


      //  Save the options.

      envout (&options);


      checkList->clear ();

      string = tr ("ASCII Input file : %1").arg (input_file_name);
      checkList->addItem (string);

      string = tr ("Area file : %1").arg (area_file_name);
      checkList->addItem (string);

      string = tr ("Output file : %1").arg (output_file_name);
      checkList->addItem (string);


      QListWidgetItem *cur;

      string = QString (tr ("Output file grid size (meters) : %1")).arg (options.grid_size, 6, 'f', 1);
      cur = new QListWidgetItem (string);
      checkList->addItem (cur);
      checkList->setCurrentItem (cur);
      checkList->scrollToItem (cur);

      break;
    }
}



void datumSurface::cleanupPage (int id)
{
  switch (id)
    {
    case 0:
      break;

    case 1:
      break;
    }
}



void datumSurface::slotHelpClicked ()
{
  QWhatsThis::enterWhatsThisMode ();
}



//  This is where the fun stuff happens.

void 
datumSurface::slotCustomButtonClicked (int id __attribute__ ((unused)))
{
  char               file[512], out_file[512], egm_file[512], string[512], area_file[512], latstring[128], lonstring[128], mslstring[128], mllwstring[128];
  int32_t            polygon_count, chrtr2_hnd = -1;
  int32_t            gridcols, gridrows, input_count = 0, egm_hnd = -1;
  double             lat, lon, polygon_x[1000], polygon_y[1000], x_grid_size, y_grid_size, center_x, center_y, nx, ny, msl, egm;
  double             half_y, half_x;
  float              egmr, *array = NULL;
  NV_F64_COORD3      xyz[1000], xyz0[1000], xyz1[1000];
  NV_F64_XYMBR       mbr, new_mbr;
  uint8_t            egm_out = NVFalse;
  CHRTR2_HEADER      chrtr2_header;
  CHRTR2_RECORD      chrtr2_record;
  FILE               *fp = NULL;
  QListWidgetItem    *cur;
  QString            qstring;
  gridThread         grid_thread[2];


  QApplication::setOverrideCursor (Qt::WaitCursor);


  button (QWizard::FinishButton)->setEnabled (false);
  button (QWizard::BackButton)->setEnabled (false);
  button (QWizard::CustomButton1)->setEnabled (false);


  //  If we didn't have an input file we must just want an EGM08 grid.

  if (input_file_name.isEmpty ())
    {
      egm_out = NVTrue;
    }
  else
    {
      strcpy (file, input_file_name.toLatin1 ());

      if ((fp = fopen (file, "r")) == NULL)
        {
          qstring = QString (tr ("Unable to open input file %1\nReason : %2")).arg (input_file_name).arg (QString (strerror (errno)));
          QMessageBox::critical (this, "datumSurface", qstring);
          exit (-1);
        }
    }


  //  Clear the CHRTR2 header.

  memset (&chrtr2_header, 0, sizeof (CHRTR2_HEADER));


  //  Get the area file's MBR and polygon bounds.

  strcpy (area_file, area_file_name.toLatin1 ());

  if (!get_area_mbr (area_file, &polygon_count, polygon_x, polygon_y, &mbr))
    {
      qstring = QString (tr ("Error reading area file %1\nReason : %2")).arg (area_file_name).arg (QString (strerror (errno)));
      QMessageBox::critical (this, "datumSurface", qstring);
      exit (-1);
    }


  //  Compute the grid sizes in lat and lon degrees at the center of the requested area.

  center_x = mbr.min_x + (mbr.max_x - mbr.min_x) / 2.0;
  center_y = mbr.min_y + (mbr.max_y - mbr.min_y) / 2.0;

  newgp (center_y, center_x, 0.0, options.grid_size, &ny, &nx);
  y_grid_size = ny - center_y;
  half_y = y_grid_size / 2.0;
  newgp (center_y, center_x, 90.0, options.grid_size, &ny, &nx);
  x_grid_size = nx - center_x;
  half_x = x_grid_size / 2.0;


  //  Compute the total grid size.

  gridcols = NINT ((mbr.max_x - mbr.min_x) / x_grid_size + 1);
  gridrows = NINT ((mbr.max_y - mbr.min_y) / y_grid_size + 1);


  //  This is the MBR in grid cell units.  One cell = 1.0 unit.

  new_mbr.min_x = 0.0;
  new_mbr.min_y = 0.0;
  new_mbr.max_x = (double) gridcols;
  new_mbr.max_y = (double) gridrows;


  qstring = QString (tr ("Grid rows : %1")).arg (gridrows);
  cur = new QListWidgetItem (qstring);
  checkList->addItem (cur);
  checkList->setCurrentItem (cur);
  checkList->scrollToItem (cur);
  qstring = QString (tr ("Grid columns : %1")).arg (gridcols);
  cur = new QListWidgetItem (qstring);
  checkList->addItem (cur);
  checkList->setCurrentItem (cur);
  checkList->scrollToItem (cur);


  input_count = 0;


  //  If we're not just building an EGM08 grid we need to read in the geodetic information from the input file.

  if (!egm_out)
    {
      //  Load the measured data points into the input arrays.

      if (fp != NULL)
        {
          while (fgets (string, 512, fp) != NULL)
            {
              //  Make sure that some bozo didn't put blank lines in their text file.

              if (strchr (string, ','))
                {
                  //  Yes, I know that strtok destroys the input string.  I don't really care in this case.

                  strcpy (latstring, strtok (string, ","));
                  strcpy (lonstring, strtok (NULL, ","));
                  strcpy (mslstring, strtok (NULL, ","));
                  strcpy (mllwstring, strtok (NULL, ","));

                  posfix (latstring, &lat, POS_LAT);
                  posfix (lonstring, &lon, POS_LON);

                  sscanf (mslstring, "%lf", &msl);
                  sscanf (mllwstring, "%lf", &xyz0[input_count].z);

                  egm = (double) get_egm08 (lat, lon);

                  xyz[input_count].x = xyz0[input_count].x = xyz1[input_count].x = (lon - mbr.min_x) / x_grid_size;
                  xyz[input_count].y = xyz0[input_count].y = xyz1[input_count].y = (lat - mbr.min_y) / y_grid_size;
                  xyz[input_count].z = (msl - egm) - xyz0[input_count].z;
                  xyz1[input_count].z = msl - egm;

                  input_count++;
                }
            }

          fclose (fp);
        }


      //  If there is just one point we use it for all parts of the surface.  If there are no input points we must be just making the
      //  EGM08 surface.

      if (!input_count)
        {
          qstring = QString (tr ("No input points read from file %1")).arg (input_file_name);
          QMessageBox::critical (this, "datumSurface", qstring);
          exit (-1);
        }
      else
        {
          strcpy (out_file, output_file_name.toLatin1 ());


          //  Populate the chrtr2 header prior to creating the file.

          strcpy (chrtr2_header.creation_software, VERSION);
          chrtr2_header.z_units = CHRTR2_METERS;
          chrtr2_header.mbr.wlon = mbr.min_x;
          chrtr2_header.mbr.slat = mbr.min_y;
          chrtr2_header.width = gridcols;
          chrtr2_header.height = gridrows;
          chrtr2_header.lat_grid_size_degrees = y_grid_size;
          chrtr2_header.lon_grid_size_degrees = x_grid_size;
          chrtr2_header.min_z = -326.00;
          chrtr2_header.max_z = 326.00;
          chrtr2_header.z_scale = 100.0;
          chrtr2_header.min_z0 = -326.00;
          chrtr2_header.max_z0 = 326.00;
          chrtr2_header.z0_scale = 100.0;
          strcpy (chrtr2_header.z0_name, "MLLW");
          chrtr2_header.min_z1 = -326.00;
          chrtr2_header.max_z1 = 326.00;
          chrtr2_header.z1_scale = 100.0;
          chrtr2_header.horizontal_uncertainty_scale = 0.0;
          chrtr2_header.vertical_uncertainty_scale = 0.0;


          //  Try to create and open the chrtr2 file.

          chrtr2_hnd = chrtr2_create_file (out_file, &chrtr2_header);
          if (chrtr2_hnd < 0)
            {
              qstring = QString (tr ("Error creating CHRTR2 file %1\nReason : %2")).arg (output_file_name).arg (QString (chrtr2_strerror ()));
              QMessageBox::critical (this, "datumSurface", qstring);
              exit (-1);
            }
        }
    }


  switch (input_count)
    {
      //  No input points (we must be dumping the EGM08 surface).

    case 0:

      sprintf (egm_file, "./%s", gen_basename (area_file));
      strcpy (&egm_file[strlen (egm_file) - 4], "_egm08.ch2");


      //  Populate the chrtr2 header prior to creating the file.

      strcpy (chrtr2_header.creation_software, VERSION);
      chrtr2_header.z_units = CHRTR2_METERS;
      chrtr2_header.mbr.wlon = mbr.min_x;
      chrtr2_header.mbr.slat = mbr.min_y;
      chrtr2_header.width = gridcols;
      chrtr2_header.height = gridrows;
      chrtr2_header.lat_grid_size_degrees = y_grid_size;
      chrtr2_header.lon_grid_size_degrees = x_grid_size;
      chrtr2_header.min_z = -326.00;
      chrtr2_header.max_z = 326.00;
      chrtr2_header.z_scale = 100.0;
      chrtr2_header.horizontal_uncertainty_scale = 0.0;
      chrtr2_header.vertical_uncertainty_scale = 0.0;


      //  Try to create and open the chrtr2 file.

      egm_hnd = chrtr2_create_file (egm_file, &chrtr2_header);
      if (egm_hnd < 0)
        {
          qstring = QString (tr ("Error creating CHRTR2 file %1\nReason : %2")).arg (QString (egm_file)).arg (QString (chrtr2_strerror ()));
          QMessageBox::critical (this, "datumSurface", qstring);
          exit (-1);
        }


      progress.pbox->setTitle (tr ("No input data files"));
      progress.pbar->setRange (0, 100);
      progress.pbar->setValue (100);

      progress.wbox->setTitle (tr ("Writing output file %1").arg (QFileInfo (output_file_name).fileName ()));
      progress.wbar->setRange (0, gridrows);
      qApp->processEvents ();


      for (int32_t i = 0 ; i < gridrows ; i++)
        {
          lat = mbr.min_y + i * y_grid_size + half_y;

          for (int32_t j = 0 ; j < gridcols ; j++)
            {
              lon = mbr.min_x + j * x_grid_size + half_x;

              egmr = get_egm08 (lat, lon);

              chrtr2_record.z = egmr;
              chrtr2_record.status = CHRTR2_REAL;
              chrtr2_write_record_row_col (egm_hnd, i, j, chrtr2_record);
            }

          progress.wbar->setValue (i);
          qApp->processEvents ();
        }

      progress.wbar->setValue (gridrows);
      qApp->processEvents ();

      break;


      //  Only one input point (we don't use MISP).

    case 1:

      progress.pbox->setTitle (tr ("Input file %1").arg (QFileInfo (input_file_name).fileName ()));
      progress.pbar->setRange (0, 100);
      progress.pbar->setValue (100);

      progress.wbox->setTitle (tr ("Writing output file %1").arg (QFileInfo (output_file_name).fileName ()));
      progress.wbar->setRange (0, gridrows);
      qApp->processEvents ();
 
      for (int32_t i = 0 ; i < gridrows ; i++)
        {
          lat = mbr.min_y + i * y_grid_size + half_y;

          for (int32_t j = 0 ; j < gridcols ; j++)
            {
              lon = mbr.min_x + j * x_grid_size + half_x;

              egmr = get_egm08 (lat, lon);

              chrtr2_record.z = egmr + xyz[0].z;
              chrtr2_record.z0 = xyz0[0].z;
              chrtr2_record.z1 = xyz1[0].z;

              if (fabs (lat - xyz[0].y) <= half_y && fabs (lon - xyz[0].x) <= half_x)
                {
                  chrtr2_record.status = CHRTR2_REAL;
                }
              else
                {
                  chrtr2_record.status = CHRTR2_INTERPOLATED;
                }

              chrtr2_write_record_row_col (chrtr2_hnd, i, j, chrtr2_record);


              //  If we asked for the EGM08 surface, write the record to the EGM08 CHRTR2 file.

              if (egm_out)
                {
                  chrtr2_record.z = egmr;
                  chrtr2_record.status = CHRTR2_REAL;
                  chrtr2_write_record_row_col (egm_hnd, i, j, chrtr2_record);
                }
            }

          progress.wbar->setValue (i);
          qApp->processEvents ();
        }

      progress.wbar->setValue (gridrows);
      qApp->processEvents ();

      break;


      //  Multiple input points (use MISP).

    default:

      progress.pbox->setTitle (tr ("Computing MISP Z0 surface for input file %1").arg (QFileInfo (input_file_name).fileName ()));
      progress.pbar->setRange (0, 0);
      progress.pbar->setValue (-1);
      qApp->processEvents ();


      //  Initialize the MISP engine.

      misp_init (1.0, 1.0, 0.05, 4, 20.0, 20, 999999.0, -999999.0, -3, new_mbr);


      //  Load the differences between the measured MSL/datum/ellipsoid diferences and the EGM08 MSL/datum/ellipsoid differences.
      //  In other words, we're gridding the entire datum shift, not just Z0 or Z1.

      for (int32_t i = 0 ; i < input_count ; i++) misp_load (xyz[i]);


      //  We're starting the grid processing concurrently using a thread.  Note that we're using the Qt::DirectConnection type
      //  for the signal/slot connections.  This causes all of the signals emitted from the thread to be serviced immediately.
      //  Why are we running misp_proc in a thread???  Because it's the only way to get the stupid progress bar to update so
      //  that the user will know that the damn program is still running.  Sheesh!

      complete = NVFalse;
      connect (&grid_thread[0], SIGNAL (completed ()), this, SLOT (slotGridCompleted ()), Qt::DirectConnection);

      grid_thread[0].grid ();


      //  We can't move on until the thread is complete but we want to keep our progress bar updated.  This is a bit tricky 
      //  because you can't update the progress bar from within slots connected to thread signals.  Those slots are considered part
      //  of the mispThread and not part of the GUI thread.  When the thread is finished we move on to the retrieval step.

      while (!complete)
        {
#ifdef NVWIN3X
          Sleep (50);
#else
          usleep (50000);
#endif

          qApp->processEvents ();
        }

      progress.pbox->setTitle (tr ("MISP Z0 surface for %1 complete").arg (QFileInfo (input_file_name).fileName ()));
      progress.pbar->setRange (0, 100);
      progress.pbar->setValue (100);
      qApp->processEvents ();


      complete = NVFalse;


      array = (float *) malloc ((gridcols + 1) * sizeof (float));

      if (array == NULL)
        {
          perror ("Allocating array in datum_surface");
          exit (-1);
        }

      progress.wbox->setTitle (tr ("Retrieving MISP Z0 surface"));
      progress.wbar->setRange (0, gridrows);


      //  Retrieve the grid of differences from MISP.

      for (int32_t i = 0 ; i < gridrows ; i++)
        {
          if (!misp_rtrv (array)) break;

          lat = mbr.min_y + i * y_grid_size + half_y;


          for (int32_t j = 0 ; j < gridcols ; j++)
            {
              lon = mbr.min_x + j * x_grid_size + half_x;


              //  Get the EGM08 value for the grid point.

              egmr = get_egm08 (lat, lon);


              memset (&chrtr2_record, 0, sizeof (CHRTR2_RECORD));


              //  Since the grid we are retrieving is the difference between the measured grid and the EGM08 model we have to add
              //  the EGM08 value to the difference value to get a surface that is shaped like EGM08 but warped to the measured points.

              chrtr2_record.z = egmr + array[j];


              //  Check for the real bit being set (i.e. this grid contains one of the measured input data points).

              if (bit_test (array[j], 0))
                {
                  chrtr2_record.status = CHRTR2_REAL;
                }
              else
                {
                  chrtr2_record.status = CHRTR2_INTERPOLATED;
                }


              //  Write the record to the CHRTR2 file with just the Elevation field filled.  We're using the elevation field for the
              //  total correction value.

              chrtr2_write_record_row_col (chrtr2_hnd, i, j, chrtr2_record);


              //  If we asked for the EGM08 surface, write out the record to the EGM08 CHRTR2 file.

              if (egm_out)
                {
                  chrtr2_record.z = egmr;
                  chrtr2_record.status = CHRTR2_REAL;
                  chrtr2_write_record_row_col (egm_hnd, i, j, chrtr2_record);
                }
            }

          progress.wbar->setValue (i);
          qApp->processEvents ();
        }

      progress.wbox->setTitle (tr ("Writing output file %1").arg (QFileInfo (output_file_name).fileName ()));
      progress.wbar->setRange (0, gridrows);
      progress.wbar->setValue (0);
      qApp->processEvents ();


      //  Initialize the MISP engine for the Z1 separation surface.

      misp_init (1.0, 1.0, 0.05, 4, 20.0, 20, 999999.0, -999999.0, -3, new_mbr);


      //  Load the differences between the measured MSL/ellipsoid diference and the EGM08 MSL/ellipsoid differences.
      //  In other words, we're gridding Z1.  This will allow us to subtract the total value from Z1 to get Z0.

      for (int32_t i = 0 ; i < input_count ; i++) misp_load (xyz1[i]);


      //  We're starting the grid processing concurrently using a thread.  Note that we're using the Qt::DirectConnection type
      //  for the signal/slot connections.  This causes all of the signals emitted from the thread to be serviced immediately.
      //  Why are we running misp_proc in a thread???  Because it's the only way to get the stupid progress bar to update so
      //  that the user will know that the damn program is still running.  Sheesh!

      complete = NVFalse;
      connect (&grid_thread[1], SIGNAL (completed ()), this, SLOT (slotGridCompleted ()), Qt::DirectConnection);

      grid_thread[1].grid ();


      //  We can't move on until the thread is complete but we want to keep our progress bar updated.  This is a bit tricky 
      //  because you can't update the progress bar from within slots connected to thread signals.  Those slots are considered part
      //  of the mispThread and not part of the GUI thread.  When the thread is finished we move on to the retrieval step.

      while (!complete)
        {
#ifdef NVWIN3X
          Sleep (50);
#else
          usleep (50000);
#endif

          qApp->processEvents ();
        }

      progress.pbox->setTitle (tr ("MISP Z1 surface for %1 complete").arg (QFileInfo (input_file_name).fileName ()));
      progress.pbar->setRange (0, 100);
      progress.pbar->setValue (100);
      qApp->processEvents ();


      progress.wbox->setTitle (tr ("Writing output file %1").arg (QFileInfo (output_file_name).fileName ()));
      progress.wbar->setRange (0, gridrows);


      //  Retrieve the grid of differences from MISP.

      for (int32_t i = 0 ; i < gridrows ; i++)
        {
          if (!misp_rtrv (array)) break;

          lat = mbr.min_y + i * y_grid_size + half_y;


          for (int32_t j = 0 ; j < gridcols ; j++)
            {
              lon = mbr.min_x + j * x_grid_size + half_x;


              //  Get the EGM08 value for the grid point.

              egmr = get_egm08 (lat, lon);


              //  Read the record that we wrote in the first loop so we can retrieve the total correction value.

              chrtr2_read_record_row_col (chrtr2_hnd, i, j, &chrtr2_record);


              //  Since the grid we are retrieving is the difference between the measured grid and the EGM08 model we have to add
              //  the EGM08 value to the difference value to get a surface that is shaped like EGM08 but warped to the measured points.
              //  This will be Z1.

              chrtr2_record.z1 = egmr + array[j];


              //  Z0 can now be computed by subtracting the total correction from Z1.

              chrtr2_record.z0 = chrtr2_record.z1 - chrtr2_record.z;
 

              //  Write out the record with the newly computed Z0 and Z1 values.

              chrtr2_write_record_row_col (chrtr2_hnd, i, j, chrtr2_record);


              //  If we asked for the EGM08 surface, write the record to the EGM08 CHRTR2 file.

              if (egm_out)
                {
                  chrtr2_record.z = egmr;
                  chrtr2_record.status = CHRTR2_REAL;
                  chrtr2_write_record_row_col (egm_hnd, i, j, chrtr2_record);
                }
            }

          progress.wbar->setValue (i);
          qApp->processEvents ();
        }

      progress.wbar->setValue (gridrows);
      qApp->processEvents ();


      free (array);
      break;
    }


  if (input_count) chrtr2_close_file (chrtr2_hnd);

  if (egm_out) chrtr2_close_file (egm_hnd);


  button (QWizard::FinishButton)->setEnabled (true);
  button (QWizard::CancelButton)->setEnabled (false);


  QApplication::restoreOverrideCursor ();


  checkList->addItem (" ");
  cur = new QListWidgetItem (tr ("Separation surface generation complete, press Finish to exit."));

  checkList->addItem (cur);
  checkList->setCurrentItem (cur);
  checkList->scrollToItem (cur);
}



void datumSurface::slotGridCompleted ()
{
  complete = NVTrue;
}
