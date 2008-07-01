/*-- Schiffsschmied --*/

#strict 2
#include L450

local aGerust, pConstruction, aComponents, iCompCnt, iOldCat, iBuild, iBuildedComp;

public func Initialize() {
  aGerust = aComponents = CreateArray();
  SetEntrance(true);
}

public func ContainedDig(object pCaller) {
  [$TxtConstruct$]
  // schon eine Baustelle?
  if(pConstruction)
    return;
  CreateMenu( , pCaller, this);
  var i, idDef;
  while(1) {
    idDef = GetDefinition(i, C4D_Vehicle);
    if(!idDef)
      break;
	if(DefinitionCall(idDef,"IsShipsmithProduct"))
	  AddMenuItem(Format("$Construct$", GetName( , idDef)), "CreateConstruction", idDef, pCaller);
    i++;
  }
}

public func CreateConstruction(id idConstruct) {
  var iWdt = GetDefWidth(idConstruct);
  var iNum = iWdt / GetGerustWidth();
  if(iNum <= 0)
    iNum = 1;
  var i;
  while(iNum--) {
    aGerust[i] = CreateObject(LGRU, -95 + GetGerustWidth() / 2 + i * GetGerustWidth(), +35, GetOwner()); //Start -95, +35
	aGerust[i]->~Init(this);
	i++;
  }
  // Konstruktion erstellen
  pConstruction = CreateObject(idConstruct, -95 + GetDefWidth(idConstruct) / 2, +37, GetOwner());
  pConstruction->SetCon(3);
  // Kategorie ändern (damit nicht anfassbar/unbenutzbar etc.)
  iOldCat = GetCategory(pConstruction);
  pConstruction->SetCategory(C4D_StaticBack);
  // Eingang ausschalten und SolidMask entfernen
  pConstruction->SetEntrance(0);
  pConstruction->SetSolidMask();
  aComponents = GetComponents(idConstruct, false);
  iCompCnt = GetComponents(idConstruct, true);
}

public func ConstructionCompleted() {
  for(var gerust in aGerust) {
    if(!gerust)
	  continue;
	gerust->ConstructionCompleted();
  }
  pConstruction->~SetCategory(iOldCat);
  // nochmal initialisieren (bei Fehlern Spezialbehandlung mit if())
  pConstruction->~Initialize();
  // hochsetzen wegen eventullen Vertices
  pConstruction->~SetPosition(GetX(pConstruction),GetY(pConstruction)-10);
  // zurücksetzen
  pConstruction = iBuild = iOldCat = iCompCnt = 0;
}

local iBuilding; // Wartezeit in Frames, bis das nächste mal geprüft wird, wenn gerade gebaut wird.

protected func CheckContent() {
  // keine Baustelle?
  if(!pConstruction)
    return;
  if(!GerustCompleted())
    if(!Random(2))
      return Sound("Build*");
    else
      return;
  // Effekte!
  Smoke(-204,-18,Random(10));
  if(!Random(2))
    Sound("Build*");
  // noch nicht weitermachen?
  if(iBuilding)
    return iBuilding--;
  var i = ContentsCount(,this), idComp, aComp;
  var j;
  while(i--) {
    idComp = GetID(Contents(i));
	j = GetLength(aComponents) - 1;
	while(j >= 0) {
	  aComp = aComponents[j];
	  // wird noch benötigt?
	  if(aComp[0] == idComp && aComp[1] > 0) {
	    aComponents[j][1]--;
		RemoveObject(Contents(i));
		iBuildedComp++;
        if(iBuild < 1000)
          iBuild = (1000/iCompCnt)*iBuildedComp;
		// nächster Bauschritt?
		if(pConstruction->~NextBuildStep(iBuild))
		  SetCon(iBuild / 10, pConstruction);
        if(GetCon(pConstruction) == 100)
		  return ConstructionCompleted();
		// je nach Wert des Rohstoffes warten (Wert * 5 Frames)
        iBuilding = GetValue( , idComp) * 5;
		return;
	  }
	  j--;
	}
  }
  if(iBuildedComp == iCompCnt) {
    iBuild = 1000;
    SetCon(100, pConstruction);
    ConstructionCompleted();
  }
}

private func GetGerustWidth() { return 243; }

// Liefert die Komponenten als brauchbaren Array bzw. die Anzahl aller Komponenten
public func GetComponents(id idFrom, bool bGetCount) {
  if(!idFrom)
    return;
  var aComp = CreateArray(), idComp, iCnt, iComponentCnt, aTemp;
  var i = 1, j;
  while(i) {
    idComp = GetComponent( , i - 1, , idFrom);
	if(!idComp)
	  break;
	iCnt = GetComponent(idComp, , , idFrom);
	if(bGetCount) {
	  iComponentCnt += iCnt;
	  i++;
	  continue;
	}
	else {
	  aTemp = 0;
	  aTemp = CreateArray();
	  aTemp[0] = idComp;
	  aTemp[1] = iCnt;
	  aComp[j] = aTemp;
	  j++;
	  i++;
	}
  }
  if(bGetCount)
    return iComponentCnt;
  if(!bGetCount)
    return aComp;
}

local bGerust;

public func GerustComplete() { bGerust = true; }
public func GerustCompleted() { return bGerust; }