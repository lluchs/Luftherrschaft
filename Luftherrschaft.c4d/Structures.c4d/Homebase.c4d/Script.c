/*-- Heimathaus --*/

#strict

#include L103
#include DOOR
#include LHSS

//static const LBSE_MaxFill = 500;
private func MaxFill() { return 500; }

private func FillSound() { return Sound("pfft"); }

/*
protected func Collection(object pObj) {
	if(GetID(pObj) != LCAB)
		return;
	var iBottleChange = pObj -> DoFill(pObj -> MaxFill());
	var iChange = DoFill(-iBottleChange);
	var iChange2 = iChange + iBottleChange;
	pObj -> BarrelDoFill(-iChange2);
}
*/

local fSound;
local target;
local progress;

local restime; // Ben�tigte Gesamtzeit zum Forschen
  
/* Forschung */

protected func ResearchObjects(idImage, pClonk) { return(SelectResearchCategory(idImage, pClonk, C4D_Object())); }
protected func ResearchVehicles(idImage, pClonk) { return(SelectResearchCategory(idImage, pClonk, C4D_Vehicle())); }
protected func ResearchStructures(idImage, pClonk) { return(SelectResearchCategory(idImage, pClonk, C4D_Structure())); }

public func ResearchMenu(clonk)
{
  // Hauptmen� erzeugen
  CreateMenu(RSR1, clonk, this());
  if (target) AddMenuItem("$TxtContinuedevelopement$", "SelectResearch", target, clonk, 0, clonk);
  AddMenuItem("$TxtResearchs$", "ResearchObjects", RSR3, clonk, 0, clonk);
  AddMenuItem("$TxtResearchs$", "ResearchVehicles", RSR4, clonk, 0, clonk);
  AddMenuItem("$TxtResearchs$", "ResearchStructures", RSR5, clonk, 0, clonk);
}

protected func SelectResearchCategory(idImage, pClonk, dwCategory)
{
  // Auswahlmen� erzeugen
  CreateMenu(idImage, pClonk, this(), 0, "$TxtNothingelsetoresearc$");
  // Alle geladenen Baupl�ne �berpr�fen
  var i,idDef,iPlr=GetOwner(pClonk);
  while (idDef = GetDefinition(i++, dwCategory))
    // Passende Kategorie
    if (GetCategory(0, idDef) & C4D_Knowledge())
      // Dem Spieler noch nicht bekannt
      if (!GetPlrKnowledge(iPlr,idDef))
      {
        // Keine Forschungsgrundlage n�tig oder Forschungsgrundlage vorhanden
        var idBase = DefinitionCall(idDef,"GetResearchBase"), fHasBase, j;
        fHasBase = !idBase || GetPlrKnowledge(iPlr,idBase); j=1;
        // Erweiterte Forschungsgrundlagen
        while (idBase = DefinitionCall(idDef, Format("GetResearchBase%d", ++j)))
          if (!GetPlrKnowledge(iPlr,idBase)) fHasBase=0;
        // Ist schon durch etwas anderes erforschbar? (Zugbr�cke rechts -> Zugbr�cke links etc.)
        var link;
        if (link = DefinitionCall(idDef, "GetResearchLink"))
          if (GetName(0, link)) // Linkdefinition auch tats�chlich geladen?
            if(!GetPlrKnowledge(iPlr, link) ) // Linkdefinition noch nicht erforscht? (passiert durch SetPlrKnowledge-Aufrufe oder Szenariovorgaben)
              continue;
        // Men�eintrag hinzuf�gen
        if (fHasBase) AddMenuItem("$TxtResearchs$", "SelectResearch", idDef, pClonk, 0, pClonk);
      }
  return(1);  
}

public func SelectResearch(def, clonk)
{
  // Anfangen
  AddCommand(clonk, "Call", this(), def, 0, 0, 0, "StartResearch", 10, 1);
  // Vorher ins Labor gehen
  if (Contained(clonk) != this())
  {
    AddCommand(clonk, "Wait", 0, 0, 0, 0, 40);
    AddCommand(clonk, "Enter", this());
  } 
}
  
private func CancelResearch()
{
  if (GetChar(GetAction()) == GetChar("R"))
  	SetAction("Idle");
  //SetEntrance(1);
  // Abbrechen ohne Erfolg?
  if(target) {
  	// Druckverlust
  	DoFill(-Random(50));
  }
  return 1;
}

