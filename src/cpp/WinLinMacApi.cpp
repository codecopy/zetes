// Global includes/methods

#include <sstream>
#include <string>

#include "WinLinMacApi.h"

using namespace std;

string simpleLocateResource(const string& path, const string& filename)
{
	stringstream ss;
	ss << path << "/" << filename;

	return ss.str();
}


#ifdef __APPLE__
// MacOS X includes/methods

#include </usr/include/sys/syslimits.h>

#include <CoreFoundation/CFBundle.h>

string WinLinMacApi::locateResource(const string& path, const string& filename)
{
	string res = "";
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	if (mainBundle)
	{
		CFStringRef cfFileName = CFStringCreateWithCString(NULL, filename.c_str(), kCFStringEncodingASCII);
		CFStringRef cfPath = CFStringCreateWithCString(NULL, path.c_str(), kCFStringEncodingASCII);

		CFURLRef resourceURL = CFBundleCopyResourceURL(mainBundle, cfFileName, NULL, cfPath);
		if (resourceURL)
		{
			char fileurl[PATH_MAX + 1];
			if (CFURLGetFileSystemRepresentation(resourceURL, true, (UInt8*)fileurl, PATH_MAX))
			{
				res = fileurl;
			}
			else
			{
				res = simpleLocateResource(path, filename);
			}
			CFRelease(resourceURL);
		}
		else
		{
			res = simpleLocateResource(path, filename);

		}
		CFRelease(cfFileName);
		CFRelease(cfPath);
	}
	else
	{
		// from the current directory
		res = simpleLocateResource(path, filename);
	}
	return res;
}

string WinLinMacApi::locateExecutable()
{
	string res = "";
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	if (mainBundle)
	{
		CFURLRef executableURL = CFBundleCopyExecutableURL(mainBundle);	// CFBundleCopyBundleURL
		if (executableURL)
		{
			CFURLRef executablePathUrl = CFURLCreateCopyDeletingLastPathComponent(kCFAllocatorDefault, executableURL);

			CFStringRef executablePath = CFURLCopyPath(executablePathUrl);	// CFURLCopyFileSystemPath
			if (executablePath)
			{
				char fileurl[PATH_MAX + 1];
				if (CFStringGetCString(executablePath, fileurl, PATH_MAX, kCFStringEncodingUTF8))
				{
					res = fileurl;
				}
				else
				{
					res = "./";
				}
				CFRelease(executablePath);
			}
			else
			{
				res = "./";
			}
			CFRelease(executablePath);
			CFRelease(executableURL);
		}
	}
	else
	{
		// from the current directory
		res = "./";
	}
	return res;
}
/*char *GetExecutableLocation()
{
	CFBundleRef bundle = CFBundleGetMainBundle();
	CFURLRef executableURL = CFBundleCopyExecutableURL(bundle);
	CFStringRef executablePath = CFURLCopyFileSystemPath(executableURL, kCFURLPOSIXPathStyle);
	CFIndex maxLength = CFStringGetMaximumSizeOfFileSystemRepresentation(executablePath);
	char *result = malloc(maxLength);

	if (result)
	{
		if(!CFStringGetFileSystemRepresentation(executablePath, result, maxLength))
		{
			free(result);
			result = NULL;
		}
	}

	CFRelease(executablePath);
	CFRelease(executableURL);

	return result;
}*/


#elif __WIN32__

// Win32 includes/methods
#include <windows.h>
#include <direct.h>
#define GetCurrentDir _getcwd

BOOL FileExists(string szPath)
{
  DWORD dwAttrib = GetFileAttributes(szPath.c_str());

  return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
         !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

string WinLinMacApi::locateExecutable()
{
	char szAppPath[MAX_PATH] = "";
	char szAppDirectory[MAX_PATH]= "";

	if (!GetModuleFileName(NULL, szAppPath, MAX_PATH - 1))
	{
		// Trying to locate the resource locally...
		return ".\\";
	}

	strncpy(szAppDirectory, szAppPath, strrchr(szAppPath, '\\') - szAppPath);
	szAppDirectory[MAX_PATH - 1] = '\0';				// For sure...


	return szAppDirectory;
}

string WinLinMacApi::locateResource(const string& path, const string& filename)
{
	string appDirectory = locateExecutable();

	stringstream ss;
	ss << appDirectory << "\\" << path << "\\" << filename;

	if (!FileExists(ss.str()))
	{
		// Trying to locate the resource locally...
		return simpleLocateResource(path, filename);
	}

	return ss.str();
}

#endif
