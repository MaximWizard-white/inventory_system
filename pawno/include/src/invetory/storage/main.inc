forward OnStorageCreated(index, type_storage, name_storage[], subject_id, Float:x, Float:y, Float:z, &interiorid, &virtualworld, &Text3D:TextStorage);
//Callback is called when creating a storage
// interiorid, 
// virtualworld, 
// Text3D:TextStorage
// Can be modified


public OnPlayerClickTextDraw(playerid, Text:clickedid)
{
    if(clickedid == Text:INVALID_TEXT_DRAW)
	{
		if(IsPlayerStorageOpened(playerid))
		    HideStorageForPlayer(playerid);
	}

    #if defined stg_OnPlayerClickTextDraw
        return stg_OnPlayerClickTextDraw(playerid, Text:clickedid);
    #else
        return 1;
    #endif
}



public OnPlayerDisconnect(playerid, reason)
{
	HideStorageForPlayer(playerid);
	#if defined stg_OnPlayerDisconnect
		return stg_OnPlayerDisconnect(playerid, reason);
	#else
		return 1;
	#endif
}


public OnGameModeInit()
{
    storage_global_TD[E_S_BGround] = TextDrawCreate(380.4166, 169.5185, "LD_SPAC:white"); // 
	TextDrawTextSize(storage_global_TD[E_S_BGround], 176.0000+3.6, 182.0000+3.6);
	TextDrawAlignment(storage_global_TD[E_S_BGround], 1);
	TextDrawColor(storage_global_TD[E_S_BGround], 943004927);
	TextDrawBackgroundColor(storage_global_TD[E_S_BGround], 255);
	TextDrawFont(storage_global_TD[E_S_BGround], 4);
	TextDrawSetProportional(storage_global_TD[E_S_BGround], 0);
	TextDrawSetShadow(storage_global_TD[E_S_BGround], 0);

    for(new i = 0; i < MAX_STORAGE_ID; i++)
	{
		for(new j = 0; j < MAX_SLOTS_STORAGE; j++)
		{
			storage_data_cell[i][j][E_Invetory_IndexItem] = -1;
		}
	}

    for(new i = 0; i < MAX_PLAYERS; i++)
	{
		for(new j = 0; j < MAX_CELLS_INVETORY; j++)
		{
			invetory_info[i][j][E_Invetory_IndexItem] = -1;
		}
		
		for(new j = 0; j < MAX_ITEM_IN_PAGE; j++)
		{
			
			storage_PTD[i][j][E_Cell] = 
			storage_PTD[i][j][E_Value] =  PlayerText:INVALID_TEXT_DRAW;

			
		}
		
	}//SET NULL

    #if defined stg_OnGameModeInit
        return stg_OnGameModeInit();
    #else
        return 1;
    #endif
}





public OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
    if(IsPlayerStorageOpened(playerid))//If the storage is open
	{
		for(new i = 0; i < MAX_SLOTS_STORAGE; i++)
		{
			if(storage_PTD[playerid][i][E_Cell] == PlayerText:INVALID_TEXT_DRAW)
				break;
			if(storage_PTD[playerid][i][E_Cell] == playertextid)
			{
				new old_cell = GetPVarInt(playerid, "old_cell");
				new clickstorage = GetPVarInt(playerid, "ClickStorage");
				new storage_index = GetPVarInt(playerid, "IndexStorage");
				if(old_cell < 0 && storage_data_cell[storage_index][i][E_Invetory_IndexItem] < 0)//
					return true;
				
				else if(!clickstorage && old_cell >= 0)//If you selected something in the inventory and clicked on the slot in the storage
				{
					//Swap
					return SwapInvetoryToStorage(playerid, storage_index , old_cell, i);
				}
				
				//
				if(old_cell < 0)
				{
					PlayerTextDrawChangeBGColor(playerid, playertextid, 0xb07d2bFF);
					SetPVarInt(playerid, "ClickStorage", 1);//We put 1, because it is not the main inventory.
					SetPVarInt(playerid, "old_cell", i);
				}
				else if(old_cell == i)
				{
					PlayerTextDrawChangeBGColor(playerid, playertextid, 0x7F7F7FFF);
					DeletePVar(playerid, "ClickStorage");//We put 0, since the exact same slot has already been selected
					SetPVarInt(playerid, "old_cell", -1);
				}
				else
				{
					return SwapSlotStorage(playerid,storage_index , i, old_cell);
				}
				break;
			}
		}
	}
    #if defined stg_OnPlayerClickPlayerTextDraw
        return stg_OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid);
    #else
        return 1;
    #endif
}








