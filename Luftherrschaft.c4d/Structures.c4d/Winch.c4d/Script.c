/*-- Seilwinde --*/

#strict 2

local Rope,Pull,Seilende,Phase;
local basement; // Fundament

protected func Initialize() {
  SetAction("Winding");
  return(1);
}

func RopeAskChangeLength( len, by) {
  if(!Rope)
    if(!(Rope=FindObject(1E1E,0,0,0,0,0,"Connect",this()))) return 0;
  if( by != Rope) return 0;
  var w;
  if(GetActionTarget(0,Rope) == this)
    w=Angle(
      GetVertex(1,0,Rope),
      GetVertex(1,1,Rope),
      GetX()+GetVertex(0,0),
      GetY()+GetVertex(0,1));
  if(GetActionTarget(1,Rope) == this)
    w=Angle(
      GetVertex(GetVertexNum(Rope)-2,0,Rope),
      GetVertex(GetVertexNum(Rope)-2,1,Rope),
      GetX()+GetVertex(0,0),
      GetY()+GetVertex(0,1));
  w+=90;
  SetVertexXY(0,Sin(w,13)-1,-Cos(w,13)-3);
  Phase=((Phase+24)%24);
  SetPhase(Phase/4);
  var a = 0;
  if(Pull<=0) if( len >= 25) a = 1;
  if(Pull<0) if( len <= 20) if(EnergyCheck(10)) { a -= 1; DoEnergy(-1); }
  if(Pull<0) if( len <= 10) if(EnergyCheck(10)) { a -= 1; }//DoEnergy(-1); }
  if(Pull>0) if( len >= 0) if(ObjectCall( Rope, "GetLength") < 2000) a = 3;
  Phase += a;
  return a;
}

public func GetVertexToConnect() { return 1; }

/* Steuerung */

public func ControlUp() {
  Pull=-5;
  return 1;
}

public func ControlDownSingle() {
  Pull=5;
  return 1;
}

public func ControlLeft() {
  Pull=0;
  return 1;
}

public func ControlRight() {
  Pull=0;
  return 1;
}

/* Fundament */

protected func Construction() {
  basement = CreateObject(BAS1,-2,+8,GetOwner());
  return(1);
}

protected func Destruction() {
  if(basement) RemoveObject(basement);
  return(1);
}