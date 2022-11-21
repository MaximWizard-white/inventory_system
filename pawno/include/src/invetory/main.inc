
forward OnPlayerClickInvSlot(&playerid, &index_item);	//Auto-callable function, when clicking on the slot

//

#define CHAIN_FORWARD:%0_%2(%1)=%3; \
	forward %0_%2(%1); \
	public %0_%2(%1) <_ALS : _ALS_x0, _ALS : _ALS_x1> { return (%3); } \
	public %0_%2(%1) <> { return (%3); }
	
#define CHAIN_PUBLIC:%0(%1) %0(%1) <_ALS : _ALS_go>




//



public OnPlayerConnect(playerid)
{

    
    ///
    #if defined inv_OnPlayerConnect
        return inv_OnPlayerConnect(playerid);
    #else
        return 1;
    #endif
}





public OnPlayerClickTextDraw(playerid, Text:clickedid)
{
    if(clickedid == Text:INVALID_TEXT_DRAW)
	{
		if(IsPlayerInvetoryOpened(playerid))
			HideInvetoryForPlayer(playerid);
	}

    if(IsPlayerInvetoryOpened(playerid))
    {
        if(invetory_global_TD[E_NextButton] == clickedid ) //
            return Nextpage_invetory(playerid);

        else if(invetory_global_TD[E_BackButton] == clickedid)//
            return Downpage_invetory(playerid);
        
        else if(invetory_global_TD[E_AddButton] == clickedid)//
            return AddSlotInvetory(playerid);

        else if(invetory_global_TD[E_DeleteButton] == clickedid)//
            return DeleteSlotInvetory(playerid);
        //
    }
    #if defined inv_OnPlayerClickTextDraw
        return inv_OnPlayerClickTextDraw(playerid, Text:clickedid);
    #else
        return 1;
    #endif
}








public OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
    if(IsPlayerInvetoryOpened(playerid))//If you open the inventory
	{
		for(new i = 0; i < MAX_ITEM_IN_PAGE; i++)
		{
			if(invetory_PTD[playerid][i][E_Cell] == PlayerText:INVALID_TEXT_DRAW)
				break;//e are completing the loop since the slot has not been created and the creation is linear

			if(invetory_PTD[playerid][i][E_Cell] == playertextid)
			{
				if(page_invetory[playerid] < 0)
					page_invetory[playerid] = 1;

				new now_cell = i+(MAX_ITEM_IN_PAGE*(page_invetory[playerid]-1));//Current cell
				new old_cell = GetPVarInt(playerid, "old_cell");//The previous cell that was already involved
				new index = invetory_info[playerid][now_cell][E_Invetory_IndexItem];
				if(index >= 0)
				{
					if(!OnPlayerClickInvSlot(playerid, index))
						return 0;
				}
				//
				#if defined STORAGE_System_Added//If the storage system is connected
					if(GetPVarInt(playerid, "ClickStorage") && old_cell >= 0)//If something is already selected in the repository
					{
						//Making a vault swap to inventory
						return SwapInvetoryToStorage(playerid,GetPVarInt(playerid, "IndexStorage"), now_cell, old_cell);
					}
				#endif
				//
				if(now_cell >= 0 && now_cell < MAX_CELLS_INVETORY && old_cell < 0)//If the cell is empty
				{
					if(invetory_info[playerid][now_cell][E_Invetory_IndexItem] < 0)
						return false;
				}
				
				if(old_cell >= 0)//If you have already chosen a slot
				{
					if(old_cell == now_cell)//And it coincides with the same one - we reset it
					{
						PlayerTextDrawChangeBGColor(playerid, invetory_PTD[playerid][i][E_Cell], 0x7F7F7FFF);
						SetPVarInt(playerid, "old_cell",-1);
						return false;
					}
					//If it does not match, we swap the values
					SwapSlotInvetory(playerid, old_cell, now_cell, i);
				}
				else
				{
					SetPVarInt(playerid, "old_cell",now_cell);
					PlayerTextDrawBackgroundColor(playerid, invetory_PTD[playerid][i][E_Cell], 0xb07d2bFF);
					PlayerTextDrawShow(playerid, invetory_PTD[playerid][i][E_Cell]);
				}
				break;
			}
		}
	}
    #if defined inv_OnPlayerClickPlayerTextDraw
        return inv_OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid);
    #else
        return 1;
    #endif
}