public OnPlayerConnect(playerid)
{
    SetPVarInt(playerid, "IndexStorage", -1);
    #if defined stg_OnPlayerConnect
        return stg_OnPlayerConnect(playerid);
    #else
        return 1;
    #endif
}



stock ShowStorageForPlayer(playerid, index_storage)
{
	if(IsPlayerStorageOpened(playerid))
		return false;
	SetPVarInt(playerid, "IndexStorage", index_storage);
	TextDrawShowForPlayer(playerid, storage_global_TD[E_S_BGround]);
	for(new i = 0; i < MAX_SLOTS_STORAGE; i++)
	{
		new index_model = storage_data_cell[index_storage][i][E_Invetory_IndexItem];
		//
		storage_PTD[playerid][i][E_Cell] = CreatePlayerTextDraw(playerid, 383.7500+((i % 5)*35.0), 171.5924+((i / 5)*34.0), "_"); // 
		PlayerTextDrawTextSize(playerid, storage_PTD[playerid][i][E_Cell], 34.0000, 33.0000);
		PlayerTextDrawAlignment(playerid, storage_PTD[playerid][i][E_Cell], 1);
		PlayerTextDrawColor(playerid, storage_PTD[playerid][i][E_Cell], 0x7F7F7FFF);
		PlayerTextDrawBackgroundColor(playerid, storage_PTD[playerid][i][E_Cell], 0x7F7F7FFF);
		PlayerTextDrawFont(playerid, storage_PTD[playerid][i][E_Cell], 5);
		PlayerTextDrawSetProportional(playerid, storage_PTD[playerid][i][E_Cell], 0);
		PlayerTextDrawSetShadow(playerid, storage_PTD[playerid][i][E_Cell], 0);
		PlayerTextDrawSetSelectable(playerid, storage_PTD[playerid][i][E_Cell], true);

		if(index_model >= 0)
		{
			new model_id = items_invetory[index_model][Item_Model];
			new Float:x = items_invetory[index_model][Item_RotX];
			new Float:y = items_invetory[index_model][Item_RotY];
			new Float:z = items_invetory[index_model][Item_RotZ];
			new Float:s = items_invetory[index_model][Item_Scale];
			PlayerTextDrawSetPreviewModel(playerid, storage_PTD[playerid][i][E_Cell], model_id );//
			PlayerTextDrawSetPreviewRot(playerid, storage_PTD[playerid][i][E_Cell], x, y, z, s);//
			
		}
		else
		{
			PlayerTextDrawSetPreviewModel(playerid, storage_PTD[playerid][i][E_Cell], 1649);//
			PlayerTextDrawSetPreviewRot(playerid, storage_PTD[playerid][i][E_Cell], 0.0000, 0.0000, 0.0000, 0.0000);
		}
		PlayerTextDrawShow(playerid, storage_PTD[playerid][i][E_Cell]);
	}
	CreateValueSlotStorageAll(playerid, index_storage);
	if(storage_name[playerid] == PlayerText:INVALID_TEXT_DRAW)
	{
		storage_name[playerid] = CreatePlayerTextDraw(playerid, 488.3331, 151.1109, storage_info[index_storage][Storage_Name]); // 
		PlayerTextDrawLetterSize(playerid, storage_name[playerid], 0.4000, 1.6000);
		PlayerTextDrawAlignment(playerid, storage_name[playerid], 2);
		PlayerTextDrawColor(playerid, storage_name[playerid], -1);
		PlayerTextDrawSetOutline(playerid, storage_name[playerid], 1);
		PlayerTextDrawBackgroundColor(playerid, storage_name[playerid], 255);
		PlayerTextDrawFont(playerid, storage_name[playerid], 1);
		PlayerTextDrawSetProportional(playerid, storage_name[playerid], 1);
		PlayerTextDrawSetShadow(playerid, storage_name[playerid], 0);
		PlayerTextDrawShow(playerid, storage_name[playerid]);
	}
	SelectTextDraw(playerid, 0xFF0000FF);
	return 1;
}




