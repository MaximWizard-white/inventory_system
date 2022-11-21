#define INVALID_COORDS_STORAGE 	    (Float:0xFFFFFFFF)  //невалидное значение координат в сторейдже


#define MAX_SLOTS_STORAGE	(25)//Максимальное кол-во мест в хранилище
#define MAX_STORAGE_ID		(100)//Максимальное кол-во хранилищ


new total_storage = 0;//Всего хранилищ
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
	Storage_Type,//Тип хранилища.  Например мусорный бак, сундук, автомобильный багажник, бардачок и т д - Система пишется в самом моде
	Storage_ID_Subject,//Ид субъекта , например ид автомобиля, актёра, муорного бака, сундука и т д. Для динамики - обязателен. Для статики нет.
	Text3D:Storage_3D_Text,//3д текст хранилища
	Float:Storage_Coords_X,//Координаты хранилища, если они есть.
	Float:Storage_Coords_Y,//Координаты хранилища, если они есть.
	Float:Storage_Coords_Z,//Координаты хранилища, если они есть.
	Storage_VirtualWorld,//Виртуальный мир хранилища
	Storage_Interior,//Интерьер хранилища
	Storage_Name[64],//Имя хранилища на киррилице
	
}
new storage_info[MAX_STORAGE_ID][E_STORAGE_INFO];//Информация о хранилище
//
new storage_data_cell[MAX_STORAGE_ID][MAX_SLOTS_STORAGE][E_Invetory];//Данные о предметах в самом хранилище
//////////