public OnGameModeInit()
{
    for(new i = 0; i < MAX_PLAYERS; i++)
	{
		for(new j = 0; j < MAX_CELLS_INVETORY; j++)
		{
			invetory_info[i][j][E_Invetory_IndexItem] = -1;
		}
		
		for(new j = 0; j < MAX_ITEM_IN_PAGE; j++)
		{
			invetory_PTD[i][j][E_Cell] = 
			invetory_PTD[i][j][E_Value] = PlayerText:INVALID_TEXT_DRAW;
		}
		
	}//SET NULL
    Invetory_CreateGlobalTextDraw();//Create TD
    #if defined inv_OnGameModeInit
        return inv_OnGameModeInit();
    #else
        return 1;
    #endif
}





public OnPlayerDisconnect(playerid, reason)
{
	HideInvetoryForPlayer(playerid);
	#if defined inv_OnPlayerDisconnect
		return inv_OnPlayerDisconnect(playerid, reason);
	#else
		return 1;
	#endif
}





stock ShowInvetoryForPlayer(playerid)
{
	if(IsPlayerInvetoryOpened(playerid))
		return false;
	if(page_invetory[playerid] < 1)
	{
		CreateSlotInvetoryAll(playerid);//Creating cells
		CreateValueSlotInvetoryAll(playerid);//Creating values for the cells are empty
	}
	SetPVarInt(playerid, "old_cell", -1);
	page_invetory[playerid] = 1;

	for(new E_GlobalInvetoryTD:i; i < E_GlobalInvetoryTD; i++)
		TextDrawShowForPlayer(playerid, invetory_global_TD[i]);
	
	invetory_pages[playerid] = CreatePlayerTextDraw(playerid, 282.5667, 334.3297, "_"); // pages
	PlayerTextDrawLetterSize(playerid, invetory_pages[playerid], 0.1975, 1.0296);
	PlayerTextDrawAlignment(playerid, invetory_pages[playerid], 2);
	PlayerTextDrawColor(playerid, invetory_pages[playerid], -1);
	PlayerTextDrawSetOutline(playerid, invetory_pages[playerid], 1);
	PlayerTextDrawBackgroundColor(playerid, invetory_pages[playerid], 255);
	PlayerTextDrawFont(playerid, invetory_pages[playerid], 1);
	PlayerTextDrawSetProportional(playerid, invetory_pages[playerid], 1);
	PlayerTextDrawSetShadow(playerid, invetory_pages[playerid], 0);
	PlayerTextDrawShow(playerid, invetory_pages[playerid]);
	ShowInventoryItems(playerid);
	SelectTextDraw(playerid, 0xFF0000FF);
	return 1;
}




stock CreateItemInvetory(const NameItem[] = "", ModelItem = -1, Float:Rot_X = 0.000, Float:Rot_Y = 0.000, Float:Rot_Z = 0.000, Float:Zoom = 0.000)
{
	if(total_invetory_items >= MAX_ITEMS || total_invetory_items < 0)
		return print("[Error Invetory #001] The item was not created.");
	format(items_invetory[total_invetory_items][Item_Name], MAX_STRING_ITEM, "%s", NameItem);
	items_invetory[total_invetory_items][Item_Model] = ModelItem;
	items_invetory[total_invetory_items][Item_RotX] = Rot_X;
	items_invetory[total_invetory_items][Item_RotY] = Rot_Y;
	items_invetory[total_invetory_items][Item_RotZ] = Rot_Z;
	items_invetory[total_invetory_items][Item_Scale] = Zoom;
	total_invetory_items++;
	printf("[Invetory] Item %s was added successfully.",NameItem);
	return total_invetory_items-1;
}