stock SwapInvetoryToStorage(playerid, index_storage, cell_inventory, cell_storage)
{
	//
	PlayerTextDrawHide(playerid, storage_PTD[playerid][cell_storage][E_Cell]  );
	PlayerTextDrawBackgroundColor(playerid, storage_PTD[playerid][cell_storage][E_Cell], 0x7F7F7FFF);

	new tmp = invetory_info[playerid][cell_inventory][E_Invetory_IndexItem];
	invetory_info[playerid][cell_inventory][E_Invetory_IndexItem] = storage_data_cell[index_storage][cell_storage][E_Invetory_IndexItem];
	storage_data_cell[index_storage][cell_storage][E_Invetory_IndexItem] = tmp;


	tmp = invetory_info[playerid][cell_inventory][E_Invetory_Value];
	invetory_info[playerid][cell_inventory][E_Invetory_Value] = storage_data_cell[index_storage][cell_storage][E_Invetory_Value];
	storage_data_cell[index_storage][cell_storage][E_Invetory_Value] = tmp;



	new index_model = storage_data_cell[index_storage][cell_storage][E_Invetory_IndexItem];

	if(index_model >= 0)//If there is a model
	{
		PlayerTextDrawSetPreviewModel(playerid, storage_PTD[playerid][cell_storage][E_Cell], items_invetory[index_model][Item_Model]);
		PlayerTextDrawSetPreviewRot(playerid, storage_PTD[playerid][cell_storage][E_Cell], items_invetory[index_model][Item_RotX], items_invetory[index_model][Item_RotY], items_invetory[index_model][Item_RotZ], items_invetory[index_model][Item_Scale]);
		ChangeValueSlotStorage(playerid,index_storage , cell_storage);
	}
	else//If not, default
	{
		
		if(storage_PTD[playerid][cell_storage][E_Value] != PlayerText:INVALID_TEXT_DRAW)
			PlayerTextDrawSetString(playerid, storage_PTD[playerid][cell_storage][E_Value], "_");
		PlayerTextDrawSetPreviewModel(playerid, storage_PTD[playerid][cell_storage][E_Cell], 1649);// 1649
		PlayerTextDrawSetPreviewRot(playerid, storage_PTD[playerid][cell_storage][E_Cell], 0.0000, 0.0000, 0.0000, 0.0000);
	}
	PlayerTextDrawShow(playerid, storage_PTD[playerid][cell_storage][E_Cell]);



	new index_old_TD = cell_inventory-(MAX_ITEM_IN_PAGE*page_invetory[playerid])+MAX_ITEM_IN_PAGE;

	if(index_old_TD < MAX_ITEM_IN_PAGE && index_old_TD >= 0)//If it is on our page, then we update the second slot
	{
		PlayerTextDrawHide(playerid, invetory_PTD[playerid][index_old_TD][E_Cell]);
		index_model = invetory_info[playerid][cell_inventory][E_Invetory_IndexItem];
		if(index_model >= 0)
		{
			new model_id = items_invetory[index_model][Item_Model];
			PlayerTextDrawSetPreviewModel(playerid, invetory_PTD[playerid][index_old_TD][E_Cell], model_id);
			PlayerTextDrawSetPreviewRot(playerid, invetory_PTD[playerid][index_old_TD][E_Cell], items_invetory[index_model][Item_RotX], items_invetory[index_model][Item_RotY], items_invetory[index_model][Item_RotZ], items_invetory[index_model][Item_Scale]);
			ChangeValueSlotInvetory(playerid, index_old_TD, cell_inventory);
		}
		else
		{
			if(invetory_PTD[playerid][index_old_TD][E_Value] != PlayerText:INVALID_TEXT_DRAW)
				PlayerTextDrawSetString(playerid, invetory_PTD[playerid][index_old_TD][E_Value], "_");
			PlayerTextDrawSetPreviewModel(playerid, invetory_PTD[playerid][index_old_TD][E_Cell], 1649);// 
			PlayerTextDrawSetPreviewRot(playerid, invetory_PTD[playerid][index_old_TD][E_Cell], 0.0000, 0.0000, 0.0000, 0.0000);//
		}
		
		PlayerTextDrawBackgroundColor(playerid, invetory_PTD[playerid][index_old_TD][E_Cell], 0x7F7F7FFF);
		
		PlayerTextDrawShow(playerid, invetory_PTD[playerid][index_old_TD][E_Cell]);
	}
	SetPVarInt(playerid, "old_cell", -1);
	SetPVarInt(playerid, "ClickStorage", 0);
	return 1;
}


