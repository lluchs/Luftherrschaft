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
    return 0;
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
  if(_inherited(...)) return 1;
  
  var pulley, tstruct, crope;
  SetComDir(COMD_Stop);
  // Check for collection limit    
  if(GetDefCoreVal("CollectionLimit", "DefCore", GetID(this)))
    if(ContentsCount(0,this) >= GetDefCoreVal( "CollectionLimit", "DefCore", GetID(this)))
      return 0;
    // Check line pickup
    if(LinePickUp( this)) return 1;
    while(tstruct = FindObject( 0, 0, -1, 0, 0, 0, 0, 0, 0, tstruct))
      if(LinePickUp( tstruct)) return 1;
    while(tstruct = FindObject( 0, 0, 9, 0, 0, 0, 0, 0, 0, tstruct))
      if(LinePickUp( tstruct)) return 1;
    return 0;
}

private func LinePickUp( obj) {
  var crope, pulley;
  if( crope = FindObject( 1E1E, 0, 0, 0, 0, 0, "Connect", obj)) {
    // Check line connected to linekit at other end
    if ( (GetID( GetActionTarget( 0, crope)) == 161E) || (GetID( GetActionTarget( 1, crope)) == 161E)) {
      Sound("Error");
      Message("$NotFixed$", this, GetName( crope));
      return 0;
    }
    // Pick up line and new linekit
    if( !( pulley = CreateObject( 161E, 0, 0, GetOwner( crope)))) return 0;
    Enter(this, pulley);
    Sound("Connect");
    if (GetActionTarget( 0, crope)==obj) ObjectSetAction( crope, "Connect", pulley);
    if (GetActionTarget( 1, crope)==obj) ObjectSetAction( crope, "Connect", 0, pulley);
    ObjectCall( pulley, "Attach2Rope", crope);
    Message("$Disconnect$", obj, GetName( crope), GetName(obj));
    return 1;
  }
  return 0;
}

/* Kontext Menü Einträge Für Seil */
/*
public func ContextPullRope() {
  [$PullRope$|Image=LRPE|Condition=RopeAttached]
  
}*/

/* Bedingungen */

public func RopeAttached() {
  // hat kein Seil in der Hand?
  if(!Contents()->~IsRope()) return 0;
  // fest gemacht?
  if(!Contents()->~RopeAttached()) return 0;
  return 1;
}