stock ShowInventoryItems(playerid)
{
	

	for(new i = 0; i < MAX_ITEM_IN_PAGE; i++)//We clean all the cells if they have not been cleaned yet
	{	
		
		if(invetory_PTD[playerid][i][E_Cell] != PlayerText:INVALID_TEXT_DRAW)
		{
			PlayerTextDrawHide(playerid, invetory_PTD[playerid][i][E_Cell]);
			PlayerTextDrawSetPreviewModel(playerid, invetory_PTD[playerid][i][E_Cell], 1649);//
			PlayerTextDrawSetPreviewRot(playerid, invetory_PTD[playerid][i][E_Cell], 0.0000, 0.0000, 0.0000, 0.0000);//
			PlayerTextDrawBackgroundColor(playerid, invetory_PTD[playerid][i][E_Cell], 0x7F7F7FFF);
		}

		if(invetory_PTD[playerid][i][E_Value] != PlayerText:INVALID_TEXT_DRAW)
			PlayerTextDrawSetString(playerid, invetory_PTD[playerid][i][E_Value], "_"),
			PlayerTextDrawHide(playerid, invetory_PTD[playerid][i][E_Cell]);
	}

	new lastindex = GetLastIndexTD(playerid);
	if(lastindex == -1)
		return false;

	new cell = (page_invetory[playerid]-1)*MAX_ITEM_IN_PAGE;
	if(cell < 0)
		cell = 0;

	new model_id = -1;
	
	for(new i = 0; i < lastindex; i++)
	{	
		new index_model = invetory_info[playerid][cell][E_Invetory_IndexItem];//Calculating the array index
		if(index_model >= 0 && index_model < MAX_ITEMS)
		{
			model_id = items_invetory[index_model][Item_Model];
			new Float:x = items_invetory[index_model][Item_RotX];
			new Float:y = items_invetory[index_model][Item_RotY];
			new Float:z = items_invetory[index_model][Item_RotZ];
			new Float:s = items_invetory[index_model][Item_Scale];
			PlayerTextDrawSetPreviewModel(playerid, invetory_PTD[playerid][i][E_Cell], model_id );//
			PlayerTextDrawSetPreviewRot(playerid, invetory_PTD[playerid][i][E_Cell], x, y, z, s);//
		}
		if(GetPVarInt(playerid, "old_cell") == cell)
        {
            #if defined STORAGE_System_Added//If the storage system is included
                if(!GetPVarInt(playerid, "ClickStorage"))
                    PlayerTextDrawBackgroundColor(playerid, invetory_PTD[playerid][i][E_Cell], 0xb07d2bFF);
            #else
                PlayerTextDrawBackgroundColor(playerid, invetory_PTD[playerid][i][E_Cell], 0xb07d2bFF);
            #endif
           
        }
		//
		
		if(invetory_info[playerid][cell][E_Invetory_Value] > 0)
			ChangeValueSlotInvetory(playerid, i, cell);

		PlayerTextDrawShow(playerid, invetory_PTD[playerid][i][E_Cell]);

		cell++;
	}
	Updatepage_invetory(playerid);
	return 1;
}


stock ChangeValueSlotInvetory(playerid, i, cell)
{
	if(invetory_info[playerid][cell][E_Invetory_Value] > 0)
	{
		new str[8];
		format(str, sizeof str, "%d", invetory_info[playerid][cell][E_Invetory_Value]);
		PlayerTextDrawSetString(playerid, invetory_PTD[playerid][i][E_Value], str);
		PlayerTextDrawShow(playerid, invetory_PTD[playerid][i][E_Value]);
	}
	return true;
}


stock CreateValueSlotInvetoryAll(playerid)
{
	for(new i = 0; i < MAX_ITEM_IN_PAGE; i++)
	{
		invetory_PTD[playerid][i][E_Value] = CreatePlayerTextDraw(playerid, 193.2166+((i % 5)*35.0), 195.2666+((i / 5)*34.0), "_"); // 
		PlayerTextDrawLetterSize(playerid, invetory_PTD[playerid][i][E_Value], 0.1283, 0.9362);
		PlayerTextDrawAlignment(playerid, invetory_PTD[playerid][i][E_Value], 3);
		PlayerTextDrawFont(playerid, invetory_PTD[playerid][i][E_Value], 2);
		PlayerTextDrawColor(playerid, invetory_PTD[playerid][i][E_Value], -1);
		PlayerTextDrawSetProportional(playerid, invetory_PTD[playerid][i][E_Value], 1);
		PlayerTextDrawSetShadow(playerid, invetory_PTD[playerid][i][E_Value], 0);
	}
	
	return true;
}


stock GetLastIndexTD(playerid)
{
	new TD_index_item = -1;

	if(MAX_ITEM_IN_PAGE >= total_indexes_inv[playerid])
		TD_index_item = total_indexes_inv[playerid];

	else if((MAX_ITEM_IN_PAGE*page_invetory[playerid]) > total_indexes_inv[playerid])
		TD_index_item = MAX_ITEM_IN_PAGE-(MAX_ITEM_IN_PAGE*page_invetory[playerid]-(total_indexes_inv[playerid]));

	else 
		TD_index_item = MAX_ITEM_IN_PAGE;
	return TD_index_item;
}






stock Nextpage_invetory(playerid)
{
	new getnumpages = GetNumberOfPages(playerid);
	page_invetory[playerid]++;
	if(page_invetory[playerid] > getnumpages)
	{
		page_invetory[playerid] = getnumpages;
		return false;
	}
	return ShowInventoryItems(playerid);
}


stock Downpage_invetory(playerid)
{
	page_invetory[playerid]--;
	if(page_invetory[playerid] <= 0)
	{
		page_invetory[playerid] = 1;
		return false;
	}
	return ShowInventoryItems(playerid);
}