stock ChangeValueSlotStorage(playerid, index_storage, i)
{
	if(storage_data_cell[index_storage][i][E_Invetory_Value] > 0)
	{
		new str[8];
		format(str, sizeof str, "%d", storage_data_cell[index_storage][i][E_Invetory_Value]);
		PlayerTextDrawSetString(playerid, storage_PTD[playerid][i][E_Value] , str);
		PlayerTextDrawShow(playerid, storage_PTD[playerid][i][E_Value]);
	}
	return true;
}







stock SwapSlotStorage(playerid, index_storage, cell, old_cell)
{

	new tmp = storage_data_cell[index_storage][old_cell][E_Invetory_IndexItem];
	storage_data_cell[index_storage][old_cell][E_Invetory_IndexItem] = storage_data_cell[index_storage][cell][E_Invetory_IndexItem];
	storage_data_cell[index_storage][cell][E_Invetory_IndexItem] = tmp;

	tmp = storage_data_cell[index_storage][old_cell][E_Invetory_Value];
	storage_data_cell[index_storage][old_cell][E_Invetory_Value] = storage_data_cell[index_storage][cell][E_Invetory_Value];
	storage_data_cell[index_storage][cell][E_Invetory_Value] = tmp;
	




	new index_model = storage_data_cell[index_storage][old_cell][E_Invetory_IndexItem];
	
	if(index_model >= 0)
	{
		new model_id = items_invetory[index_model][Item_Model];
		PlayerTextDrawSetPreviewModel(playerid, storage_PTD[playerid][old_cell][E_Cell], model_id);
		PlayerTextDrawSetPreviewRot(playerid, storage_PTD[playerid][old_cell][E_Cell], items_invetory[index_model][Item_RotX], items_invetory[index_model][Item_RotY], items_invetory[index_model][Item_RotZ], items_invetory[index_model][Item_Scale]);
		ChangeValueSlotStorage(playerid ,index_storage , old_cell);
	}
	else
	{
		if(storage_PTD[playerid][old_cell][E_Cell] != PlayerText:INVALID_TEXT_DRAW)
			PlayerTextDrawSetString(playerid, storage_PTD[playerid][old_cell][E_Value], "_");
		PlayerTextDrawSetPreviewModel(playerid, storage_PTD[playerid][old_cell][E_Cell], 1649);// 1649
		PlayerTextDrawSetPreviewRot(playerid, storage_PTD[playerid][old_cell][E_Cell], 0.0000, 0.0000, 0.0000, 0.0000);
	}


	index_model = storage_data_cell[index_storage][cell][E_Invetory_IndexItem];
	if(index_model >= 0)
	{
		new model_id = items_invetory[index_model][Item_Model]; 
		PlayerTextDrawSetPreviewModel(playerid, storage_PTD[playerid][cell][E_Cell], model_id);
		PlayerTextDrawSetPreviewRot(playerid, storage_PTD[playerid][cell][E_Cell], items_invetory[index_model][Item_RotX], items_invetory[index_model][Item_RotY], items_invetory[index_model][Item_RotZ], items_invetory[index_model][Item_Scale]);
		ChangeValueSlotStorage(playerid, index_storage, cell);
	}
	else
	{
		if(storage_PTD[playerid][cell][E_Value] != PlayerText:INVALID_TEXT_DRAW)
			PlayerTextDrawSetString(playerid, storage_PTD[playerid][cell][E_Value], "_");
		PlayerTextDrawSetPreviewModel(playerid, storage_PTD[playerid][cell][E_Cell], 1649);// 1649
		PlayerTextDrawSetPreviewRot(playerid, storage_PTD[playerid][cell][E_Cell], 0.0000, 0.0000, 0.0000, 0.0000);
	}
	PlayerTextDrawChangeBGColor(playerid, storage_PTD[playerid][cell][E_Cell], 0x7F7F7FFF);
	PlayerTextDrawChangeBGColor(playerid, storage_PTD[playerid][old_cell][E_Cell], 0x7F7F7FFF);
	SetPVarInt(playerid, "ClickStorage", 0);
	SetPVarInt(playerid, "old_cell", -1);
	return 1;
}




