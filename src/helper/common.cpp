#include "../../inc/helper/common.h"

// standard headers
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "../../inc/helper/constants.h"

#ifdef __linux__ 
// linux specific headers
#include <unistd.h>

#elif _WIN32 // windows code goes here
// windows specific header

#include <Windows.h>
#include <strsafe.h>

// Open GL headers
#include <GL/gl.h>

char* getcwd(char* cwd, size_t size)
{
    return (cwd);
}

#else
// platform support not added

#endif


// macros

// file scoped global variables
static char _currdir[256] ; 
static char _filename[256] ; 
static FILE* _pFile = NULL; 


// external functions
extern void set_title(const char* const);

// internal function declaration
char* currentDateTime(void);


////////////

static char log_buffer[MAX_LOG_LENGTH];

const char* filename(const char* filewithpath)
{
    int len = strlen(filewithpath);
    while(len > 0) {
        char c = filewithpath[len -1];
        if(c == PATH_SEPARATOR)
            return (filewithpath + len);
        len--;
    }
    return (filewithpath);
}

const char* removepath(const char* filewithpath)
{
    char cwd[MAX_LOG_LENGTH];
    getcwd(cwd, sizeof(cwd));
    const char* result = strstr(filewithpath, cwd);
    if(result != NULL)
    {
        return (filewithpath + strlen(cwd)+1);
    }

    return (filewithpath);
}

int log_printf_internal(char const* const filewithpath, char const* const funcname, int linenum, char const* const buffer)
{
    int _Result = 0;
    char firstCall = 0;
    if(0 == strlen(_filename)) {
        strcpy(_filename, "log.txt");
        firstCall = 1;
    }
    
	if ((_pFile = fopen( _filename, firstCall?"w":"a")) == NULL)
	{
		_Result = -1;
	}
    else
    {
        _Result = fprintf(_pFile, "%s %s(%d): %s: %s", currentDateTime(), filename(filewithpath), linenum, funcname, buffer);
        // _Result = fprintf(_pFile, "%s %s(%d):%s(): %s", currentDateTime(), filewithpath, linenum, funcname, buffer);
        // _Result = fprintf(_pFile, "%s", buffer);
		fclose(_pFile);
        _pFile = NULL;
    }
    return _Result;	
}

#ifdef __linux__ 
    //linux code goes here
    // dummy macro
#include <sys/time.h>
#include <string.h>

char* currentDateTime(void)
{
    time_t timer = 0;
    struct tm* tm_info = NULL;

    // timer = time(NULL);
    tm_info = localtime(&timer);
    memset(log_buffer, 0, MAX_LOG_LENGTH);
    strftime(log_buffer, MAX_LOG_LENGTH, "%Y-%m-%d %X", tm_info);
    return (log_buffer);
};

int log_open(char const* FileName , char const* Mode)
{
    char timebuff[20] = {};
    time_t now = time (0);

    struct timeval tval;
    gettimeofday(&tval, NULL);

    struct tm *sTm = gmtime (&now);
    // struct tm *sTm = gmtime (&tval.tv_sec);
    strftime (timebuff, sizeof(timebuff), "%Y-%m-%d %H:%M:%S", sTm);
    // printf("%s.%06ld %s(%d): %s\n",timebuff, (long int)tval.tv_usec, __FILE__, __LINE__, __FUNCTION__);

    int retval = -1;
	if ((_pFile = fopen(FileName, Mode)) == NULL)
	{
		retval = -1;
    }
	else
	{
        strcpy(_filename, FileName);
        retval = 0;
        fclose(_pFile);
        _pFile = NULL;
	}

    return retval;
}

int log_printf(char const* const filewithpath, char const* const funcname, int linenum, char const* const format, ...)
{

    char buffer[MAX_LOG_LENGTH] = {};
    va_list _ArgList;
    __builtin_va_start(_ArgList, format);
    vsnprintf(buffer, MAX_LOG_LENGTH, format, _ArgList);
    __builtin_va_end(_ArgList);

    return log_printf_internal(filewithpath, funcname, linenum, buffer);
}

char* vararg2string(const char* format, ...)
{
    va_list _ArgList;
    memset(log_buffer, 0, MAX_LOG_LENGTH);
    __builtin_va_start(_ArgList, format);
    vfprintf(_pFile, format, _ArgList);
    __builtin_va_end(_ArgList);
    return (log_buffer);
}

#elif _WIN32

// windows code goes here
#include <strsafe.h>

char* currentDateTime()
{
    time_t now = time(NULL);
    struct tm curr_time = *localtime(&now);
    memset(log_buffer, 0, MAX_LOG_LENGTH);
    strftime(log_buffer, MAX_LOG_LENGTH, "%Y-%m-%d %X", &curr_time);
    return (log_buffer);
};

// file operations
int log_open(char const* FileName , char const* Mode)
{
    printf("File %s(%d): %s\n", __FILE__, __LINE__, __FUNCTION__);
    int retval = -1;
	if (fopen_s(&_pFile, FileName, Mode) != 0)
	{
		retval = -1;
	}
	else
	{
        strcpy(_filename, FileName);
        retval = 0;
        fclose(_pFile);
        _pFile = NULL;
	}

    return retval;
}

int log_printf(char const* const filewithpath, char const* const funcname, int linenum, char const* const format, ...)
{
    char buffer[MAX_LOG_LENGTH] = {};

    va_list _ArgList;
    __crt_va_start(_ArgList, format);
    _vsnprintf_l(buffer, MAX_LOG_LENGTH, format, NULL, _ArgList);
    __crt_va_end(_ArgList);

    int retval = log_printf_internal(filewithpath, funcname, linenum, buffer);

    // adding to log to title
    set_title(buffer);

    return (retval);
}

char* vararg2string(const char* format, ...)
{
    va_list _ArgList;
    memset(log_buffer, 0, MAX_LOG_LENGTH);
    __crt_va_start(_ArgList, format);
    _vsnprintf_l(log_buffer, MAX_LOG_LENGTH, format, NULL, _ArgList);
    __crt_va_end(_ArgList);
    return (log_buffer);
}

#else
// architecture not supported
#endif

int log_close(void)
{
    if(_pFile)  {
        fclose(_pFile);
        _pFile = NULL;
    }
    return (0);
}

float degToRad(float degrees) {

    return (degrees * M_PI / 180.0f);
}
