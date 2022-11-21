#define INVALID_COORDS_STORAGE 	    (Float:0xFFFFFFFF)  //invalid value of coordinates


#define MAX_SLOTS_STORAGE	(25)//Maximum number of storage spaces
#define MAX_STORAGE_ID		(100)//Maximum number of storages


new total_storage = 0;//
enum E_GlobalStorageTD
{
	Text:E_S_BGround
}
new Text:storage_global_TD[E_GlobalStorageTD];
new PlayerText:storage_name[MAX_PLAYERS] = {PlayerText:INVALID_TEXT_DRAW,PlayerText:INVALID_TEXT_DRAW,...};


//

new storage_PTD[MAX_PLAYERS][MAX_SLOTS_STORAGE][E_InvetoryTD];
//

enum E_STORAGE_INFO
{
	Storage_Type,//Storage type.  For example, a garbage can, a trunk, a car trunk, a glove compartment, etc. - The system is written in the very fashion
	Storage_ID_Subject,//The ID of the subject , for example, the ID of the car, actor, trash can, trunk, etc. For dynamics - required. Not for static.
	Text3D:Storage_3D_Text,//ID 3d text storage
	Float:Storage_Coords_X,//The coordinates of the storage, if any.
	Float:Storage_Coords_Y,//The coordinates of the storage, if any.
	Float:Storage_Coords_Z,//The coordinates of the storage, if any.
	Storage_VirtualWorld,//
	Storage_Interior,//
	Storage_Name[64],//The name of the storage in Cyrillic
	
}
new storage_info[MAX_STORAGE_ID][E_STORAGE_INFO];//Info about storage
//
new storage_data_cell[MAX_STORAGE_ID][MAX_SLOTS_STORAGE][E_Invetory];//Data about items in the storage itself
//////////
