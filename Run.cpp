#include <dirent.h>
#include <stdio.h>
#include <cstdio>
#include <string>
using namespace std;

#include "rapidjson/filewritestream.h"
#include "rapidjson/writer.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
using namespace rapidjson;

const char* launchProfiles = "C:\\Users\\Will\\Twitch\\Minecraft\\Install\\launcher_profiles.json";
const char* modPacks = "C:\\Users\\Will\\Twitch\\Minecraft\\Instances";
const char* launchArgs1 = "-Xmx8192m -Xms256m -XX:PermSize=256m -Dminecraft.applet.TargetDirectory=\""; //add path to instance folder onto end then do launchArgs1 + path + launchArgs2
const char* launchArgs2 = "\" -Dfml.ignorePatchDiscrepancies=true -Dfml.ignoreInvalidMinecraftCertificates=true -Duser.language=en -Duser.country=US -d64 -server -XX:+AggressiveOpts -XX:ParallelGCThreads=8 -XX:+UseConcMarkSweepGC -XX:+UnlockExperimentalVMOptions -XX:+UseParNewGC -XX:+ExplicitGCInvokesConcurrent -XX:MaxGCPauseMillis=10 -XX:GCPauseIntervalMillis=50 -XX:+UseFastAccessorMethods -XX:+OptimizeStringConcat -XX:NewSize=84m -XX:+UseAdaptiveGCBoundary -XX:NewRatio=3 -Dfml.readTimeout=90 -Ddeployment.trace=true -Ddeployment.log=true -Ddeployment.trace.level=all";

int main(){
    
	FILE* fo = fopen(launchProfiles, "rb"); // non-Windows use "r"
 
	char readBuffer[65536];
	FileReadStream is(fo, readBuffer, sizeof(readBuffer));
	
	Document doc;
	doc.Parse(readBuffer);
	
	doc.RemoveMember("profiles");
	printf("Removed old profiles\n");
	Value profiles(kObjectType);
	doc.AddMember("profiles", profiles, doc.GetAllocator());


    DIR *dir = opendir(modPacks);

    struct dirent *entry = readdir(dir);
    while (entry != NULL)
    {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".")!=0 && strcmp(entry->d_name, "..")!=0){
            printf("Modpack found:	%s\n", entry->d_name);
			string packPathStart(modPacks);
			string packPathEnd(entry->d_name);
			string packPathStr = packPathStart + "\\" + packPathEnd + "";
			const char* packPath = packPathStr.c_str();
			printf("Modpack path:	%s\n", packPath);
			
			string nameStr = entry->d_name;
			const char* name = nameStr.c_str();
			Value pack(kObjectType);		
			Value packName;
			size_t nameLen = strlen(name);  
			packName = StringRef(name, nameLen);
			pack.AddMember("name", packName, doc.GetAllocator());
			
			Value javaArgs;
			string javaArgsStart(launchArgs1);
			string javaArgsEnd(launchArgs2);
			string javaArgsStr = javaArgsStart + packPath + javaArgsEnd;
			const char* javaArgsChar = javaArgsStr.c_str();
			size_t argsLen = strlen(javaArgsChar);  
			javaArgs = StringRef(javaArgsChar, argsLen);
			pack.AddMember("javaArgs", javaArgs, doc.GetAllocator());
			
			Value type;
			const char* typeChar = "custom";
			size_t typeLen = strlen(typeChar);  
			type = StringRef(typeChar, typeLen);
			pack.AddMember("type", type, doc.GetAllocator());
			printf("%s\n", name);
			doc["profiles"].AddMember(StringRef(name, nameLen), pack, doc.GetAllocator());
		}
        entry = readdir(dir);
    }
	
	FILE* fw = fopen("output.json", "wb"); // non-Windows use "w"
 
	char writeBuffer[65536];
	FileWriteStream os(fw, writeBuffer, sizeof(writeBuffer));
	 
	Writer<FileWriteStream> writer(os);
	doc.Accept(writer);
	 
	fclose(fo);
	
    closedir(dir);
	
	return 0;
	
}