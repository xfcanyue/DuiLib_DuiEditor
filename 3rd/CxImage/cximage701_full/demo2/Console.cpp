// Console.cpp : CxImage console demo.

#include "stdafx.h"
#include <io.h>

#include "ximage.h"

TCHAR* FindExtension(const TCHAR * name);
int	convert_image_types( TCHAR *input_image , int typein , TCHAR *output_image , int typeout );
int	convert_image_type(  TCHAR *input_image , int typein , TCHAR *output_image , int typeout );

////////////////////////////////////////////////////////////////////////////////
#ifdef UNICODE
int wmain( int argc, TCHAR *argv[] )
#else
int main( int argc, TCHAR *argv[] )
#endif
{
	int status = 0;

#define ENABLE_BATCH_CONVERSION 1

    if (argc<3) {
        _ftprintf(stderr, CxImage::GetVersion());
        _ftprintf(stderr, _T("\nConsole demo\n"));
        _ftprintf(stderr, _T("  single file convresion: %s input_file output_file\n"), argv[0]);
        _ftprintf(stderr, _T("  - example #1: %s test.jpg test.tif\n"), argv[0]);
#if ENABLE_BATCH_CONVERSION
        _ftprintf(stderr, _T("  batch file convresion: %s *.input_extesion *.output_extension\n"), argv[0]);
        _ftprintf(stderr, _T("  - example #2: %s *.bmp *.png\n"), argv[0]);
#endif
        return 1;
    }

	// See if arg 1 is a valid image type
	TCHAR* extin = FindExtension(argv[1]);
	_tcslwr(extin);
	int typein = CxImage::GetTypeIdFromName(extin);
	if (typein == CXIMAGE_FORMAT_UNKNOWN) {
        _ftprintf(stderr, _T("unknown extension for %s\n"), argv[1]);
        return 1;
	}

	// See if arg 2 is a valid image type
	TCHAR* extout = FindExtension(argv[2]);
	_tcslwr(extout);
	int typeout = CxImage::GetTypeIdFromName(extout);
	if (typeout == CXIMAGE_FORMAT_UNKNOWN) {
        _ftprintf(stderr, _T("unknown extension for %s\n"), argv[2]);
        return 1;
	}

#if ENABLE_BATCH_CONVERSION
	status = convert_image_types( argv[1] , typein , argv[2] , typeout );
#else
	status =  convert_image_type( argv[1] , typein , argv[2] , typeout );
#endif

	printf("Done!\n");
	return status;
}

////////////////////////////////////////////////////////////////////////////////
TCHAR* FindExtension(const TCHAR * name)
{
	int len = _tcslen(name);
	int i;
	for (i = len-1; i >= 0; i--){
		if (name[i] == '.'){
			return (TCHAR*)(name+i+1);
		}
	}
	return (TCHAR*)(name+len);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \author [Ian Knight]
 */
int	convert_image_types( TCHAR *input_image , int typein , TCHAR *output_image , int typeout )
{
	TCHAR  file_name[ _MAX_PATH ];
	long find_handle;
	int status = 0;

#ifdef UNICODE
	struct _wfinddata_t c_file;
#else
	struct _finddata_t c_file;
#endif

	// search for all input file types
	find_handle = _tfindfirst( input_image , &c_file );
	if (find_handle > 0)
	{
		do {
			_tcscpy( file_name , c_file.name ); // Get the found file name
			file_name[ _tcschr( c_file.name , '.' ) - c_file.name + 1 ] = 0; // cut to the file extention
			_tcscat( file_name , (TCHAR*)( _tcschr( output_image , '.' ) + 1 )  ); 
			_tprintf( _T("Converting [ %s ] to [ %s ]\n") , c_file.name , file_name );
			status = convert_image_type( c_file.name , typein , file_name , typeout );
			if( status != 0 )
				break;
		} while( _tfindnext( find_handle , &c_file ) == 0 );
		_findclose( find_handle );
	}

	return status;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \author [Ian Knight]
 */
int	convert_image_type( TCHAR *input_image , int typein , TCHAR *output_image , int typeout )
{
	CxImage image;

	if(!image.Load(input_image,typein))
	{
		_ftprintf(stderr, _T("%s\n"), image.GetLastError());
		_ftprintf(stderr, _T("error loading %s\n"), input_image);
		return 1;
	}

	if(!image.Save(output_image,typeout))
	{
		_ftprintf(stderr, _T("%s\n"), image.GetLastError());
		_ftprintf(stderr, _T("error saving %s\n"), output_image);
		return 1;
	}

	return 0;
}