protected func StartResearch(clonk, def)
{
  // Spieler hat diesen Bauplan schon
  if (GetPlrKnowledge(clonk->GetOwner(), def))
  {
    Message("$TxtAlreadyDeveloped$", this(), GetName(0, def));
    Sound("Error");
    return; 
  }
  // Bisherige Forschung abbrechen
  CancelResearch();
  // nur, wenn ansonsten inaktiv oder schon am Forschen
  if (!ActIdle()) if (GetChar(GetAction()) != GetChar("R")) return(0);
  // Gesamtzeit setzen
  restime = DefinitionCall(def, "GetResearchTime", this(), clonk);
  if(!restime)
  	restime = 100;
  // Druckcheck
	if(GetAmount() < restime) {
		var aFlaschen = FindObjects(Find_Container(this), Find_ID(LCAB), Find_Func("GetAmount"));
		for(var pFlasche in aFlaschen) {
			var iDiff = restime - GetAmount();
			DoFill(Abs(pFlasche -> DoFill(-iDiff)));
			if(GetAmount() == restime)
				break;
		}
	}
	if(GetAmount() < restime) {
		PlayerMessage(GetOwner(), "$TxtNoCompressedAirBottle$", this);
		Sound("Error");
		return;
	}
  // Besitzer anpassen (der Besitzer des forschenden Clonks erh�lt den Bauplan)
  SetOwner(GetOwner(clonk));
  // Neues Forschungsziel: Fortschritt zur�cksetzen
  if (def != target) {
  	progress = 0;
  	target = def;
  }
  // Eingang deaktivieren
  SetEntrance(0);
  // Forschungsaktion starten
  SetAction("Research");
  return (1);
}

private func Researching()
{
  // Kein Ziel
  if (!target) return(0);
  // Effekte
  if (Random(5))
  	CreateParticle("Smoke",8*Random(2),-13,RandomX(5,8),0,RandomX(10,30),RGB(255,255,255));
  //Smoke(5+8*Random(2),-23,3+Random(8));
  // Fortschritt
  var Process = Min(++progress*4 / restime, 3);
  Process = Format("Research%d", Process);
  if (Process ne GetAction()) SetAction(Process);
  // Forschung abbrechen, wenn irgendein Clonk im Inneren das Geb�ude verlassen m�chte
  var pObj;
  while (pObj = FindObject(0, 0,0,0,0, OCF_CrewMember, 0,0, this(), pObj))
    if (pObj->GetCommand() eq "Exit")
      return(CancelResearch());
  // Weitermachen
  if (!restime) restime=100;
  if (progress < restime) return(1);
  // Forschung abgeschlossen
  Sound("ResearchDone");
  // Dem Spieler den neuen Bauplan verpassen
  SetPlrKnowledge(GetOwner(), target);  
  // Alle Clonks suchen, die ein Men� f�r dieses Labor ge�ffnet haben - noch nicht implementierbar, da kein GetMenuTarget()...
  /*var pClonk;
  while (pClonk = FindObject(0, 0,0,0,0, OCF_CrewMember(), 0,0, 0, pClonk))
    if ((pClonk->GetMenu() == RSR1) && (pClonk->GetMenuTarget() == this())
      // Men� neu �ffnen, damit Forschung-Fortsetzen-Eintr�ge aktualisiert werden
      ResearchMenu(pClonk);*/  
  // Gelinkte Forschungsziele sind automatisch auch erforscht
  // (Linke Zugbr�cke -> Rechte Zugbr�cke etc.)
  var def_num, def;
  while (def = GetDefinition(def_num++)) 
  {
    var link;
    if (link = DefinitionCall(def, "GetResearchLink"))
      if (link == target)
        SetPlrKnowledge(GetOwner(), def);
  }
  // Nachricht ausgeben
  Message("$Txtsdeveloped$", this(), GetName(0, target));
  // der f�r die Forschung ben�tigte Druck entfernen
  DoFill(-restime);
  // Forschung beenden
  target = 0;
  CancelResearch();
  // Fertig
  return(1);  
}
  
private func GetResearchGoal(id idType, object pWorker)
{
  // Kategorie
  var category;
  
  if(idType == RSR3) category = C4D_Object();
  if(idType == RSR4) category = C4D_Vehicle();
  if(idType == RSR5) category = C4D_Structure();
  
  // Alle geladenen Definitionen �berpr�fen
  var i = -1, id;
  while (id = GetDefinition(++i,category) )
    // Ist ein Bauplan
    if(GetCategory(0, id) & C4D_Knowledge() )
      // Dem Spieler noch nicht bekannt
      if(!GetPlrKnowledge(GetOwner(pWorker), id) )
      {
        // Grundlage bekannt
        var base = DefinitionCall(id, "GetResearchBase");
        if(!base || GetPlrKnowledge(GetOwner(pWorker), base))
          return(id);
      }

  // Zur Zeit kein weiteres Ziel
  return(0);
}

/* Kontext */

public func ContextResearch(clonk) 
{
  [$TxtResearch$|Image=RSR1|Condition=IsBuilt]
  return (ResearchMenu(clonk));
}

public func ContextResearchStop(pClonk) {
	[$TxtCancelresearch$|Image=RSR2|Condition=IsResearching]
	PlayerMessage(GetOwner(pClonk), "$TxtResearchStop$", this);
	return CancelResearch();
}

protected func IsBuilt() 
{
  return (GetCon () >= 100);
}

public func IsResearching() {
	if(GetAction() eq "Research")
		return 1;
	return;
}
    
/* Steuerung */

/*public func ContainedUp(object pClonk)
{
  [$TxtResearch$|Image=RSR1]
  ResearchMenu(pClonk);
  return(1);
}*/

public func ContainedDown(object pClonk)
{
	[$TxtDown$]
	if(IsResearching()) {
		PlayerMessage(GetOwner(pClonk), "$TxtResearchStop$", this);
  	CancelResearch();
  	return 1;
  }
  return;
}
