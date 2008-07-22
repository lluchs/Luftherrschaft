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
    //EffectCall(construct, effectnum, "ScaffoldComplete", this);
    construct->~GerustComplete();
    BuildStop();
  }
  else
    if(!Random(3))
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

/* Gerüstbau */

global func AddScaffold(object pStructure, int iStartOverlay) {
  if(!pStructure)
    pStructure = this;
  if(!iStartOverlay)
    iStartOverlay = 1;
  // ist schon fertig?
  if(GetCon(pStructure) == 100)
    return;
  AddEffect("IntScaffolding", pStructure, 100, 2, , LGRU, iStartOverlay);
}

public func FxIntScaffoldingStart(pTarget, iEffectNumber, bTemp, iStartOverlay) {
  EffectVar(0, pTarget, iEffectNumber) = GetDefHeight(GetID(pTarget));
  EffectVar(1, pTarget, iEffectNumber) = CreateArray();
  EffectVar(2, pTarget, iEffectNumber) = 1; // Anzahl kompletter Reihen
  
  var i = GetDefWidth(GetID(pTarget)) / 226, j;
  var pTemp, x, y;
  if(i == 0)
    i = 1;
  while(i) {
    x = -GetObjWidth(pTarget) / 2 + j * 226;
    y = GetObjHeight(pTarget) / 2;
    if(i == 1)
      x = 0;
    pTemp = pTarget->~CreateObject(LGRU, x, y, NO_OWNER);
    pTemp->~Init(pTarget, iEffectNumber);
    EffectVar(1, pTarget, iEffectNumber)[j] = pTemp;
    i--;
    j++;
  }
}

public func FxIntScaffoldingTimer(pTarget, iEffectNumber, iTime) {
  var i;
  // Auf Vollständigkeit prüfen
  for(var pScaffold in EffectVar(1, pTarget, iEffectNumber)) {
    if(!pScaffold->~Complete())
     continue;
    i++;
  }
  if(i == GetLength(EffectVar(1, pTarget, iEffectNumber))) {
    if(EffectVar(0, pTarget, iEffectNumber) < 0)
      return;
    EffectVar(0, pTarget, iEffectNumber) -= 94;
    if(EffectVar(0, pTarget, iEffectNumber) < 0)
      return;
    i = GetDefWidth(GetID(pTarget)) / 226;
    var pTemp, x, y, j = GetLength(EffectVar(1, pTarget, iEffectNumber));
    if(i == 0)
      i = 1;
    while(i) {
      x = -GetObjWidth(pTarget) / 2 + j * 226;
      y = GetObjHeight(pTarget) / 2 - EffectVar(2, pTarget, iEffectNumber) * 94;
      if(i == 1)
        x = 0;
      pTemp = pTarget->~CreateObject(LGRU, x, y, NO_OWNER);
      pTemp->~Init(pTarget, iEffectNumber);
      EffectVar(1, pTarget, iEffectNumber)[j] = pTemp;
      i--;
      j++;
    }
    EffectVar(2, pTarget, iEffectNumber)++;
  }
  if(GetCon(pTarget) >= 100) {
    for(var pScaffold in EffectVar(1, pTarget, iEffectNumber)) {
      pScaffold->~BuildDown();
    }
  }
}