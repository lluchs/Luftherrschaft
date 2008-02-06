#strict
#include TREE

private func ReproductionAreaSize() { return(80); }
private func ReproductionRate()     { return(23); }
private func MaxTreeCount()         { return(4); }

protected func Damage(iDamage) {
	CastObjects(CRYS, RandomX(1,5), iDamage / 2);
	RemoveObject();
}

protected func Incineration() { Extinguish(); }

public func Construction() {
  SetAction("Exist");		// zufällige Animationsphase

  SetDir(Random(4));
  SetPosition(GetX(),GetY()-5);	// baum-syndrom
}

public func Reproduction()
{
  // Ist noch Platz für einen Baum?
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
public func ContextChop(pClonk)		// per Kontextmenü pflücken
{
  [$TxtPick$|Image=MUSH|Condition=IsStanding]
  Pick();
  return 1;
}*/

protected func RejectEntrance() {
  if(GetCon()<100) return 1;
}

/*public func Entrance() {		// per Einsammeln pflücken
  [$TxtPick$]
  if(IsStanding()) Pick();
  return 1;
}

public func Pick() {			// pflücken
  Sound("Grab");
  var iDir = GetDir();
  SetAction("Idle");
  SetAction("Exist");
  SetDir(iDir);
  SetCategory(16);
}*/


/*public func Activate(object pClonk)	// essen
{
  [$TxtEat$]
  Eat(pClonk);
  return(1);
}

protected func Eat(object pClonk)
{
    pClonk->~Feed(20);
    DoMagicEnergy(4,pClonk);
    RemoveObject();
    return(1);
}*/

protected func Existing() {
  if(IsStanding()) return;
  // re-seed
  if(!Contained())
    if(!GetYDir())
      if(!GetXDir())
        SetCategory(C4D_StaticBack);
}

public func IsStanding() { return(GetCategory() & C4D_StaticBack); }	// steht noch

/*func IsAlchemContainer() { return(true); }
func AlchemProcessTime() { return(80); }*/

protected func Hit() { Sound("CrystalHit*"); }
