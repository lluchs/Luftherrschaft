/*- Das Seil als aufnehmbares Objekt-*/

#strict 2

local Rope,PushPull,Mode;

func RopeAskChangeLength(int iLength, object pObj) {
  if(pObj != Rope) return 0;
  if(!(GetOCF(Contained()) & OCF_CrewMember)) {
    if(Rope->~GetLength() >= 1000) {
      Exit();
      return 0;
    }
    if(iLength >= 0) return 1 + Random(2);
    return 0;
  }
  // Bei zu starkem Zug nachgeben  
  if (PushPull <= 0) if(iLength >= 25) return 1;
  // Am Seil ziehen
  if (PushPull == -1) if(iLength <= 20) return -1;
  if (PushPull == -2) if(iLength <= 20) return -2;
  // Nicht zu viel Seil lassen
  if (PushPull >= 0) if (iLength <= -10) return -1;
  // Seil geben
  if (PushPull == 1) if (iLength >= 0) if (Rope->~GetLength() < 1000) return (1 + Random (2));
  if (PushPull == 2) if (iLength >= 0) if (Rope->~GetLength() < 1000) return (2 + Random (2));
  return 0;
}

public func ControlDigDouble(object pClonk) {
  [$OpenMenu$]
  // "hängt am Haken"?
  if(pHook) {
    // einsammeln
    if(GetProcedure(pClonk) == "WALK")
      CollectHook();
    return 1;
  }
  // Seil abnhemen
  if(GetActionTarget(!Mode,Rope) && ObjectDistance(GetActionTarget(!Mode,Rope),this) < 30) {
    Disconnect();
    return;
  }
  var pObjs,pObj;
  if(GetProcedure(pClonk) == "FLIGHT") {
    pObjs=FindObjects(Find_AtPoint(),Find_Exclude(Contained()),Find_NoContainer(),Find_OCF(OCF_Living | OCF_Grab | OCF_Chop | OCF_Collectible));
    for(pObj in pObjs) {
      if(!pObj)
        continue;
      if(pObj == this)
        continue;
      // keine Befestigungsmöglichkeit?
      if(!CanAttach(pObj))
        continue;
      // Verbinden
      if(Connect(pObj))
        break;
    }
  }
  SetComDir(COMD_Stop,pClonk);
  CreateMenu(GetID(this), pClonk, 0, 0, "$NotAvailable$");
  if(Rope) {
    if(PushPull != -1) AddMenuItem("$PullRope$","SetPushOrPull(-1)", GetID(this), pClonk);
    if(PushPull != 1) AddMenuItem("$ExtendRope$","SetPushOrPull(1)", GetID(this), pClonk);
    if(PushPull != 0) AddMenuItem("$HoldRope$","SetPushOrPull(0)", GetID(this), pClonk);
    if(ObjectDistance(GetActionTarget(!Mode,Rope),this) < 30) AddMenuItem("$Disconnect$", "Disconnect", 171E, pClonk);
  }        
  var obj, OCF = OCF_Living | OCF_Grab | OCF_Chop | OCF_Collectible;
  for(obj in FindObjects(Find_Distance(15),Find_OCF(OCF),Find_NoContainer()))
    // hängt schon dran?
    if((GetActionTarget(0,Rope) != obj) && (GetActionTarget(1,Rope) != obj))
    AddMenuItem(Format("$Connect$", GetName(obj)), Format("Connect(Object(%d))",ObjectNumber(obj)), GetID(obj), pClonk);
  
  return(1);
}

func Attach2Rope(crope) {
  if(GetActionTarget( 0, crope) == this) Mode=0;
  if(GetActionTarget( 1, crope) == this) Mode=1;
  PushPull=0;
  Rope=crope;
}

func SetPushOrPull(int iMode) {
  PushPull = iMode;
  return 1;
}

/* Enterhaken spezial Behandlung */

local pHook, iSwing, iSpeed;

public func SetRope(object _Rope, object _Hook) {
  Rope = _Rope;
  pHook = _Hook;
  return 1;
}
// funktioniert noch nicht
public func CheckForSwing() {
  // bestimmte Aktionen unterbinden
  if(GetAction(Contained()) == "Tumble" || GetAction(Contained()) == "Dive")
    Contained()->~SetAction("Jump");
  if(!pHook) return 0;
  if(!Contained()) return iSwing = 0;
  if(Abs(GetXDir(Contained())) > 0)
    if(Inside(GetX(), GetX(pHook) + 3, GetX(pHook) - 3))
      iSwing++;
}

public func CollectHook() {
  Enter(Contained(), pHook);
  // Zuerst das Seil
  if(Rope) RemoveObject(Rope);
  // dann mich
  RemoveObject();
  return 1;
}

/* Enterhaken Steuerung */

public func ControlUp(object pClonk) {
  if(GetProcedure(pClonk) == "FLIGHT")
    PushPull=-1;
  return 0;
}

