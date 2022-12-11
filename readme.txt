Установка:

Ко всем include
#include <camper>

В OnGameModeInit после подключения к БД
Camper_SetMysqlConnectionHandle(/*переменная для подключения*/); //Например dbHandle


В самый конец OnPlayerSpawn добавляем:
OnPlayerSpawnCamper(playerid);

В конец мода


//Деньги
stock camper_GetPlayerMoney(playerid)
    return PlayerInfo[playerid][pMoney]; //PlayerInfo[playerid][pMoney] Меняем на свое

stock camper_SetPlayerMoney(playerid, cmp_money)
{
    PlayerInfo[playerid][pMoney] += cmp_money; //PlayerInfo[playerid][pMoney] Меняем на свое
}

//Маты
stock camper_GetPlayerMats(playerid)
    return PlayerInfo[playerid][pMats]; //PlayerInfo[playerid][pMats] Меняем на свое

stock camper_SetPlayerMats(playerid, cmp_mats)
{
    layerInfo[playerid][pMats] += cmp_mats; //PlayerInfo[playerid][pMats] Меняем на свое
}

//Наркотики
stock camper_GetPlayerDrugs(playerid)
    return PlayerInfo[playerid][pDrugs]; //PlayerInfo[playerid][pDrugs] Меняем на свое

stock camper_SetPlayerDrugs(playerid, cmp_drugs, bool: cmp_action)
{
    PlayerInfo[playerid][pDrugs] += cmp_drugs; //PlayerInfo[playerid][pDrugs] Меняем на свое
}