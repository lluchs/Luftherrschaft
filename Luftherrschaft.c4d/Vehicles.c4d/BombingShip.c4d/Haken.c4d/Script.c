/* Haken */

#strict 2

local pRope, iPushPull, pShip, iCount, pBomb;

public func SetShip(object pObj) {
	pShip = pObj;
	pRope = CreateObject(LHR2, 0, 0, GetOwner());
	pRope -> Activate(pShip, this);
	return 1;
}

public func RopeAskChangeLength(int iLength, object pObj) {
  if(pObj != pRope)
  	return;
  //Log("Distanz: %d", ObjectDistance(this, pShip));
  if(iCount >= 50 && ObjectDistance(this, pShip) < 60) {
  	while(Contents())
  		Enter(pShip, Contents());
  	if(pBomb)
  		Enter(pShip, pBomb);
  	pShip -> CloseBombing();
  	RemoveObject(pRope);
  	RemoveObject();
  	return;
  }
  else
  	iCount++;
  if(!iPushPull) iPushPull = 1;
  return iPushPull;
}

protected func ContainedUp() {
	[Hoch|Image=LIRP]
	iPushPull = -1;
	while(GetContact() & CNAT_Bottom)
		SetPosition(GetX() - 1, GetY());
	return 1;
}

protected func ContainedDown() {
	[Runter|Image=LIRP]
	iPushPull = 1;
	return 1;
}

// mangels Grafiken leider nicht möglich (Klappe per Overlay oder so?)
protected func ContainedLeft() {
	[Schiff nach links|Image=LVBS]
	ProtectedCall(pShip, "ContainedLeft", ...);
	return 1;
}

protected func ContainedRight() {
	[Schiff nach rechts|Image=LVBS]
	ProtectedCall(pShip, "ContainedRight", ...);
	return 1;
}

protected func ContainedDig() {
	[Schiff stoppen|Image=LVBS]
	pShip -> SetComDir(COMD_Stop);
	return 1;
}

protected func ContainedDigDouble() {
	var pBombe = FindObject2(Find_ID(LBMB), Find_Distance(30), Sort_Distance());
	if(!pBombe) {
		Sound("Error");
		return;
	}
	//pBombe -> Attach(this);
	pBomb = pBombe;
	return 1;
}

protected func Destruction() {
	while(Contents())
		Enter(pShip, Contents());
  if(pRope)
  	RemoveObject(pRope);
}
