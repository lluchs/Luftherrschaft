/*-- Barometerregel --*/

#strict 2

protected func Initialize() {
	if(!FrameCounter())
		return;
  var iPlr, iRest = GetPlayerCount();
  for(iPlr; iRest > 0; iPlr++) {
    // hat einen Spielernamen und ist kein Skriptspieler?
    if(GetPlayerName(iPlr) && GetPlayerType(iPlr) != C4PT_Script) {
      CreateObject(BRMT, 0, 0, iPlr);
      iRest--;
    }
  }
  return 1;
}
