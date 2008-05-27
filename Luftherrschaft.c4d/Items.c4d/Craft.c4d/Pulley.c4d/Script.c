/*- Das Seil als aufnehmbares Objekt-*/

#strict 2

local Rope,PushPull,Objekt,Mode;

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
  // Nicht zu viel Seil lassen
  if (PushPull >= 0) if (iLength <= -10) return -1;
  // Seil geben
  if (PushPull == 1) if (iLength >= 0) if (Rope->~GetLength() < 1000) return (1 + Random (2));
  return 0;
}

public func Activate() {
  [$OpenMenu$]
  return 0;
}

public func ControlDigDouble(object pClonk) {
  // Clonk anhalten
  SetComDir(COMD_Stop,pClonk);
  CreateMenu(GetID(this), pClonk, 0, 0, "$NotAvailable$");
  if(Rope) {
    if(PushPull != -1) AddMenuItem("$PullRope$","Set(PushPull,-1)", GetID(this), pClonk);
    if(PushPull != 1) AddMenuItem("$ExtendRope$","Set(PushPull,1)", GetID(this), pClonk);
    if(PushPull != 0) AddMenuItem("$HoldRope$","Set(PushPull,0)", GetID(this), pClonk);
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

/* Enginecalls */

func Departure() {
  if( Rope) SetCategory(C4D_Vehicle | C4D_SelectMaterial | C4D_SelectKnowledge | C4D_SelectHomebase);
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

protected func Connect(object pConnectWith) {
  SetCategory(C4D_Vehicle | C4D_SelectMaterial | C4D_SelectKnowledge | C4D_SelectHomebase);
  PushPull=1;
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
  if(Rope) RemoveObject(Rope);
  return 1;
}

public func GetRope() { return Rope; }

/* Eigenschaften */

public func IsRope() { return 1; }