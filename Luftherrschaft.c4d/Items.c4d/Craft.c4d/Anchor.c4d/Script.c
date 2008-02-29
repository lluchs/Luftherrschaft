/*- Anker -*/
#strict
local Boat;

public Launch:
  Boat=Par();
  //SetAction("Attachground");  
return(1);
                 
public Raise:
  if(GetAction()eq"Attachground") SetAction("Idle");
return();

Hit:
  if(Boat){                             
    SetAction("Attachground");
    ObjectCall(Boat,"AnchorHit",this());
  }
return();

GetVertexToConnect: return(1);

public func Entrance (obj) {
  Raise ();
}