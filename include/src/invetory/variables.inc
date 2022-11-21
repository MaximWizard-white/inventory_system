
#if defined STORAGE_System_Added
	#endinput
#endif
#define STORAGE_System_Added

#define MAX_ITEM_IN_PAGE	        (20)    //ћаксимальное кол-во айтемов на странице
//
#define MAX_CELLS_INVETORY	        (100)   //ћаксимальное кол-во слотов которые можно создать
//
#define MAX_ITEMS	                (100)   //ћаксимальное кол-во предметов которые можно создать
//
new total_invetory_items = 0;//—колько создано айтемов уже всего
//

enum E_InvetoryTD
{
	PlayerText:E_Cell,
	PlayerText:E_Value
}
new invetory_PTD[MAX_PLAYERS][MAX_ITEM_IN_PAGE][E_InvetoryTD];
new PlayerText:invetory_pages[MAX_PLAYERS];

enum E_GlobalInvetoryTD
{
	Text:E_BGround,
	Text:E_NameInvetory,
	Text:E_AddButton,
	Text:E_AddText,
	Text:E_DeleteButton,
	Text:E_DeleteText,
	Text:E_BackButton,
	Text:E_BackText,
	Text:E_NextButton,
	Text:E_NextText,
}
new Text:invetory_global_TD[E_GlobalInvetoryTD];


enum E_Invetory
{
	E_Invetory_IndexItem,
	E_Invetory_Value
}

new invetory_info[MAX_PLAYERS][MAX_CELLS_INVETORY][E_Invetory];
new total_indexes_inv[MAX_PLAYERS];
new page_invetory[MAX_PLAYERS];

#define MAX_STRING_ITEM	32

enum E_Item_Invetory
{
	Item_Name[MAX_STRING_ITEM],
	Item_Model,
	Float:Item_RotX,
	Float:Item_RotY,
	Float:Item_RotZ,
	Float:Item_Scale
}

new items_invetory[MAX_ITEMS][E_Item_Invetory] = 
{
	{"", -1, 0.000, 0.000, 0.000, 0.000},
	{"", -1, 0.000, 0.000, 0.000, 0.000},...

};