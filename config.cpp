class CfgPatches
{
	class Logger
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {
			"DZ_Data",
			"DZ_Scripts"
		};
	};   
};

class CfgMods
{
	class Logger
	{
		dir = "inkihh\Logger";
		picture = "";
		action = "";
		hideName = 0;
		hidePicture = 0;
		name = "Logger";
		credits = "";
		author = "inkihh";
		authorID = "0";
		version = "1";
		extra = 0;
		type = "mod";

		dependencies[] =
		{
			"Game",
			"World",
			"Mission"
		};

		class defs
		{
			class gameScriptModule
			{
				value="";
				files[] =
				{
					"inkihh\Logger\scripts\3_Game"
				};
			};
			class worldScriptModule
			{
				value="";
				files[] =
				{
					"inkihh\Logger\scripts\4_World"
				};
			};

			class missionScriptModule
			{
				value="";
				files[] =
				{
					"inkihh\Logger\scripts\5_Mission"
				};
			};
		};
	};
};
