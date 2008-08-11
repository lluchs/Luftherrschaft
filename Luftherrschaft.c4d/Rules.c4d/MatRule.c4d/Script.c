/*-- Materialregel --*/

#strict 2

/*func Initialize() {
  var plr, rest_plrs = GetPlayerCount();
  for(plr; rest_plrs > 0; plr++) {
    // hat einen Spielernamen und ist kein Skriptspieler?
    if(GetPlayerName(plr) && GetPlayerType(plr) != C4PT_Script) {
      CreateObject(PLMT,,,plr);
      rest_plrs--;
    }
  }
  return 1;
}

// Timer alle 300 Frames, falls Laufzeitbeitritt aktiv.
public func Timer() {
  var plr, rest_plrs = GetPlayerCount();
  for(plr; rest_plrs > 0; plr++) {
    // hat einen Spielernamen und ist kein Skriptspieler?
    if(GetPlayerName(plr) && GetPlayerType(plr) != C4PT_Script) {
      if(!FindObject2(Find_ID(PLMT), Find_Owner(plr)))
        CreateObject(PLMT,,,plr);
      rest_plrs--;
    }
  }
}*/

protected func InitializePlayer(int iPlr) {
	if(GetPlayerType(iPlr) != C4PT_Script)
		CreateObject(PLMT, 0, 0, iPlr);
}

public func Activate(int iByPlr) {
  // Bla... Beschreibung ...Bla
  MessageWindow(GetDesc(), iByPlr);
}
