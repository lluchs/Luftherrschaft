#strict
#include TREE

local iPhase;

private func ReproductionAreaSize() { return 100; }
private func ReproductionRate()     { return 23; }
private func MaxTreeCount()         { return 6; }

protected func Damage(iDamage) {
	CastObjects(CRYS, RandomX(1,5), iDamage / 2);
	RemoveObject();
}

protected func Incineration() { Extinguish(); }

public func Construction() {
	if(GetAction() eq "Idle") {
  	SetAction("Hang");		// zuf�llige Animationsphase
  	iPhase = Random(4);
  	// Zuf�llige Richtung
  	if (Random(1)) SetDir(DIR_Right);
  }
  // H�he anpassen
  while (!GBackSolid(0, 12))
    SetPosition(GetX(), GetY() + 1);
  // Drehung nach Erdoberfl�che
  var x_off = 18 * GetCon() / 100;
  var y_off = 15 * GetCon() / 100;
  var slope = GetSolidOffset(-x_off, y_off) - GetSolidOffset(x_off, y_off);
  SetR(slope);
  SetPhase(iPhase);
  return 1;
}

private func GetSolidOffset(int x, int y)
{
  var i;
  for (i = -15; GBackSolid(x, y - i) && (i < 15); i++);
  return i;
}

public func Reproduction()
{
  // Ist noch Platz f�r einen Baum?
  var iSize = ReproductionAreaSize();
  var iOffset = iSize / -2;
  if (ObjectCount(GetID(), iOffset, iOffset, iSize, iSize)<MaxTreeCount()) {
    // OK, hin damit
    var pMush = PlaceVegetation(GetID(), iOffset, iOffset, iSize, iSize, 10);
    return 1;
  }
  // Kein Platz ;'(
  return;
}

/*
public func ContextChop(pClonk)		// per Kontextmen� pfl�cken
{
  [$TxtPick$|Image=MUSH|Condition=IsStanding]
  Pick();
  return 1;
}*/

protected func RejectEntrance() {
  return GetAction() eq "Hang";
}

protected func ControlDigDouble() {
	if(GetAction() eq "Hang")
		Set(1);
	return;
}

protected func Seed() {
  if(IsStanding()) return;
  // re-seed
  if(!Contained() && !GetYDir() && !GetXDir())
  	Set(0);
  return _inherited(...);
}

public func IsStanding() { return GetCategory() & C4D_StaticBack; }	// steht noch

/*func IsAlchemContainer() { return(true); }
func AlchemProcessTime() { return(80); }*/

protected func Hit() { Sound("CrystalHit*"); }

public func Set(int iPar) {
	if(!iPar) {
		SetAction("Hang");
		SetPhase(iPhase);
		return 1;
	}
	if(iPar == 1) {
		SetAction("Object");
		SetPhase(iPhase);
		return 1;
	}
	return;
}