stock CreateStorage(type_storage = 0, name_storage[], subject_id = -1, Float:x = INVALID_COORDS_STORAGE, Float:y = INVALID_COORDS_STORAGE, Float:z = INVALID_COORDS_STORAGE, interiorid = 0, virtualworld = 0, Text3D:TextStorage = Text3D:INVALID_3DTEXT_ID)
{
	

	if(total_storage < 0 || total_storage >= MAX_STORAGE_ID)
		return print("The storage was not created. Reason: Overflow of slots.");	
	else if(!OnStorageCreated(total_storage, type_storage, name_storage, subject_id, Float:x, Float:y, Float:z, interiorid, virtualworld, Text3D:TextStorage))
		return print("The storage was not created. Reason: Return 0 in the callback  handler.");

	
	
	
	storage_info[total_storage][Storage_ID_Subject] = subject_id;
	storage_info[total_storage][Storage_Type] = type_storage;
	storage_info[total_storage][Storage_Coords_X] = x;
	storage_info[total_storage][Storage_Coords_Y] = y;
	storage_info[total_storage][Storage_Coords_Z] = z;
	storage_info[total_storage][Storage_3D_Text] = TextStorage;
	storage_info[total_storage][Storage_VirtualWorld] = virtualworld;
	storage_info[total_storage][Storage_Interior] = interiorid;
	strcat(storage_info[total_storage][Storage_Name], name_storage);
	
	printf("Storage %d was successfully created.", total_storage);
	total_storage++;
	return total_storage-1;
}
stock CreateValueSlotStorageAll(playerid, index_storage)
{
	
	//
	for(new i = 0; i < MAX_SLOTS_STORAGE; i++)
	{
		if(storage_PTD[playerid][i][E_Value] == PlayerText:INVALID_TEXT_DRAW)
		{
			storage_PTD[playerid][i][E_Value] = CreatePlayerTextDraw(playerid, 416.2166+((i % 5)*35.0), 195.2666+((i / 5)*34.0), "_"); // пусто
			PlayerTextDrawLetterSize(playerid, storage_PTD[playerid][i][E_Value], 0.1283, 0.9362);
			PlayerTextDrawAlignment(playerid, storage_PTD[playerid][i][E_Value], 3);
			PlayerTextDrawColor(playerid, storage_PTD[playerid][i][E_Value], -1);
			PlayerTextDrawBackgroundColor(playerid, storage_PTD[playerid][i][E_Value], 255);
			PlayerTextDrawFont(playerid, storage_PTD[playerid][i][E_Value], 2);
			PlayerTextDrawSetProportional(playerid, storage_PTD[playerid][i][E_Value], 1);
			PlayerTextDrawSetShadow(playerid, storage_PTD[playerid][i][E_Value], 0);
			ChangeValueSlotStorage(playerid,index_storage, i);
		}
	}
	return true;
}





