const path = require('path');
const fs = require('fs');
const { exec } = require("child_process");

var launchCommand = "C:\\Users\\Will\\Twitch\\Minecraft\\Install\\minecraft.exe --workDir \"C:\\Users\\Will\\Twitch\\Minecraft\\Install\""

var launchProfiles = "C:\\Users\\Will\\Twitch\\Minecraft\\Install\\launcher_profiles.json";
var modPacks = "C:\\Users\\Will\\Twitch\\Minecraft\\Instances";
var launchArgs1 = "-Xmx8192m -Xms256m -XX:PermSize=256m -Dminecraft.applet.TargetDirectory=\""; //add path to instance folder onto end then do launchArgs1 + path + launchArgs2
var launchArgs2 = "\" -Dfml.ignorePatchDiscrepancies=true -Dfml.ignoreInvalidMinecraftCertificates=true -Duser.language=en -Duser.country=US -d64 -server -XX:+AggressiveOpts -XX:ParallelGCThreads=8 -XX:+UseConcMarkSweepGC -XX:+UnlockExperimentalVMOptions -XX:+UseParNewGC -XX:+ExplicitGCInvokesConcurrent -XX:MaxGCPauseMillis=10 -XX:GCPauseIntervalMillis=50 -XX:+UseFastAccessorMethods -XX:+OptimizeStringConcat -XX:NewSize=84m -XX:+UseAdaptiveGCBoundary -XX:NewRatio=3 -Dfml.readTimeout=90 -Ddeployment.trace=true -Ddeployment.log=true -Ddeployment.trace.level=all";

const editJsonFile = require("edit-json-file");
 
let profilesFile = editJsonFile(launchProfiles);

//console.log(profilesFile);

function getDirectories(path) {
  return fs.readdirSync(path).filter(function (file) {
    return fs.statSync(path+'/'+file).isDirectory();
  });
}

var packs = getDirectories(modPacks);

//console.log(packs);

profilesFile.unset("profiles");

for (var i = 0; i < packs.length; i++){
	
	
	var name = packs[i];
	var folderPath = modPacks+"\\"+name+"\\";
	var launchArgs = launchArgs1 + folderPath + launchArgs2;
	
	let manifestFile = editJsonFile(folderPath+"manifest.json");
	var forgeVer = manifestFile.get("minecraft.modLoaders")[0].id;
	
	profilesFile.set("profiles."+name, {
		name:name,
		type:"custom",
		gameDir:folderPath,
		javaArgs : launchArgs,
		lastVersionId:forgeVer,
		resolution : {
			height : 480,
			width: 854
		}
	});
}

//console.log(profilesFile.toObject());
profilesFile.save();

exec(launchCommand, (error, stdout, stderr) => {
    if (error) {
        console.log(`error: ${error.message}`);
        return;
    }
    if (stderr) {
        console.log(`stderr: ${stderr}`);
        return;
    }
    console.log(`stdout: ${stdout}`);
});