stock AddSlotInvetory(playerid)
{
	if(total_indexes_inv[playerid] >= MAX_CELLS_INVETORY)
		return false;
	new TDindex = GetLastIndexTD(playerid);
	if(TDindex >= 0 && TDindex < MAX_ITEM_IN_PAGE) 
		PlayerTextDrawShow(playerid, invetory_PTD[playerid][TDindex][E_Cell]);
	total_indexes_inv[playerid]++;
	Updatepage_invetory(playerid);
	return true;
}


stock DeleteSlotInvetory(playerid)
{
	new TDindex = GetLastIndexTD(playerid)-1;
	if(TDindex >= 0)
	{
		if(total_indexes_inv[playerid]-MAX_ITEM_IN_PAGE*page_invetory[playerid] <= 0)
		{
			//An additional check to see if you are on your page? Should delete the TD?
			PlayerTextDrawHide(playerid,invetory_PTD[playerid][TDindex][E_Cell]); 
			PlayerTextDrawColor(playerid, invetory_PTD[playerid][TDindex][E_Cell], 0x7F7F7FFF);
			PlayerTextDrawBackgroundColor(playerid, invetory_PTD[playerid][TDindex][E_Cell], 0x7F7F7FFF);
			PlayerTextDrawSetPreviewModel(playerid, invetory_PTD[playerid][TDindex][E_Cell], 1649);//default 1649
			PlayerTextDrawSetPreviewRot(playerid, invetory_PTD[playerid][TDindex][E_Cell], 0.0000, 0.0000, 0.0000, 0.0000);//0.0000, 0.0000, 0.0000, 0.0000 default
			if(invetory_PTD[playerid][TDindex][E_Value] != PlayerText:INVALID_TEXT_DRAW)
			{
				PlayerTextDrawSetString(playerid, invetory_PTD[playerid][TDindex][E_Value], "_");
				PlayerTextDrawHide(playerid,invetory_PTD[playerid][TDindex][E_Cell]); 
			}
			
		}
	}
	if(GetPVarInt(playerid, "old_cell") == total_indexes_inv[playerid]-1)
		SetPVarInt(playerid, "old_cell", -1);
	
	
	if(total_indexes_inv[playerid] > 0)
	{
		total_indexes_inv[playerid]--;
		invetory_info[playerid][total_indexes_inv[playerid]][E_Invetory_IndexItem] = -1;
		invetory_info[playerid][total_indexes_inv[playerid]][E_Invetory_Value] = 0;
	}

	if(GetNumberOfPages(playerid) < page_invetory[playerid])//If you suddenly deleted all the objects from the page
		return Downpage_invetory(playerid);//we are transferring you to the previous one
	Updatepage_invetory(playerid);
	return true;
}


stock Updatepage_invetory(playerid)//
{
	new str[12];
	format(str, sizeof str, "%d/%d",page_invetory[playerid], GetNumberOfPages(playerid));
	PlayerTextDrawSetString(playerid, invetory_pages[playerid], str);
	return true;
}





stock CreateSlotInvetoryAll(playerid)
{
	for(new i = 0; i < MAX_ITEM_IN_PAGE; i++)
	{
		if(invetory_PTD[playerid][i][E_Cell] == PlayerText:INVALID_TEXT_DRAW)
		{
			invetory_PTD[playerid][i][E_Cell] = CreatePlayerTextDraw(playerid, 160.9832+((i % 5)*35.0), 171.5925+((i / 5)*34.0), "LD_SPAC:white"); 
			PlayerTextDrawTextSize(playerid, invetory_PTD[playerid][i][E_Cell], 34.0000, 33.0000);
			PlayerTextDrawAlignment(playerid, invetory_PTD[playerid][i][E_Cell], 1);
			PlayerTextDrawColor(playerid, invetory_PTD[playerid][i][E_Cell], 0x7F7F7FFF);
			PlayerTextDrawBackgroundColor(playerid, invetory_PTD[playerid][i][E_Cell], 0x7F7F7FFF);
			PlayerTextDrawFont(playerid, invetory_PTD[playerid][i][E_Cell], 5);
			PlayerTextDrawSetProportional(playerid, invetory_PTD[playerid][i][E_Cell], 0);
			PlayerTextDrawSetShadow(playerid, invetory_PTD[playerid][i][E_Cell], 0);
			PlayerTextDrawSetSelectable(playerid, invetory_PTD[playerid][i][E_Cell], true);
			PlayerTextDrawSetPreviewModel(playerid, invetory_PTD[playerid][i][E_Cell], 1649);
			PlayerTextDrawSetPreviewRot(playerid, invetory_PTD[playerid][i][E_Cell], 0.0000, 0.0000, 0.0000, 0.0000);
		}
	}
	return true;
}



