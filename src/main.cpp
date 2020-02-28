#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include "rapidjson.hpp"

using std::string;
using std::cout;
using std::cin;
using std::endl;
using rapidjson::FileReadStream;
using rapidjson::Document;
using rapidjson::Value;


#define TEST_SUPPLY_FILE_PATH

int restoreHierachy(string&, string&);
int processFile(string&, string&, string&);
int createDirectory(string&);

string lastFileDirectory;

int main()
{
  string jsonFilePath, outputFolderPath;
  
# ifdef TEST_SUPPLY_FILE_PATH
#		ifdef WIN32
			jsonFilePath = "E:/MultiMC/assets/indexes/1.15.json";
			outputFolderPath = "E:/!BKUP/ASSETS";
#		else
			jsonFilePath = "/home/rigoligo/.local/share/multimc/assets/indexes/1.16.json";
			outputFolderPath = "/run/media/rigoligo/storage/ASSETS/";
# endif
# else
  cout << "请输入assets/indexes/版本号.json的绝对路径：";
  cin.getline(jsonFilePath);
  cout << "请输入输出目录的绝对路径：";
  cin.getline(outputFolderPath);
# endif
  
  restoreHierachy(jsonFilePath, outputFolderPath);
  
  return 0;
}

int restoreHierachy(string &jsonPath, string &outputPath)
{
  std::FILE *parseFile = std::fopen(jsonPath.c_str(), "r");

  char *readbuffer = new char[65536];
  FileReadStream fileReader(parseFile, readbuffer, 65536);
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
  
  string objHash, objFilename;
  
  for(auto i = objObjects->MemberBegin(); i != objObjects->MemberEnd(); i++)
  {
    objFilename = i->name.GetString();
    objHash = i->value["hash"].GetString();
    
    cout << objHash << ": " << objFilename << endl;
  }
  
  
  return 0;
};

int processFile(string& hash, string& filename, string& fullDestination)
{

}

int createDirectory(string& fullPath)
{
	
}