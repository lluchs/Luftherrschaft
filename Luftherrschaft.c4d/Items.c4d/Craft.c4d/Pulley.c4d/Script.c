/*- Das Seil als aufnehmbares Objekt-*/
#strict
local Rope,PushPull,Objekt,Mode;

func RopeAskChangeLength (len, obj) {
  if (obj != Rope) return ();
  if (!(GetOCF(Contained ()) & OCF_CrewMember())) {
    if (Rope -> GetLength () >= 1000) {
      Exit ();
      return ();
    }
    if (len >= 0) return (1 + Random (2));
    return ();
  }
  // Bei zu starkem Zug nachgeben  
  if (PushPull <= 0) if (len >= 25) return (1);
  // Am Seil ziehen
  if (PushPull == -1) if (len <= 20) return (-1);
  // Nicht zu viel Seil lassen
  if (PushPull >= 0) if (len <= -10) return (-1);
  // Seil geben
  if (PushPull == 1) if (len >= 0) if (ObjectCall (Rope, "GetLength") < 1000) return (1 + Random (2));
  return ();
}

Activate: [Menu öffnen] return();

ControlDigDouble:
  // Clonk anhalten
  SetComDir(COMD_Stop(),Par(0));
  CreateMenu( GetID( this( )), Par( ), 0, 0, "Zurzeit nicht möglich. Wenden sie sich an den Administrator.");
  if( Rope)
  {                                                                                                                                    
    if(PushPull!=-1) AddMenuItem( GetLangString( 1), "Set(PushPull,-1)", GetID( this( )), Par( ));
    if(PushPull!=1) AddMenuItem( GetLangString( 2), "Set(PushPull,1)", GetID( this( )), Par( ));
    if(PushPull!=0) AddMenuItem( GetLangString( 3), "Set(PushPull,0)", GetID( this( )), Par( ));
    if(ObjectDistance(GetActionTarget(!Mode,Rope))<30) AddMenuItem( GetLangString( 4), "Trenne", 171E, Par( ), 0, 0, "Nimmt das Seil ab.");
  }        
  var obj,OCF=OCF_Living()|OCF_Grab()|OCF_Chop()|OCF_Collectible();
  while( obj= FindObject(0,-10,-25,20,50, OCF, 0, 0, NoContainer(), obj))
    if((GetActionTarget(0,Rope)!=obj)&&(GetActionTarget(1,Rope)!=obj))
    AddMenuItem( Format( GetLangString( 5), GetName( obj)), "Verbinde", GetID( obj), Par( ), 0, obj, "Knotet das Seil fest.");
return(1);

func Attach2Rope(crope) {
  if( GetActionTarget( 0, crope) == this( )) Mode=0;
  if( GetActionTarget( 1, crope) == this( )) Mode=1;
  PushPull=0;
  Rope=crope;
}

Departure:
  if( Rope) SetCategory(3588);
  PushPull=1;
return( );

RejectEntrance:
//  if(Rope) return(!(GetOCF(Par())&OCF_CrewMember()));
return();

Entrance:
  SetCategory(3600);
return( );

Verbinde://ID, Objekt
  SetCategory(3588);
  PushPull=1;
  if( Rope)
  {
    if( Mode) ObjectSetAction( Rope, "Connect", 0, Par( 1));
    if( !Mode) ObjectSetAction( Rope, "Connect", Par( 1));
    ObjectCall(Rope,"CheckConVertices");
    RemoveObject();
    return( 1);
  }
  if( !Rope)
  {
    if( !Par( 1)) return( );
    Mode=0;
    //Objekt= Contained( );
    ObjectCall( Rope= CreateObject(1E1E,0,0,GetOwner()), "Activate", this(), Par(1));
    return( 1);
  }
return( 1);

private func CreateLine(idType, iOwner, pFrom, pTo) {
  var pLine;
  if( !pFrom || !pTo) return( );
  if( !( pLine = CreateObject(idType,0,0,iOwner))) return( );
  if( GetVertexNum( pLine) < 2) AddVertex(0,0,pLine);
  if( GetVertexNum( pLine) < 2) AddVertex(0,0,pLine);
  SetVertex( 0, 0, GetX( pFrom), pLine);
  SetVertex( 0, 1, GetY( pFrom)+GetObjHeight(pFrom)/4, pLine);
  SetVertex( 1, 0, GetX( pTo), pLine);
  SetVertex( 1, 1, GetY( pTo)+GetObjHeight(pTo)/4, pLine);
  ObjectSetAction( pLine, GetAction( pLine), pFrom, pTo);
  return( pLine);
}

Trenne:
  SetCategory(3600);
  if( Rope) RemoveObject( Rope);
return( 1);
  