stock SwapSlotInvetory(playerid, old_cell, now_cell, index_now_TD)
{
	new index_old_TD = old_cell-(MAX_ITEM_IN_PAGE*page_invetory[playerid])+MAX_ITEM_IN_PAGE;
	//
	new tmp = invetory_info[playerid][old_cell][E_Invetory_IndexItem];
	invetory_info[playerid][old_cell][E_Invetory_IndexItem] = invetory_info[playerid][now_cell][E_Invetory_IndexItem];
	invetory_info[playerid][now_cell][E_Invetory_IndexItem] = tmp;


	tmp = invetory_info[playerid][old_cell][E_Invetory_Value];
	invetory_info[playerid][old_cell][E_Invetory_Value] = invetory_info[playerid][now_cell][E_Invetory_Value];
	invetory_info[playerid][now_cell][E_Invetory_Value] = tmp;

	new index_model = invetory_info[playerid][now_cell][E_Invetory_IndexItem];
	if(index_model >= 0)
	{
		new model_id = items_invetory[index_model][Item_Model];
		PlayerTextDrawSetPreviewModel(playerid, invetory_PTD[playerid][index_now_TD][E_Cell], model_id);
		PlayerTextDrawSetPreviewRot(playerid, invetory_PTD[playerid][index_now_TD][E_Cell], items_invetory[index_model][Item_RotX], items_invetory[index_model][Item_RotY], items_invetory[index_model][Item_RotZ], items_invetory[index_model][Item_Scale]);
		ChangeValueSlotInvetory(playerid, index_now_TD, now_cell);
	}
	else
	{
		PlayerTextDrawSetPreviewModel(playerid, invetory_PTD[playerid][index_now_TD][E_Cell], 1649);//
		PlayerTextDrawSetPreviewRot(playerid, invetory_PTD[playerid][index_now_TD][E_Cell], 0.0000, 0.0000, 0.0000, 0.0000);
	}
	PlayerTextDrawShow(playerid, invetory_PTD[playerid][index_now_TD][E_Cell]);

	if(index_old_TD < MAX_ITEM_IN_PAGE && index_old_TD >= 0)//If it is on our page, then we update the second slot
	{

		index_model = invetory_info[playerid][old_cell][E_Invetory_IndexItem];
		if(index_model >= 0)
		{
			new model_id = items_invetory[index_model][Item_Model];
			PlayerTextDrawSetPreviewModel(playerid, invetory_PTD[playerid][index_old_TD][E_Cell], model_id);
			PlayerTextDrawSetPreviewRot(playerid, invetory_PTD[playerid][index_old_TD][E_Cell], items_invetory[index_model][Item_RotX], items_invetory[index_model][Item_RotY], items_invetory[index_model][Item_RotZ], items_invetory[index_model][Item_Scale]);
			ChangeValueSlotInvetory(playerid, index_old_TD, old_cell);
		}
		else
		{
			if(invetory_PTD[playerid][index_old_TD][E_Value] != PlayerText:INVALID_TEXT_DRAW)
				PlayerTextDrawSetString(playerid, invetory_PTD[playerid][index_old_TD][E_Value], "_");
			PlayerTextDrawSetPreviewModel(playerid, invetory_PTD[playerid][index_old_TD][E_Cell], 1649);//
			PlayerTextDrawSetPreviewRot(playerid, invetory_PTD[playerid][index_old_TD][E_Cell], 0.0000, 0.0000, 0.0000, 0.0000);//

		}
		
		PlayerTextDrawChangeBGColor(playerid, invetory_PTD[playerid][index_now_TD][E_Cell], 0x7F7F7FFF);
		PlayerTextDrawChangeBGColor(playerid, invetory_PTD[playerid][index_old_TD][E_Cell], 0x7F7F7FFF);
	}
	SetPVarInt(playerid, "old_cell",-1);
	return 1;
}






