/*-- Steamboat Willie! --*/

#strict 2
#include L103
#include SAVS

static const LSMS_MaxFill_Pressure = 2000;
static const LSMS_MaxFill_Water = 600;
static const LSMS_MaxFill_Coal = 6;

local fWait, fOff;

protected func Initialize() {
	SetAction("NoEnergy");
	fWait = 0;
	return _inherited(...);
}

protected func Collection(object pObj)
{
  Sound("Clonk");
}

/* Produktion */

private func ProduceEnergy()
{
	//Log("ProduceEnergy");
	if(fOff) {
		SetAction("NoEnergy");
		fOff = 0;
		return;
	}
	if(!IsFull("Pressure") && GetAmount("Water") >= 200 && GetAmount("Coal") >= 2) { // Neuer Druck erstellen
  	// Rauch ANPASSEN!!
  	CreateParticle("Smoke", -10, -10, 0, -10, RandomX(5, 10), RGBa(0, 0, 0, RandomX(100, 255)));
  	DoFill(-200, "Water"); DoFill(-2, "Coal");
  	DoFill(400, "Pressure");
  	//if(DoFill(400, "Pressure") < 400)
  		//Sound("pfft");
  }
  var iChange;
  if(iChange = DoFill(-25, "Pressure") == -25)
  	// Energieerzeugung
  	DoEnergy(+25);
  else {
  	DoFill(Abs(iChange), "Pressure");
  	//Log("Kein Druck mehr");
  	Message("Druckvorrat aufgebraucht!", this);
  	// Fertig
  	SetAction("NoEnergy");
  }
  if(GetEnergy() == GetDefCoreVal("Energy", "Physical") / 1000) {
  	fWait = 1;
  	//Log("Energielager voll");
  	Message("Energielager voll!", this);
  	SetAction("NoEnergy");
  }
}

protected func NoEnergy() {
	if(fWait && CheckEnergyNeedChain()) {
		SetAction("ProduceEnergy");
		fWait = 0;
	}
}

public func IsOff() { return GetAction() == "NoEnergy" && !fWait; }
public func IsOn() { return !IsOff(); }

/* Steuerung */

protected func ControlUp() {
	if(GetAction() == "NoEnergy" && !fWait)
		SetAction("ProduceEnergy");
	else if(GetAction() == "ProduceEnergy")
		fOff = 1;
	else if(fWait)
		fWait = 0;
}

protected func ContextOn() {
	[An|Image=LSMN|Condition=IsOff]
	ControlUp(...);
}

protected func ContextOff() {
	[Aus|Image=LSMN|Condition=IsOn]
	ControlUp(...);
}

/* Inhaltsüberprüfung */

protected func RejectCollect(id idObj, object pObj) {
	//Log("Aufgerufen, %i", idObj);
	if(idObj == LCAB) {
		DoFill(Abs(pObj -> DoFill(-pObj -> MaxFill("Pressure"))), "Pressure");
		return 1;
	}
	if(idObj == WBRL) {
		var iOldFill = pObj -> GetAmount();
		pObj -> BarrelDoFill(iOldFill - DoFill(Abs(pObj -> BarrelDoFill(-pObj -> BarrelMaxFill())), "Water"));
		//Sound("Splash1");
		return 1;
	}
	if(idObj == COAL) {
		if(DoFill(1, "Coal")) {
			RemoveObject(pObj);
			//Sound("Clonk");
			return;
		}
	}
	return 1;
}

private func FillSound(Key, int iChange) {
	if(iChange <= 0)
		return;
	if(Key == "Water")
		Sound("Splash1");
	if(Key == "Coal")
		Sound("Clonk");
	if(Key == "Pressure")
		Sound("pfft");
	return 1;
}
