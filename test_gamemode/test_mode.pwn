

#if !defined IsValidVehicle
	native IsValidVehicle(vehicleid);
#endif

#include <a_samp>
#include <Pawn.CMD>
#include <sscanf2>
#include <streamer>
//
#define FOREACH_I_PlayerVehiclesStream	
#include <foreach>
//
#pragma warning disable 239
//
#include <src\invetory\variables.inc>
#include <src\invetory\main.inc>

#include <src\invetory\storage\variables.inc>
#include <src\invetory\storage\main.inc>


new storage_vehicle_boot[MAX_VEHICLES] = {-1,-1,-1,...};//показывает индекс хранилища, если он есть. если он равен -1 - хранилища нет
#define STORAGE_VEHICLE_BOOT	1

enum E_ITEMS
{
	E_ITEMS_HELMET,
	E_ITEMS_CAP,
	E_ITEMS_LANTERN,
	E_ITEMS_BOOT,
	E_ITEMS_NEW_YEARS_HAT,
	E_ITEMS_GLOVE
}
new items[E_ITEMS];


main()
{
	print("\n----------------------------------");
	print("Welcome to the my new mode");
	print("----------------------------------\n");
}
/////////////////////////////////////////////////////////////Модули//////////////////////////////////////////////////
//#include <src\Game_Tag\main.inc>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////







public OnGameModeExit()
{
	
	return 1;
}




public OnPlayerConnect(playerid)
{
	
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	
	return 1;
}

public OnPlayerSpawn(playerid)
{
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
	return 1;
}

public OnVehicleSpawn(vehicleid)
{
	return 1;
}

public OnVehicleDeath(vehicleid, killerid)
{
	return 1;
}

public OnPlayerText(playerid, text[])
{
	
	return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
	
	return 0;
}

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	return 1;
}

public OnPlayerExitVehicle(playerid, vehicleid)
{
	return 1;
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	return 1;
}

public OnPlayerEnterCheckpoint(playerid)
{
	return 1;
}

public OnPlayerLeaveCheckpoint(playerid)
{
	return 1;
}

public OnPlayerEnterRaceCheckpoint(playerid)
{
	return 1;
}

public OnPlayerLeaveRaceCheckpoint(playerid)
{
	return 1;
}

public OnRconCommand(cmd[])
{
	return 1;
}

public OnPlayerRequestSpawn(playerid)
{
	return 1;
}

public OnObjectMoved(objectid)
{
	return 1;
}

public OnPlayerObjectMoved(playerid, objectid)
{
	return 1;
}

public OnPlayerPickUpPickup(playerid, pickupid)
{
	return 1;
}

public OnVehicleMod(playerid, vehicleid, componentid)
{
	return 1;
}

public OnVehiclePaintjob(playerid, vehicleid, paintjobid)
{
	return 1;
}

public OnVehicleRespray(playerid, vehicleid, color1, color2)
{
	return 1;
}

public OnPlayerSelectedMenuRow(playerid, row)
{
	return 1;
}

public OnPlayerExitedMenu(playerid)
{
	return 1;
}

public OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
{
	return 1;
}



public OnRconLoginAttempt(ip[], password[], success)
{
	return 1;
}

public OnPlayerUpdate(playerid)
{
	return 1;
}

public OnPlayerStreamIn(playerid, forplayerid)
{
	return 1;
}

public OnPlayerStreamOut(playerid, forplayerid)
{
	return 1;
}

public OnVehicleStreamIn(vehicleid, forplayerid)
{
	return 1;
}

public OnVehicleStreamOut(vehicleid, forplayerid)
{
	return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	
	return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
	SetTimer("Spawn", 0, 0);
	return 1;
}

forward Spawn();
public Spawn ()
{
	SpawnPlayer(0);
	
	return 1;
}

public OnPlayerClickPlayer(playerid, clickedplayerid, source)
{
	return 1;
}



public OnPlayerClickTextDraw(playerid, Text:clickedid)
{
	
	return 1;
}

public OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
	
	
	return 1;
}



//
CMD:test(playerid)
{
	ShowInvetoryForPlayer(playerid);//
	return true;
}
CMD:test2(playerid, params[])
{
	new model;
	if(sscanf(params, "d", model))
		return false;
	if(400 <= model && model <= 611)
	{
		new Float:x,Float:y,Float:z, Float:angle;
		GetPlayerPos(playerid, x, y, z);
		GetPlayerFacingAngle(playerid, angle);
		new TestVeh = CreateVehicle(model, x+(floatsin(-angle, degrees)*6.0), y+(floatcos(-angle, degrees)*6.0), z, angle, random(255), random(255), -1);//Создаем тачку
		new str[123];
		format(str, sizeof str, "[BootSystem] Veh(%d)_Model(%d)",TestVeh, model);
		storage_vehicle_boot[TestVeh] = CreateStorage(STORAGE_VEHICLE_BOOT,str, TestVeh);//Создаем и прикрепляем к ней багажник
		return true;
	}
	SendClientMessage(playerid, -1, "Не верно введена модель транспортного средства.");
	return true;
}


