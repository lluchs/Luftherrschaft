/*-- Bombenschiff --*/

#strict 2

/* Türsteuerung */

local fDoorOpen;

protected func ActivateEntrance(object pObj) {
	if(fDoorOpen)
		return 1;
	SetOverlayAction("Door", 2, false, true, 0, "DoorOpened");
	Sound("DoorOpen");
	fDoorOpen = 1;
	return 1;
}

public func DoorOpened(string szAction, int iOverlay) {
	SetEntrance(1);
	AddEffect("IntCloseDoor", this, 100, 15, this, GetID(), szAction, iOverlay);
}

public func FxIntCloseDoorStart(object pTarget, int iEffectNumber, int iTemp, string szAction, int iOverlay) {
	if(iTemp)
		return;
	EffectVar(0, pTarget, iEffectNumber) = szAction;
	EffectVar(1, pTarget, iEffectNumber) = iOverlay;
}

public func FxIntCloseDoorStop(object pTarget, int iEffectNumber, int iReason) {
	if(!iReason) {
		SetEntrance(0);
		SetOverlayAction(EffectVar(0, pTarget, iEffectNumber), EffectVar(1, pTarget, iEffectNumber), true, true, 0, "DoorClosed");
		Sound("DoorClose");
	}
}

public func DoorClosed(string szAction, int iOverlay) {
	fDoorOpen = 0;
	SetGraphics(0, this, 0, iOverlay);
}

protected func Ejection() {
	if(GetEffect("IntCloseDoor", this))
		ChangeEffect("IntCloseDoor", this, 0, 0, 15);
}

protected func Collection2() {
	Ejection();
}

local pController, bBombingOpened;

private func GetBombingX() {
	var iX = -42;
	if(GetDir() == DIR_Right)
		iX = Abs(iX);
	return iX;
}

private func GetBombingY() {
	return 56;
}

protected func ThrowBomb() {
	var pBomb = FindObject2(Find_Container(this), Find_ID(LBMB));
	var iXDir = GetXDir() / 10, iYDir = GetYDir() / 10;
	if(!iXDir) { // richtige Drehung wenn sich das Schiff nicht bewegt
		if(GetDir() == DIR_Right)
			iXDir = 1;
		else
			iXDir = -1;
	}
	Exit(pBomb, GetBombingX(), GetBombingY(), 0, iXDir, iYDir);
	CloseBombing();
}

protected func ContainedThrow(object pClonk) {
	["Bombe schmeißen!"|Image=LBMB]
	if(BombingIsOpen() || !FindObject2(Find_Container(this), Find_ID(LBMB)))
		return;
	OpenBombing("ThrowBomb");
	return 1;
}

private func SetAction(szAction) { // Tür als Overlay
	if(szAction == "OpenDoor")
		return SetOverlayAction(szAction, 2, false, true, 0, "CloseDoor");
	return inherited(szAction, ...);
}

public func CloseDoor(string szAction, int iOverlay) {
	return SetOverlayAction(szAction, iOverlay, true, true);
}

public func OpenBombing(string szEndCall) {
	if(BombingIsOpen())
		return;
	bBombingOpened = 1;
	return SetOverlayAction("OpenBombing", 1, szEndCall);
}

public func CloseBombing(string szEndCall) {
	if(!BombingIsOpen())
		return;
	bBombingOpened = 0;
	return SetOverlayAction("CloseBombing", 1, szEndCall);
}

public func BombingIsOpen() { return bBombingOpened; }

protected func ContainedDig(object pClonk) {
	["Bombe aufsammeln"|Image=_HAK]
	if(OpenBombing("CreateHook")) {
		pController = pClonk;
	}
	return 1;
}

protected func CreateHook() {
	if(!pController || (pController -> Contained()) != this) {
		CloseBombing();
		return;
	}
	//SetAction("FloatIdle");
	SetComDir(COMD_Stop);
	var pHaken = CreateObject(LOHK, GetBombingX(), GetBombingY(), GetOwner());
	pHaken -> SetShip(this);
	pController -> Enter(pHaken);
}

protected func GetVertexToConnect() {
	return;
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
    if (GetAction()=="DropOff") return 1;
    if (GetAction()=="Turn") return 1;
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
    if (GetAction()=="DropOff") return 1;
    if (GetAction()=="Turn") return 1;
    SetAction("FloatPropel");
    DoDirection(COMD_Left);
  }
  return 1;
}

protected func ContainedCommand(string szCommand,object pTarget,int iX,int iY)
{
  // Pilot verlässt das Schiff
  if (szCommand == "Exit") {
  	ClearCommand();
  	NoPilotCheck();
    return;
  }
  // Bewegungskommando vom Piloten
  if (szCommand == "MoveTo")
    return SetCommand(this,szCommand,pTarget,iX,iY);
  return;
}

private func ClearCommand() { SetCommand(this,"None"); }

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
  SetAction("OpenDoor");
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

/* Bau */

public func IsShipsmithProduct() { return true; }

// siehe Bauschiff für Beschreibung
public func NextBuildStep(int iPromille) {
  if(iPromille < 251)
    return true;
  if(iPromille > 838)
    return true;
  return false;
}