stock IsPlayerStorageOpened(playerid)
	return GetPVarInt(playerid, "IndexStorage") > -1 ? true : false;







stock HideStorageForPlayer(playerid)
{
	if(IsPlayerStorageOpened(playerid))
	{
		SetPVarInt(playerid, "IndexStorage", -1);
		TextDrawHideForPlayer(playerid, storage_global_TD[E_S_BGround]);
		for(new i = 0; i < MAX_SLOTS_STORAGE; i++)
		{
			//
			if(storage_PTD[playerid][i][E_Value] != PlayerText:INVALID_TEXT_DRAW)
			{
				PlayerTextDrawDestroy(playerid, storage_PTD[playerid][i][E_Value]);
				storage_PTD[playerid][i][E_Value] = PlayerText:INVALID_TEXT_DRAW;
			}
			if(storage_PTD[playerid][i][E_Cell] != PlayerText:INVALID_TEXT_DRAW)
			{
				PlayerTextDrawDestroy(playerid, storage_PTD[playerid][i][E_Cell]);
				storage_PTD[playerid][i][E_Cell] = PlayerText:INVALID_TEXT_DRAW;
			}
		}
		if(storage_name[playerid] != PlayerText:INVALID_TEXT_DRAW)
		{
			PlayerTextDrawDestroy(playerid, storage_name[playerid]);
			storage_name[playerid] = PlayerText:INVALID_TEXT_DRAW;
		}
		CancelSelectTextDraw(playerid);
	}
	return 1;
}








///


#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif

#define OnGameModeInit stg_OnGameModeInit
#if defined stg_OnGameModeInit
    forward stg_OnGameModeInit();
#endif





#if defined _ALS_OnPlayerClickPlayerTD
    #undef OnPlayerClickPlayerTextDraw
#else
    #define _ALS_OnPlayerClickPlayerTD
#endif

#define OnPlayerClickPlayerTextDraw stg_OnPlayerClickPlayerTextDraw
#if defined stg_OnPlayerClickPlayerTextDraw
    forward stg_OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid);
#endif





#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect stg_OnPlayerConnect
#if defined stg_OnPlayerConnect
    forward stg_OnPlayerConnect(playerid);
#endif



#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif

#define OnPlayerDisconnect stg_OnPlayerDisconnect
#if defined stg_OnPlayerDisconnect
	forward stg_OnPlayerDisconnect(playerid, reason);
#endif







#if defined _ALS_OnPlayerClickTextDraw
    #undef OnPlayerClickTextDraw
#else
    #define _ALS_OnPlayerClickTextDraw
#endif

#define OnPlayerClickTextDraw stg_OnPlayerClickTextDraw
#if defined stg_OnPlayerClickTextDraw
    forward stg_OnPlayerClickTextDraw(playerid, Text:clickedid);
#endif





public OnStorageCreated(index, type_storage, name_storage[], subject_id, Float:x, Float:y, Float:z, &interiorid, &virtualworld, &Text3D:TextStorage)
{
	
	#if defined stg_OnStorageCreated
		return stg_OnStorageCreated(index, type_storage, name_storage, subject_id, Float:x, Float:y, Float:z, interiorid, virtualworld, TextStorage);
	#else
		return 1;
	#endif
}






#if defined _ALS_OnStorageCreated
	#undef OnStorageCreated
#else
	#define _ALS_OnStorageCreated
#endif

#define OnStorageCreated stg_OnStorageCreated
#if defined stg_OnStorageCreated
	forward stg_OnStorageCreated(index, type_storage, name_storage[], subject_id, Float:x, Float:y, Float:z, &interiorid, &virtualworld, &Text3D:TextStorage);
#endif