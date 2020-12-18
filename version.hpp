
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




#ifndef VERSION

#define     VERSION     "PFM Software - datumSurface V2.14 - 08/07/19"

#endif

/*!<pre>

    The following comments are from the original, command line, datum_surface program's version.h file.

    ******************************************************************************************

    Version 1.00
    Jan C. Depner
    04/01/10

    First working version.


    Version 1.01
    Jan C. Depner
    04/29/10

    Fixed posfix and fixpos calls to use numeric constants instead of strings for type.


    Version 1.02
    Jan C. Depner
    08/30/10

    Removed GMT gridding option.  Due to the low frequency of the surfaces we really didn't need to use
    GMT (for tensioning).  Removing GMT option got rid of unneeded complexity.  Added ability to just
    dump the EGM08 surface (-e option without an input file).


    Version 1.03
    Jan C. Depner
    03/31/11

    Now saves the z0 and z1 separation values in the chrtr2 record.  We still store the total separation in
    the z field of the chrtr2 record.  We're saving the z0 and z1 fields for possible future inclusion into
    BAG files.

    ******************************************************************************************


    Version 2.00
    Jan C. Depner
    04/21/11

    First working version.


    Version 2.01
    Jan C. Depner
    06/14/11

    Fixed references to GeoTIFF that should have said CHRTR2.  Fixed output file name glitch.


    Version 2.03
    Jan C. Depner
    07/22/11

    Using setSidebarUrls function from nvutility to make sure that current working directory (.) and
    last used directory are in the sidebar URL list of QFileDialogs.


    Version 2.04
    Jan C. Depner
    11/30/11

    Converted .xpm icons to .png icons.


    Version 2.05
    Jan C. Depner (PFM Software)
    12/09/13

    Switched to using .ini file in $HOME (Linux) or $USERPROFILE (Windows) in the ABE.config directory.  Now
    the applications qsettings will not end up in unknown places like ~/.config/navo.navy.mil/blah_blah_blah on
    Linux or, in the registry (shudder) on Windows.


    Version 2.06
    Jan C. Depner (PFM Software)
    03/21/14

    Removed set_egm08_grid_type call.  We're just using the 1X1 minute grid.


    Version 2.07
    Jan C. Depner (PFM Software)
    07/01/14

    - Replaced all of the old, borrowed icons with new, public domain icons.  Mostly from the Tango set
      but a few from flavour-extended and 32pxmania.


    Version 2.08
    Jan C. Depner (PFM Software)
    07/23/14

    - Switched from using the old NV_INT64 and NV_U_INT32 type definitions to the C99 standard stdint.h and
      inttypes.h sized data types (e.g. int64_t and uint32_t).


    Version 2.09
    Jan C. Depner (PFM Software)
    03/31/15

    - Added ability to use ESRI Polygon, PolygonZ, PolygonM, PolyLine, PolyLineZ, or PolyLineM geographic shape
      files as area files.


    Version 2.10
    Jan C. Depner (PFM Software)
    05/07/15

    - Corrected help text for ASCII text file input.  It incorrectly stated that the DATUM was the difference between
      the local datum and the ellipsoid.  It is actually the difference between the local datum and MSL.


    Version 2.11
    Jan C. Depner (PFM Software)
    05/12/15

    - Make sure that program doesn't barf if some bozo puts a blank line in their text input file.


    Version 2.12
    Jan C. Depner (PFM Software)
    08/27/16

    - Now uses the same font as all other ABE GUI apps.  Font can only be changed in pfmView Preferences.


    Version 2.13
    Jan C. Depner (PFM Software)
    09/26/17

    - A bunch of changes to support doing translations in the future.  There is a generic
      datumSurface_xx.ts file that can be run through Qt's "linguist" to translate to another language.


    Version 2.14
    Jan C. Depner (PFM Software)
    08/07/19

    - Now that get_area_mbr supports shape files we don't need to handle it differently from the other
      area file types.

</pre>*/
