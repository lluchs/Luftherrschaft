/*-- Der Enterhaken --*/

#strict

local Rope, Seilende;
    
protected func Departure(object pContainer) {               
  SetAction ("Idle");
  if(!GetXDir())
  	return;
  if(Rope)
  	return;
  Seilende = CreateObject(1A1E, 0, 0, GetOwner());
  Rope = CreateObject(171E, 0, 0, GetOwner());
  Rope -> Activate(Seilende, this);
  //ObjectSetAction (Rope,"Connect",Seilende,this());
  //ObjectCall(Seilende, "SetRope", Rope, this());
  Seilende -> SetRope(Rope, this);
  Enter(pContainer, Seilende);
  SetXDir(GetXDir()* 2);
  SetYDir(GetYDir()* 3);
  Sound("Catapult");
	return;
}
            
protected func Entrance() {               
  SetAction("Idle");
	return;
}

public func RopeBreak() {                                           
  Rope = 0;                                          
  if(Seilende)
  	RemoveObject(Seilende);
  SetAction("Idle");                               
	return;
}

protected func Hit() {
  Sound("Woodhit*");
  if(!Rope)
  	return;
  SetXDir();
  SetYDir();
  Rope -> SetAction("ConnectSingle", Seilende, this);
  Rope -> SetLength(Rope -> CalcLength());
  //ObjectCall( Rope, "SetLength", ObjectCall( Rope, "CalcLength"));
	return 1;
}

private func Contacting(string szAction) {
  SetAction(szAction);
	return 1;
}

protected func ContactLeft()		{ return Contacting("Hookleft");	}
protected func ContactRight()		{ return Contacting("Hookright");	}
protected func ContactBottom()	{ return Contacting("Hookdown");	}
protected func ContactTop()			{ return Contacting("Hookup");		}
