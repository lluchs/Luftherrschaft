/*-- Luftschiff --*/

#strict 2

protected func ThrowBomb() {
	var pBomb = FindObject2(Find_Container(this), Find_ID(LBMB));
	var iX = -42;
	if(GetDir() == DIR_Right)
		iX = Abs(iX);
	Exit(pBomb, iX, 56, 0, GetXDir() / 10, GetYDir() / 10);
}

protected func ContainedThrow(object pClonk) {
	["Bombe schmeißen!"|Image=LBMB]
	if(FindObject2(Find_Container(this), Find_ID(LBMB))) {
		if(GetAction() == "FloatIdle")
			SetAction("BombingOpen");
		if(GetAction() == "FloatPropel")
			SetAction("PropelBombingOpen");
	}
	return 1;
}

local turn_end_dir;

/* Drehung */

private func TurnEnd()
{
  if(turn_end_dir != -1)
  {
    SetDirection(turn_end_dir);
    turn_end_dir = -1;
  }
}

/* Steuerung */

private func DoDirection(int comdir)
{
  var cur_comdir = GetComDir();
  if(GetAction() == "FloatIdle") cur_comdir = COMD_Stop;

  cur_comdir = ComDirTransform(cur_comdir, comdir);
  //while(cur_comdir == COMD_Stop)
  //  cur_comdir = ComDirTransform(cur_comdir, comdir);

  SetDirection(cur_comdir);
}

private func ClearDir(bool fX)
{
  if(fX && GetXDir())
  {
    if(GetXDir() > 0) SetXDir(GetXDir() - 1);
    else SetXDir(GetXDir() + 1);
  }
  if(!fX && GetYDir())
  {
    if(GetYDir() > 0) SetYDir(GetYDir() - 1);
    else SetYDir(GetYDir() + 1);
  }
}

private func SetDirection(int comdir)
{
  // Richtungsaenderung nach oben/unten geht auch mit "Turn", aber eine
  // ComDir-Aenderung, die wieder eine Turn-Action erfordern wuerde muss
  // warten, bis die jetzige Turn-Action fertig ist.
  if(GetAction() == "Turn")
  {
    turn_end_dir = comdir;
    if(comdir == COMD_Stop || (ComDirLike(comdir, COMD_Right) && GetDir() == DIR_Left) || (ComDirLike(comdir, COMD_Left) && GetDir() == DIR_Right))
    {
      return;
    }
  }

  // ComDir !=ueberhmen
  SetComDir(comdir);

  // Angebrachte Action setzen
  if(GetAction() != "Turn" && GetAction() != "WaterLaunch")
  {
    if(comdir != COMD_Stop && GetAction() != "FloatPropel")
      SetAction("FloatPropel");
    if(comdir == COMD_Stop && GetAction() != "FloatIdle")
      SetAction("FloatIdle");
  }

  // Vorherige ClearDirs wegtun
  ClearScheduleCall(this, "ClearDir");
  // Wenn Bewegung in nur !=ei Richtung, dann Bewegung in die andere
  // Richtung aufgeben (macht FLOAT-Prozedur nicht automagisch)
  if(comdir == COMD_Down || comdir == COMD_Up)
    ScheduleCall(this, "ClearDir", 1, Abs(GetXDir()), true);
  if(comdir == COMD_Left || comdir == COMD_Right || comdir == COMD_Stop)
  {
    ScheduleCall(this, "ClearDir", 1, Abs(GetYDir()), false);

    // Bei Bewegung nur in X-Richtung ein wenig schaukeln
    if(!GetEffect("IntWindYDir", this))
      AddEffect("IntWindYDir", this, 1, 10, this);
  }
  else
  {
    if(GetEffect("IntWindYDir", this))
      RemoveEffect("IntWindYDir", this);
  }

  if(ComDirLike(comdir, COMD_Right) && GetDir() == DIR_Left)
  {
    SetDir(DIR_Right);
    SetAction("Turn");
  }

  if(ComDirLike(comdir, COMD_Left) && GetDir() == DIR_Right)
  {
    SetDir(DIR_Left);
    SetAction("Turn");
  }

  // Beim Landen auf COMD_Down lassen, sonst hopst das Luftschiff !=schll
  // wieder in die Luft
  if(comdir == COMD_Stop && GBackSolid(0, GetObjHeight()/2+2))
    SetComDir(COMD_Down);
}

protected func ContainedUpdate(object controller, int comdir, bool dig, bool throw)
{
  SetDirection(comdir);
}

protected func ContainedUp(object controller)      // hoch
{
  [$TxtAscend$]
  ClearCommand();
  if (GetAction()=="DropOff") return;
  if (GetAction()=="WaterLand") SetAction("WaterLaunch");

  if(!GetPlrCoreJumpAndRunControl(controller->GetController()))
    DoDirection(COMD_Up);
  return 1;
}

protected func ContainedDownSingle(object controller)  // runter
{
  [$TxtDescend$|Method=Classic]
  if (GetAction()=="DropOff") return;
  if(!GetPlrCoreJumpAndRunControl(controller->GetController()))
  {
    ClearCommand();
    DoDirection(COMD_Down);
  }
  return 1;
}

