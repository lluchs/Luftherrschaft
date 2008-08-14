/*-- Globale Scripte --*/

#strict 2

/* Gerüstbau */

global func AddScaffold(object pStructure) {
  if(!pStructure)
    pStructure = this;
  // ist schon fertig?
  if(GetCon(pStructure) >= 100)
    return;
  AddEffect("IntScaffolding", pStructure, 100, 2);
}

global func FxIntScaffoldingStart(object pTarget, int iEffectNumber, bool bTemp) {
  EffectVar(0, pTarget, iEffectNumber) = CreateArray();
  EffectVar(1, pTarget, iEffectNumber) = 0;
}

global func FxIntScaffoldingTimer(pTarget, iEffectNumber, iTime) {
  var j;
  // Reihe komplett?
  for(var obj in EffectVar(0, pTarget, iEffectNumber)) {
    if(!obj)
      continue;
    if(obj->~Complete())
      j++;
  }
  if(GetLength(EffectVar(0, pTarget, iEffectNumber)) == j) {
    var num, x, y, scaffold;
    y = GetDefHeight(GetID(pTarget)) / 2 - EffectVar(1, pTarget, iEffectNumber) * 92;
    if(y < -GetDefHeight(GetID(pTarget)) / 2)
    x = -GetDefWidth( GetID(pTarget)) / 2;
    num = Max(GetDefWidth(GetID(pTarget)) / 227, 1);
    while(num) {
      x += 227;
      scaffold = CreateObject(LGRU, x + 113, y - 46, GetOwner(pTarget));
      scaffold->Init(pTarget, iEffectNumber);
      num--;
    }
    EffectVar(1, pTarget, iEffectNumber)--;
  }
}