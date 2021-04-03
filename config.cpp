class CfgPatches
{
	class Logbuddy
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
		dir = "inkihh\Logbuddy";
		picture = "";
		action = "";
		hideName = 0;
		hidePicture = 0;
		name = "Logbuddy";
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
					"inkihh\Logbuddy\scripts\3_Game"
				};
			};
			class worldScriptModule
			{
				value="";
				files[] =
				{
					"inkihh\Logbuddy\scripts\4_World"
				};
			};

			class missionScriptModule
			{
				value="";
				files[] =
				{
					"inkihh\Logbuddy\scripts\5_Mission"
				};
			};
		};
	};
};