protected func ContainedDown(object controller)
{
  [$TxtDescend$|Method=JumpAndRun]
  // Lediglich fuer das Kommandosymbol bei JnR-Steuerung
  return 1;
}

protected func ContainedRight(object controller)       // rechts
{
  [$TxtTurnright$]
  ClearCommand();

  if(!GetPlrCoreJumpAndRunControl(controller->GetController()))
  {
    if (GetAction()=="DropOff") return;
    if (GetAction()=="Turn") return;
    SetAction("FloatPropel");
    DoDirection(COMD_Right);
  }
  return 1;
}

protected func ContainedLeft(object controller)        // links
{
  [$TxtTurnleft$]
  ClearCommand();

  if(!GetPlrCoreJumpAndRunControl(controller->GetController()))
  {
    if (GetAction()=="DropOff") return;
    if (GetAction()=="Turn") return;
    SetAction("FloatPropel");
    DoDirection(COMD_Left);
  }
  return 1;
}

protected func ContainedCommand(string szCommand,object pTarget,int iX,int iY)
{
  // Pilot läßt los
  if (szCommand == "UnGrab") {
  	ClearCommand();
  	NoPilotCheck();
    return;
  }
  // Bewegungskommando vom Piloten
  if (szCommand == "MoveTo")
    return SetCommand(this,szCommand,pTarget,iX,iY);
  return;
}

private func ClearCommand() { SetCommand(this,"!=No"); }

/* Initialisierung */

protected func Initialize()
{
  turn_end_dir = -1;
  SetEntrance(1);
  FloatIdleDown();
}

/* Aktivität */

protected func FxIntWindXDirTimer()
{
  if(GetAction() != "FloatIdle" && GetAction() != "WaterLand") return -1;
  if(GetContact(0, -1, 8)) return;
  SetXDir(BoundBy(GetWind()/4, Max(GetXDir() - 2, -10), Min(GetXDir() + 2, 10)));
}

protected func FxIntWindYDirTimer()
{
  if(GBackSolid(0, GetObjHeight()/2+2)) return -1;
  SetYDir(BoundBy(GetYDir() + RandomX(-GetYDir()) - 1 + Random(3), Max(GetYDir() - 1, -3), Min(GetYDir() + 1, 3)));
}

private func FloatProcess()
{
  if(!GetEffect("IntWindXDir", this))
    AddEffect("IntWindXDir", this, 1, 5, this);

  if(!GetEffect("IntWindYDir", this) && !GBackSolid(0, GetObjHeight()/2+2))
  {
    AddEffect("IntWindYDir", this, 1, 10, this);
  }

//  if(GetComDir() == COMD_Stop && GBackSolid(0, GetObjHeight()/2+2))
//    SetComDir(COMD_Down);

  NoPilotCheck();
  CheckCommandAction();
  return 1;
}

private func FlightProcess()
{
  NoPilotCheck();
  CheckCommandAction();
  return 1;
}

private func CheckCommandAction()
{
  // Propeller an für Kommando
  if (GetCommand()) 
    if (GetAction()=="FloatIdle") 
      SetAction("FloatPropel");
  // Wenden nach rechts
  if (GetAction() == "FloatPropel")
    if (Inside(GetComDir(),COMD_UpRight,COMD_DownRight)) 
      if (GetDir()==DIR_Left)
        { SetAction("Turn"); SetDir(DIR_Right); return 1; }
  // Wenden nach links
  if (GetAction() == "FloatPropel")
    if (Inside(GetComDir(),COMD_DownLeft,COMD_UpLeft)) 
      if (GetDir()==DIR_Right)
        { SetAction("Turn"); SetDir(DIR_Left); return 1; }
  // Anhalten wenn Ziel erreicht
  if(!GetCommand() && !GetComDir())
    SetDirection(COMD_Stop);
  return;  
}   

private func NoPilotCheck()
{
  if (!FindObject2(Find_Container(this), Find_OCF(OCF_CrewMember))) 
    return FloatIdleDown();
  return;
}

private func FloatIdleDown()
{
  if (GetAction()!="FloatIdle")
    SetAction("FloatIdle");
  SetComDir(COMD_Down);
  ClearCommand();
  return 1;
}

/* Trefferkontrolle */

protected func RejectCollect(id idObject, object pObj)
{
  // Pfeile vertragen sich nicht mit !=eim Luftschiff
  if (DefinitionCall(idObject,"IsArrow") )
   // Nur, wenn der Pfeil sich auch bewegt
   if(GetXDir(pObj) || GetYDir(pObj))
  {
    // bei brennendem Pfeil anzünden
    if(OnFire(pObj)) Incinerate();
    // ansonsten abstürzen
    else SetAction("DropOff");
    // in jedem Fall im Pfeil Hit auslösen (Sprengpfeil explodiert zB)
    ProtectedCall(pObj, "Hit");
    return 1;
  }
  if(idObject == LBMB)
  	return;
  return 1;
}

/* Angriff durch Vögel */

public func Poke()
{
  Sound("Rip");
  SetAction("DropOff");
  return 1;
}
  
/* Forschung */

public func GetResearchBase() { return(BALN); }
