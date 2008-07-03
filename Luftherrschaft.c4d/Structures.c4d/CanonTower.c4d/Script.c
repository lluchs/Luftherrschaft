/*-- Turm --*/

#strict

#include BAS3
#include DOOR

local pCannon;

func Initialize() {
  //Flagge erzeugen (für Spielerfarbe)
  CreateObject(LFLG,-5,-23);
  SetEntrance(true);
  return(1);
}

/* Steuerung */

public func ContainedLeft(object pCaller) {
  [$TurnLeft$]
  if(!pCannon) return 0;
  pCannon->~TurnLeft(pCaller);
  pCannon->~ProjectilePreview();
  return 1;
}

public func ContainedRight(object pCaller) {
  [$TurnRight$]
  if(!pCannon) return 0;
  pCannon->~TurnRight(pCaller);
  pCannon->~ProjectilePreview();
  return 1;
}

public func ContainedDig(object pCaller) {
  [$CannonMenu$]
  if(pCannon)
    if(pCannon->~HardcodedMenu()) {
    CreateMenu(GetID(pCannon), pCaller, pCannon);
    pCannon->~HardcodedMenu(pCaller);
  }
  CreateMenu( , pCaller, this);
  AddMenuItem(Format("$AttachSingleCannon$", GetValue( , CTC1)),"AttachCannon(CTC1)", CTC1, pCaller);
  AddMenuItem(Format("$AttachDoubleCannon$", GetValue( , CTC2)),"AttachCannon(CTC2)", CTC2, pCaller);
  if(pCannon)
    AddMenuItem("$UnattachCannon$", "UnattachCannon", GetID(pCannon), pCaller);
}

public func ContainedThrow(object pCaller) {
  [$FireMenu$]
  // hab gar keine Kanone?
  if(!pCannon)
    return 0;
  // will nachlegen?
  if(Contents(0, pCaller))
    return 0;
  // noch nicht bereit zum schießen
  if(!pCannon->ReadyToFire())
    return Sound("Error");
  CreateMenu( , pCaller, this);
  var i = ContentsCount(,this);
  while(i--) {
    // verschwunden?
    if(!Contents(i))
      continue;
    var idContents = GetID(Contents(i));
    // Kanone, Lebewesen oder Fahrzeug? nicht feuern!
    if(idContents == CTC1 || idContents == CTC2 || GetAlive(Contents(i)) || GetCategory(Contents(i)) & C4D_Vehicle || Contents(i) == pFireFirst)
      continue;
    AddMenuItem(Format("$Fire$", GetName(Contents(i))), Format("CannonFire(Object(%d), Object(%d))", ObjectNumber(Contents(i)), ObjectNumber(pCaller)), GetID(Contents(i)), pCaller);
  }
  return 1;
}

/* MenuCalls */

protected func AttachCannon(id idCannon) {
  if(GetWealth(GetOwner()) < GetValue( , idCannon)) {
    Message("$MoreMoney$", this, GetWealth(GetOwner()), GetName( , idCannon));
    Sound("Error");
    return 0;
  }
  else
    SetWealth(GetOwner(), GetWealth(GetOwner()) - GetValue( , idCannon));
  if(pCannon) UnattachCannon();
  pCannon = CreateObject(idCannon, 0, -20, GetOwner());
  pCannon->~SetAction("Wait");
  pCannon->~SetPhase(Random(10) + 1);
  pCannon->~ProjectilePreview();
  Sound("Connect");
}

protected func UnattachCannon() {
  RemoveObject(pCannon);
}

local pFireFirst;

protected func CannonFire(object pFire, object pCaller) {
  // keine Kanone?
  if(!pCannon)
    return 0;
  // Doppelkanone und noch keine zweite auswahl?
  if(GetID(pCannon) == CTC2 && !pFireFirst) {
    pFireFirst = pFire;
    ContainedThrow(pCaller);
    return 0;
  }
  if(GetID(pCannon) == CTC2)
    pCannon->~CannonFire(pFireFirst,pFire);
  if(GetID(pCannon) == CTBW)
    pCannon->~CannonFire();
  else
    pCannon->~CannonFire(pFire);
  pFireFirst = 0;
  return 1;
}