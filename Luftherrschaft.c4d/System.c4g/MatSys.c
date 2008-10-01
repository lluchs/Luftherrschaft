/* Globale Materialsystemfunktionen */

#strict 2

global func GetMatSys(int iPlr, bool bDoNotCreateMatSys) {
	if(GetType(aMaterialSystem) != C4V_Array)	{
		if(!bDoNotCreateMatSys)
			CreateObject(LHMT, 0, 0, NO_OWNER);
		else
			return;
	}
	return aMaterialSystem[iPlr];
}
global func MatSysGetAmount(int iPlr, id Key) {
	return GetMatSys(iPlr) -> GetAmount(Key);
}

global func MatSysDoFill(int iChange, int iPlr, id Key) {
	return GetMatSys(iPlr) -> DoFill(iChange, Key);
}

global func GetMatSysIDs() {
	var aIDs = [TOOL, BRIK, WOOD, CRYS];
	FusionArrays(aIDs, GameCall("SpecialMatSysIDs")); // vertauschte Reihenfolge, da von rechts nach links
	return aIDs;
}
