/*-- Materialregel --*/

#strict 2

protected func Initialize() {
	if(!FrameCounter())
		return;
  var iPlr, iRest = GetPlayerCount();
  for(iPlr; iRest > 0; iPlr++) {
    // hat einen Spielernamen und ist kein Skriptspieler?
    if(GetPlayerName(iPlr) && GetPlayerType(iPlr) != C4PT_Script) {
      CreateObject(LHMS, 0, 0, iPlr);
      iRest--;
    }
  }
  return 1;
}
/*
// Timer alle 300 Frames, falls Laufzeitbeitritt aktiv.
public func Timer() {
  var iPlr, iRest = GetPlayerCount();
  for(iPlr; iRest > 0; iPlr++) {
    // hat einen Spielernamen und ist kein Skriptspieler?
    if(GetPlayerName(iPlr) && GetPlayerType(iPlr) != C4PT_Script) {
      if(!FindObject2(Find_ID(PLMT), Find_Owner(iPlr)))
        CreateObject(PLMT,,,iPlr);
      iRest--;
    }
  }
}*/

protected func InitializePlayer(int iPlr) {
	if(GetPlayerType(iPlr) != C4PT_Script)
		CreateObject(LHMS, 0, 0, iPlr);
}

public func Activate(int iByPlr) {
  // Bla... Beschreibung ...Bla
  MessageWindow(GetDesc(), iByPlr);
}

global func RecreateMatSys() {
	for(var pObj in FindObjects(Find_Or(Find_ID(PLMT), Find_ID(LHII))))
		RemoveObject(pObj);
	return ProtectedCall(FindObject(LHMT), "Initialize");
}
