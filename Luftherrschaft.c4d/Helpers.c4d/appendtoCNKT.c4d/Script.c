/* Bausatz - Appendto aufgrund des Materialsystems */
#strict 2
#appendto CNKT

local aMenuItems;

public func Activate(pClonk)
{ 
  [$TxtCreateconstructionsi$]
  // Clonk anhalten
  SetComDir(COMD_Stop, pClonk);
  // Kann der Clonk überhaupt bauen?
  if (pClonk && !GetPhysical("CanConstruct", PHYS_Current, pClonk))
    { PlayerMessage(GetController(pClonk), "$TxtCantConstruct$", this, GetName(pClonk)); return false; }
  // Menü erzeugen und mit Bauplänen des Spielers füllen
  CreateMenu(CXCN, pClonk, this, 1, "$TxtNoconstructionplansa$");
  var idType, i = 0;
  aMenuItems = CreateArray();
  while (idType = GetPlrKnowledge(GetOwner(pClonk), 0, i++, C4D_Structure))
    // Keine Trapper/Indianer Sachen mit Bausatz bauen. (Außer für Wachtürme/Palisaden, die es ausdrücklich zulassen)
    if(idType->~IsConkitBuilding() || (!idType->~IsIndianHandcraft() && !idType->~IsTrapperHandcraft())) {
      AddMenuItem("$TxtConstructions$", "CreateConstructionSite", idType, pClonk);
      aMenuItems[i - 1] = idType;
  }
  CheckMenu();
  return 1;
}

protected func CheckMenu() {
	if(GetMenu(Contained()) != CXCN || !FindObject(CNMT))
		return;
	var idType = aMenuItems[GetMenuSelection(Contained())];
	if(!idType)
		return;
	GetMatSys() -> MaterialCheck(idType);
	ScheduleCall(this, "CheckMenu", 5);
}

protected func MenuQueryCancel() {
	GetMatSys() -> LocalN("fNoStatusMessage") = 0;
	ClearScheduleCall(this, "CheckMenu");
}

private func GetMatSys() {
	return FindObject2(Find_ID(PLMT), Find_Owner(GetOwner(Contained())));
}

protected func CreateConstructionSite(id idType)
{
	MenuQueryCancel();
  // Nur wenn der Clonk steht und sich im Freien befindet
  if (GetAction(Contained()) != "Walk") return;
  if (Contained(Contained())) return;
  // Pruefen, ob das Gebaeude hier gebaut werden kann
  if (idType->~RejectConstruction(0, 10, Contained()) ) return;
  // Hat das Materialsystem genügen Material?
  var	iNeededWood = idType -> ~GetNeededWood(),
  		iNeededBrick = idType -> ~GetNeededBrick(),
  		iNeededTool = idType -> ~GetNeededTool();
  if(FindObject(CNMT) && (
  	GetMatSys() -> GetAmount(WOOD) < iNeededWood  ||
  	GetMatSys() -> GetAmount(BRIK) < iNeededBrick ||
  	GetMatSys() -> GetAmount(TOOL) < iNeededTool
  )) return;
  GetMatSys() -> DoFill(-iNeededWood, WOOD);
  GetMatSys() -> DoFill(-iNeededBrick, BRIK);
  GetMatSys() -> DoFill(-iNeededTool, TOOL);
  return inherited(idType, ...);
}
