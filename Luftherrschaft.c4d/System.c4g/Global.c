/*-- Globale Scripte --*/

#strict 2

/* Gerüstbau */

static const LGRU_Wdt = 103; // ist nicht die tatsächliche Breite eines Gerüstteils, sondern die Breite des "vorderen" teils.
static const LGRU_Hgt = 141;

global func AddScaffold(object pStructure) {
  if(!pStructure)
    pStructure = this;
  // ist schon fertig?
  if(GetCon(pStructure) >= 100)
    return;
  AddEffect("IntScaffolding", pStructure, 100, 2);
}

global func FxIntScaffoldingStart(object pTarget, int iEffectNumber, bool bTemp) {
	
  // restliche Reihen (senkrecht)
  EffectVar(1, pTarget, iEffectNumber) = Max(GetDefHeight(GetID(pTarget)) / LGRU_Hgt, 1);
  // Die oberste Reihe
  EffectVar(0, pTarget, iEffectNumber) = CreateArray(Max(GetDefWidth(GetID(pTarget)) / LGRU_Wdt, 1));
}

global func FxIntScaffoldingTimer(object pTarget,int iEffectNumber,int iTime) {
  
  // Auf Fertigstellung prüfen
  for(var scaffold in EffectVar(0, pTarget, iEffectNumber)) {
  	// es wurden noch keine erstellt?
  	if(!scaffold)
  		break;
  	// noch nicht fertig?
    if(!scaffold->~Complete())
    	return 1;
  }
  // alle aktuellen Gerüste fertig!
  // dann auf zur nächste Reihe!
  var num;
  var max_cnt = Max(GetDefWidth(GetID(pTarget)) / LGRU_Wdt, 1); // Anzahl Gerüste in der Breite
  var max_y = Max(GetDefHeight(GetID(pTarget)) / LGRU_Hgt, 1); // Anzahl Gerüste in der Höhe
  var y = -(max_y - EffectVar(1, pTarget, iEffectNumber)) * LGRU_Hgt + GetObjHeight(pTarget) / 2; // Y-Position der Reihe
  // Schon drüber?
  if(y <= -GetDefHeight(GetID(pTarget)) / 2)
    return -1;
  
  while(++num <= max_cnt) {
    // X-Position ermitteln
    var x = -GetObjWidth(pTarget) / 2 + (num - 1) * LGRU_Wdt + LGRU_Wdt / 2;
    // Gerüste erstellen (Position relativ zur Baustelle)
    var scaffold = pTarget->~CreateObject(LGRU, x, y, GetOwner(pTarget));
    // Startparameter übergeben (Baustelle, Gerüst darunter)
    scaffold->Init(pTarget, EffectVar(0, pTarget, iEffectNumber)[num]);
    // Speichern.
    EffectVar(0, pTarget, iEffectNumber)[num] = scaffold;
  }
  
  // Um 1 verringern, da eine weitere Reihe fertig ist.
  EffectVar(1, pTarget, iEffectNumber) = EffectVar(1, pTarget, iEffectNumber) - 1;
  // Wenn alle fertig sind, dann Effekt entfernen
  if(EffectVar(1, pTarget, iEffectNumber) == -1)
    return -1;
}

// CreateConstruction() überladen, damit die Gerüste auch gebaut werden.
global func CreateConstruction() {
  var obj = _inherited(...);
  if(obj)
  obj->AddScaffold();
  return obj;
}