stock GiveItemToPlayer(playerid, index_item, value)
{

	if(index_item >= MAX_ITEMS || index_item < 0)
		return 0;//If an invalid index is specified
	else if(items_invetory[index_item][Item_Model] < 0)
		return 0;//If a non-existent item is specified
	for(new i = 0; i < total_indexes_inv[playerid]; i++)
	{
		if(invetory_info[playerid][i][E_Invetory_IndexItem] < 0)
		{
			invetory_info[playerid][i][E_Invetory_IndexItem] = index_item;
			invetory_info[playerid][i][E_Invetory_Value] = value;
			new str[123];
			format(str, sizeof str, "You were given - %s in the amount of %d", items_invetory[index_item][Item_Name],value);
			SendClientMessage(playerid, -1, str);

			new index_old_TD = i-(MAX_ITEM_IN_PAGE*page_invetory[playerid])+MAX_ITEM_IN_PAGE;

			if(index_old_TD < MAX_ITEM_IN_PAGE && index_old_TD >= 0)//If it is on our page, then we update the second slot
			{
				PlayerTextDrawHide(playerid, invetory_PTD[playerid][index_old_TD][E_Cell]);
				new index_model = invetory_info[playerid][i][E_Invetory_IndexItem];
				if(index_model >= 0)
				{
					new model_id = items_invetory[index_model][Item_Model];
					PlayerTextDrawSetPreviewModel(playerid, invetory_PTD[playerid][index_old_TD][E_Cell], model_id);
					PlayerTextDrawSetPreviewRot(playerid, invetory_PTD[playerid][index_old_TD][E_Cell], items_invetory[index_model][Item_RotX], items_invetory[index_model][Item_RotY], items_invetory[index_model][Item_RotZ], items_invetory[index_model][Item_Scale]);
					ChangeValueSlotInvetory(playerid, index_old_TD, i);
				}
				PlayerTextDrawBackgroundColor(playerid, invetory_PTD[playerid][index_old_TD][E_Cell], 0x7F7F7FFF);
				PlayerTextDrawShow(playerid, invetory_PTD[playerid][index_old_TD][E_Cell]);
			}
			return 1;
		}
	}
	return 0;
}










