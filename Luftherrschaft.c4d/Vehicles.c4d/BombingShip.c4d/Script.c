/*-- Bombenschiff --*/

#strict 2

#include DOOR
#include BLMP

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
	if(WildcardMatch(szAction, "*Door*"))
		return SetOverlayAction(szAction, 2);
	return inherited(szAction, ...);
}

private func SetOverlayAction(string szAction, int iOverlay, string szEndCall) {
	if(!szAction)
		return;
	if(!iOverlay)
		iOverlay = GFX_Overlay;
	SetGraphics(0, 0, 0, iOverlay, GFXOV_MODE_Action, szAction);
	if(szEndCall)
		ScheduleCall(this, szEndCall, GetActMapVal("Length", szAction) * GetActMapVal("Delay", szAction));
	return 1;
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

/* Überladungen von BLMP */

protected func ContainedUpdate() {
  return ControlUpdate(...);
}

protected func ContainedUp()      // hoch
{
  [$TxtAscend$]
  return ControlUp(...);
}

protected func ContainedDownSingle()  // runter
{
  [$TxtDescend$|Method=Classic]
  return ControlDownSingle(...);
}

protected func ContainedDown(object controller)
{
  [$TxtDescend$|Method=JumpAndRun]
  // Lediglich fuer das Kommandosymbol bei JnR-Steuerung
  return 1;
}

protected func ContainedRight()       // rechts
{
  [$TxtTurnright$]
  ControlRight(...);
  return 1;
}

protected func ContainedLeft()        // links
{
  [$TxtTurnleft$]
  ControlLeft(...);
  return 1;
}

protected func ContainedCommand()
{
  return ControlCommand(...);
}

/* Initialisierung */

protected func Initialize() {

  SetEntrance(1);
  return _inherited(...);
}

/* Trefferkontrolle */

protected func RejectCollect(id idObject, object pObj) {
  if(idObject == LBMB || pObj -> GetOCF() & OCF_CrewMember)
  	return;
  return _inherited(idObject, pObj);
}
  
/* Forschung */

public func GetResearchBase() { return BALN; }

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
