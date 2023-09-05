## Установка
- Ко всем ```include```
```pawn
#define USE_CUSTOM_FUEL_FOR_CAMPER //Если нужно добавить топливо из вашего мода
#include <camper>
```

- В ```OnGameModeInit``` после подключения к БД
```pawn
camper_InitMysqlHandle(/*переменная для подключения*/); //Например dbHandle
```


- В самый конец ```OnPlayerSpawn``` добавляем:
```pawn
OnPlayerSpawnCamper(playerid);
```


- В конец мода
```pawn
//Топливо
stock SetCamperFuel(vehicleid)
{
    VehicleInfo[vehicleid] = 100; // VehicleInfo Меняем на свой массив, где хранится топливо
}

//Деньги
stock camper_GetPlayerMoney(playerid)
{
    return PlayerInfo[playerid][pMoney]; //PlayerInfo[playerid][pMoney] Меняем на свое
}

stock camper_SetPlayerMoney(playerid, cmp_money)
{
    PlayerInfo[playerid][pMoney] += cmp_money; //PlayerInfo[playerid][pMoney] Меняем на свое
}

//Маты
stock camper_GetPlayerMats(playerid)
{
    return PlayerInfo[playerid][pMats]; //PlayerInfo[playerid][pMats] Меняем на свое
}

stock camper_SetPlayerMats(playerid, cmp_mats)
{
    layerInfo[playerid][pMats] += cmp_mats; //PlayerInfo[playerid][pMats] Меняем на свое
}

//Наркотики
stock camper_GetPlayerDrugs(playerid)
{
    return PlayerInfo[playerid][pDrugs]; //PlayerInfo[playerid][pDrugs] Меняем на свое
}

stock camper_SetPlayerDrugs(playerid, cmp_drugs)
{
    PlayerInfo[playerid][pDrugs] += cmp_drugs; //PlayerInfo[playerid][pDrugs] Меняем на свое
}
```
