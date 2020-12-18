<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1">
<context>
    <name>datumSurface</name>
    <message>
        <location filename="datumSurface.cpp" line="94"/>
        <source>&amp;Run</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="datumSurface.cpp" line="96"/>
        <source>Start generating the separation surface</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="datumSurface.cpp" line="163"/>
        <source>ASCII Input file : %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="datumSurface.cpp" line="166"/>
        <source>Area file : %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="datumSurface.cpp" line="169"/>
        <source>Output file : %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="datumSurface.cpp" line="175"/>
        <source>Output file grid size (meters) : %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="datumSurface.cpp" line="250"/>
        <source>Unable to open input file %1
Reason : %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="datumSurface.cpp" line="268"/>
        <source>Error reading area file %1
Reason : %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="datumSurface.cpp" line="301"/>
        <source>Grid rows : %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="datumSurface.cpp" line="306"/>
        <source>Grid columns : %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="datumSurface.cpp" line="363"/>
        <source>No input points read from file %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="datumSurface.cpp" line="401"/>
        <location filename="datumSurface.cpp" line="441"/>
        <source>Error creating CHRTR2 file %1
Reason : %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="datumSurface.cpp" line="447"/>
        <source>No input data files</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="datumSurface.cpp" line="451"/>
        <location filename="datumSurface.cpp" line="489"/>
        <location filename="datumSurface.cpp" line="667"/>
        <location filename="datumSurface.cpp" line="716"/>
        <source>Writing output file %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="datumSurface.cpp" line="485"/>
        <source>Input file %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="datumSurface.cpp" line="543"/>
        <source>Computing MISP Z0 surface for input file %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="datumSurface.cpp" line="586"/>
        <source>MISP Z0 surface for %1 complete</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="datumSurface.cpp" line="603"/>
        <source>Retrieving MISP Z0 surface</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="datumSurface.cpp" line="710"/>
        <source>MISP Z1 surface for %1 complete</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="datumSurface.cpp" line="797"/>
        <source>Separation surface generation complete, press Finish to exit.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="datumSurfaceHelp.hpp" line="40"/>
        <source>Pressing this button will begin the process of generating the CHRTR2 file.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>runPage</name>
    <message>
        <location filename="runPage.cpp" line="47"/>
        <source>Process Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="runPage.cpp" line="58"/>
        <source>Processing input data</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="runPage.cpp" line="66"/>
        <source>Progress of input data processing.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="runPage.cpp" line="73"/>
        <source>Writing CHRTR2 output file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="runPage.cpp" line="81"/>
        <source>Progress of writing CHRTR2 output file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="runPage.cpp" line="88"/>
        <source>Process status</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>startPage</name>
    <message>
        <location filename="startPage.cpp" line="51"/>
        <source>Introduction</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="startPage.cpp" line="53"/>
        <source>datumSurface is a Qt wizard program the builds a datum surface using the EGM08 model and one or more shift points that are made up of position, MSL offset from the ellipsoid and MLLW (or other datum[i.e. z0]) offset from MSL.&lt;br&gt;&lt;br&gt;The input data points, which consist of a position, a measured separation value between MSL and the ellipsoid, and a measured separation value between MSL and the desired datum (usually MLLW), are used to create a cubic spline interpolated surface that will be used to determine the datum shift value for the sonar and/or LIDAR data files.  This is done by subtracting the EGM08 ellipsoid/geoid separation model value at the location of the measured input data points from the measured MSL/ellipsoid separation at that position.  The MLLW separation value is then subtracted from the result.&lt;br&gt;&lt;br&gt;Each of these points is input to a cubic spline interpolation function to create a surface that represents the difference between the EGM08 model of the ellipsoid/geoid separation and the measured ellipsoid/MLLW separation.  This surface, and the final surface, will be created at a user defined grid size.  Due to the low frequency nature of these surfaces, and the fact that we are using the one-minute EGM08 model, the grid size can be fairly large (we use 100 meters usually).&lt;br&gt;&lt;br&gt;Once the difference surface is created we then query the EGM08 model at the specified grid spacing.  The results of the query are added to the difference surface value at each grid node and then output to a CHRTR2 grid file.  This grid file is the final datum shift surface to be applied to the sonar and/or LIDAR data files.&lt;br&gt;&lt;br&gt;What we have actually done here is to warp the shape of the EGM08 ellipsoid/geoid separation model to our known measured points.  The assumption being that the EGM08 model is a better representation of the shape of the separation surface in areas where we do not have measured separation values than an interpolated surface would be.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="startPage.cpp" line="74"/>
        <source>datumSurface is a Qt wizard program that reads an ASCII file containing one or more ellipsoid/MSL and MSL/datum offsets at geographic locations and outputs a CHRTR2 grid file that is made up of the EGM08 model warped to the input points.  It can also, optionally, output a CHRTR2 grid file that contains just the EGM08 surface.  In the input ASCII file each entry must consist of latitude, longitude, MSL ellipsoid offset, MLLW (or other datum) offset from MSL.  MSL ellipsoid offset is the distance from MSL to the ellipsoid.  Positive values indicate that MSL is above the ellipsoid.  MLLW offset is the distance from MLLW to MSL.  All fields must be comma separated.  Select an input file below.  You may then select an area file, change the default output file name, and set the bin size for the output file.  Note that the bin size can be fairly large since the final field will not have much high frequency content.  Normally 100 meters is considered to be reasonable.&lt;br&gt;&lt;br&gt;Help is available by clicking on the Help button and then clicking on the item for which you need information.  For a more involved explanation of this program click the Help button and then click on this text.  Click &lt;b&gt;Next&lt;/b&gt; to continue or &lt;b&gt;Cancel&lt;/b&gt; to exit.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="startPage.cpp" line="100"/>
        <source>ASCII Input File</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="startPage.cpp" line="107"/>
        <location filename="startPage.cpp" line="130"/>
        <location filename="startPage.cpp" line="153"/>
        <source>Browse...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="startPage.cpp" line="123"/>
        <source>Area File</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="startPage.cpp" line="147"/>
        <source>Output CHRTR2 File</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="startPage.cpp" line="163"/>
        <source>Grid size (meters)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="startPage.cpp" line="169"/>
        <source>Change the grid size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="startPage.cpp" line="205"/>
        <source>datumSurface Open Input File</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="startPage.cpp" line="215"/>
        <location filename="startPage.cpp" line="219"/>
        <source>All files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="startPage.cpp" line="243"/>
        <source>datumSurface Output File</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="startPage.cpp" line="286"/>
        <source>datumSurface Area File</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="startPage.cpp" line="297"/>
        <location filename="startPage.cpp" line="301"/>
        <source>Area file (*.ARE *.are *.afs *.shp *.SHP)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="startPage.cpp" line="326"/>
        <source>Cannot open shape file %1!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="startPage.cpp" line="339"/>
        <source>Shape file %1 is not a polygon or polyline file!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="startPage.cpp" line="354"/>
        <source>Number of vertices (%1) of shape file %2 is too few for a polygon!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="startPage.cpp" line="368"/>
        <source>Shape file %1 does not appear to be geographic!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="startPageHelp.hpp" line="40"/>
        <source>Use the browse button to select an ASCII input file.  You cannot modify the text in the &lt;b&gt;Input File&lt;/b&gt; text window.  The reason for this is that the file must exist in order for the program to run.  This file is not required to run datumSurface but if it is not supplied you will only get an output file containing the EGM08 surface.  The file format is as follows:&lt;br&gt;&lt;br&gt;LAT,LON,MSL,DATUM&lt;br&gt;&lt;br&gt;Where:&lt;br&gt;&lt;br&gt;&lt;ul&gt;&lt;li&gt;LAT = latitude&lt;/li&gt;&lt;li&gt;LON = longitude&lt;/li&gt;&lt;li&gt;MSL = offset of Mean Sea Level from the ellipsoid&lt;/li&gt;&lt;li&gt;DATUM = offset of the local datum (like MLLW) from MSL&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;&lt;br&gt;Latitude and longitude may be in any of the standard position formats:&lt;br&gt;&lt;br&gt;&lt;ul&gt;&lt;li&gt;Hemisphere Degrees decimal                 : S 28.4532&lt;/li&gt;&lt;li&gt;Hemisphere Degrees minutes decimal         : S 28 27.192&lt;/li&gt;&lt;li&gt;Hemisphere Degrees minutes seconds decimal : S 28 27 11.52&lt;/li&gt;&lt;li&gt;Signed Degrees decimal                 : -28.4532&lt;/li&gt;&lt;li&gt;Signed Degrees minutes decimal         : -28 27.192&lt;/li&gt;&lt;li&gt;Signed Degrees minutes seconds decimal : -28 27 11.52&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;&lt;br&gt;There may be multiple lines per file (for multiple locations).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="startPageHelp.hpp" line="64"/>
        <source>Use this button to select the ASCII input file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="startPageHelp.hpp" line="67"/>
        <source>You may enter a new file name to be used for the output CHRTR2 file or modify the default file name that was placed here when you selected the area file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="startPageHelp.hpp" line="71"/>
        <source>Use this button to select a pre-existing CHRTR2 file to be overwritten.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="startPageHelp.hpp" line="74"/>
        <source>You must use the Browse button to select an area file name to be used to define the area of the output CHRTR2 file.  The area file may be in the ISS-60 survey planner area format (.ARE), the NAVO generic area format (.are), the Army Corps area format (.afs), or ESRI shape file format.  Shape files must be either Polygon, PolygonZ, PolygonM, PolyLine, PolyLineZ, or PolyLineM format and must be geographic (not projected).  For PolyLine files the first point will be duplicated to close the polygon.  Whether using Polygon or PolyLine files, only the first shape will be used if there are multiple shapes in the file.&lt;br&gt;&lt;br&gt;Generic area format files (.are) contain a simple list of polygon points.  The points may be in any of the following formats:&lt;ul&gt;&lt;li&gt;Hemisphere Degrees Minutes Seconds.decimal&lt;/li&gt;&lt;li&gt;Hemisphere Degrees Minutes.decimal&lt;/li&gt;&lt;li&gt;Hemisphere Degrees.decimal&lt;/li&gt;&lt;li&gt;Sign Degrees Minutes Seconds.decimal&lt;/li&gt;&lt;li&gt;Sign Degrees Minutes.decimal&lt;/li&gt;&lt;li&gt;Sign Degrees.decimal&lt;/li&gt;&lt;/ul&gt;The lat and lon must be entered one per line, separated by a comma.  You do not need to repeat the first point, the polygon will be closed automatically.&lt;br&gt;&lt;br&gt;Note that the &lt;b&gt;Next&lt;/b&gt; button will not work until you select an area file.  When one is selected the default output file name will be supplied in the &lt;b&gt;Output File&lt;/b&gt; text window.  The output CHRTR2 file name can be edited since it may be a new file &lt;i&gt;or&lt;/i&gt; a pre-existing file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="startPageHelp.hpp" line="98"/>
        <source>Use this button to select an area file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="startPageHelp.hpp" line="101"/>
        <source>Change the output file grid spacing.  Due to the low frequency nature of the output surface, and the fact that we are using the one-minute EGM08 model, the grid size can be fairly large (we use 100 meters usually).</source>
        <translation type="unfinished"></translation>
    </message>
</context>
</TS>
