/*-- Spielermaterialien --*/

#strict 2

#include SAVS

// evtl. variabel als local?
static const PLMT_MaxFill_WOOD = 1000;
static const PLMT_MaxFill_BRIK = 1000;
static const PLMT_MaxFill_TOOL = 1000;
//local wood, ziegel, werkzeug;
local pWoodIcon, pBrickIcon, pToolIcon;

public func Initialize() {
  pWoodIcon = CreateObject(LHII, 0, 0, GetOwner());
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
  pToolIcon -> Set("TOOL");
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
  pWoodIcon->SetStatusMessage(Format("@%d", GetAmount(WOOD)));
  pBrickIcon->SetStatusMessage(Format("@%d", GetAmount(BRIK)));
  pToolIcon->SetStatusMessage(Format("@%d", GetAmount(TOOL)));
}

public func MaterialCheck(id idType) {
	fNoStatusMessage = 1;
	pWoodIcon -> BuildMessage(idType -> ~GetNeededWood(), GetAmount(WOOD));
	pBrickIcon -> BuildMessage(idType -> ~GetNeededBrick(), GetAmount(BRIK));
	pToolIcon -> BuildMessage(idType -> ~GetNeededTool(), GetAmount(TOOL));
}
