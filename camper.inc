/*

	Чтобы установить топливо для ДНК, создайте у себя в моде функцию
	И перед подключением инклуда, объявите константу #define USE_CUSTOM_FUEL_FOR_CAMPER

	Пример:

	#define USE_CUSTOM_FUEL_FOR_CAMPER
	#include <camper>

	Функция: 

	stock SetCamperFuel(vehicleid)
	{
		VehicleInfo[vehicleid] = 100; // VehicleInfo Меняем на свой массив, где хранится топливо
	}

*/


#if defined cache_get_field_content
    #define CAMPER_MYSQL_TAG:
#else
    #define CAMPER_MYSQL_TAG: MySQL:
#endif

static CAMPER_MYSQL_TAG:dbHandle = CAMPER_MYSQL_TAG:0;

#if defined cache_get_field_content
	#define cache_get_value_name_float(%0,%1,%2) \
		%2=cache_get_field_content_float(%0,%1,dbHandle)
#endif


#if defined cache_get_field_content
	#define cache_get_value_name_int(%0,%1,%2) \
		%2=cache_get_field_content_int(%0,%1,dbHandle)
#endif


#if defined cache_get_field_content
	#define cache_get_value_name(%0,%1,%2,%3) \
		cache_get_field_content(%0,%1,%2,dbHandle,%3)
#endif


#if defined _streamer_included
	#define CreateCamperObject(%0,%1,%2,%3,%4,%5,%6) \
		CreateDynamicObject(%0, %1, %2, %3, %4, %5, %6, -1, -1, -1)

	#define SetCamperObjectMaterial(%0,%1,%2,%3,%4,%5) \
		SetDynamicObjectMaterial(%0,%1,%2,%3,%4,%5)

	#define SetCamperObjectMaterialText(%0,%1,%2,%3,%4,%5,%6,%7,%8,%9) \
		SetDynamicObjectMaterialText(%0,%1,%2,%3,%4,%5,%6,%7,%8,%9)

#else
	#define CreateCamperObject(%0,%1,%2,%3,%4,%5,%6) \
		CreateObject(%0, %1, %2, %3, %4, %5, %6)

	#define SetCamperObjectMaterial(%0,%1,%2,%3,%4,%5) \
		SetObjectMaterial(%0,%1,%2,%3,%4,%5)

	#define SetCamperObjectMaterialText(%0,%2,%1,%3,%4,%5,%6,%7,%8,%9) \
		SetObjectMaterialText(%0,%1,%2,%3,%4,%5,%6,%7,%8,%9)
		
#endif


#define COLOR_CAMPERS						0xBA2821FF

const
	MODEL_VEHICLE_CAMPER					= 508,
	CAMPER_SPAWN							= -1,

	Float: BUY_CAMPER_POS_X					= 0.0,
	Float: BUY_CAMPER_POS_Y					= 0.0,
	Float: BUY_CAMPER_POS_Z					= 0.0,
	Float: BUY_CAMPER_POS_A					= 0.0,

	Float: PLACE_BUY_CAMPER_POS_X			= 986.5852,
	Float: PLACE_BUY_CAMPER_POS_Y			= -1300.9677,
	Float: PLACE_BUY_CAMPER_POS_Z			= 13.3828,

	D_CAMPER_CLOSE_DIALOG					= 5999,

	D_BUY_CAMPER_MENU						= 6000,
	D_CAMPER_SELECT_COLOR					= 6001,
	D_CAMPER_SAFE_MENU						= 6002,

	D_CAMPER_SAFE_MONEY						= 6003,
	D_CAMPER_SAFE_MATS						= 6004,
	D_CAMPER_SAFE_DRUGS						= 6005,

	D_CAMPER_SAFE_TAKE_MONEY				= 6006,
	D_CAMPER_SAFE_GIVE_MONEY				= 6007,

	D_CAMPER_SAFE_TAKE_MATS					= 6008,
	D_CAMPER_SAFE_GIVE_MATS					= 6009,

	D_CAMPER_SAFE_TAKE_DRUGS				= 6010,
	D_CAMPER_SAFE_GIVE_DRUGS				= 6011,
	
	D_CAMPER_MENU							= 6012,

	D_CAMPER_SELL_MENU						= 6013,
	D_CAMPER_SELL_PLAYER_ID					= 6014,
	D_CAMPER_SELL_MONEY						= 6015,
	D_CAMPER_PLAYER_BUY_CAMPER				= 6016,
	D_BUY_CAMPER_IMPROV_MENU				= 6017,
	D_BUY_CAMPER_IMPROV_INT					= 6018,
	D_BUY_CAMPER_IMPROV_COLOR				= 6019,
	D_BUY_CAMPER_IMPROV_WHEELS				= 6020,

	MAX_LENGTH_NAME_CAMPER					= 40
;


static
	gPlayerCamperSellMoney[MAX_PLAYERS],
	gPlayerSpawnCamper[MAX_PLAYERS],
	gPlayerCamperSellId[MAX_PLAYERS]		= {INVALID_PLAYER_ID, ...},
	gVehicleCamperID[MAX_PLAYERS]			= {INVALID_VEHICLE_ID, ...},
	gIntoCamperVehicleID[MAX_PLAYERS]		= {INVALID_VEHICLE_ID, ...}
;

static enum e_CamperClass
{
	camper_name_class[MAX_LENGTH_NAME_CAMPER],
	camper_int_class,
	camper_price_class,
	Float: camper_class_pos_x,
	Float: camper_class_pos_y,
	Float: camper_class_pos_z,
	Float: camper_class_pos_a
}

static const gCamperClass[][e_CamperClass] =
{
	/*Название | Инт | Цена | Координаты*/
	{"Дешевый вариант", 1, 50_000, 2512.7520, -1729.1099, 778.6371, 89.3518},
	{"Средний вариант", 1, 100_000, 2483.1226, -871.6552, 2883.3989, 3.8108}
};

static enum e_CamperInfo
{
	c_ID,
	c_Owner[MAX_PLAYER_NAME + 1],
	c_Class,
	c_Lock,
	c_Color,
	c_Money,
	c_Mats,
	c_Drugs,
	c_Wheels,
	c_Hydraulics,
	c_Nitro,
	c_VehicleId,
	Float: c_PosX,
	Float: c_PosY,
	Float: c_PosZ,
	Float: c_PosA,
	Text3D: c_Label
}
static CamperInfo[MAX_VEHICLES][e_CamperInfo];


