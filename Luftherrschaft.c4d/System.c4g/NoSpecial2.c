/*-- Damit man nicht das System beim Fliegenden H�ndler austricksen kann --*/

#strict
#appendto CLNK

protected func ControlSpecial2()
{
  [$CtrlMenuDesc$|Image=CXTX]
  // In einem Geb�ude oder Fahrzeug: das Kontextmen� des Geb�udes �ffnen
  if (Contained())
    if ((Contained()->GetCategory() & C4D_Structure) || (Contained()->GetCategory() & C4D_Vehicle))
     if (Contained()->GetID() != LSMN)
      return(SetCommand(this(),"Context",0,0,0,Contained()), ExecuteCommand());
  // Fasst ein Objekt an: Kontextmen� des angefassten Objekts �ffnen
  if (GetAction() eq "Push")
   if (GetActionTarget()->GetID() != LSMN)
    return(SetCommand(this(),"Context",0,0,0,GetActionTarget()), ExecuteCommand());
  // Tr�gt ein Objekt: Kontextmen� des ersten getragenen Objekts �ffnen
  if (Contents(0))
    return(SetCommand(this(),"Context",0,0,0,Contents(0)), ExecuteCommand());
  // Ansonsten das Kontextmen� des Clonks �ffnen
  return(SetCommand(this(),"Context",0,0,0,this()), ExecuteCommand());
}