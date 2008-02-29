/*-- Der Enterhaken --*/
#strict
local Rope, Seilende;       
Departure:               
  SetAction ("Idle");
  if (!GetXDir ()) return ();
  if (Rope) return ();
  Seilende = CreateObject (1A1E, 0, 0, GetOwner());
  Rope = CreateObject (171E, 0, 0, GetOwner());
  Rope -> Activate (Seilende, this ());
  //ObjectSetAction (Rope,"Connect",Seilende,this());
  ObjectCall( Seilende, "SetRope", Rope, this());
  Enter(Par(), Seilende);
  SetXDir( GetXDir( )* 2);
  SetYDir( GetYDir( )* 3);
  Sound( "Catapult");
return( );
            
Entrance:               
  SetAction( "Idle");
return( );

RopeBreak:                                           
  Rope=0;                                          
  if(Seilende) RemoveObject(Seilende);
  SetAction( "Idle");                               
return( );

Hit:
  Sound( "Woodhit*");
  if(!Rope) return();
  SetXDir();
  SetYDir();
  ObjectSetAction(Rope,"ConnectSingle",Seilende, this());
  ObjectCall( Rope, "SetLength", ObjectCall( Rope, "CalcLength"));
return();

Contacting:
  SetAction( Par());
return( );

ContactLeft: return(Contacting("Hookleft"));
ContactRight: return(Contacting("Hookright"));
ContactBottom: return(Contacting("Hookdown"));
ContactTop: return(Contacting("Hookup"));