stock Invetory_CreateGlobalTextDraw()
{
	


	invetory_global_TD[E_BGround] = TextDrawCreate(158.3333, 169.5185, "LD_SPAC:white"); // Inventory back
	TextDrawTextSize(invetory_global_TD[E_BGround], 176.0000+3.6, 182.0000+3.6);
	TextDrawAlignment(invetory_global_TD[E_BGround], 1);
	TextDrawColor(invetory_global_TD[E_BGround], 555819519);
	TextDrawBackgroundColor(invetory_global_TD[E_BGround], 255);
	TextDrawFont(invetory_global_TD[E_BGround], 4);
	TextDrawSetProportional(invetory_global_TD[E_BGround], 0);
	TextDrawSetShadow(invetory_global_TD[E_BGround], 0);

	invetory_global_TD[E_NameInvetory] = TextDrawCreate(266.2500, 151.1110, "Inventory"); // Name
	TextDrawLetterSize(invetory_global_TD[E_NameInvetory], 0.4000, 1.6000);
	TextDrawAlignment(invetory_global_TD[E_NameInvetory], 1);
	TextDrawColor(invetory_global_TD[E_NameInvetory], -1);
	TextDrawSetOutline(invetory_global_TD[E_NameInvetory], 1);
	TextDrawBackgroundColor(invetory_global_TD[E_NameInvetory], 255);
	TextDrawFont(invetory_global_TD[E_NameInvetory], 1);
	TextDrawSetProportional(invetory_global_TD[E_NameInvetory], 1);
	TextDrawSetShadow(invetory_global_TD[E_NameInvetory], 0);

	invetory_global_TD[E_AddButton] = TextDrawCreate(160.9499, 332.2333, "LD_SPAC:white"); // Add (Inventory)
	TextDrawTextSize(invetory_global_TD[E_AddButton], 31.0000, 15.0000);
	TextDrawAlignment(invetory_global_TD[E_AddButton], 1);
	TextDrawColor(invetory_global_TD[E_AddButton], 8388863);
	TextDrawBackgroundColor(invetory_global_TD[E_AddButton], 255);
	TextDrawFont(invetory_global_TD[E_AddButton], 4);
	TextDrawSetProportional(invetory_global_TD[E_AddButton], 0);
	TextDrawSetShadow(invetory_global_TD[E_AddButton], 0);
	TextDrawSetSelectable(invetory_global_TD[E_AddButton], true);

	invetory_global_TD[E_AddText] = TextDrawCreate(163.2166, 334.3665, "Cell++"); // Add (Inventory)
	TextDrawLetterSize(invetory_global_TD[E_AddText], 0.2604, 1.1385);
	TextDrawAlignment(invetory_global_TD[E_AddText], 1);
	TextDrawColor(invetory_global_TD[E_AddText], -1);
	TextDrawSetOutline(invetory_global_TD[E_AddText], 1);
	TextDrawBackgroundColor(invetory_global_TD[E_AddText], 255);
	TextDrawFont(invetory_global_TD[E_AddText], 1);
	TextDrawSetProportional(invetory_global_TD[E_AddText], 1);
	TextDrawSetShadow(invetory_global_TD[E_AddText], 0);
	

	invetory_global_TD[E_DeleteButton] = TextDrawCreate(196.7832, 332.2333, "LD_SPAC:white"); // Delete a cell
	TextDrawTextSize(invetory_global_TD[E_DeleteButton], 31.0000, 15.0000);
	TextDrawAlignment(invetory_global_TD[E_DeleteButton], 1);
	TextDrawColor(invetory_global_TD[E_DeleteButton], -1523963137);
	TextDrawBackgroundColor(invetory_global_TD[E_DeleteButton], 255);
	TextDrawFont(invetory_global_TD[E_DeleteButton], 4);
	TextDrawSetProportional(invetory_global_TD[E_DeleteButton], 0);
	TextDrawSetShadow(invetory_global_TD[E_DeleteButton], 0);
	TextDrawSetSelectable(invetory_global_TD[E_DeleteButton], true);

	invetory_global_TD[E_DeleteText] = TextDrawCreate(199.0500, 334.3665, "Cell--"); // Delete a cell
	TextDrawLetterSize(invetory_global_TD[E_DeleteText], 0.2604, 1.1385);
	TextDrawAlignment(invetory_global_TD[E_DeleteText], 1);
	TextDrawColor(invetory_global_TD[E_DeleteText], -1);
	TextDrawSetOutline(invetory_global_TD[E_DeleteText], 1);
	TextDrawBackgroundColor(invetory_global_TD[E_DeleteText], 255);
	TextDrawFont(invetory_global_TD[E_DeleteText], 1);
	TextDrawSetProportional(invetory_global_TD[E_DeleteText], 1);
	TextDrawSetShadow(invetory_global_TD[E_DeleteText], 0);

	invetory_global_TD[E_BackButton] = TextDrawCreate(236.7830, 331.6518, "LD_SPAC:white"); // back
	TextDrawTextSize(invetory_global_TD[E_BackButton], 31.0000, 15.0000);
	TextDrawAlignment(invetory_global_TD[E_BackButton], 1);
	TextDrawColor(invetory_global_TD[E_BackButton], -1061109505);
	TextDrawBackgroundColor(invetory_global_TD[E_BackButton], 255);
	TextDrawFont(invetory_global_TD[E_BackButton], 4);
	TextDrawSetProportional(invetory_global_TD[E_BackButton], 0);
	TextDrawSetShadow(invetory_global_TD[E_BackButton], 0);
	TextDrawSetSelectable(invetory_global_TD[E_BackButton], true);

	invetory_global_TD[E_BackText] = TextDrawCreate(252.3832, 333.7850, "<<<"); // 
	TextDrawLetterSize(invetory_global_TD[E_BackText], 0.2604, 1.1385);
	TextDrawAlignment(invetory_global_TD[E_BackText], 2);
	TextDrawColor(invetory_global_TD[E_BackText], -1);
	TextDrawSetOutline(invetory_global_TD[E_BackText], 1);
	TextDrawBackgroundColor(invetory_global_TD[E_BackText], 255);
	TextDrawFont(invetory_global_TD[E_BackText], 1);
	TextDrawSetProportional(invetory_global_TD[E_BackText], 1);
	TextDrawSetShadow(invetory_global_TD[E_BackText], 0);

	invetory_global_TD[E_NextButton] = TextDrawCreate(298.0332, 331.6518, "LD_SPAC:white"); // 
	TextDrawTextSize(invetory_global_TD[E_NextButton], 31.0000, 15.0000);
	TextDrawAlignment(invetory_global_TD[E_NextButton], 1);
	TextDrawColor(invetory_global_TD[E_NextButton], -1061109505);
	TextDrawBackgroundColor(invetory_global_TD[E_NextButton], 255);
	TextDrawFont(invetory_global_TD[E_NextButton], 4);
	TextDrawSetProportional(invetory_global_TD[E_NextButton], 0);
	TextDrawSetShadow(invetory_global_TD[E_NextButton], 0);
	TextDrawSetSelectable(invetory_global_TD[E_NextButton], true);

	invetory_global_TD[E_NextText] = TextDrawCreate(313.6332, 333.7850, ">>>"); // 
	TextDrawLetterSize(invetory_global_TD[E_NextText], 0.2604, 1.1385);
	TextDrawAlignment(invetory_global_TD[E_NextText], 2);
	TextDrawColor(invetory_global_TD[E_NextText], -1);
	TextDrawSetOutline(invetory_global_TD[E_NextText], 1);
	TextDrawBackgroundColor(invetory_global_TD[E_NextText], 255);
	TextDrawFont(invetory_global_TD[E_NextText], 1);
	TextDrawSetProportional(invetory_global_TD[E_NextText], 1);
	TextDrawSetShadow(invetory_global_TD[E_NextText], 0);


	
}






