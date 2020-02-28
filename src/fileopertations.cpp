#include <string>
#include <iostream>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef _WIN32
# include <direct.h>
#endif
#include <fcntl.h> //open
#include <unistd.h> //read write close
#include <cstdio> //BUFSIZ

using std::string;


bool isDirectoryExist(const string& path)
{
#ifdef _WIN32
  struct _stat info;
  if (_stat(path.c_str(), &info) != 0)
  {
    return false;
  }
  return (info.st_mode & _S_IFDIR) != 0;
#else 
  struct stat info;
  if (stat(path.c_str(), &info) != 0)
  {
    return false;
  }
  return (info.st_mode & S_IFDIR) != 0;
#endif
}

int copyFile(const string &sourcePath, const string &destinationPath)
{
  size_t size;
  char *buf = new char[BUFSIZ];

  int sourceHandle = open(sourcePath.c_str(), 00, 0); //00 for readonly
  int destinationHandle = open(destinationPath.c_str(), O_WRONLY | O_CREAT , 0644); //01 for writeonly

  while((size = read(sourceHandle, buf, BUFSIZ)) > 0)
  {
    write(destinationHandle, buf, size);
  }

  close(sourceHandle);
  close(destinationHandle);

  delete[] buf;

  return 0;
}

int createDirectory(const string& fullPath)
{
#ifdef _WIN32
  int ret = _mkdir(fullPath.c_str());
#else
  mode_t mode = 0755;
  int ret = mkdir(fullPath.c_str(), mode);
#endif
  if (ret == 0)
    return true;

  switch (errno)
  {
    case ENOENT: 
      // parent didn't exist, try to create it
      {
#ifndef _WIN32
        int pos = fullPath.find_last_of('/');
        if (pos == std::string::npos)
#else
        int pos = fullPath.find_last_of('\\');
        if (pos == std::string::npos)
#endif
          return false;
        if (!createDirectory(fullPath.substr(0, pos)))
          return false;
      }
      // now, try to create again
#ifdef _WIN32
      return 0 == _mkdir(fullPath.c_str());
#else 
      return 0 == mkdir(fullPath.c_str(), mode);
#endif

    case EEXIST:
      // done!
      return isDirectoryExist(fullPath);

    default:
      return false;
  }
}