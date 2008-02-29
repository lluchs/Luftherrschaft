/*-- Brückensegment --*/

#strict

// Local 0: frei/verankert

public func GetVertexToConnect() { return(2); }

protected Initialize:
  // Im Freien erzeugte Brückensegmente sind verankert
  if (!Contained())
    {
    Local()=1;
    }
  return(1);

protected Entrance:
  // Im Behälter: lösen
  SetLocal();
  return(1);

public ControlDigDouble:
  [Verankern]
  //if(Contents()) return(ObjectCall(Contents(),"Activate",Par()));
  // Lösen
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
  // Aus der Erde lösen
  while (Stuck() && ++Var(0)<12)
    SetPosition(GetX(),GetY()-1);
  if(!Stuck()) SetMass(6,this());
  return(1);