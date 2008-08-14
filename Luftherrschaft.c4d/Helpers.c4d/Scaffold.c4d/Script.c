/*-- Gerüst --*/

#strict 2

local construct, effectnum, builddir, build;

protected func Initialize() {
  SetVisibility(VIS_All);
  if(builddir != 1) {
    SetCon(2);
    build = 20;
  }
  BuildUp();
}

public func Init(object pConstruct, int iEffectNum) {
  construct = pConstruct;
  effectnum = iEffectNum;
}

private func Building() {
  if(!construct)
    RemoveObject();
  build += builddir * 10;
  if(NextBuildStep(build))
    SetCon(build / 10);
  if(GetCon() == 100 && builddir == 1) {
    // nur wenn kein Effekt da ist, ans Bauobjekt senden (ausgebaut, da der Effekt die Werte abfrägt)
    //if(!EffectCall(construct, effectnum, "ScaffoldComplete", this))
    construct->~GerustComplete();
    BuildStop();
  }
  else
    if(!Random(6))
      Sound("Build*");
}

public func NextBuildStep(int iMille) {
  if(iMille < 340)
    return true;
  if(Inside(iMille, 415, 745))
    return true;
  if(iMille > 790)
    return true;
  return false;
}

public func BuildDown() {
  builddir = -1;
}

public func BuildStop() {
  builddir = 0;
}

public func BuildUp() {
  builddir = 1;
}

public func Complete() {
  if(GetCon() == 100) return true;
  return false;
}

public func ConstructionCompleted() { BuildDown(); }