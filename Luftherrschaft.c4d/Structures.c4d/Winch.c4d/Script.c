/*-- Seilwinde --*/
#strict
local Rope,Pull,Seilende,Phase;

func RopeAskChangeLength( len, by) {
  if(!Rope)
    if(!(Rope=FindObject(1E1E,0,0,0,0,0,"Connect",this()))) return();
  if( by != Rope) return();
  var w;
  if(GetActionTarget(0,Rope)==this())
    w=Angle(
      GetVertex(1,0,Rope),
      GetVertex(1,1,Rope),
      GetX()+GetVertex(0,0),
      GetY()+GetVertex(0,1));
  if(GetActionTarget(1,Rope)==this())
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
  return (a);
}

GetVertexToConnect: return(1);

protected Initialize:
  SetAction("Winding");
  return(1);

ControlUp: Pull=-5; return(1);
ControlDownSingle: Pull=5; return(1);
ControlLeft: Pull=0; return(1);
ControlRight: Pull=0; return(1);
/*ControlDigDouble:
  if(!Rope)
  {
    Rope=Connect( Par(), this(), 3, 20);
    if(!Seilende) Seilende=CreateObject(161E,0,0,GetOwner());
  }
  Local(3,Rope)=0;
  ObjectCall(Seilende,"FetchRope",Rope,Par());
return();*/

/* Fundament (Local 9) */
protected Construction:
  SetLocal(9,CreateObject(BAS1,-2,+8,GetOwner()));
  return(1);
  
protected Destruction:
  if (Local(9)) RemoveObject(Local(9));
  return(1);