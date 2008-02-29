BreakMessage:
  // Meldung bei Leitungsbausatz am einen oder anderen Ende ausgeben
  if (Not( Equal(GetID( SetVar(0,GetActionTarget(0)) ),LNKT) ))
    SetVar(0,GetActionTarget(1));
  Message("Rope broke|Blame the Engine",Var(0));
  return(1);