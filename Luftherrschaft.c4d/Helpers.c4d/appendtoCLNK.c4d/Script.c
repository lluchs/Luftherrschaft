/*-- Clonk Append --*/

#strict 2
#appendto CLNK

private func Control2Contents(string szControl) {
  // Getragenes Objekt hat spezielle Steuerungsauswertung
  if(Contents(0))
    if(Contents(0)->~ActivateSupported())
      if(szControl == "ControlDigDouble")
        szControl = "Activate";
  if(Contents(0))
    if(eval(Format("Contents(0)->~%s(this)",szControl)))
      return 1;
  return 0;
}

/* Steuerung überladen, damit Inhalt zuerst ausführt */

protected func ControlUp(object pClonk) {
  if(Control2Contents("ControlUp")) return 1;
  return _inherited(...);
}

protected func ControlDown(object pClonk) {
  if(Control2Contents("ControlDown")) return 1; 
  return _inherited(...);
}

protected func ControlLeft(object pClonk) {
  if(Control2Contents("ControlLeft")) return 1;
  return _inherited(...);
}

protected func ControlRight(object pClonk) {
  if(Control2Contents("ControlRight")) return 1; 
  return _inherited(...);
}

protected func ControlLeftDouble(object pClonk) {
  if(Control2Contents("ControlLeftDouble")) return 1; 
  return _inherited(...);
}

protected func ControlRightDouble(object pClonk) {
  if(Control2Contents("ControlRightDouble")) return 1; 
  return _inherited(...);
}

protected func ControlDig(object pClonk) {
  if ( Control2Contents("ControlDig") ) return 1; 
  return _inherited(...);
}
protected func ControlUpDouble(object pClonk) {
  if ( Control2Contents("ControlUp") ) return 1; 
  return _inherited(...);
}
protected func ControlDownDouble(object pClonk) {
  if ( Control2Contents("ControlDown") ) return 1; 
  return _inherited(...);
}

protected func ControlThrow(object pClonk) {
  // Bei vorherigem Doppel-Stop nur Ablegen  
  if(GetPlrDownDouble(GetOwner())) return _inherited(...);
  if(Control2Contents("ControlThrow")) return 1; 
  return _inherited(...);
}

protected func ControlDigDouble(object pClonk) {
  if(Control2Contents("ControlDigDouble")) return 1;
  var rope,obj,pulley;
  for(obj in FindObjects(Find_AtPoint(),Find_Distance(30),Find_OCF(OCF_Living | OCF_Grab | OCF_Chop | OCF_Collectible))) {
    if(rope=FindObject2(Find_ID(1E1E),Find_ActionTarget(obj))) {
      pulley=CreateContents(LRPE,pClonk);
      rope->~SetObject(pulley,GetActionTargetNum(obj,rope));
      Sound("Connect");
      return 1;
    }
  }
  if(_inherited(...)) return 1;
}

/* Kontext Menü Einträge Für Seil */

public func ContextPullRope() {
  [$PullRope$|Image=LRPE|Condition=RopeAttached]
  Contents()->~SetPushOrPull(-1);
}

public func ContextPushRope() {
  [$PushRope$|Image=LRPE|Condition=RopeAttached]
  Contents()->SetPushOrPull(1);
}

public func ContextConnectRope() {
  [$ConnectRope$|Image=LRPE|Condition=RopeCanAttach]
  CreateMenu(LRPE,this,this);
  for(var obj in FindObjects(Find_NoContainer(),Find_AtPoint(),Find_OCF(OCF_Living | OCF_Grab | OCF_Chop | OCF_Collectible))) {
    AddMenuItem(Format("$ConnectWith$",GetName(obj)),Format("Contents()->~Connect(Object(%d))",ObjectNumber(obj)),GetID(obj),this);
  }
}

/* Bedingungen */

public func RopeAttached() {
  if(!Contents()) return 0;
  // hat kein Seil in der Hand?
  if(!Contents()->~IsRope()) return 0;
  // fest gemacht?
  if(!Contents()->~RopeAttached()) return 0;
  return 1;
}

public func RopeCanAttach() {
  if(!Contents()) return 0;
  if(!Contents()->~IsRope()) return 0;
  if(FindObject2(Find_NoContainer(),Find_AtPoint(),Find_OCF(OCF_Living | OCF_Grab | OCF_Chop | OCF_Collectible))) return 1;
  return 0;
}