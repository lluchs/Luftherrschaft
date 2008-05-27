/*-- Das Seilende --*/

#strict 2

local Pull, Rope, Hook;
                
public func SetRope(object pRope, object pHook) {
  Rope = pRope;
  Hook = pHook;
}

func Departure () {
  Pull = 10;
}

func Entrance () {
  Pull = 0;
}

/* Steuerung  */

func ControlLeft(object pClonk) {
  if(GetProcedure (pClonk) == "FLIGHT")
    Pull = 0;
  else
    Pull = 1;
}

func ControlRight (object pClonk) {
  if(GetProcedure(pClonk) == "FLIGHT")
    Pull = 0;
  else
    Pull = 1;
}

func ControlUp(object pClonk) {
  var strAction = GetProcedure(pClonk);
  if(strAction == "FLIGHT" || strAction == "WALK") {
    if(Pull > 0) Pull = -2;
    Pull--;
    return 0;
  }
}

func ControlDown (object pClonk) {
  Pull = 2;
}

func Activate(object pClonk) {
  [$Release$]
  var id=GetID(Hook);
  // nur bei Enterhaken/Lasso
  if(id == 191E || id == 1K1E)
    Enter(pClonk, Hook);
  RemoveObject(Rope);
  RemoveObject();
  return 1;
}



/* Seil gerissen */

public func RopeBreak() { return RemoveObject(); }

func RopeAskChangeLength(int iLength,object pObj) {
  if(pObj != Rope) return 0;
  if(Contained(Contained())) return Activate(Contained());
  // Ohne Kontrolle dem Wunsch des Seils nachgeben
  if(!Contained()) return iLength + 1;
  // Bei zu starkem Zug kann der CLonk das Seil nicht mehr festhalten
  if(Pull <= 0) if(iLength >= 0) if(pObj->~GetLength() < 1000) return 1;
  // Nicht zu viel Seil lassen
  if(Pull >= 0) if (iLength <= -10) return -1;
  // Am Seil ziehen
  if(Pull < 0) if(iLength <= 3) return Pull;
  if(Pull < 0) if(iLength <= 6) return -1;
  // Seil geben, nicht zuviel
  if(Pull > 0) if(iLength >= 0) if(pObj->~GetLength() < 1000) return Pull;
  return 0;
}

/* Eigenschaften */

public func IsRope() { return 1; }
public func ActivateSupported() { return 1; }