CMD:giveitem(playerid, params[])
{
	new item,value;
	if(sscanf(params, "dd", item, value))
		return SendClientMessage(playerid, -1, "/giveitem [item] [value]");
	else if(item < 0 || item >= MAX_ITEMS)
		return SendClientMessage(playerid, -1, "Неверный ид предмета!");
	else if(value < 0 || value > 5000)
		return SendClientMessage(playerid, -1, "Слишком большое, либо слишком малое число!");
	else if(GiveItemToPlayer(playerid, item, value) == 0)
		return SendClientMessage(playerid, -1, "Доступных слотов нет");
	return true;
}







stock PlayerTextDrawChangeBGColor(playerid, PlayerText:playertextid, color)
{
	PlayerTextDrawHide(playerid, playertextid);//
	PlayerTextDrawBackgroundColor(playerid, playertextid, color);
	PlayerTextDrawShow(playerid, playertextid);
}






public OnGameModeInit()
{
	
	

	SetGameModeText("My new Mode");
	AddPlayerClass(0, 1154.2834,-1767.0160,16.5938, 0.0, 0, 0, 0, 0, 0, 0);
	//
	




//public OnGameModeInit()
//{

	items[E_ITEMS_HELMET] = CreateItemInvetory("Шлем", 18645, 0.000, 0.000, 0.000, 1.000);
	items[E_ITEMS_CAP] = CreateItemInvetory("Кепка FBI", 18636, 0.000, 0.000, 0.000, 1.000);
	items[E_ITEMS_LANTERN] = CreateItemInvetory("Фонарик", 18641, 0.000, 0.000, 0.000, 1.000);
	items[E_ITEMS_BOOT] = CreateItemInvetory("Сапог", 11735, 0.000, 0.000, 0.000, 1.000);
	items[E_ITEMS_NEW_YEARS_HAT] = CreateItemInvetory("Новогодняя шапка", 19064, 0.000, 0.000, 0.000, 1.000); 
	items[E_ITEMS_GLOVE] = CreateItemInvetory("Перчатка", 19556, 0.000, 0.000, 0.000, 1.000); 
	return 1;
}







public OnPlayerClickInvSlot(&playerid, &index_item)
{
	if(index_item == items[E_ITEMS_HELMET])
	{
		SendClientMessage(playerid, -1, "Вы не можете взаимодействовать со шлемом!");
		return 0;
	}
	return 1;
}



public OnStorageCreated(index, type_storage, name_storage[], subject_id, Float:x, Float:y, Float:z, &interiorid, &virtualworld, &Text3D:TextStorage)
{
	if(type_storage == STORAGE_VEHICLE_BOOT)//Тип - багажник
	{
		if(!IsValidVehicle(subject_id) || storage_vehicle_boot[subject_id] > -1)
			return 0;
		TextStorage = CreateDynamic3DTextLabel("Багажник Нажмите\n ~k~~SNEAK_ABOUT~ для вдаимодействия", -1, Float:x, Float:y, Float:z, 8.0);
		GetVehicleModelInfo(GetVehicleModel(subject_id), VEHICLE_MODEL_INFO_WHEELSREAR, x, y, z);
		AttachDynamic3DTextLabelToVeh(TextStorage, subject_id, x-1.0, y-1.0, z+0.5);
		return 1;
	}
	return 0;
}



stock Float:GetPlayerDistanceToVehBoot(playerid, vehicleid)
{
    new Float:x,Float:y,Float:z, Float:angle,Float:distance;
    GetVehicleModelInfo(GetVehicleModel(vehicleid), 1, x, distance, z);
    distance = distance/2 + 0.1;
    GetVehiclePos(vehicleid, x, y, z);
    GetVehicleZAngle(vehicleid, angle);
    x += (distance * floatsin(-angle+180, degrees));
    y += (distance * floatcos(-angle+180, degrees));
	return GetPlayerDistanceFromPoint(playerid, x, y, z);
}




stock AttachDynamic3DTextLabelToVeh(Text3D:labelid, vehicleid, Float:offsetx, Float:offsety, Float:offsetz)
{
    Streamer_SetIntData(STREAMER_TYPE_3D_TEXT_LABEL, labelid, E_STREAMER_ATTACHED_VEHICLE, vehicleid);
    Streamer_SetFloatData(STREAMER_TYPE_3D_TEXT_LABEL, labelid, E_STREAMER_ATTACH_OFFSET_X, offsetx);
    Streamer_SetFloatData(STREAMER_TYPE_3D_TEXT_LABEL, labelid, E_STREAMER_ATTACH_OFFSET_Y, offsety);
    return Streamer_SetFloatData(STREAMER_TYPE_3D_TEXT_LABEL, labelid, E_STREAMER_ATTACH_OFFSET_Z, offsetz);
}




//

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if(newkeys == 1024)
	{
		foreach(new v : PlayerVehiclesStream[playerid])//Смотрим все машины в зоне стрима
		{
			if(storage_vehicle_boot[v] > -1)
			{
				if(GetPlayerDistanceToVehBoot(playerid, v) < 1.0)
				{
					ShowInvetoryForPlayer(playerid);//
					ShowStorageForPlayer(playerid, storage_vehicle_boot[v]);
					break;
				}
				
			}
		}
	}
	return 1;
}

