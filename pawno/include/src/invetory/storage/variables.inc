#define INVALID_COORDS_STORAGE 	    (Float:0xFFFFFFFF)  //���������� �������� ��������� � ���������


#define MAX_SLOTS_STORAGE	(25)//������������ ���-�� ���� � ���������
#define MAX_STORAGE_ID		(100)//������������ ���-�� ��������


new total_storage = 0;//����� ��������
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
	Storage_Type,//��� ���������.  �������� �������� ���, ������, ������������� ��������, �������� � � � - ������� ������� � ����� ����
	Storage_ID_Subject,//�� �������� , �������� �� ����������, �����, �������� ����, ������� � � �. ��� �������� - ����������. ��� ������� ���.
	Text3D:Storage_3D_Text,//3� ����� ���������
	Float:Storage_Coords_X,//���������� ���������, ���� ��� ����.
	Float:Storage_Coords_Y,//���������� ���������, ���� ��� ����.
	Float:Storage_Coords_Z,//���������� ���������, ���� ��� ����.
	Storage_VirtualWorld,//����������� ��� ���������
	Storage_Interior,//�������� ���������
	Storage_Name[64],//��� ��������� �� ���������
	
}
new storage_info[MAX_STORAGE_ID][E_STORAGE_INFO];//���������� � ���������
//
new storage_data_cell[MAX_STORAGE_ID][MAX_SLOTS_STORAGE][E_Invetory];//������ � ��������� � ����� ���������
//////////