public func ControlDown(object pClonk) {
  if(GetProcedure(pClonk) == "FLIGHT")
    PushPull=1;
  return 0;
}

public func ControlLeft(object pClonk) {
  if(GetProcedure(pClonk) != "FLIGHT") return 0;
  SetPushOrPull(Rope_Hold);
  var speed = Abs(GetSpeed(pClonk)), angle = GetMoveAngle(pClonk);
  var max_acc=SwingAcceleration(angle);
  speed += Abs(GetSpeed(pClonk)) * max_acc / 100;
  SetSpeed(Sin(angle,speed),-Cos(angle,speed),pClonk);
  return 0;
}

public func ControlRight(object pClonk) {
  if(GetProcedure(pClonk) != "FLIGHT") return 0;
  SetPushOrPull(Rope_Hold);
  var speed = Abs(GetSpeed(pClonk)), angle = GetMoveAngle(pClonk);
  var max_acc=SwingAcceleration(angle);
  speed += Abs(GetSpeed(pClonk)) * max_acc / 100;
  SetSpeed(Sin(angle,speed),-Cos(angle,speed),pClonk);
  return 0;
}

public func ControlUpDouble(object pClonk) {
  if(GetProcedure(pClonk) == "FLIGHT")
    PushPull=-2;
  return 0;
}

public func ControlDownDouble(object pClonk) {
  if(GetProcedure(pClonk) == "FLIGHT")
    PushPull=+2;
  return 0;
}

public func SwingAcceleration(int iMoveAngle) {
  var new_angle;
  if(iMoveAngle < 180) {
    if(iMoveAngle > 90)
      return 10;
    return (iMoveAngle-45)*10/45;
  }
  if(iMoveAngle > 180) {
    if(iMoveAngle < 270)
      return 10;
    return (360-(iMoveAngle+45))*10/45;
  }
}

/* Enginecalls */

func Departure() {
  if(Rope) SetCategory(C4D_Vehicle | C4D_SelectMaterial | C4D_SelectKnowledge | C4D_SelectHomebase);
  PushPull=1;
  return 0;
}

func RejectEntrance() {
  return 0;
}

func Entrance() {
  SetCategory(C4D_Object | C4D_SelectMaterial | C4D_SelectKnowledge | C4D_SelectHomebase);
  return 0;
}

/* Seilfunktionen */

protected func Connect(object pConnectWith) {
  SetCategory(C4D_Vehicle | C4D_SelectMaterial | C4D_SelectKnowledge | C4D_SelectHomebase);
  PushPull=1;
  Sound("Connect");
  if(Rope) {
    if(Mode) Rope->SetAction("Connect", 0, pConnectWith);
    if(!Mode) Rope->SetAction("Connect", pConnectWith);
    Rope->~CheckConVertices();
    RemoveObject();
    return( 1);
  }
  if(!Rope) {
    if(!pConnectWith) return 0;
    Mode=0;
    Rope = CreateObject(1E1E,0,0,GetOwner());
    Rope->Activate(this, pConnectWith);    
    return 1;
  }
  return 1;
}

private func CreateLine(idType,int iOwner,object pFrom,object pTo) {
  var pLine;
  if(!pFrom || !pTo) return 0;
  if(!(pLine = CreateObject(idType,0,0,iOwner))) return 0;
  if(GetVertexNum(pLine) < 2) AddVertex(0,0,pLine);
  if(GetVertexNum(pLine) < 2) AddVertex(0,0,pLine);
  SetVertex( 0, 0, GetX( pFrom), pLine);
  SetVertex( 0, 1, GetY( pFrom)+GetObjHeight(pFrom)/4, pLine);
  SetVertex( 1, 0, GetX( pTo), pLine);
  SetVertex( 1, 1, GetY( pTo)+GetObjHeight(pTo)/4, pLine);
  SetActionTargets(pFrom,pTo,pLine);
  return pLine;
}

protected func Disconnect() {
  SetCategory(C4D_Object | C4D_SelectMaterial | C4D_SelectKnowledge | C4D_SelectHomebase);
  Sound("Connect");
  if(Rope) RemoveObject(Rope);
  return 1;
}

public func GetRope() { return Rope; }

private func CanAttach(object pObj) {
  if(!pObj) return 0;
  // Bäume möglich
  if(pObj->~IsTree()) return 1;
  // Sonderfunktion?
  if(pObj->~IsRopeAttachable()) return 1;
  // keine Gebäude
  if(GetCategory(pObj) & C4D_Structure) return 0;
  // keine sammelbaren Dinge (Stein, Flint etc)
  if(GetOCF(pObj) & OCF_Collectible) return 0;
  // sonst möglich
  return 1;
}

func RopeAttached() {
  if(!Rope) return 0;
  return 1;
}

func Departure() {
  // halten
  PushPull=0;
}

/* Eigenschaften */

public func IsRope() { return 1; }