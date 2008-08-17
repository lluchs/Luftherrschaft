/*-- Globale Scripte --*/

#strict 2

/* Gerüstbau */

static LGRU_Wdt,LGRU_Hgt;

global func AddScaffold(object pStructure) {
  LGRU_Wdt = GetDefWidth(LGRU);
  LGRU_Hgt = GetDefHeight(LGRU);
  if(!pStructure)
    pStructure = this;
  // ist schon fertig?
  if(GetCon(pStructure) >= 100)
    return;
  AddEffect("IntScaffolding", pStructure, 100, 2);
}

global func FxIntScaffoldingStart(object pTarget, int iEffectNumber, bool bTemp) {
  EffectVar(0, pTarget, iEffectNumber) = CreateArray();        // Alle Gerüste
  EffectVar(1, pTarget, iEffectNumber) = Max(GetDefHeight(GetID(pTarget)) / LGRU_Hgt, 1); // Restreihen (senkrecht)
  EffectVar(2, pTarget, iEffectNumber) = Max(GetDefWidth(GetID(pTarget)) / LGRU_Wdt, 1) * Max(GetDefHeight(GetID(pTarget)) / LGRU_Hgt, 1);
}

global func FxIntScaffoldingTimer(pTarget, iEffectNumber, iTime) {
  var j;
  // Reihe komplett?
  for(var obj in EffectVar(0, pTarget, iEffectNumber)) {
    if(!obj) // zerstört worden.
      continue;
    if(obj->~Complete())
      j++;
  }
  
  // Reihe fertig?
  if(GetLength(EffectVar(0, pTarget, iEffectNumber)) == j) {
    var num = Max(GetDefWidth(GetID(pTarget)) / LGRU_Wdt,1);
    var max_y = Max(GetDefHeight(GetID(pTarget)) / LGRU_Hgt, 1);
    var y = -(max_y - EffectVar(1, pTarget, iEffectNumber)) * LGRU_Hgt + GetObjHeight(pTarget) / 2;
    if(y <= -GetDefHeight(GetID(pTarget)) / 2)
      return -1;
    while(num--) {
      var x = -GetObjWidth(pTarget) / 2 + (num) * LGRU_Wdt;
      var scaffold = pTarget->~CreateObject(LGRU, x, y, GetOwner(pTarget));
      scaffold->Init(pTarget, iEffectNumber);
    }
    EffectVar(1, pTarget, iEffectNumber) = EffectVar(1, pTarget, iEffectNumber) - 1;
  }
}

// Kleiner Zusatz, damit die Gerüste gebaut werden.
global func CreateConstruction() {
  var obj = _inherited(...);
  obj->AddScaffold();
  return obj;
}