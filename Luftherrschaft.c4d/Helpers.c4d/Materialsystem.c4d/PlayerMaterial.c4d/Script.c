/*-- Spielermaterialien --*/

#strict 2

#include SAVS

// evtl. variabel als local?
//static const PLMT_MaxFill = 1000;
private func MaxFill() { return 1000; }
/*static const PLMT_MaxFill_WOOD = 1000;
static const PLMT_MaxFill_BRIK = 1000;
static const PLMT_MaxFill_TOOL = 1000;*/

static aMaterialSystem;
//local wood, ziegel, werkzeug;
local hIcons; // HashGet(GetMatSys()->LocalN("hIcons"), WOOD)

public func Initialize() {
	if(GetType(aMaterialSystem) != C4V_Array)
		aMaterialSystem = CreateArray(GetPlayerCount());
	aMaterialSystem[GetOwner()] = this;
	var aIDs = GetMatSysIDs();
	var iX = -166, pIcon;
	hIcons = CreateHash();
	for(var idObj in aIDs) {
		pIcon = CreateObject(LHII, 0, 0, GetOwner());
		pIcon -> SetPosition(iX);
		pIcon -> Set(idObj);
		var iWidth = 566, iHeight = 566, iXAdjust = 28000, iYAdjust = 13000;
		if(idObj != BRIK && idObj != TOOL) {
			iXAdjust *= 2;
			iXAdjust += 3000;
			iYAdjust *= 2;
			iWidth += 150;
			iHeight += 34;
		}
		pIcon -> SetObjDrawTransform(iWidth, 0, iXAdjust, 0, iHeight, iYAdjust, 0, GFX_Overlay);
		HashPut(hIcons, idObj, pIcon);
		iX -= 40;
	}
  /*pWoodIcon = CreateObject(LHII, 0, 0, GetOwner());
  pWoodIcon -> SetPosition(-246, +0);
  SetObjDrawTransform(566, 0, 0, 0, 566, 0, pWoodIcon);
  pWoodIcon -> Set("WOOD");
  pBrickIcon = CreateObject(LHII, 0, 0, GetOwner());
  pBrickIcon -> SetPosition(-206, +0);
  SetObjDrawTransform(566, 0, 0, 0, 566, 0, pBrickIcon);
  pBrickIcon -> Set("BRIK");
  pToolIcon = CreateObject(LHII, 0, 0, GetOwner());
  pToolIcon->SetPosition(-166, +0);
  SetObjDrawTransform(566, 0, 0, 0, 566, 0, pToolIcon);
  pToolIcon -> Set("TOOL");*/
  /*
  Kurzinfo
  Endgröße 34 x 17 
  Position -6 x 5 gold
  pos      -46 x 5 score
  pos      -86 x 5 crew
  -->
  pos      -126 x 5 tool
  pos      -166 x 5 brick
  pos      -206 x 5 wood
  und 60 mehr, weil Offset bei -1 x -1 ist.
  */
}

local fNoStatusMessage;
public func Timer() {
	if(fNoStatusMessage)
		return;
	var iter = HashIter(hIcons), node;
	while(node = HashIterNext(iter))
		node[1] -> SetStatusMessage(Format("@%d", GetAmount(node[0])));
  /*pWoodIcon->SetStatusMessage(Format("@%d", GetAmount(WOOD)));
  pBrickIcon->SetStatusMessage(Format("@%d", GetAmount(BRIK)));
  pToolIcon->SetStatusMessage(Format("@%d", GetAmount(TOOL)));*/
}

public func MaterialCheck(id idType) {
	fNoStatusMessage = 1;
	var iter = HashIter(hIcons), node;
	while(node = HashIterNext(iter))
		node[1] -> BuildMessage(idType -> GetDefCoreComponent(node[0]), GetAmount(node[0]));
	/*pWoodIcon -> BuildMessage(idType -> GetDefCoreComponent(WOOD), GetAmount(WOOD));
	pBrickIcon -> BuildMessage(idType -> GetDefCoreComponent(BRIK), GetAmount(BRIK));
	pToolIcon -> BuildMessage(idType -> GetDefCoreComponent(TOOL), GetAmount(TOOL));*/
}

global func GetMatSys(int iPlr) {
	return aMaterialSystem[iPlr];
}
global func MatSysGetAmount(int iPlr, id Key) {
	return aMaterialSystem[iPlr] -> GetAmount(Key);
}

global func MatSysDoFill(int iChange, int iPlr, id Key) {
	return aMaterialSystem[iPlr] -> DoFill(iChange, Key);
}

global func GetMatSysIDs() {
	var aIDs = [TOOL, BRIK, WOOD];
	FusionArrays(aIDs, GameCall("SpecialMatSysIDs")); // vertauschte Reihenfolge, da von rechts nach links
	return aIDs;
}
