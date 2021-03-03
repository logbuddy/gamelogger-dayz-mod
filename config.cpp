class CfgPatches
{
	class GameLogger
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
	class GameLogger
	{
		dir = "inkihh\GameLogger";
		picture = "";
		action = "";
		hideName = 0;
		hidePicture = 0;
		name = "GameLogger";
		credits = "";
		author = "inkihh";
		authorID = "0";
		version = "1";
		extra = 0;
		type = "mod";

		dependencies[] =
		{
			"World",
			"Mission"
		};

		class defs
		{
			class worldScriptModule
			{
				value="";
				files[] =
				{
					"inkihh\GameLogger\scripts\4_World"
				};
			};

			class missionScriptModule
			{
				value="";
				files[] =
				{
					"inkihh\GameLogger\scripts\5_Mission"
				};
			};
		};
	};
};
