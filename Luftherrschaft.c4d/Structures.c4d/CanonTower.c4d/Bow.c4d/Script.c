/*-- Ballista --*/

#strict 2
#include CTC1

local idArrow;

public func CannonFire() {
  var pArrow;
  if(pArrow = FindContents(idArrow)) {
    return _inherited(pArrow);
  }
  return _inherited(CreateContents(idArrow));
}

// Spezielles Menü
// CreateMenu() wurde schon ausgeführt, so dass Callbacks in diesem Objekt sind, 
// also pCommandObject = this
// Wenn ein andere Menuart gebraucht wird, dann einfach  CloseMenu() und CreateMenu() machen
public func HardcodedMenu(object pMenuObj) {
  // zum überprüfen, ob die Funktion vorhanden ist wird kein Clonk geliefert
  if(!pMenuObj)
    return true;
  var i, idDef;
  while(1) {
    idDef = GetDefinition(i, C4D_Object);
    if(!idDef)
      return;
    if(DefinitionCall(idDef, "IsArrow"))
      AddMenuItem(Format("$Select$", GetName( , idDef)), "SelectArrowType", idDef, pMenuObj);
    i++;
  }
}

private func SelectArrowType(id idArrowType) {
  idArrow = idArrowType;
}

public func ProjSpeed() { return 150; }
public func ReloadTime() { return 4; }
public func ShootSound() { return "Arrow"; }