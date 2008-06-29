/*-- Turm --*/

#strict

#include BAS3
#include DOOR

local pCannon;

func Initialize() {
  //Flagge erzeugen (für Spielerfarbe)
  CreateObject(LFLG,-5,-23);
  SetEntrance(1);
  return(1);
}

/* Steuerung */

public func ContainedLeft(object pCaller) {
  [$TurnLeft$]
  if(!pCannon) return 0;
  pCannon->~TurnLeft(pCaller);
  return 1;
}

public func ContainedRight(object pCaller) {
  [$TurnRight$]
  if(!pCannon) return 0;
  pCannon->~TurnRight(pCaller);
  return 1;
}

public func ContainedUp(object pCaller) {
  [$Stop$]
  if(!pCannon) return 0;
  pCannon->~StopTurning();
  return 1;
}

public func ContainedDig(object pCaller) {
  [$CannonMenu$]
  CreateMenu( , pCaller, this);
  AddMenuItem(Format("$AttachSingleCannon$", GetValue( , CTC1)),"AttachCannon(CTC1)", CTC1, pCaller);
  AddMenuItem(Format("$AttachDoubleCannon$", GetValue( , CTC2)),"AttachCannon(CTC2)", CTC2, pCaller);
  if(pCannon)
    AddMenuItem("$UnattachCannon$", "UnattachCannon", GetID(pCannon), pCaller);
}

public func ContainedThrow(object pCaller) {
  [$FireMenu$]
  if(!pCannon)
    return 0;
  CreateMenu( , pCaller, this);
  var i = ContentsCount(this);
  while(i--) {
    // verschwunden?
    if(!Contents(i))
      continue;
    var idContents = GetID(Contents(i));
    // Kanone, Lebewesen oder Fahrzeug? nicht feuern!
    if(idContents == CTC1 || idContents == CTC2 || GetAlive(Contents(i)) || GetCategory(Contents(i)) & C4D_Vehicle)
      continue;
    AddMenuItem(Format("$Fire$", GetName(Contents(i))), Format("CannonFire(Object(%d))", ObjectNumber(Contents(i))), GetID(Contents(i)), pCaller);
  }
}

/* MenuCalls */

protected func AttachCannon(id idCannon) {
  if(GetWealth(GetOwner()) < GetValue( , idCannon))
    return 0;
  else
    SetWealth(GetOwner(), GetWealth(GetOwner()) - GetValue( , idCannon));
  pCannon = CreateObject(idCannon, 0, -20, GetOwner());
  pCannon->~SetAction("Wait");
  pCannon->~SetPhase(Random(11) + 1);
}

protected func UnattachCannon() {
  RemoveObject(pCannon);
}

protected func CannonFire(object pFire) {
  if(!pCannon)
    return 0;
  pCannon->~Fire(pFire);
  return 1;
}