stock HideInvetoryForPlayer(playerid)
{
	if(IsPlayerInvetoryOpened(playerid))
	{
		for(new i = 0; i < MAX_ITEM_IN_PAGE; i++)
		{
			if(invetory_PTD[playerid][i][E_Value] != PlayerText:INVALID_TEXT_DRAW)
			{
				PlayerTextDrawDestroy(playerid, invetory_PTD[playerid][i][E_Value]);
				invetory_PTD[playerid][i][E_Value] = PlayerText:INVALID_TEXT_DRAW;
			}
			if(invetory_PTD[playerid][i][E_Cell] != PlayerText:INVALID_TEXT_DRAW)
			{
				PlayerTextDrawDestroy(playerid, invetory_PTD[playerid][i][E_Cell]);
				invetory_PTD[playerid][i][E_Cell] = PlayerText:INVALID_TEXT_DRAW;
			}
		}
		SetPVarInt(playerid, "old_cell", -1);
		page_invetory[playerid] = 0;
		for(new E_GlobalInvetoryTD:i; i < E_GlobalInvetoryTD; i++)
			TextDrawHideForPlayer(playerid, invetory_global_TD[i]);
		if(invetory_pages[playerid] != PlayerText:INVALID_TEXT_DRAW)
		{
			PlayerTextDrawDestroy(playerid, invetory_pages[playerid]); 
			invetory_pages[playerid] = PlayerText:INVALID_TEXT_DRAW;
		}
		CancelSelectTextDraw(playerid);
	}
	return 1;
}



stock IsPlayerInvetoryOpened(playerid)/// if the page is zero, we do nothing. Since the inventory is not open
{
	if(page_invetory[playerid] > 0)
		return true;
	return false;
}

stock GetNumberOfPages(playerid)
{
	if((total_indexes_inv[playerid] >= MAX_ITEM_IN_PAGE) && (total_indexes_inv[playerid] % MAX_ITEM_IN_PAGE) == 0)
		return (total_indexes_inv[playerid] / MAX_ITEM_IN_PAGE);
	else 
		return (total_indexes_inv[playerid] / MAX_ITEM_IN_PAGE) + 1;
}


#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif

#define OnGameModeInit inv_OnGameModeInit
#if defined inv_OnGameModeInit
    forward inv_OnGameModeInit();
#endif







#if defined _ALS_OnPlayerClickPlayerTD
    #undef OnPlayerClickPlayerTextDraw
#else
    #define _ALS_OnPlayerClickPlayerTD
#endif

#define OnPlayerClickPlayerTextDraw inv_OnPlayerClickPlayerTextDraw
#if defined inv_OnPlayerClickPlayerTextDraw
    forward inv_OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid);
#endif







#if defined _ALS_OnPlayerClickTextDraw
    #undef OnPlayerClickTextDraw
#else
    #define _ALS_OnPlayerClickTextDraw
#endif

#define OnPlayerClickTextDraw inv_OnPlayerClickTextDraw
#if defined inv_OnPlayerClickTextDraw
    forward inv_OnPlayerClickTextDraw(playerid, Text:clickedid);
#endif



#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif

#define OnPlayerDisconnect inv_OnPlayerDisconnect
#if defined inv_OnPlayerDisconnect
	forward inv_OnPlayerDisconnect(playerid, reason);
#endif



#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect inv_OnPlayerConnect
#if defined inv_OnPlayerConnect
    forward inv_OnPlayerConnect(playerid);
#endif



	
public OnPlayerClickInvSlot(&playerid, &index_item)
{
	return WC_OnPlayerClickInvSlot(playerid, index_item);
}
#if defined _ALS_OnPlayerClickInvSlot
	#undef OnPlayerClickInvSlot
#else
	#define _ALS_OnPlayerClickInvSlot
#endif
#define OnPlayerClickInvSlot(%0) CHAIN_PUBLIC:WC_OnPlayerClickInvSlot(%0)
CHAIN_FORWARD:WC_OnPlayerClickInvSlot(&playerid, &index_item) = 1;



