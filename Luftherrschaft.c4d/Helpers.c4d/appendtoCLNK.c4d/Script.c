#strict
#appendto CLNK

private Control2Contents:
  // Getragenes Objekt hat spezielle Steuerungsauswertung
  if ( ObjectCall(Contents(),Par(0),this()) )
    return(1);
 return(0);

/* Steuerung Werfen */
protected ControlThrow:
  // Bei vorherigem Doppel-Stop nur Ablegen  
  if ( GetPlrDownDouble(GetOwner()) ) return(_inherited(Par()));
  if ( Control2Contents("ControlThrow") ) return(1); 
 return(_inherited(Par()));

/* Steuerung Richtung (an Inhalt weitergeben, sonst internen Befehl ausführen) */
protected ControlUp:
  if ( Control2Contents("ControlUp") ) return(1);
 return(_inherited(Par()));
protected ControlDown:
  if ( Control2Contents("ControlDown") ) return(1); 
 return(_inherited(Par()));
protected ControlLeft:
  if ( Control2Contents("ControlLeft") ) return(1);
 return(_inherited(Par()));
protected ControlRight:
  if ( Control2Contents("ControlRight") ) return(1); 
 return(_inherited(Par()));
protected ControlLeftDouble:
  if ( Control2Contents("ControlLeftDouble") ) return(1); 
 return(_inherited(Par()));
protected ControlRightDouble:
  if ( Control2Contents("ControlRightDouble") ) return(1); 
 return(_inherited(Par()));
protected func ControlDigDouble( ) {
  if ( Control2Contents("ControlDigDouble") ) return(1);
  if( _inherited( )) return( 1);
  
  var pulley, tstruct, crope;
  SetComDir(COMD_Stop());
  // Check for collection limit    
  if( GetDefCoreVal( "CollectionLimit", "DefCore", GetID( )))
    if( ContentsCount( ) >= GetDefCoreVal( "CollectionLimit", "DefCore", GetID( )))
      return( );
    // Check line pickup
    if( LinePickUp( this())) return( 1);
    while( tstruct = FindObject( 0, 0, -1, 0, 0, 0, 0, 0, 0, tstruct))
      if( LinePickUp( tstruct)) return( 1);
    while( tstruct = FindObject( 0, 0, 9, 0, 0, 0, 0, 0, 0, tstruct))
      if( LinePickUp( tstruct)) return( 1);
    return();
  }
private func LinePickUp( obj) {
  var crope, pulley;
  if( crope = FindObject( 1E1E, 0, 0, 0, 0, 0, "Connect", obj)) {
    // Check line connected to linekit at other end
    if ( (GetID( GetActionTarget( 0, crope)) == 161E) || (GetID( GetActionTarget( 1, crope)) == 161E)) {
      Sound("Error");
      Message( GetLangString( 1), this( ), GetName( crope));
      return( );
    }
    // Pick up line and new linekit
    if( !( pulley = CreateObject( 161E, 0, 0, GetOwner( crope)))) return( );
    Enter( this( ), pulley);
    Sound( "Connect");
    if (GetActionTarget( 0, crope)==obj) ObjectSetAction( crope, "Connect", pulley);
    if (GetActionTarget( 1, crope)==obj) ObjectSetAction( crope, "Connect", 0, pulley);
    ObjectCall( pulley, "Attach2Rope", crope);
    Message( GetLangString( 2), obj, GetName( crope), GetName(obj));
    return( 1);
  }
  return( );
}



/*
  var linekit, tstruct, cline;
  var ocf;
  SetComDir(COMD_Stop());
  // Check physical
  if (!GetPhysical("CanConstruct")) return(0,Message( GetLangString( 1),this(),GetName()));
// - - - - - - - - - - - - - - - - - - Line pickup - - - - - - - - - - - - - - - - -
  // Check for linekit
  if (!(linekit=FindContents(LNKT))) {
    // Check for collection limit    
    if( GetDefCoreVal( "CollectionLimit", "DefCore", GetID( )))
      if( ContentsCount( ) >= GetDefCoreVal( "CollectionLimit", "DefCore", GetID( )))
        return( );
    // Check line pickup
    ocf=OCF_LineConstruct();
    if( !( tstruct = FindObject( 0, 0, -1, 0, 0, ocf)));
    if( !( cline = FindObject( 0, 0, 0, 0, 0, 0, "Connect", tstruct))) return( );
    // Check line connected to linekit at other end
    if ( (GetID( GetActionTarget( 0, cline)) == LNKT) || (GetID( GetActionTarget( 1, cline)) == LNKT)) {
      Sound("Error");
      Message( GetLangString( 2), this( ), GetName( cline));
      return( );
    }
    // Pick up line and new linekit
    if( !( linekit = CreateObject( LNKT, 0, 0, GetOwner( cline)))) return( );
    Enter( this( ), linekit);
    Sound( "Connect");
    if (GetActionTarget( 0, cline)==tstruct) ObjectSetAction( cline, "Connect", linekit);
    if (GetActionTarget( 1, cline)==tstruct) ObjectSetAction( cline, "Connect", 0, linekit);
    Message( GetLangString( 3), tstruct, GetName( cline), GetName(tstruct));
    return( 1);
  }
*/  
 //return(_inherited(Par()));
protected ControlDig:
  if ( Control2Contents("ControlDig") ) return(1); 
 return(_inherited(Par()));
protected ControlUpDouble:
  if ( Control2Contents("ControlUp") ) return(1); 
 return(_inherited(Par()));
protected ControlDownDouble:
  if ( Control2Contents("ControlDown") ) return(1); 
 return(_inherited(Par()));