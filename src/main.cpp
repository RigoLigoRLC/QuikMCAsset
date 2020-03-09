#include <iostream>
#include <string>
#include <cstdio>
#include "rapidjson.hpp"

using std::string;
using std::cout;
using std::cin;
using std::endl;
using rapidjson::FileReadStream;
using rapidjson::Document;
using rapidjson::Value;


//#define TEST_SUPPLY_FILE_PATH

int restoreHierachy(const string&, const string&);
int processFile(const string&, const string&, const string&, const string&);
int createDirectory(const string&);
bool isDirectoryExist(const string& path);
int copyFile(const string&, const string&);
void findAndReplaceString(std::string& subject, const std::string& search, const std::string& replace);

string lastFileDirectory;

int main()
{
  string jsonFilePath, outputFolderPath;
  
# ifdef TEST_SUPPLY_FILE_PATH
#		ifdef WIN32
			jsonFilePath = "E:\\MultiMC\\assets\\indexes\\1.13.json";
			outputFolderPath = "E:\\MCASSETS\\";
#		else
			jsonFilePath = "/home/rigoligo/.local/share/multimc/assets/indexes/1.15.json";
			outputFolderPath = "/mnt/iw_entertainment/ASSETS/";
#   endif
# else
    cout << "Enter the absolute path of .../assets/indexes/[GAME_VERSION].json: ";
    getline(cin, jsonFilePath);
    cout << "Enter the absolute path of output directory: ";
    getline(cin, outputFolderPath);

    //jsonFilePath.pop_back();
    //outputFolderPath.pop_back();
# endif
# ifdef _WIN32
    if(outputFolderPath[outputFolderPath.size() - 1] != '\\')
      outputFolderPath += '\\';
# else
    if(outputFolderPath[outputFolderPath.size() - 1] != '/')
      outputFolderPath += '/';
# endif

  restoreHierachy(jsonFilePath, outputFolderPath);
  
  return 0;
}

int restoreHierachy(const string &jsonPath, const string &outputPath)
{
  std::FILE *parseFile = std::fopen(jsonPath.c_str(), "r");

  char *readbuffer = new char[BUFSIZ];
  FileReadStream fileReader(parseFile, readbuffer, BUFSIZ);
  Document parsedDoc;
  Value *objObjects;
  parsedDoc.ParseStream(fileReader);

  std::fclose(parseFile);

  if(parsedDoc.HasParseError())
  {
    cout << "\tError when parsing file \"" << jsonPath << "\":\n"
         << "\tError code " << parsedDoc.GetParseError() << " at offset "
				 << parsedDoc.GetErrorOffset() << ".\n";
    throw "Cannot parse JSON.";
  }
  
  if(parsedDoc.HasMember("objects"))
  {
    objObjects = &parsedDoc["objects"];
    if(!objObjects->IsObject())
      throw "Wrong type of \"objects\".";
  }
  else
    throw "\"objects\" not found.";
  
  string objHash, objFilename, assetObjectPath;

# ifdef WIN32
	assetObjectPath = jsonPath.substr(0, jsonPath.find_last_of('\\') - 7);
# else
	assetObjectPath = jsonPath.substr(0, jsonPath.find_last_of('/') - 7);
# endif
  
  for(auto i = objObjects->MemberBegin(); i != objObjects->MemberEnd(); i++)
  {
    objFilename = i->name.GetString();
    objHash = i->value["hash"].GetString();
#		ifdef WIN32
			findAndReplaceString(objFilename, "/", "\\");
#		endif
    cout << objHash << ": " << objFilename << endl;
    processFile(objHash, objFilename, outputPath, assetObjectPath);
  }
  
  

  return 0;
};

int processFile(const string& hash, const string& filename, const string& fullDestination,const string& assetPath)
{
  //First determine if this file is in the same directory as the last one
  //If so then just copy, don't create directory
# ifdef _WIN32
    string thisDirectory = filename.substr(0, filename.find_last_of('\\') + 1);
# else
    string thisDirectory = filename.substr(0, filename.find_last_of('/') + 1);
# endif
  if(thisDirectory != lastFileDirectory)
  {
    if(!isDirectoryExist(fullDestination + thisDirectory))
      createDirectory(fullDestination + thisDirectory);
    lastFileDirectory = thisDirectory;
  }
# ifdef _WIN32
    copyFile(assetPath + "objects\\" + hash.substr(0, 2) + "\\" + hash, fullDestination + filename);
# else
    copyFile(assetPath + "objects/" +  hash.substr(0, 2) + "/" + hash, fullDestination + filename);
# endif
		return 0;
}

void findAndReplaceString(std::string& subject, const std::string& search, const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos)
	{
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
}