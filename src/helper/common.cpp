#include "../../inc/helper/common.h"

// standard headers
#include <stdio.h>
#include <stdlib.h>

#include "../../inc/helper/constants.h"

#ifdef __linux__ 
// linux specific headers

#elif _WIN32 // windows code goes here
// windows specific header

#include <Windows.h>
#include <strsafe.h>

// Open GL headers
#include <GL/gl.h>

#else
// platform support not added

#endif


// macros
#include <time.h>


static char _filename[256] ; 
static FILE* _pFile = NULL; 


////////////

static char log_buffer[MAX_LOG_LENGTH];

const char* removepath(const char* filewithpath)
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

#ifdef __linux__ 
    //linux code goes here
    // dummy macro
#include <sys/time.h>
#include <string.h>

char* currentDateTime()
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
	}

    fclose(_pFile);
    _pFile = NULL;
    return retval;
}

int log_printf(char const* const filewithpath, char const* const funcname, int linenum, char const* const format, ...)
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
        char myBuffer[MAX_LOG_LENGTH] = {};
        va_list _ArgList;
        memset(log_buffer, 0, MAX_LOG_LENGTH);
        __builtin_va_start(_ArgList, format);
        vfprintf(_pFile, format, _ArgList);
        __builtin_va_end(_ArgList);

        // _Result = fprintf(_pFile, "%s %s(%d): %s: %s", currentDateTime(), removepath(filewithpath), linenum, funcname, myBuffer);
        _Result = fprintf(_pFile, "%s %s(%d): %s: %s", currentDateTime(), filewithpath, linenum, funcname, myBuffer);
        
    }
    fclose(_pFile);
    return _Result;	
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

// external functions
extern void set_title(char*);

int log_printf(char const* const filewithpath, char const* const funcname, int linenum, char const* const format, ...)
{
    int _Result = 0;
    char firstCall = 0;
    if(0 == strlen(_filename)) {
        strcpy(_filename, "log.txt");
        firstCall = 1;
    }
    
    char myBuffer[MAX_LOG_LENGTH] = {};
	if (fopen_s(&_pFile, _filename, firstCall?"w":"a") != 0)
	{
		_Result = -1;
	}
    else
    {
        va_list _ArgList;
        __crt_va_start(_ArgList, format);
        _vsnprintf_l(myBuffer, MAX_LOG_LENGTH, format, NULL, _ArgList);
        __crt_va_end(_ArgList);

        _Result = fprintf(_pFile, "%s %s(%d): %s() %s", currentDateTime(), removepath(filewithpath), linenum, funcname, myBuffer);
        // _Result = fprintf(_pFile, "%s %s(%d): %s() %s", currentDateTime(), filewithpath, linenum, funcname, myBuffer);
        // _Result = fprintf(_pFile, "%s", myBuffer);
        fflush(_pFile);
        fclose(_pFile);
        _pFile = NULL;
    }
    // adding to log to title
    set_title(myBuffer);
    return _Result;	
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


