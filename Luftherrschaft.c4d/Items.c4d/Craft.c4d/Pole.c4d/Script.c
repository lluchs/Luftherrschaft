/*-- Br�ckensegment --*/

#strict

// Local 0: frei/verankert

public func GetVertexToConnect() { return(2); }

protected Initialize:
  // Im Freien erzeugte Br�ckensegmente sind verankert
  if (!Contained())
    {
    Local()=1;
    }
  return(1);

protected Entrance:
  // Im Beh�lter: l�sen
  SetLocal();
  return(1);

public ControlDigDouble:
  [Verankern]
  //if(Contents()) return(ObjectCall(Contents(),"Activate",Par()));
  // L�sen
  SetR();
  if (Local(0)) return(Release());  
  // Verankern
  if (!GBackSolid(0,+2))
    SetPosition(GetX(),GetY()+2);
  Sound("Connect");
  Local()=1;                     
  if(Stuck()) SetMass(100,this());
  return(1);

private Release:
  SetLocal();
  Sound("Connect");
  // Aus der Erde l�sen
  while (Stuck() && ++Var(0)<12)
    SetPosition(GetX(),GetY()-1);
  if(!Stuck()) SetMass(6,this());
  return(1);