public OnGameModeInit()
{
	print("**********************************************************************");
	print("Дом на колесах by Saibot - Загружен");

#if defined cache_get_field_content_int
	printf("");
	print("Загрузка данных - версия MySQL 38 - 39.");
#else
	printf("");
	print("Загрузка данных - версия MySQL 40 - 41.");
#endif

	mysql_tquery(
		dbHandle,
		"CREATE TABLE IF NOT EXISTS `info_camper` ( \
			`ID` int(11) NOT NULL AUTO_INCREMENT, \
			`Owner` varchar(24) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL, \
			`Class` int(11) NOT NULL DEFAULT '0', \
			`Lock` int(11) NOT NULL DEFAULT '0', \
			`Color` int(11) NOT NULL DEFAULT '0', \
			`Money` int(11) NOT NULL DEFAULT '0', \
			`Mats` int(11) NOT NULL DEFAULT '0', \
			`Drugs` int(11) NOT NULL DEFAULT '0', \
			`Spawn` int(11) NOT NULL DEFAULT '0', \
			`Wheels` int(11) NOT NULL DEFAULT '0', \
			`Hydraulics` int(11) NOT NULL DEFAULT '0', \
			`Nitro` int(11) NOT NULL DEFAULT '0', \
			`CamperPosX` float NOT NULL DEFAULT '0', \
			`CamperPosY` float NOT NULL DEFAULT '0', \
			`CamperPosZ` float NOT NULL DEFAULT '0', \
			`CamperPosA` float NOT NULL DEFAULT '0', \
			PRIMARY KEY (ID) \
		) \
		ENGINE = INNODB, \
		CHARACTER SET utf8mb4, \
		COLLATE utf8mb4_general_ci"
	);
	CamperObject();
#if defined cmp_OnGameModeInit
		return cmp_OnGameModeInit();
	#else
		return 1;
	#endif
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif
#define OnGameModeInit cmp_OnGameModeInit
#if defined cmp_OnGameModeInit
	forward cmp_OnGameModeInit();
#endif


public OnPlayerConnect(playerid)
{
	new
		camper_name[MAX_PLAYER_NAME + 1],
		string[64 + MAX_PLAYER_NAME]
	;
	GetPlayerName(playerid, camper_name, MAX_PLAYER_NAME + 1);

	format(string, sizeof(string),
		"SELECT \
			* \
		FROM `info_camper` \
		WHERE `Owner` = '%s'",
		camper_name
	);
	mysql_tquery(dbHandle, string, "@_OnLoadCamper", "d", playerid);
	
#if defined cmp_OnPlayerConnect
		return cmp_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect cmp_OnPlayerConnect
#if defined cmp_OnPlayerConnect
	forward cmp_OnPlayerConnect(playerid);
#endif


public OnPlayerDisconnect(playerid, reason)
{
	if (gVehicleCamperID[playerid] != INVALID_VEHICLE_ID)
	{
		new	camperid = gVehicleCamperID[playerid];

		DestroyVehicle(camperid);
		ClearVehicleCamper(camperid);

	#if defined _streamer_included
		DestroyDynamic3DTextLabel(CamperInfo[camperid][c_Label]);
	#else
		Delete3DTextLabel(CamperInfo[camperid][c_Label]);
	#endif	
		gVehicleCamperID[playerid] = INVALID_VEHICLE_ID;
	}
	gPlayerSpawnCamper[playerid] = 0;

	if (gPlayerCamperSellId[playerid] != INVALID_PLAYER_ID)
	{
		new targetid = gPlayerCamperSellId[playerid];

		SendClientMessage(
			targetid,
			COLOR_CAMPERS,
			!"[Camper]: {ffffff}Игрок которуму Вы предложили купить дом на колесах - покинул серевер."
		);
		gPlayerCamperSellId[targetid] = INVALID_PLAYER_ID;
		gPlayerCamperSellId[playerid] = INVALID_PLAYER_ID;
	}
	gPlayerCamperSellMoney[playerid] = 0;
#if defined cmp_OnPlayerDisconnect
	return cmp_OnPlayerDisconnect(playerid, reason);
#else
	return 1;
#endif
}
#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect cmp_OnPlayerDisconnect
#if defined    cmp_OnPlayerDisconnect
	forward cmp_OnPlayerDisconnect(playerid, reason);
#endif


public OnVehicleDeath(vehicleid, killerid)
{
	new camperid = CamperInfo[vehicleid][c_VehicleId];

	if (vehicleid == camperid)
	{
#if defined foreach
	foreach(Player, i)
#else
	for (new i = 0, size = GetPlayerPoolSize(); i <= size; ++i)
#endif
		{
			if (gIntoCamperVehicleID[i] != camperid)
			{
				continue;
			}
			SetPlayerHealth(i, 0.0);
			gIntoCamperVehicleID[i] = INVALID_VEHICLE_ID;

			SendClientMessage(
				i,
				COLOR_CAMPERS,
				!"[Camper]: {ffffff}Дом на колесах уничтожен."
			);
		}
	}
#if defined cmp_OnVehicleDeath
		return cmp_OnVehicleDeath(vehicleid, killerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnVehicleDeath
	#undef OnVehicleDeath
#else
	#define _ALS_OnVehicleDeath
#endif
#define OnVehicleDeath cmp_OnVehicleDeath
#if defined cmp_OnVehicleDeath
	forward cmp_OnVehicleDeath(vehicleid, killerid);
#endif


public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if (newkeys & KEY_WALK)
	{
		if (IsPlayerInRangeOfPoint(playerid, 1.5, PLACE_BUY_CAMPER_POS_X, PLACE_BUY_CAMPER_POS_Y, PLACE_BUY_CAMPER_POS_Z))
		{
			if (gVehicleCamperID[playerid] != INVALID_VEHICLE_ID)
			{
				if (GetSpawnCamper(playerid) == 0)
				{
					SendClientMessage(
						playerid,
						COLOR_CAMPERS,
						!"[Camper]: {ffffff}Вам нужно заспавнить ДНК."
					);
					return CamperMenu(playerid);
				}
				ShowImproveMenu(playerid);
				return 1;
			}
			new string[33 + (( 5 + (- 2 + MAX_LENGTH_NAME_CAMPER) + (- 2 + 11) * sizeof(gCamperClass)))];

			string = "{ffffff}Название\t{ffffff}Цена\n";

			for(new i; i < sizeof(gCamperClass); i++)
			{
				format(string, sizeof(string),
					"%s%s\t%d$\n",
					string,
					gCamperClass[i][camper_name_class],
					gCamperClass[i][camper_price_class]
				);
			}
			ShowPlayerDialog(
				playerid,
				D_BUY_CAMPER_MENU,
				DIALOG_STYLE_TABLIST_HEADERS,
				!"Продавец",
				string,
				!"Далее",
				!"Выход"
			);
			return 1;
		}
	}
	if (newkeys & KEY_CTRL_BACK)
	{
		new
			Float: camp_x,
			Float: camp_y,
			Float: camp_z
		;
		for (new c = 1, size = GetVehiclePoolSize(); c <= size; ++c)
		{
			if (!GetVehiclePos(CamperInfo[c][c_VehicleId], camp_x, camp_y, camp_z))
			{
				continue;
			}
			GetCamperVehiclePos(CamperInfo[c][c_VehicleId], camp_x, camp_y, camp_z);

			if (IsPlayerInRangeOfPoint(playerid, 1.0, camp_x, camp_y, camp_z))
			{
				if (CamperInfo[CamperInfo[c][c_VehicleId]][c_Lock])
				{
					SendClientMessage(
						playerid,
						COLOR_CAMPERS,
						!"[Camper]: {ffffff}Дом на колесах - закрыт." 
					);
					return 0;
				}
				new camp_class = CamperInfo[CamperInfo[c][c_VehicleId]][c_Class];

				SetCamperInterior(playerid,
					gCamperClass[camp_class][camper_class_pos_x],
					gCamperClass[camp_class][camper_class_pos_y],
					gCamperClass[camp_class][camper_class_pos_z],
					gCamperClass[camp_class][camper_class_pos_a],
					c + 1,
					gCamperClass[camp_class][camper_int_class]
				);
				gIntoCamperVehicleID[playerid] = CamperInfo[c][c_VehicleId];
				return 1;
			}
		}
		if (gIntoCamperVehicleID[playerid] != INVALID_VEHICLE_ID)
		{
			new
				vehicleid = gIntoCamperVehicleID[playerid],
				exit_camp_class = CamperInfo[vehicleid][c_Class]
			;
			if (IsPlayerInRangeOfPoint(playerid, 2.0,
				gCamperClass[exit_camp_class][camper_class_pos_x],
				gCamperClass[exit_camp_class][camper_class_pos_y],
				gCamperClass[exit_camp_class][camper_class_pos_z])
				&& gIntoCamperVehicleID[playerid] != INVALID_VEHICLE_ID
			)
			{
				GetCamperVehiclePos(vehicleid, camp_x, camp_y, camp_z);
				SetCamperInterior(playerid, camp_x, camp_y, camp_z, 0.0, 0, 0);

				gIntoCamperVehicleID[playerid] = INVALID_VEHICLE_ID;
				return 1;
			}
		}
	}
#if defined cmp_OnPlayerKeyStateChange
		return cmp_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerKeyStateChange
	#undef OnPlayerKeyStateChange
#else
	#define _ALS_OnPlayerKeyStateChange
#endif
#define OnPlayerKeyStateChange cmp_OnPlayerKeyStateChange
#if defined cmp_OnPlayerKeyStateChange
	forward cmp_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
#endif



public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	switch (dialogid)
	{
		case D_CAMPER_MENU:
		{
			if (!response)
			{
				return 0;
			}
			new camperid = gVehicleCamperID[playerid];
			
			switch (listitem)
			{
				case 0:
				{
					if (GetSpawnCamper(playerid) == 0)
					{
						SendClientMessage(
							playerid,
							COLOR_CAMPERS,
							!"[Camper]: {ffffff}Вам нужно заспавнить ДНК."
						);
						CamperMenu(playerid);
						return 0;
					}
					new string[147 + (- 2 + 11) + MAX_PLAYER_NAME + (- 2 + MAX_LENGTH_NAME_CAMPER)];

					GameTextForPlayer(
						playerid,
						CamperInfo[camperid][c_Lock] == 0 ? !"~w~Camper ~r~Lock" : !"~w~Camper ~g~UnLock",
						3000,
						4
					);
					CamperInfo[camperid][c_Lock] = !CamperInfo[camperid][c_Lock];

					format(string, sizeof(string),
						"{5d9978}Класс: {ffffff}%s\n\
						{5d9978}Владелец: {ffffff}%s\n\
						{5d9978}Дверь: {ffffff}%s\n\n\
						{ffffff}Войти в домик - [H]",
						gCamperClass[CamperInfo[camperid][c_Class]][camper_name_class],
						CamperInfo[camperid][c_Owner],
						CamperInfo[camperid][c_Lock] == 0 ? "Открыта" : "Закрыта"
					);
				#if defined _streamer_included
					UpdateDynamic3DTextLabelText(CamperInfo[camperid][c_Label], COLOR_CAMPERS, string);
				#else
					Update3DTextLabelText(CamperInfo[camperid][c_Label], COLOR_CAMPERS, string);
				#endif
					return CamperMenu(playerid);
				}
				case 1:
				{
					if (GetSpawnCamper(playerid) == 0)
					{
						SendClientMessage(
							playerid,
							COLOR_CAMPERS,
							!"[Camper]: {ffffff}Вам нужно заспавнить ДНК."
						);
						CamperMenu(playerid);
						return 0;
					}
					if (GetPVarInt(playerid, !"camper_place") == 1)
					{
						DisablePlayerRaceCheckpoint(playerid);
						GameTextForPlayer(playerid, !"~w~GPS Tracker ~r~Off", 3000, 4);
						DeletePVar(playerid, !"camper_place");
						return CamperMenu(playerid);
					}
					new
						Float: camp_x,
						Float: camp_y,
						Float: camp_z
					;
					GetVehiclePos(camperid, camp_x, camp_y, camp_z);
					SetPlayerRaceCheckpoint(playerid, 1, camp_x, camp_y, camp_z, 0.0, 0.0, 0.0, 4.0);

					SetPVarInt(playerid, !"camper_place", 1);

					GameTextForPlayer(playerid, !"~w~GPS Tracker ~g~On", 3000, 4);
					return CamperMenu(playerid);
				}
				case 2:
				{
					new
						string[173 + (- 8 + 48) + (- 2 + 11)],
						Float: camp_x,
						Float: camp_y,
						Float: camp_z,
						Float: camp_a
					;
					if (GetSpawnCamper(playerid) == 0)
					{
						SendClientMessage(
							playerid,
							COLOR_CAMPERS,
							!"[Camper]: {ffffff}Вам нужно заспавнить ДНК."
						);
						return CamperMenu(playerid);
					}
					GetVehiclePos(camperid, camp_x, camp_y, camp_z);
					GetVehicleZAngle(camperid, camp_a);

					format(string, sizeof(string),
						"UPDATE `info_camper` \
						SET `CamperPosX` = '%f', \
							`CamperPosY` = '%f', \
							`CamperPosZ` = '%f', \
							`CamperPosA` = '%f' \
						WHERE `ID` = '%d'",
						camp_x,
						camp_y,
						camp_z,
						camp_a,
						CamperInfo[camperid][c_ID]
					);
					mysql_tquery(dbHandle, string);

					CamperInfo[camperid][c_PosX] = camp_x;
					CamperInfo[camperid][c_PosY] = camp_y;
					CamperInfo[camperid][c_PosZ] = camp_z;
					CamperInfo[camperid][c_PosA] = camp_a;

					SendClientMessage(
						playerid,
						COLOR_CAMPERS,
						!"[Camper]: {ffffff}Дом на колесах успешно припаркован."
					);
					return CamperMenu(playerid);
				}
				case 3:
				{
					if (camperid != gIntoCamperVehicleID[playerid])
					{
						SendClientMessage(
							playerid,
							COLOR_CAMPERS,
							!"[Camper]: {ffffff}Вы должны находится в своем ДНК."
						);
						return CamperMenu(playerid);
					}
					ShowPlayerDialog(
						playerid,
						D_CAMPER_SAFE_MENU,
						DIALOG_STYLE_LIST,
						!"Сейф",
						!"Деньги\n\
						Материалы\n\
						Наркотики",
						!"Выбрать",
						!"Закрыть"
					);
					return 1;
				}
				case 4:
				{
					if (GetSpawnCamper(playerid) == 0)
					{
						SendClientMessage(
							playerid,
							COLOR_CAMPERS,
							!"[Camper]: {ffffff}Вам нужно заспавнить ДНК."
						);
						return CamperMenu(playerid);
					}
					gPlayerSpawnCamper[playerid] = !gPlayerSpawnCamper[playerid];

					SendClientMessage(playerid, 
						COLOR_CAMPERS,
						gPlayerSpawnCamper[playerid] == 0 ? 
						"[Camper]: {ffffff}Место возрождение - обычное." :
						"[Camper]: {ffffff}Место возрождение - дом на колесах."
					);
					return CamperMenu(playerid);
				}
				case 5:
				{
					if (GetSpawnCamper(playerid) == 0)
					{
						SendClientMessage(
							playerid,
							COLOR_CAMPERS,
							!"[Camper]: {ffffff}Вам нужно заспавнить ДНК."
						);
						return CamperMenu(playerid);
					}
					ShowPlayerDialog(
						playerid,
						D_CAMPER_SELL_MENU,
						DIALOG_STYLE_LIST,
						"Меню ДНК",
						"{BA2821}»{ffffff} Государству\n\
						{BA2821}»{ffffff} Игроку",
						!"Выбрать",
						!"Назад"
					);
					return 1;
				}
				case 6:
				{
					if (GetPlayerVirtualWorld(playerid) || GetPlayerInterior(playerid))
					{
						SendClientMessage(
							playerid,
							COLOR_CAMPERS,
							!"[Camper]: {ffffff}Недоступно в интерьере."
						);
						return CamperMenu(playerid);
					}
					new
						Float: p_x,
						Float: p_y,
						Float: p_z,
						Float: p_a,
						string[173 + (- 8 + 48) + (- 2 + 11)],
						Float: dist = 6.0
					;
					GetPlayerPos(playerid, p_x, p_y, p_z);
					GetPlayerFacingAngle(playerid, p_a);

					p_x += dist* -floatsin(p_a, degrees);
					p_y += dist* floatcos(p_a, degrees);

					SetVehiclePos(camperid, p_x, p_y, p_z);
					GetVehicleZAngle(camperid, p_a);
					SetVehicleVirtualWorld(camperid, 0);

					format(string, sizeof(string),
						"UPDATE `info_camper` \
						SET `CamperPosX` = '%f', \
							`CamperPosY` = '%f', \
							`CamperPosZ` = '%f', \
							`CamperPosA` = '%f' \
						WHERE `ID` = '%d'",
						p_x,
						p_y,
						p_z,
						p_a,
						CamperInfo[camperid][c_ID]
					);
					mysql_tquery(dbHandle, string);

					CamperInfo[camperid][c_PosX] = p_x;
					CamperInfo[camperid][c_PosY] = p_y;
					CamperInfo[camperid][c_PosZ] = p_z;
					CamperInfo[camperid][c_PosA] = p_a;

					SendClientMessage(
						playerid,
						COLOR_CAMPERS,
						!"[Camper]: {ffffff}Дом на колесах заспавнен возле Вас."
					);
					return CamperMenu(playerid);
				}
			}
			return 1;
		}
		case D_BUY_CAMPER_MENU:
		{
			if (!response)
			{
				return 0;
			}
			new camper_class = listitem;

			if (camper_GetPlayerMoney(playerid) < gCamperClass[camper_class][camper_price_class])
			{
				SendClientMessage(
					playerid,
					COLOR_CAMPERS,
					!"Недостаточно денег"
				);
				return 0;
			}
			ShowPlayerDialog(
				playerid,
				D_CAMPER_SELECT_COLOR,
				DIALOG_STYLE_INPUT,
				!"Продавец",
				!"\n{ffffff}Введите цвет для фургона от 0 до 255\n",
				!"Выбрать",
				!"Отмена"
			);
			SetPVarInt(playerid, !"get_camper_class", camper_class);
			return 1;
		}
		case D_CAMPER_SELECT_COLOR:
		{
			if (!response)
			{
				return 0;
			}
			new camper_color = strval(inputtext);

			if (!(0 <= camper_color <= 255))
			{
				ShowPlayerDialog(
					playerid,
					D_CAMPER_SELECT_COLOR,
					DIALOG_STYLE_INPUT,
					!"Продавец",
					!"\n{ffffff}Введите цвет для фургона от 0 до 255\n",
					!"Выбрать", !"Отмена"
				);
				return 1;
			}
			new
				c_player_name[MAX_PLAYER_NAME + 1],
				string[167 + (- 4 + 6) + (- 8 + 48) + MAX_PLAYER_NAME],
				camper_class = GetPVarInt(playerid, !"get_camper_class"),
				price = gCamperClass[camper_class][camper_price_class]
			;
			GetPlayerName(playerid, c_player_name, MAX_PLAYER_NAME + 1);

			camper_SetPlayerMoney(playerid, - price);

			format(string, sizeof(string),
				"INSERT INTO `info_camper` (`Owner`, `Class`, `Color`, `CamperPosX`, `CamperPosY`, `CamperPosZ`, `CamperPosA`) \
					VALUES ('%s', '%d', '%d', '%f', '%f', '%f', '%f')",
				c_player_name,
				camper_class,
				camper_color,
				BUY_CAMPER_POS_X,
				BUY_CAMPER_POS_Y,
				BUY_CAMPER_POS_Z,
				BUY_CAMPER_POS_A

			);
			mysql_tquery(
				dbHandle,
				string,
				"@_CamperCreate",
				"ddd",
				playerid,
				camper_class,
				camper_color
			);
			return 1;
		}
		case D_CAMPER_SAFE_MENU:
		{
			if (!response)
			{
				return 0;
			}
			new
				camperid = gVehicleCamperID[playerid],
				string[24 + (- 2 + 11)]
			;
			switch (listitem)
			{
				case 0:
				{
					format(string, sizeof(string),
						"Денег в сейфе: %d$",
						CamperInfo[camperid][c_Money]
					);
					ShowPlayerDialog(
						playerid,
						D_CAMPER_SAFE_MONEY,
						DIALOG_STYLE_LIST,
						string,
						!"Взять\n\
						Положить",
						!"Выбрать",
						!"Закрыть"
					);
					return 1;
				}
				case 1:
				{
					format(string, sizeof(string),
						"Материалов в сейфе: %d$",
						CamperInfo[camperid][c_Mats]
					);
					ShowPlayerDialog(
						playerid,
						D_CAMPER_SAFE_MATS,
						DIALOG_STYLE_LIST,
						string,
						!"Взять\n\
						Положить",
						!"Выбрать",
						!"Закрыть"
					);
					return 1;
				}
				case 2:
				{
					format(string, sizeof(string),
						"Наркотиков в сейфе: %d$",
						CamperInfo[camperid][c_Drugs]
					);
					ShowPlayerDialog(
						playerid,
						D_CAMPER_SAFE_DRUGS,
						DIALOG_STYLE_LIST,
						string,
						!"Взять\n\
						Положить",
						!"Выбрать",
						!"Закрыть"
					);
					return 1;
				}
			}
		}
		case D_CAMPER_SAFE_MONEY:
		{
			if (!response)
			{
				return 0;
			}
			switch (listitem)
			{
				case 0:
				{
					new
						camperid = gVehicleCamperID[playerid],
						string[18 + (- 2 + 11)]
					;
					format(string, sizeof(string),
						"Денег в сейфе: %d$",
						CamperInfo[camperid][c_Money]
					);
					ShowPlayerDialog(
						playerid,
						D_CAMPER_SAFE_TAKE_MONEY,
						DIALOG_STYLE_INPUT,
						string,
						!"{ffffff}Введите сумму для снятия",
						!"Выбрать",
						!"Закрыть"
					);
					return 1;
				}
				case 1:
				{
					ShowPlayerDialog(
						playerid,
						D_CAMPER_SAFE_GIVE_MONEY,
						DIALOG_STYLE_INPUT,
						!"Сейф",
						!"\n{ffffff}Введите сумму, которую хотите положить в сейф\n",
						!"Выбрать", !"Закрыть"
					);
					return 1;
				}
			}
		}
		case D_CAMPER_SAFE_MATS:
		{
			if (!response)
			{
				return 0;
			}
			switch (listitem)
			{
				case 0:
				{
					new
						camperid = gVehicleCamperID[playerid],
						string[23 + (- 2 + 11)]
					;
					format(string, sizeof(string),
						"Материалов в сейфе: %d$",
						CamperInfo[camperid][c_Mats]
					);
					ShowPlayerDialog(
						playerid,
						D_CAMPER_SAFE_TAKE_MATS,
						DIALOG_STYLE_INPUT,
						string,
						!"{ffffff}Введите количество материалов, чтобы забрать",
						!"Выбрать",
						!"Закрыть"
					);
					return 1;
				}
				case 1:
				{
					ShowPlayerDialog(
						playerid,
						D_CAMPER_SAFE_GIVE_MATS,
						DIALOG_STYLE_INPUT,
						!"Сейф",
						!"\n{ffffff}Введите количество материалов, чтобы положить в сейф\n",
						!"Выбрать",
						!"Закрыть"
					);
					return 1;
				}
			}
		}
		case D_CAMPER_SAFE_DRUGS:
		{
			if (!response)
			{
				return 0;
			}
			switch (listitem)
			{
				case 0:
				{
					new
						camperid = gVehicleCamperID[playerid],
						string[23 + (- 2 + 11)]
					;
					format(string, sizeof(string),
						"Наркотиков в сейфе: %d$",
						CamperInfo[camperid][c_Drugs]
					);
					ShowPlayerDialog(
						playerid,
						D_CAMPER_SAFE_TAKE_DRUGS,
						DIALOG_STYLE_INPUT,
						string,
						!"{ffffff}Введите количество наркотиков, чтобы забрать",
						!"Выбрать",
						!"Закрыть"
					);
					return 1;
				}
				case 1:
				{
					ShowPlayerDialog(
						playerid,
						D_CAMPER_SAFE_GIVE_DRUGS,
						DIALOG_STYLE_INPUT,
						!"Сейф",
						!"\n{ffffff}Введите количество наркотиков, чтобы положить в сейф\n",
						!"Выбрать",
						!"Закрыть"
					);
					return 1;
				}
			}
			return 1;
		}
		case D_CAMPER_SAFE_TAKE_MONEY:
		{
			if (!response)
			{
				return 0;
			}
			new
				camperid = gVehicleCamperID[playerid],
				money = strval(inputtext)
			;
			if (CamperInfo[camperid][c_Money] < money)
			{
				ShowPlayerDialog(
					playerid,
					D_CAMPER_SAFE_TAKE_MONEY,
					DIALOG_STYLE_INPUT,
					!"Сейф",
					!"\n{ffffff}В сейфе нет такой суммы\n",
					!"Выбрать", !"Закрыть"
				);
				return 1;
			}
			camper_SetPlayerMoney(playerid, CamperInfo[camperid][c_Money]);

			CamperInfo[camperid][c_Money] -= money;

			new string[71 +(- 2 + 1) + (- 2 + 11)];

			format(string, sizeof(string),
				"UPDATE `info_camper` \
				SET `Money` = '%d' \
				WHERE `ID` = '%d'",
				CamperInfo[camperid][c_Money],
				CamperInfo[camperid][c_ID]
			);
			mysql_tquery(dbHandle, string);

			SendClientMessage(
				playerid,
				COLOR_CAMPERS,
				!"[Camper]: {ffffff}Вы успешно взяли деньги с сейфа."
			);
			return 1;
		}
		case D_CAMPER_SAFE_GIVE_MONEY:
		{
			if (!response)
				return 1;

			new money = strval(inputtext);

			if (camper_GetPlayerMoney(playerid) < money)
			{
				ShowPlayerDialog(
					playerid,
					D_CAMPER_SAFE_GIVE_MONEY,
					DIALOG_STYLE_INPUT,
					!"Сейф",
					!"\n{ffffff}У Вас нет такой суммы на руках\n",
					!"Выбрать",
					!"Закрыть"
				);
				return 1;
			}
			camper_SetPlayerMoney(playerid, - money);

			new	camperid = gVehicleCamperID[playerid];

			CamperInfo[camperid][c_Money] += money;

			new string[71 +(- 2 + 1) + (- 2 + 11)];

			format(string, sizeof(string),
				"UPDATE `info_camper` \
				SET `Money` = '%d' \
				WHERE `ID` = '%d'",
				CamperInfo[camperid][c_Money],
				CamperInfo[camperid][c_ID]
			);
			mysql_tquery(dbHandle, string);

			SendClientMessage(
				playerid,
				COLOR_CAMPERS,
				!"[Camper]: {ffffff}Вы успешно положили деньги в сейф."
			);
			return 1;
		}
		case D_CAMPER_SAFE_TAKE_MATS:
		{
			if (!response)
			{
				return 0;
			}
			new
				camperid = gVehicleCamperID[playerid],
				mats = strval(inputtext)
			;
			if (CamperInfo[camperid][c_Mats] < mats)
			{
				ShowPlayerDialog(
					playerid,
					D_CAMPER_SAFE_TAKE_MATS,
					DIALOG_STYLE_INPUT,
					!"Сейф",
					!"\n{ffffff}В сейфе нет такого количества материалов\n",
					!"Выбрать",
					!"Закрыть"
				);
				return 1;
			}
			camper_SetPlayerMats(playerid, CamperInfo[camperid][c_Mats]);

			CamperInfo[camperid][c_Mats] -= mats;

			new string[70 +(- 2 + 1) + (- 2 + 11)];

			format(string, sizeof(string),
				"UPDATE `info_camper` \
				SET `Mats` = '%d' \
				WHERE `ID` = '%d'",
				CamperInfo[camperid][c_Mats],
				CamperInfo[camperid][c_ID]
			);
			mysql_tquery(dbHandle, string);

			SendClientMessage(
				playerid,
				COLOR_CAMPERS,
				!"[Camper]: {ffffff}Вы успешно взяли материалы с сейфа."
			);
			return 1;
		}
		case D_CAMPER_SAFE_GIVE_MATS:
		{
			if (!response)
			{
				return 0;
			}
			new mats = strval(inputtext);

			if (camper_GetPlayerMats(playerid) < mats)
			{
				ShowPlayerDialog(
					playerid,
					D_CAMPER_SAFE_GIVE_MATS,
					DIALOG_STYLE_INPUT,
					!"Сейф",
					!"\n{ffffff}У Вас нет такого количества метериалов\n",
					!"Выбрать",
					!"Закрыть"
				);
				return 1;
			}
			camper_SetPlayerMats(playerid, mats);

			new camperid = gVehicleCamperID[playerid];

			CamperInfo[camperid][c_Mats] += mats;

			new string[70 +(- 2 + 1) + (- 2 + 11)];

			format(string, sizeof(string),
				"UPDATE `info_camper` \
				SET `Mats` = '%d' \
				WHERE `ID` = '%d'",
				CamperInfo[camperid][c_Mats],
				CamperInfo[camperid][c_ID]
			);
			mysql_tquery(dbHandle, string);

			SendClientMessage(
				playerid,
				COLOR_CAMPERS,
				!"[Camper]: {ffffff}Вы успешно положили материалы в сейф."
			);
			return 1;
		}
		case D_CAMPER_SAFE_TAKE_DRUGS:
		{
			if (!response)
			{
				return 0;
			}
			new
				camperid = gVehicleCamperID[playerid],
				drugs = strval(inputtext)
			;
			if (CamperInfo[camperid][c_Drugs] < drugs)
			{
				ShowPlayerDialog(
					playerid,
					D_CAMPER_SAFE_TAKE_DRUGS,
					DIALOG_STYLE_INPUT,
					!"Сейф",
					!"\n{ffffff}В сейфе нет такого количества наркотиков\n",
					!"Выбрать",
					!"Закрыть"
				);
				return 1;
			}
			camper_SetPlayerDrugs(playerid, CamperInfo[camperid][c_Drugs]);

			CamperInfo[camperid][c_Drugs] -= drugs;

			new string[71 +(- 2 + 1) + (- 2 + 11)];

			format(string, sizeof(string),
				"UPDATE `info_camper` \
				SET `Drugs` = '%d' \
				WHERE `ID` = '%d'",
				CamperInfo[camperid][c_Drugs],
				CamperInfo[camperid][c_ID]
			);
			mysql_tquery(dbHandle, string);

			SendClientMessage(
				playerid,
				COLOR_CAMPERS,
				!"[Camper]: {ffffff}Вы успешно взяли наркотики с сейфа."
			);
			return 1;
		}
		case D_CAMPER_SAFE_GIVE_DRUGS:
		{
			if (!response)
			{
				return 0;
			}
			new drugs = strval(inputtext);

			if (camper_GetPlayerDrugs(playerid) < drugs)
			{
				ShowPlayerDialog(
					playerid,
					D_CAMPER_SAFE_GIVE_DRUGS,
					DIALOG_STYLE_INPUT,
					!"Сейф",
					!"\n{ffffff}У Вас нет такого количества наркотиков\n",
					!"Выбрать",
					!"Закрыть"
				);
				return 1;
			}
			camper_SetPlayerDrugs(playerid, drugs);

			new camperid = gVehicleCamperID[playerid];

			CamperInfo[camperid][c_Drugs] += drugs;

			new string[71 + (- 2 + 1) + (- 2 + 11)];

			format(string, sizeof(string),
				"UPDATE `info_camper` \
				SET `Drugs` = '%d' \
				WHERE `ID` = '%d'",
				CamperInfo[camperid][c_Drugs],
				CamperInfo[camperid][c_ID]
			);
			mysql_tquery(dbHandle, string);

			SendClientMessage(
				playerid,
				COLOR_CAMPERS,
				!"[Camper]: {ffffff}Вы успешно положили наркотики в сейф."
			);
			return 1;
		}
		case D_CAMPER_SELL_MENU:
		{
			if (!response)
			{
				CamperMenu(playerid);
				return 0;
			}
			switch (listitem)
			{
				case 0:
				{
					new	
						string[52 + (- 2 + 11)],
						camperid = gVehicleCamperID[playerid],
						classid = CamperInfo[camperid][c_Class]
					;
					format(string, sizeof(string), 
						"DELETE FROM `info_camper` \
						WHERE `ID` = '%i'",
						CamperInfo[camperid][c_ID]
					);
					mysql_tquery(dbHandle, string);

					DestroyVehicle(camperid);
					ClearVehicleCamper(camperid);
				#if defined _streamer_included
					DestroyDynamic3DTextLabel(CamperInfo[camperid][c_Label]);
				#else
					Delete3DTextLabel(CamperInfo[camperid][c_Label]);
				#endif	

					DisablePlayerRaceCheckpoint(playerid);
					gVehicleCamperID[playerid] = INVALID_VEHICLE_ID;
					gPlayerSpawnCamper[playerid] = 0;

					SendClientMessage(
						playerid,
						COLOR_CAMPERS,
						!"[Camper]: {ffffff}Дом на колесах успешно продан."
					);
				#if defined foreach
					foreach(Player, i)
					#else
				for (new i = 0, size = GetPlayerPoolSize(); i <= size; ++i)
					#endif
					{
						if (gIntoCamperVehicleID[i] != camperid)
							continue;

						SetPlayerHealth(i, 0.0);
					}
					camper_SetPlayerMoney(playerid, gCamperClass[classid][camper_price_class]);
					return 1;
				}
				case 1:
				{
					if (gPlayerCamperSellId[playerid] != INVALID_PLAYER_ID)
					{
						ShowPlayerDialog(
							playerid,
							D_CAMPER_SELL_MENU,
							DIALOG_STYLE_LIST,
							"Меню ДНК",
							"{BA2821}»{ffffff} Государству\n\
							{BA2821}»{ffffff} Игроку",
							!"Выбрать",
							!"Выход"
						);
						SendClientMessage(
							playerid,
							COLOR_CAMPERS,
							!"[Camper]: {ffffff}Вы уже предложили, кому-то купить дом на колесах."
						);
						return 1;
					}
					ShowPlayerDialog(
						playerid,
						D_CAMPER_SELL_PLAYER_ID,
						DIALOG_STYLE_INPUT,
						"Меню ДНК",
						"Введите ID игрока",
						!"Далее",
						!"Назад"
					);
					return 1;
				}
			}
		}
		case D_CAMPER_SELL_PLAYER_ID:
		{
			if (!response)
			{
				ShowPlayerDialog(
					playerid,
					D_CAMPER_SELL_MENU,
					DIALOG_STYLE_LIST,
					"Меню ДНК",
					"{BA2821}»{ffffff} Государству\n\
					{BA2821}»{ffffff} Игроку",
					!"Выбрать",
					!"Выход"
				);
				return 0;
			}
			new targetid = strval(inputtext);

			if (!IsPlayerConnected(targetid))
			{
				ShowPlayerDialog(
					playerid,
					D_CAMPER_SELL_PLAYER_ID,
					DIALOG_STYLE_INPUT,
					"Меню ДНК",
					"Введите ID игрока",
					!"Далее", 
					!"Назад"
				);
				SendClientMessage(
					playerid,
					COLOR_CAMPERS,
					!"[Camper]: {ffffff}Игрок не найден."
				);
				return 1;
			}
			if (targetid == playerid)
			{
				ShowPlayerDialog(
					playerid,
					D_CAMPER_SELL_PLAYER_ID,
					DIALOG_STYLE_INPUT,
					"Меню ДНК",
					"Введите ID игрока",
					!"Далее",
					!"Назад"
				);
				SendClientMessage(
					playerid,
					COLOR_CAMPERS,
					!"[Camper]: {ffffff}Вы указали свой ID."
				);
				return 1;
			}
			if (gVehicleCamperID[targetid] != INVALID_VEHICLE_ID)
			{
				ShowPlayerDialog(
					playerid,
					D_CAMPER_SELL_PLAYER_ID,
					DIALOG_STYLE_INPUT,
					"Меню ДНК",
					"Введите ID игрока",
					!"Далее",
					!"Назад"
				);
				SendClientMessage(
					playerid,
					COLOR_CAMPERS,
					!"[Camper]: {ffffff}У этого игрока, уже есть дом на колесах."
				);
				return 1;
			}
			if (gPlayerCamperSellId[targetid] != INVALID_PLAYER_ID)
			{
				ShowPlayerDialog(
					playerid,
					D_CAMPER_SELL_PLAYER_ID,
					DIALOG_STYLE_INPUT,
					"Меню ДНК",
					"Введите ID игрока",
					!"Далее",
					!"Назад"
				);
				SendClientMessage(
					playerid,
					COLOR_CAMPERS,
					!"[Camper]: {ffffff}Этому игроку уже, кто-то предложил купить дом на колесах."
				);
				return 1;
			}
			ShowPlayerDialog(
				playerid,
				D_CAMPER_SELL_MONEY,
				DIALOG_STYLE_INPUT,
				"Меню ДНК",
				"Введите цену для продажи",
				!"Далее",
				!"Назад"
			);
			gPlayerCamperSellId[playerid] = targetid;
			gPlayerCamperSellId[targetid] = playerid;
			return 1;
		}
		case D_CAMPER_SELL_MONEY:
		{
			if (!response)
			{
				ShowPlayerDialog(
					playerid,
					D_CAMPER_SELL_PLAYER_ID,
					DIALOG_STYLE_INPUT,
					"Меню ДНК",
					"Введите ID игрока",
					!"Далее",
					!"Назад"
				);
				new targetid = gPlayerCamperSellId[playerid];

				gPlayerCamperSellId[playerid] = INVALID_PLAYER_ID;
				gPlayerCamperSellId[targetid] = INVALID_PLAYER_ID;
				return 0;
			}
			new 
				money = strval(inputtext),
				targetid = gPlayerCamperSellId[playerid]
			;
			if (camper_GetPlayerMoney(targetid) < money)
			{
				ShowPlayerDialog(
					playerid,
					D_CAMPER_SELL_MONEY,
					DIALOG_STYLE_INPUT,
					"Меню ДНК",
					"Введите цену для продажи",
					!"Далее",
					!"Назад"
				);
				SendClientMessage(
					playerid,
					COLOR_CAMPERS,
					!"[Camper]: {ffffff}У игрока недостаточно денег."
				);
				return 1;
			}
			new
				string[67 + MAX_PLAYER_NAME + (- 2 + 11)],
				camper_name[MAX_PLAYER_NAME + 1]
			;
			GetPlayerName(targetid, camper_name, MAX_PLAYER_NAME + 1);

			format(string, sizeof(string),
				"[Camper]: {ffffff}Вы предложили %s купить Ваш дом на колесах за %d$",
				camper_name,
				money
			);
			SendClientMessage(playerid, COLOR_CAMPERS, string);

			GetPlayerName(playerid, camper_name, MAX_PLAYER_NAME + 1);

			format(string, sizeof(string),
				"%s предлагает Вам купить дом на колесах за %d$",
				camper_name,
				money
			);
			ShowPlayerDialog(
				playerid,
				D_CAMPER_PLAYER_BUY_CAMPER,
				DIALOG_STYLE_MSGBOX,
				!"Покупка",
				string,
				!"Купить",
				!"Отменить"
			);
			gPlayerCamperSellMoney[targetid] = money;
			return 1;
		}
		case D_CAMPER_PLAYER_BUY_CAMPER:
		{
			new targetid = gPlayerCamperSellId[playerid];

			if (!response)
			{
				SendClientMessage(
					playerid,
					COLOR_CAMPERS,
					!"[Camper]: {ffffff}Вы отказались от покупки."
				);
				SendClientMessage(
					targetid,
					COLOR_CAMPERS,
					!"[Camper]: {ffffff}Игрок отказался от покупки."
				);
				gPlayerCamperSellId[playerid] = INVALID_PLAYER_ID;
				gPlayerCamperSellId[targetid] = INVALID_PLAYER_ID;
				gPlayerCamperSellMoney[playerid] = 0;
				return 1;
			}
			new 
				string[135 + (- 2 + 11) + MAX_PLAYER_NAME + (- 2 + MAX_LENGTH_NAME_CAMPER)],
				camper_name[MAX_PLAYER_NAME + 1],
				camperid = gVehicleCamperID[targetid],
				money = gPlayerCamperSellMoney[playerid],
				indx_class = CamperInfo[camperid][c_Class]
			;
			GetPlayerName(playerid, camper_name, MAX_PLAYER_NAME + 1);

			format(string, sizeof(string),
				"UPDATE `info_camper` SET `Owner` = '%s' WHERE `ID` = '%d' LIMIT 1",
				camper_name,
				CamperInfo[camperid][c_ID]
			);
			mysql_tquery(dbHandle, string);

			gVehicleCamperID[playerid] = camperid;
			gVehicleCamperID[targetid] = INVALID_VEHICLE_ID;

			gPlayerCamperSellMoney[playerid] = 0;
			gPlayerSpawnCamper[targetid] = 0;

			gPlayerCamperSellId[playerid] = INVALID_PLAYER_ID;
			gPlayerCamperSellId[targetid] = INVALID_PLAYER_ID;

			SendClientMessage(
				playerid,
				COLOR_CAMPERS,
				!"[Camper]: {ffffff}Вы успешно приобрели Дом на колесах."
			);
			SendClientMessage(
				playerid,
				COLOR_CAMPERS,
				!"[Camper]: {ffffff}/cmenu - меню."
			);
			SendClientMessage(
				targetid,
				COLOR_CAMPERS,
				!"[Camper]: {ffffff}Игрок купил у Вас дом на колесах."
			);
			camper_SetPlayerMoney(playerid, - money);
			camper_SetPlayerMoney(targetid, money);

			format(CamperInfo[camperid][c_Owner], MAX_PLAYER_NAME + 1, camper_name);

			format(string, sizeof(string),
				"{5d9978}Класс: {ffffff}%s\n\
				{5d9978}Владелец: {ffffff}%s\n\
				{5d9978}Дверь: {ffffff}%s\n\n\
				{ffffff}Войти в домик - [H]",
				gCamperClass[indx_class][camper_name_class],
				CamperInfo[camperid][c_Owner],
				CamperInfo[camperid][c_Lock] == 0 ? "Открыта" : "Закрыта"
			);
		#if defined _streamer_included
			UpdateDynamic3DTextLabelText(CamperInfo[camperid][c_Label], COLOR_CAMPERS, string);
		#else
			Update3DTextLabelText(CamperInfo[camperid][c_Label], COLOR_CAMPERS, string);
		#endif
			return 1;
		}
		case D_BUY_CAMPER_IMPROV_MENU:
		{
			if (!response)
				return 1;
			
			switch (listitem)
			{
				case 0:
				{
					new string[33 + (( 5 + (- 2 + MAX_LENGTH_NAME_CAMPER) + (- 2 + 11) * sizeof(gCamperClass)))];

					string = "{ffffff}Название\t{ffffff}Цена\n";

					for(new i; i < sizeof(gCamperClass); i++)
					{
						format(string, sizeof(string),
							"%s%s\t%d$\n",
							string,
							gCamperClass[i][camper_name_class],
							gCamperClass[i][camper_price_class]
						);
					}
					ShowPlayerDialog(
						playerid,
						D_BUY_CAMPER_IMPROV_INT,
						DIALOG_STYLE_TABLIST_HEADERS,
						!"Продавец",
						string,
						!"Купить",
						!"Назад"
					);
					return 1;
				}
				case 1:
				{
					ShowPlayerDialog(
						playerid,
						D_BUY_CAMPER_IMPROV_COLOR, 
						DIALOG_STYLE_INPUT,
						!"Продавец",
						!"\n{ffffff}Введите цвет для фургона от 0 до 255\n",
						!"Выбрать", !"Назад"
					);
					return 1;
				}
				case 2:
				{
					ShowPlayerDialog(
						playerid,
						D_BUY_CAMPER_IMPROV_WHEELS,
						DIALOG_STYLE_LIST,
						!"Продавец",
						!"{BA2821}»{ffffff} Shadow\n\
						{BA2821}»{ffffff} Mega\n\
						{BA2821}»{ffffff} Rimshine\n\
						{BA2821}»{ffffff} Wires\n\
						{BA2821}»{ffffff} Classic\n\
						{BA2821}»{ffffff} Twist\n\
						{BA2821}»{ffffff} Cutter\n\
						{BA2821}»{ffffff} Switch\n\
						{BA2821}»{ffffff} Grove\n\
						{BA2821}»{ffffff} Import\n\
						{BA2821}»{ffffff} Dollar\n\
						{BA2821}»{ffffff} Trance\n\
						{BA2821}»{ffffff} Atomic",
						!"Купить", !"Назад"
					);
					return 1;
				}
				case 3:
				{
					if (camper_GetPlayerMoney(playerid) < 20000)
					{
						ShowImproveMenu(playerid);
						SendClientMessage(playerid, COLOR_CAMPERS, "Недостаточно денег");
						return 0;
					}
					new 
						camperid = gVehicleCamperID[playerid],
						string[79 + (- 2 + 2) + (- 2 + 11)]
					;
					format(string, sizeof(string),
						"UPDATE `info_camper` \
						SET `Hydraulics` = '1' \
						WHERE `ID` = '%d'",
						CamperInfo[camperid][c_ID]
					);
					mysql_tquery(dbHandle, string);

					CamperInfo[camperid][c_Hydraulics] = 1;

					camper_SetPlayerMoney(playerid, - 20000);

					SendClientMessage(
						playerid,
						COLOR_CAMPERS,
						!"[Camper]: {ffffff}Гидравлика успешно установлена."
					);
					AddVehicleComponent(camperid, 1087);
					SetVehicleToRespawn(camperid);
					ShowImproveMenu(playerid);
					return 1;
				}
				case 4:
				{
					if (camper_GetPlayerMoney(playerid) < 30000)
					{
						ShowImproveMenu(playerid);
						SendClientMessage(playerid, COLOR_CAMPERS, "Недостаточно денег");
						return 0;
					}
					new
						camperid = gVehicleCamperID[playerid],
						string[74 + (- 2 + 11)]
					;
					format(string, sizeof(string),
						"UPDATE `info_camper` \
						SET `Nitro` = '1' \
						WHERE `ID` = '%d'",
						CamperInfo[camperid][c_ID]
					);
					mysql_tquery(dbHandle, string);

					CamperInfo[camperid][c_Nitro] = 1;

					camper_SetPlayerMoney(playerid, - 30000);

					SendClientMessage(
						playerid,
						COLOR_CAMPERS,
						!"[Camper]: {ffffff}Нитро успешно установлено."
					);
					AddVehicleComponent(camperid, 1010);
					SetVehicleToRespawn(camperid);
					ShowImproveMenu(playerid);
					return 1;
				}
			}
		}
		case D_BUY_CAMPER_IMPROV_INT:
		{
			if (!response)
			{
				ShowImproveMenu(playerid);
				return 0;
			}
			new
				camperid = gVehicleCamperID[playerid],
				camper_class = listitem,
				string[135 + (- 2 + 11) + MAX_PLAYER_NAME + (- 2 + MAX_LENGTH_NAME_CAMPER)],
				price = gCamperClass[camper_class][camper_price_class]
			;
			if (camper_GetPlayerMoney(playerid) < price)
			{
				ShowImproveMenu(playerid);

				SendClientMessage(
					playerid,
					COLOR_CAMPERS,
					!"Недостаточно денег"
				);
				return 0;
			}
			format(string, sizeof(string),
				"UPDATE `info_camper` \
				SET `Class` = '%d' \
				WHERE `ID` = '%d'",
				camper_class,
				CamperInfo[camperid][c_ID]
			);
			mysql_tquery(dbHandle, string);

			CamperInfo[camperid][c_Class] = camper_class;

			camper_SetPlayerMoney(playerid, - price);

			SendClientMessage(
				playerid,
				COLOR_CAMPERS,
				!"[Camper]: {ffffff}Интерьер успешно изменен."
			);
			ShowImproveMenu(playerid);

			format(string, sizeof(string),
				"{5d9978}Класс: {ffffff}%s\n\
				{5d9978}Владелец: {ffffff}%s\n\
				{5d9978}Дверь: {ffffff}%s\n\n\
				{ffffff}Войти в домик - [H]",
				gCamperClass[camper_class][camper_name_class],
				CamperInfo[camperid][c_Owner],
				CamperInfo[camperid][c_Lock] == 0 ? "Открыта" : "Закрыта"
			);
		#if defined _streamer_included
			UpdateDynamic3DTextLabelText(CamperInfo[camperid][c_Label], COLOR_CAMPERS, string);
		#else
			Update3DTextLabelText(CamperInfo[camperid][c_Label], COLOR_CAMPERS, string);
		#endif
			return 1;
		}
		case D_BUY_CAMPER_IMPROV_COLOR:
		{
			if (!response)
				return ShowImproveMenu(playerid);

			new 
				camperid = gVehicleCamperID[playerid],
				camper_color = strval(inputtext),
				string[71 + (- 2 + 2) + (- 2 + 11)]
			;
			if (!(0 <= camper_color <= 255))
			{
				ShowPlayerDialog(
					playerid,
					D_BUY_CAMPER_IMPROV_COLOR,
					DIALOG_STYLE_INPUT,
					!"Продавец",
					!"\n{ffffff}Введите цвет для фургона от 0 до 255\n",
					!"Выбрать",
					!"Отмена"
				);
				return 1;
			}
			if (camper_GetPlayerMoney(playerid) < 5000)
			{
				ShowImproveMenu(playerid);
				SendClientMessage(
					playerid,
					COLOR_CAMPERS,
					!"Недостаточно денег"
				);
				return 0;
			}
			format(string, sizeof(string),
				"UPDATE `info_camper` \
				SET `Color` = '%d' \
				WHERE `ID` = '%d'",
				camper_color,
				CamperInfo[camperid][c_ID]
			);
			mysql_tquery(dbHandle, string);

			camper_SetPlayerMoney(playerid, - 5000);

			CamperInfo[camperid][c_Color] = camper_color;

			CamperInfo[camperid][c_Color] = camper_color;

			ChangeVehicleColor(camperid, camper_color, camper_color);

			#if defined USE_CUSTOM_FUEL_FOR_CAMPER
				SetCamperFuel(camperid);
			#endif

			SendClientMessage(
				playerid,
				COLOR_CAMPERS,
				!"[Camper]: {ffffff}Цвет успешно изменен."
			);
			ShowImproveMenu(playerid);
			return 1;
		}
		case D_BUY_CAMPER_IMPROV_WHEELS:
		{
			if (!response)
				return ShowImproveMenu(playerid);

			if (camper_GetPlayerMoney(playerid) < 10000)
			{
				ShowImproveMenu(playerid);

				SendClientMessage(
					playerid,
					COLOR_CAMPERS, "Недостаточно денег"
				);
				return 0;
			}
			new
				camperid = gVehicleCamperID[playerid],
				wheelsid = 1073 + listitem,
				string[72 + (- 2 + 2) + (- 2 + 11)]
			;
			camper_SetPlayerMoney(playerid, - 10000);

			format(string, sizeof(string),
				"UPDATE `info_camper` \
				SET `Wheels` = '%d' \
				WHERE `ID` = '%d'",
				wheelsid,
				CamperInfo[camperid][c_ID]
			);
			mysql_tquery(dbHandle, string);

			CamperInfo[camperid][c_Wheels] = wheelsid;

			SendClientMessage(
				playerid,
				COLOR_CAMPERS,
				!"[Camper]: {ffffff}Диски успешно изменены."
			);
			AddVehicleComponent(camperid, wheelsid);
			SetVehicleToRespawn(camperid);
			ShowImproveMenu(playerid);
			return 1;
		}
	}
#if defined cmp_OnDialogResponse
		return cmp_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnDialogResponse
	#undef OnDialogResponse
#else
	#define _ALS_OnDialogResponse
#endif
#define OnDialogResponse cmp_OnDialogResponse
#if defined cmp_OnDialogResponse
	forward cmp_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
#endif


public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	new camperid = CamperInfo[vehicleid][c_VehicleId];

	if (vehicleid == camperid && camperid != INVALID_VEHICLE_ID)
	{
		if (CamperInfo[vehicleid][c_Lock])
		{
			ClearAnimations(playerid);

			SendClientMessage(
				playerid,
				COLOR_CAMPERS,
				!"[Camper]: {ffffff}Дом на колесах - закрыт."
			);
		}
	}
#if defined cmp_OnPlayerEnterVehicle
		return cmp_OnPlayerEnterVehicle(playerid, vehicleid, ispassenger);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerEnterVehicle
	#undef OnPlayerEnterVehicle
#else
	#define _ALS_OnPlayerEnterVehicle
#endif
#define OnPlayerEnterVehicle cmp_OnPlayerEnterVehicle
#if defined cmp_OnPlayerEnterVehicle
	forward cmp_OnPlayerEnterVehicle(playerid, vehicleid, ispassenger);
#endif

public OnVehicleSpawn(vehicleid)
{
	if (vehicleid == CamperInfo[vehicleid][c_VehicleId])
	{
		if (CamperInfo[vehicleid][c_Hydraulics])
			AddVehicleComponent(vehicleid, 1087);

		if (CamperInfo[vehicleid][c_Nitro])
			AddVehicleComponent(vehicleid, 1010);

		if (CamperInfo[vehicleid][c_Wheels])
			AddVehicleComponent(vehicleid, CamperInfo[vehicleid][c_Wheels]);

		SetVehiclePos(
			vehicleid, 
			CamperInfo[vehicleid][c_PosX],
			CamperInfo[vehicleid][c_PosY],
			CamperInfo[vehicleid][c_PosZ]
		);
		SetVehicleZAngle(
			vehicleid, 
			CamperInfo[vehicleid][c_PosA]
		);
	}
#if defined cmp_OnVehicleSpawn
		return cmp_OnVehicleSpawn(vehicleid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnVehicleSpawn
	#undef OnVehicleSpawn
#else
	#define _ALS_OnVehicleSpawn
#endif
#define OnVehicleSpawn cmp_OnVehicleSpawn
#if defined cmp_OnVehicleSpawn
	forward cmp_OnVehicleSpawn(vehicleid);
#endif


cmd:cmenu(playerid)
{
	if (gVehicleCamperID[playerid] == INVALID_VEHICLE_ID)
		return 1;
	
	CamperMenu(playerid);
	return 1;
}

@_CamperCreate(playerid, camper_class, camper_color);
@_CamperCreate(playerid, camper_class, camper_color)
{
	new
		string[135 + (- 2 + 11) + MAX_PLAYER_NAME + (- 2 + MAX_LENGTH_NAME_CAMPER)],
		camper_key = cache_insert_id()
	;
	new vehicleid = AddStaticVehicleEx(MODEL_VEHICLE_CAMPER,
		BUY_CAMPER_POS_X,
		BUY_CAMPER_POS_Y,
		BUY_CAMPER_POS_Z,
		BUY_CAMPER_POS_A,
		camper_color,
		camper_color,
		CAMPER_SPAWN
	);
	#if defined USE_CUSTOM_FUEL_FOR_CAMPER
		SetCamperFuel(vehicleid);
	#endif
	gVehicleCamperID[playerid] = vehicleid;

	CamperInfo[vehicleid][c_ID] = camper_key;
	CamperInfo[vehicleid][c_VehicleId] = vehicleid;

	GetPlayerName(playerid, CamperInfo[vehicleid][c_Owner], MAX_PLAYER_NAME + 1);

	CamperInfo[vehicleid][c_PosX] = BUY_CAMPER_POS_X;
	CamperInfo[vehicleid][c_PosY] = BUY_CAMPER_POS_Y;
	CamperInfo[vehicleid][c_PosZ] = BUY_CAMPER_POS_Z;
	CamperInfo[vehicleid][c_PosA] = BUY_CAMPER_POS_A;

	CamperInfo[vehicleid][c_Color] = camper_color;
	CamperInfo[vehicleid][c_Class] = camper_class;

	format(string, sizeof(string),
		"{5d9978}Класс: {ffffff}%s\n\
		{5d9978}Владелец: {ffffff}%s\n\
		{5d9978}Дверь: {ffffff}%s\n\n\
		{ffffff}Войти в домик - [H]",
		gCamperClass[camper_class][camper_name_class],
		CamperInfo[vehicleid][c_Owner],
		CamperInfo[vehicleid][c_Lock] == 0 ? "Открыта" : "Закрыта"
	);
#if defined _streamer_included
	CamperInfo[vehicleid][c_Label] = CreateDynamic3DTextLabel(
		string,
		COLOR_CAMPERS,
		CamperInfo[vehicleid][c_PosX],
		CamperInfo[vehicleid][c_PosY],
		CamperInfo[vehicleid][c_PosZ],
		10.0,
		INVALID_PLAYER_ID,
		vehicleid,
		1
	);
#else
	CamperInfo[vehicleid][c_Label] = Create3DTextLabel(
		string,
		COLOR_CAMPERS,
		CamperInfo[vehicleid][c_PosX],
		CamperInfo[vehicleid][c_PosY],
		CamperInfo[vehicleid][c_PosZ],
		10.0,
		-1
	);
	Attach3DTextLabelToVehicle(CamperInfo[vehicleid][c_Label], vehicleid, 0.0, 0.0, 0.0);
#endif
	SendClientMessage(
		playerid,
		COLOR_CAMPERS,
		!"[Camper]: {ffffff}Вы успешно приобрели Дом на колесах."
	);
	SendClientMessage(
		playerid,
		COLOR_CAMPERS,
		!"[Camper]: {ffffff}/cmenu - меню."
	);
	SetVehicleVirtualWorld(vehicleid, playerid+1);
}

@_OnLoadCamper(playerid);
@_OnLoadCamper(playerid)
{
	if (!cache_num_rows())
	{
		return 0;
	}
	new
		string[138 + MAX_PLAYER_NAME + (- 2 + MAX_LENGTH_NAME_CAMPER) + 7],
		camper_color,
		Float: camper_pos_x,
		Float: camper_pos_y,
		Float: camper_pos_z,
		Float: camper_pos_a
	;
	#pragma tabsize 0
	cache_get_value_name_float(0, "CamperPosX", camper_pos_x);
	cache_get_value_name_float(0, "CamperPosY", camper_pos_y);
	cache_get_value_name_float(0, "CamperPosZ", camper_pos_z);
	cache_get_value_name_float(0, "CamperPosA", camper_pos_a);
	cache_get_value_name_int(0, "Color", camper_color);
	#pragma tabsize 0
	new vehicleid =
		AddStaticVehicleEx(MODEL_VEHICLE_CAMPER,
		camper_pos_x,
		camper_pos_y,
		camper_pos_z,
		camper_pos_a,
		camper_color,
		camper_color,
		CAMPER_SPAWN
	);
	#if defined USE_CUSTOM_FUEL_FOR_CAMPER
		SetCamperFuel(vehicleid);
	#endif
	cache_get_value_name(0, "Owner", CamperInfo[vehicleid][c_Owner], MAX_PLAYER_NAME + 1);
	#pragma tabsize 0
	cache_get_value_name_int(0, "ID", CamperInfo[vehicleid][c_ID]);
	cache_get_value_name_int(0, "Lock", CamperInfo[vehicleid][c_Lock]);
	cache_get_value_name_int(0, "Money", CamperInfo[vehicleid][c_Money]);
	cache_get_value_name_int(0, "Mats", CamperInfo[vehicleid][c_Mats]);
	cache_get_value_name_int(0, "Drugs", CamperInfo[vehicleid][c_Drugs]);
	cache_get_value_name_int(0, "Class", CamperInfo[vehicleid][c_Class]);
	cache_get_value_name_int(0, "Wheels", CamperInfo[vehicleid][c_Wheels]);
	cache_get_value_name_int(0, "Hydraulics", CamperInfo[vehicleid][c_Hydraulics]);
	cache_get_value_name_int(0, "Nitro", CamperInfo[vehicleid][c_Nitro]);
	cache_get_value_name_int(0, "Spawn", gPlayerSpawnCamper[playerid]);
	#pragma tabsize 0
	
	format(string, sizeof(string),
		"{5d9978}Класс: {ffffff}%s\n\
		{5d9978}Владелец: {ffffff}%s\n\
		{5d9978}Дверь: {ffffff}%s\n\n\
		{ffffff}Войти в домик - [H]",
		gCamperClass[CamperInfo[vehicleid][c_Class]][camper_name_class],
		CamperInfo[vehicleid][c_Owner],
		CamperInfo[vehicleid][c_Lock] == 0 ? "Открыта" : "Закрыта"
	);
	#if defined _streamer_included
	CamperInfo[vehicleid][c_Label] = CreateDynamic3DTextLabel(
		string,
		COLOR_CAMPERS,
		camper_pos_x,
		camper_pos_y,
		camper_pos_z,
		10.0,
		INVALID_PLAYER_ID,
		vehicleid,
		1
	);
	#else
	CamperInfo[vehicleid][c_Label] = Create3DTextLabel(
		string,
		COLOR_CAMPERS,
		camper_pos_x,
		camper_pos_y,
		camper_pos_z,
		10.0,
		-1
	);
	Attach3DTextLabelToVehicle(CamperInfo[vehicleid][c_Label], vehicleid, 0.0, 0.0, 0.0);
	#endif

	CamperInfo[vehicleid][c_PosX] = camper_pos_x;
	CamperInfo[vehicleid][c_PosY] = camper_pos_y;
	CamperInfo[vehicleid][c_PosZ] = camper_pos_z;
	CamperInfo[vehicleid][c_PosA] = camper_pos_a;
	CamperInfo[vehicleid][c_VehicleId] = vehicleid;

	gVehicleCamperID[playerid] = vehicleid;

	if (CamperInfo[vehicleid][c_Hydraulics])
		AddVehicleComponent(vehicleid, 1087);

	if (CamperInfo[vehicleid][c_Nitro])
		AddVehicleComponent(vehicleid, 1010);

	if (CamperInfo[vehicleid][c_Wheels])
		AddVehicleComponent(vehicleid, CamperInfo[vehicleid][c_Wheels]);

	if (camper_pos_x == BUY_CAMPER_POS_X && camper_pos_y == BUY_CAMPER_POS_Y && camper_pos_z == BUY_CAMPER_POS_Z)
		SetVehicleVirtualWorld(vehicleid, playerid + 1);
	
	return 1;
}

static GetCamperVehiclePos(vehicleid, &Float:camp_x, &Float:camp_y, &Float:camp_z)
{
	new
		Float: camp_distance,
		Float: camp_a
	;
	GetVehiclePos(vehicleid, camp_x, camp_y, camp_z);
	GetVehicleModelInfo(MODEL_VEHICLE_CAMPER, 1, camp_x, camp_distance, camp_z);

	GetVehiclePos(vehicleid, camp_x, camp_y, camp_z);
	GetVehicleZAngle(vehicleid, camp_a);

	camp_x += (camp_distance/3.7 * floatsin(-camp_a+ 90, degrees));
	camp_y += (camp_distance/3.7 * floatcos(-camp_a+ 90, degrees));

}

static SetCamperInterior(playerid, Float:pos_x, Float: pos_y, Float: pos_z, Float: pos_a, world = 0, int = 0)
{
	SetPlayerPos(playerid, pos_x, pos_y, pos_z);
	SetPlayerFacingAngle(playerid, pos_a);

	SetPlayerVirtualWorld(playerid, world);
	SetPlayerInterior(playerid, int);

	SetCameraBehindPlayer(playerid);
}

static CamperMenu(playerid)
{
	new
		camperid = gVehicleCamperID[playerid],
		string[237 + 15 + (- 2 + 15)]
	;
	if (GetSpawnCamper(playerid) == 0)
	{
		format(string, sizeof(string),
			"{BA2821}»{ffffff} %s\n\
			{BA2821}»{ffffff} Место расположения\n\
			{BA2821}»{ffffff} Припарковать\n\
			{BA2821}»{ffffff} Сейф\n\
			{BA2821}»{ffffff} Возрождение\n\
			{BA2821}»{ffffff} Продать\n\
			{BA2821}»{deae61} Заспавнить",
			CamperInfo[camperid][c_Lock] == 1 ? "{076933}Открыть" : "{FF6347}Закрыть"
		);
	}
	else
	{
		format(string, sizeof(string),
			"{BA2821}»{ffffff} %s\n\
			{BA2821}»{ffffff} Место расположения\n\
			{BA2821}»{ffffff} Припарковать\n\
			{BA2821}»{ffffff} Сейф\n\
			{BA2821}»{ffffff} Возрождение\n\
			{BA2821}»{ffffff} Продать",
			CamperInfo[camperid][c_Lock] == 1 ? "{076933}Открыть" : "{FF6347}Закрыть"
		);
	}
	ShowPlayerDialog(playerid, D_CAMPER_MENU, DIALOG_STYLE_LIST, "Меню ДНК", string, !"Выбрать", !"Выход");
	return 1;

}

static GetSpawnCamper(playerid)
{
	new
		camperid = gVehicleCamperID[playerid],
		Float: camp_x = CamperInfo[camperid][c_PosX],
		Float: camp_y = CamperInfo[camperid][c_PosY],
		Float: camp_z = CamperInfo[camperid][c_PosZ]
	;
	if (camp_x == BUY_CAMPER_POS_X && camp_y == BUY_CAMPER_POS_Y && camp_z == BUY_CAMPER_POS_Z)
		return 0;
		
	return 1;
}

stock OnPlayerSpawnCamper(playerid)
{
	if (gPlayerSpawnCamper[playerid])
	{
		new
			camperid = gVehicleCamperID[playerid],
			classid = CamperInfo[camperid][c_Class]
		;
		gIntoCamperVehicleID[playerid] = camperid;

		SetCamperInterior(playerid,
			gCamperClass[classid][camper_class_pos_x],
			gCamperClass[classid][camper_class_pos_y],
			gCamperClass[classid][camper_class_pos_z],
			gCamperClass[classid][camper_class_pos_a],
			camperid  + 1,
			gCamperClass[classid][camper_int_class]
		);
		gVehicleCamperID[playerid] = camperid;
	}
}


static ClearVehicleCamper(id)
{
	CamperInfo[id][c_ID] = 0;
	CamperInfo[id][c_Owner][0] = EOS;
	CamperInfo[id][c_Class] = 0;
	CamperInfo[id][c_Lock] = 0;
	CamperInfo[id][c_Color] = 0;
	CamperInfo[id][c_Money] = 0;
	CamperInfo[id][c_Mats] = 0;
	CamperInfo[id][c_Drugs] = 0;
	CamperInfo[id][c_Wheels] = 0;
	CamperInfo[id][c_Hydraulics] = 0;
	CamperInfo[id][c_Nitro] = 0;
	CamperInfo[id][c_VehicleId] = INVALID_VEHICLE_ID;
	CamperInfo[id][c_PosX] = 0.0;
	CamperInfo[id][c_PosY] = 0.0;
	CamperInfo[id][c_PosZ] = 0.0;
	CamperInfo[id][c_PosA] = 0.0;
}

static ShowImproveMenu(playerid)
{
	ShowPlayerDialog(
		playerid,
		D_BUY_CAMPER_IMPROV_MENU,
		DIALOG_STYLE_TABLIST_HEADERS,
		"Продавец",
		"{ffffff}Название\t{ffffff}Цена\n\
		{BA2821}»{ffffff} Изменить интерьер\t-\n\
		{BA2821}»{ffffff} Изменить покраску\t5000\n\
		{BA2821}»{ffffff} Купить диски\t10000\n\
		{BA2821}»{ffffff} Установить гидравлику\t20000\n\
		{BA2821}»{ffffff} Установить нитро\t30000",
		!"Далее",
		!"Выход"
	);
	return 1;
}


stock Camper_SetMysqlConnectionHandle(CAMPER_MYSQL_TAG:handle)
{
	dbHandle = handle;
}

#if defined _streamer_included
	#define CreateCamperObject(%0,%1,%2,%3,%4,%5,%6) \
		CreateDynamicObject(%0, %1, %2, %3, %4, %5, %6, -1, -1, -1)

	#define SetCamperObjectMaterialText(%0,%1,%2,%3,%4,%5,%6,%7,%8,%9) \
		SetDynamicObjectMaterialText(%0,%1,%2,%3,%4,%5,%6,%7,%8,%9)
#else
	#define CreateCamperObject(%0,%1,%2,%3,%4,%5,%6) \
		CreateObject(%0, %1, %2, %3, %4, %5, %6)

	#define SetCamperObjectMaterialText(%0,%1,%2,%3,%4,%5) \
		SetObjectMaterial(%0,%1,%2,%3,%4,%5)
		
#endif

static CamperObject()
{
//Текст в интерьере домика
	for(new i; i < sizeof(gCamperClass); i++)
	{
	#if defined _streamer_included
		CreateDynamic3DTextLabel(
			"Для выхода нажмите - [H]",
			COLOR_CAMPERS,
			gCamperClass[i][camper_class_pos_x],
			gCamperClass[i][camper_class_pos_y],
			gCamperClass[i][camper_class_pos_z],
			15.0
		);
	#else
		Create3DTextLabel(
			"Для выхода нажмите - [H]",
			COLOR_CAMPERS,
			gCamperClass[i][camper_class_pos_x],
			gCamperClass[i][camper_class_pos_y],
			gCamperClass[i][camper_class_pos_z],
			15.0,
			-1
		);
	#endif
	}
//Продавец
#if defined _streamer_included

	printf("");
	print("Streamer подключен - Объекты, актеры и текст загружены через стример.");
	print("**********************************************************************");

	CreateDynamicActor(185, 986.5852, -1300.9677, 13.3828, 270.0248);
	CreateDynamic3DTextLabel("... Продавец ...", COLOR_CAMPERS, 986.5852, -1300.9677, 14.6, 20.0);
	CreateDynamic3DTextLabel("Нажмите - [L.Alt]", 0xffffffff, 986.5852, -1300.9677, 13.5, 20.0);
#else
	printf("");
	print("Streamer не подключен - Объекты, актеры и текст загружены через стандартные функции.");
	print("**********************************************************************");

	CreateActor(185, 986.5852, -1300.9677, 13.3828, 270.0248);
	Create3DTextLabel("... Продавец ...", COLOR_CAMPERS, 986.5852, -1300.9677, 14.6, 20.0, 0);
	Create3DTextLabel("Нажмите - [L.Alt]", 0xffffffff, 986.5852, -1300.9677, 13.5, 20.0, 0);
#endif
	//Обьекты место покупки
	new tmpobjid;
	tmpobjid = CreateCamperObject(19480, 996.878173, -1295.398681, 27.025011, -0.400000, -0.200000, -88.879882);
	SetCamperObjectMaterialText(tmpobjid, 0, "Sale Camper", 120, "Tahoma", 90, 1, -65536, -16777216, 1);

	CreateCamperObject(10832, 982.02484, -1302.25842, 14.07570,   0.00000, 0.00000, 180.00000);
	CreateCamperObject(19866, 980.41522, -1310.15771, 12.95940,   0.00000, 0.00000, 90.00000);
	CreateCamperObject(19866, 985.40021, -1310.15771, 12.95940,   0.00000, 0.00000, 90.00000);
	CreateCamperObject(19866, 990.38483, -1310.15771, 12.95940,   0.00000, 0.00000, 90.00000);
	CreateCamperObject(19866, 995.37543, -1310.15771, 12.95940,   0.00000, 0.00000, 90.00000);
	CreateCamperObject(19866, 1000.35388, -1310.15771, 12.95940,   0.00000, 0.00000, 90.00000);
	CreateCamperObject(19866, 1005.33868, -1310.15771, 12.95940,   0.00000, 0.00000, 90.00000);
	CreateCamperObject(19866, 977.99731, -1302.58374, 12.95940,   0.00000, 0.00000, 0.00000);
	CreateCamperObject(19866, 977.99731, -1307.56067, 12.95940,   0.00000, 0.00000, 0.00000);
	CreateCamperObject(19966, 1008.19287, -1310.15344, 12.75640,   0.00000, 0.00000, 180.00000);
	CreateCamperObject(19124, 1008.53418, -1310.15356, 12.92070,   0.00000, 0.00000, 0.00000);
	CreateCamperObject(19124, 1013.71008, -1310.15356, 12.92070,   0.00000, 0.00000, 0.00000);
	CreateCamperObject(738, 990.10217, -1308.57080, 12.8,   0.00000, 0.00000, 0.00000);
	CreateCamperObject(738, 997.37671, -1308.57080, 12.8,   0.00000, 0.00000, 0.00000);
	CreateCamperObject(738, 1004.58691, -1308.57080, 12.8,   0.00000, 0.00000, 0.00000);
	CreateCamperObject(19866, 1016.35681, -1310.10474, 12.50890,   0.00000, 0.00000, 90.00000);
	CreateCamperObject(19866, 978.01147, -1298.70313, 12.24330,   0.00000, 0.00000, 0.00000);
	CreateCamperObject(19866, 978.01147, -1293.73682, 12.24330,   0.00000, 0.00000, 0.00000);
	CreateCamperObject(19866, 978.01147, -1297.61951, 12.95030,   0.00000, 0.00000, 0.00000);
	CreateCamperObject(738, 981.63867, -1308.57080, 12.8,   0.00000, 0.00000, 0.00000);

//Обьекты интерьера домика на колесах int №1
	CreateObject(14385, 2511.64355, -1725.43652, 779.20868,   0.00000, 0.00000, 0.00000);
	CreateCamperObject(14386, 2511.68457, -1726.33496, 779.07117,   0.00000, 0.00000, 0.00000);
	CreateCamperObject(6959, 2509.99023, -1725.06543, 777.64142,   0.00000, 0.00000, 0.00000);
	CreateCamperObject(5856, 2509.32813, -1728.60352, 779.61633,   0.00000, 0.00000, 0.00000);
	CreateCamperObject(5856, 2508.47363, -1723.26367, 780.11633,   0.00000, 0.00000, 90.00000);
	CreateCamperObject(5856, 2511.74902, -1732.35547, 780.64136,   0.00000, 0.00000, 90.00000);
	CreateCamperObject(5856, 2512.50391, -1718.44824, 780.11633,   0.00000, 0.00000, 90.00000);
	CreateCamperObject(5856, 2508.47168, -1720.57617, 780.11633,   0.00000, 0.00000, 90.00000);
	CreateCamperObject(5856, 2513.87891, -1726.26563, 777.88574,   0.00000, 0.00000, 0.00000);
	CreateCamperObject(1535, 2513.88281, -1729.93542, 777.63409,   0.00000, 0.00000, 90.51917);
	CreateCamperObject(1506, 2513.99487, -1728.44849, 779.47076,   0.00000, 0.00000, 270.26917);
	CreateCamperObject(970, 2511.74805, -1732.07617, 778.16162,   90.00000, 179.99451, 0.00000);
	CreateCamperObject(970, 2509.78320, -1732.34277, 778.18854,   90.00000, 168.45886, 281.53564);
	CreateCamperObject(970, 2513.58301, -1732.32410, 778.08844,   90.00000, 165.52246, 104.47751);
	CreateCamperObject(970, 2513.65479, -1725.87256, 778.06342,   90.00000, 191.09619, 78.89282);
	CreateCamperObject(970, 2513.97803, -1726.01221, 778.08276,   90.00000, 179.99451, 359.00000);
	CreateCamperObject(970, 2513.85938, -1725.26660, 778.63898,   90.00000, 179.99451, 0.00000);
	CreateCamperObject(970, 2513.40234, -1723.74805, 778.63898,   90.00000, 179.99451, 90.00000);
	CreateCamperObject(970, 2513.82715, -1725.81848, 778.18854,   0.00000, 0.00000, 0.00000);
	CreateCamperObject(1271, 2509.66895, -1727.24316, 779.76306,   0.00000, 0.00000, 0.00000);
	CreateCamperObject(2238, 2509.91040, -1729.96411, 779.80548,   0.00000, 0.00000, 0.00000);
	CreateCamperObject(970, 2510.55396, -1720.80042, 778.18854,   0.00000, 90.00000, 269.98901);
	CreateCamperObject(970, 2510.74414, -1719.35400, 778.06342,   90.00000, 180.00549, 178.74170);
	CreateCamperObject(2238, 2513.51245, -1724.43384, 779.59894,   0.00000, 0.00000, 0.00000);
	CreateCamperObject(2238, 2513.54004, -1723.88220, 779.57098,   0.00000, 0.00000, 0.00000);
	CreateCamperObject(2238, 2513.49609, -1722.42236, 779.68225,   0.00000, 0.00000, 0.00000);
	CreateCamperObject(2238, 2513.49609, -1721.91296, 779.62988,   0.00000, 0.00000, 0.00000);
	CreateCamperObject(2238, 2513.51782, -1723.09290, 779.81281,   0.00000, 90.00000, 89.99994);
	CreateCamperObject(2238, 2509.82715, -1726.54150, 780.41589,   0.00000, 1.24695, 0.00000);
	CreateCamperObject(2238, 2510.29443, -1726.65588, 778.85077,   0.00000, 0.00000, 18.00000);
	CreateCamperObject(14811, 2503.91211, -1725.65723, 780.97998,   0.00000, 0.00000, 179.99451);
	CreateCamperObject(14792, 2524.25366, -1730.83765, 770.23480,   0.00000, 270.00000, 84.74207);
	CreateCamperObject(3882, 2507.34399, -1730.58899, 778.89697,   0.00000, 0.00000, 270.00000);
	CreateCamperObject(3882, 2515.62158, -1727.21484, 778.73315,   0.00000, 0.00000, 173.50000);
	CreateCamperObject(14792, 2499.67480, -1731.37695, 777.90527,   0.00000, 179.99451, 0.24719);
	CreateCamperObject(3882, 2507.34375, -1730.58594, 779.45837,   0.00000, 0.00000, 270.00000);
	CreateCamperObject(3882, 2507.34180, -1729.19165, 780.48315,   0.00000, 270.00000, 272.00000);
	CreateCamperObject(3882, 2507.34302, -1728.20898, 776.28320,   0.00330, 89.74979, 266.49097);
	CreateCamperObject(3119, 2511.17334, -1731.36597, 778.46405,   0.00000, 0.00000, 279.99756);
	CreateCamperObject(2801, 2512.41235, -1725.27368, 778.33301,   0.00000, 0.00000, 160.00000);
	CreateCamperObject(18059, 2503.31372, -1726.64905, 779.76477,   270.79053, 341.55078, 341.55261);
	CreateCamperObject(14399, 2502.31055, -1729.33398, 782.71124,   0.00000, 179.99451, 0.00000);
	CreateCamperObject(14399, 2507.34692, -1723.35071, 777.56036,   359.97723, 90.24921, 359.99988);
	CreateCamperObject(14827, 2506.91602, -1715.21484, 778.83691,   0.00000, 0.00000, 270.00000);
	CreateCamperObject(951, 2514.33398, -1729.16504, 778.62836,   0.00000, 0.00000, 90.00000);
	CreateCamperObject(14515, 2480.00684, -1686.86035, 778.87195,   0.00000, 0.00000, 91.99402);
	CreateCamperObject(14515, 2510.33594, -1711.49023, 778.10120,   0.00000, 0.00000, 0.48889);
	CreateCamperObject(14515, 2542.93457, -1703.63281, 777.85095,   0.00000, 179.49463, 269.98901);
	CreateCamperObject(14515, 2542.21289, -1768.89941, 779.00488,   0.00000, 0.00000, 270.24719);
	CreateCamperObject(18105, 2513.10962, -1738.30774, 781.04999,   0.00000, 0.00000, 89.75000);
	CreateCamperObject(18109, 2518.55176, -1718.56738, 779.46002,   0.00000, 0.00000, 359.74731);
	CreateCamperObject(2074, 2512.24878, -1727.47363, 779.84137,   0.00000, 0.00000, 338.25000);
	CreateCamperObject(2342, 2512.08130, -1730.60217, 778.27698,   0.00000, 0.00000, 90.50000);
	CreateCamperObject(2213, 2512.46143, -1727.53589, 778.55298,   334.64355, 23.07678, 342.83923);
	CreateCamperObject(2212, 2514.13452, -1724.98169, 778.72101,   334.41833, 23.12863, 77.69974);
	CreateCamperObject(14651, 2514.03516, -1729.05273, 782.37476,   0.00000, 179.99451, 89.99451);
	CreateCamperObject(2486, 2513.62598, -1728.41602, 780.11041,   0.00000, 0.00000, 354.99573);
	CreateCamperObject(2542, 2509.67285, -1727.23926, 780.11279,   7.24548, 0.00000, 90.48889);
	CreateCamperObject(14521, 2516.21582, -1719.97852, 777.89374,   359.00024, 355.74829, 56.92566);
	CreateCamperObject(14521, 2512.98560, -1717.36743, 777.76862,   0.00000, 0.00000, 356.99609);
	CreateCamperObject(8251, 2501.36523, -1720.72363, 779.16528,   0.00000, 179.99451, 0.24719);
	CreateCamperObject(3069, 2509.57324, -1716.11426, 778.48383,   0.00000, 269.74182, 173.73779);
	CreateCamperObject(2768, 2512.41431, -1727.64221, 778.57001,   0.00000, 0.00000, 90.00000);
	CreateCamperObject(2215, 2513.61768, -1725.02319, 778.71051,   334.46390, 23.40106, 10.56726);
	CreateCamperObject(1510, 2511.97144, -1727.27576, 778.53699,   0.00000, 0.00000, 0.00000);
	CreateCamperObject(1485, 2511.78125, -1727.24219, 778.56299,   0.00000, 4.25000, 0.00000);
	CreateCamperObject(3503, 2512.20288, -1727.49988, 777.14349,   0.00000, 0.00000, 179.99719);
	CreateCamperObject(1897, 2510.34766, -1723.40527, 777.56226,   0.00000, 179.99451, 270.00000);
	CreateCamperObject(14482, 2517.00391, -1733.34473, 778.15485,   0.00000, 0.00000, 270.00000);
	CreateCamperObject(2006, 2510.07617, -1726.57227, 779.75000,   90.00000, 0.00000, 5.50000);
	CreateCamperObject(1933, 2512.23779, -1727.38184, 778.43787,   0.00000, 0.00000, 0.00000);
	CreateCamperObject(1916, 2512.12622, -1727.17908, 778.51294,   0.00000, 0.00000, 0.00000);
	CreateCamperObject(2141, 2513.40967, -1721.14746, 777.66742,   0.00000, 0.00000, 269.74731);
	CreateCamperObject(2372, 2510.67285, -1725.29980, 777.46191,   0.00000, 0.00000, 0.24719);
	CreateCamperObject(11353, 2509.90430, -1706.01855, 781.41321,   0.00000, 270.00000, 0.00000);
	CreateCamperObject(3002, 2510.08984, -1726.57910, 779.82001,   0.00000, 0.00000, 249.99939);
	CreateCamperObject(11292, 2507.32617, -1722.26172, 779.37451,   0.00000, 0.00000, 269.74731);
	CreateCamperObject(11292, 2505.13867, -1730.87988, 780.05017,   0.00000, 0.00000, 179.99451);
	CreateCamperObject(5856, 2510.47266, -1718.52710, 780.11633,   0.00000, 0.00000, 180.00000);
	CreateCamperObject(2225, 2511.54834, -1730.32129, 778.16162,   0.00000, 180.00000, 0.25000);
	CreateCamperObject(2812, 2509.82324, -1723.68848, 778.48224,   0.00000, 0.00000, 340.75000);
	CreateCamperObject(2816, 2510.21362, -1731.61340, 778.29999,   357.50092, 1.50143, 332.81543);
	CreateCamperObject(2813, 2509.28662, -1729.17590, 779.66187,   275.53827, 263.26288, 82.82956);
	CreateCamperObject(2813, 2509.34277, -1729.13794, 779.52472,   70.22913, 87.48904, 356.76605);
	CreateCamperObject(2831, 2510.16040, -1724.59290, 778.70001,   0.00000, 0.00000, 156.00000);
	CreateCamperObject(2827, 2509.64233, -1727.96570, 778.78723,   0.00000, 359.50000, 25.75000);
	CreateCamperObject(2827, 2509.54468, -1727.57922, 778.67438,   43.50488, 90.23724, 266.40210);
	CreateCamperObject(2824, 2513.54150, -1726.11646, 778.28772,   359.08655, 1.00000, 89.42963);
	CreateCamperObject(2853, 2509.61719, -1727.18042, 778.94037,   0.74985, 180.25049, 70.24658);
	CreateCamperObject(14830, 2498.98999, -1721.88245, 779.26385,   0.00000, 0.00000, 270.25000);
	CreateCamperObject(2247, 2510.05200, -1723.52014, 780.00183,   334.13693, 349.43109, 160.84692);
	CreateCamperObject(2684, 2511.98950, -1727.61658, 778.54401,   271.03418, 299.16052, 219.31554);
	CreateCamperObject(1324, 2511.34741, -1720.61023, 779.22156,   0.00000, 0.50000, 269.50000);
	CreateCamperObject(7313, 2509.38794, -1728.04419, 776.30499,   359.75122, 269.97278, 90.50012);
	CreateCamperObject(7313, 2509.39185, -1728.04468, 776.06177,   359.74731, 269.97253, 90.24695);
	CreateCamperObject(12954, 2509.72559, -1735.59570, 772.49335,   338.10974, 269.99451, 179.93958);
	CreateCamperObject(1897, 2510.03979, -1729.11389, 778.63708,   89.73633, 72.40540, 287.50391);
	CreateCamperObject(2860, 2513.73828, -1721.21521, 780.16400,   0.00000, 0.00000, 164.00000);
	CreateCamperObject(1209, 2513.19287, -1721.95972, 778.08490,   0.00000, 270.00000, 90.00000);
	CreateCamperObject(2558, 2514.14819, -1727.60510, 778.66211,   0.00000, 0.00000, 90.00000);
	CreateCamperObject(2872, 2509.50293, -1727.84033, 777.83728,   344.75000, 0.00000, 106.50000);
	CreateCamperObject(18866, 2511.70630, -1730.56201, 778.14502,   0.00000, 0.00000, 40.00000);
	CreateCamperObject(18635, 2513.01611, -1725.57214, 778.66998,   90.00000, 111.00000, 0.00000);
	CreateCamperObject(2673, 2509.37109, -1728.60034, 779.81000,   0.00000, 0.00000, 239.00000);

//Воторой инт
	tmpobjid = CreateObject(19378,2483.471,-867.949,2882.313,0.000,90.000,0.000);
	SetObjectMaterial(tmpobjid, 0, 2755, "ab_dojowall", "mp_apt1_roomfloor", 0);
	tmpobjid = CreateObject(19378,2493.966,-867.949,2882.313,0.000,90.000,0.000);
	SetObjectMaterial(tmpobjid, 0, 2755, "ab_dojowall", "mp_apt1_roomfloor", 0);
	tmpobjid = CreateCamperObject(19387,2483.029,-872.630,2884.149,0.000,0.000,90.000);
	SetCamperObjectMaterial(tmpobjid, 0, 3603, "bevmans01_la", "hottop5d_law", 0);
	SetCamperObjectMaterial(tmpobjid, 2, 14581, "ab_mafiasuitea", "mp_burn_ceiling", 0);
	tmpobjid = CreateCamperObject(19357,2479.823,-872.630,2884.149,0.000,0.000,90.000);
	SetCamperObjectMaterial(tmpobjid, 0, 3603, "bevmans01_la", "hottop5d_law", 0);
	tmpobjid = CreateCamperObject(19357,2486.210,-872.630,2884.149,0.000,0.000,90.000);
	SetCamperObjectMaterial(tmpobjid, 0, 3603, "bevmans01_la", "hottop5d_law", 0);
	SetCamperObjectMaterial(tmpobjid, 2, 14581, "ab_mafiasuitea", "mp_burn_ceiling", 0);
	tmpobjid = CreateCamperObject(19357,2489.416,-872.630,2884.149,0.000,0.000,90.000);
	SetCamperObjectMaterial(tmpobjid, 0, 3603, "bevmans01_la", "hottop5d_law", 0);
	tmpobjid = CreateCamperObject(19449,2478.272,-867.857,2884.149,0.000,0.000,0.000);
	SetCamperObjectMaterial(tmpobjid, 0, 3603, "bevmans01_la", "hottop5d_law", 0);
	tmpobjid = CreateCamperObject(19449,2483.018,-868.284,2884.149,0.000,0.000,90.000);
	SetCamperObjectMaterial(tmpobjid, 0, 3603, "bevmans01_la", "hottop5d_law", 0);
	tmpobjid = CreateCamperObject(1429,2482.362,-868.741,2883.456,0.000,0.000,315.000);
	SetCamperObjectMaterial(tmpobjid, 0, 14581, "ab_mafiasuitea", "cof_wood2", 0);
	SetCamperObjectMaterial(tmpobjid, 1, 14803, "bdupsnew", "Bdup2_poster", 0);
	SetCamperObjectMaterial(tmpobjid, 2, 14581, "ab_mafiasuitea", "cof_wood2", 0);
	tmpobjid = CreateCamperObject(936,2484.566,-871.661,2882.875,0.000,0.000,90.000);
	SetCamperObjectMaterial(tmpobjid, 1, 2023, "bitsnbobs", "CJ_LIGHTWOOD", 0);
	tmpobjid = CreateCamperObject(1796,2484.016,-868.870,2882.399,0.000,0.000,270.000);
	SetCamperObjectMaterial(tmpobjid, 1, 9515, "bigboxtemp1", "board64_law", 0);
	SetCamperObjectMaterial(tmpobjid, 2, 9583, "bigshap_sfw", "bridge_walls2_sfw", 0);
	tmpobjid = CreateCamperObject(19357,2489.371,-868.284,2884.149,0.000,0.000,90.000);
	SetCamperObjectMaterial(tmpobjid, 0, 3603, "bevmans01_la", "hottop5d_law", 0);
	tmpobjid = CreateCamperObject(19387,2487.961,-871.015,2884.149,0.000,0.000,0.000);
	SetCamperObjectMaterial(tmpobjid, 0, 3603, "bevmans01_la", "hottop5d_law", 0);
	tmpobjid = CreateCamperObject(19357,2487.961,-867.809,2884.149,0.000,0.000,0.000);
	SetCamperObjectMaterial(tmpobjid, 0, 3603, "bevmans01_la", "hottop5d_law", 0);
	tmpobjid = CreateCamperObject(2267,2486.178,-868.401,2884.394,0.000,0.000,0.000);
	SetCamperObjectMaterial(tmpobjid, 1, 14489, "carlspics", "AH_landscap1", 0);
	tmpobjid = CreateCamperObject(2266,2478.852,-870.510,2883.994,0.000,0.000,90.000);
	SetCamperObjectMaterial(tmpobjid, 1, 14489, "carlspics", "AH_picture2", 0);
	tmpobjid = CreateCamperObject(1498,2482.249,-872.616,2882.389,0.000,0.000,0.000);
	SetCamperObjectMaterial(tmpobjid, 1, 14489, "carlspics", "AH_landscap1", 0);
	tmpobjid = CreateCamperObject(19449,2490.855,-867.859,2884.149,0.000,0.000,0.000);
	SetCamperObjectMaterial(tmpobjid, 0, 3603, "bevmans01_la", "hottop5d_law", 0);
	tmpobjid = CreateCamperObject(19379,2483.471,-867.949,2885.989,0.000,90.000,0.000);
	SetCamperObjectMaterial(tmpobjid, 0, 6282, "beafron2_law2", "boardwalk2_la", 0);
	tmpobjid = CreateCamperObject(19379,2493.966,-867.949,2885.989,0.000,90.000,0.000);
	SetCamperObjectMaterial(tmpobjid, 0, 6282, "beafron2_law2", "boardwalk2_la", 0);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	tmpobjid = CreateCamperObject(19357,2492.624,-872.630,2884.149,0.000,0.000,90.000);
	tmpobjid = CreateCamperObject(1764,2478.782,-871.443,2882.399,0.000,0.000,90.000);
	tmpobjid = CreateCamperObject(936,2485.895,-872.145,2882.874,0.000,0.000,180.000);
	tmpobjid = CreateCamperObject(1744,2482.279,-868.370,2884.281,0.000,0.000,0.000);
	tmpobjid = CreateCamperObject(1765,2479.804,-868.781,2882.399,0.000,0.000,0.000);
	tmpobjid = CreateCamperObject(1765,2480.892,-872.124,2882.399,0.000,0.000,180.000);
	tmpobjid = CreateCamperObject(1819,2479.912,-870.947,2882.399,0.000,0.000,0.000);
	tmpobjid = CreateCamperObject(2141,2487.344,-872.057,2882.399,0.000,0.000,180.000);
	tmpobjid = CreateCamperObject(2328,2482.430,-869.859,2882.399,0.000,0.000,0.000);
	tmpobjid = CreateCamperObject(2866,2486.052,-872.015,2883.350,0.000,0.000,0.000);
	tmpobjid = CreateCamperObject(2859,2480.482,-870.484,2882.916,0.000,0.000,0.000);
	tmpobjid = CreateCamperObject(2384,2483.260,-868.635,2884.726,0.000,0.000,0.000);
	tmpobjid = CreateCamperObject(2103,2482.377,-868.668,2884.623,0.000,0.000,0.000);
	tmpobjid = CreateCamperObject(2149,2485.327,-872.333,2883.500,0.000,0.000,180.000);
	tmpobjid = CreateCamperObject(2225,2481.905,-868.896,2882.399,0.000,0.000,270.000);
	tmpobjid = CreateCamperObject(630,2479.093,-868.882,2883.425,0.000,0.000,0.000);
	tmpobjid = CreateCamperObject(630,2479.062,-872.059,2883.425,0.000,0.000,0.000);
	tmpobjid = CreateCamperObject(2527,2490.177,-869.851,2882.399,0.000,0.000,0.000);
	tmpobjid = CreateCamperObject(2525,2489.458,-872.057,2882.399,0.000,0.000,180.000);
	tmpobjid = CreateCamperObject(2524,2490.271,-870.672,2882.399,0.000,0.000,270.000);
	tmpobjid = CreateCamperObject(1846,2489.774,-871.088,2884.299,90.000,90.000,0.000);
	tmpobjid = CreateCamperObject(2074,2483.460,-870.151,2885.628,0.000,0.000,0.000);
	tmpobjid = CreateCamperObject(1494,2487.952,-871.763,2882.399,0.000,0.000,90.000);
}