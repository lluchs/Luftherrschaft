/*-- Steamboat Willie! --*/

#strict 2
#include L103
#include SAVS

static const LSMN_MaxFill = 2000;
static const LSMN_MaxFill1 = 600;
static const LSMN_MaxFill2 = 6;

local fWait, fOff;

protected func Initialize() {
	SetAction("NoEnergy");
	fWait = 0;
	return inherited(...);
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
	if(!IsFull() && GetAmount(1) >= 200 && GetAmount(2) >= 2) { // Neuer Druck erstellen
  	// Rauch ANPASSEN!!
  	CreateParticle("Smoke", -10, -10, 0, -10, RandomX(5, 10), RGBa(0, 0, 0, RandomX(100, 255)));
  	DoFill(-200, 1); DoFill(-2, 2);
  	if(DoFill(400) < 400)
  		Sound("pfft");
  }
  var iChange;
  if(iChange = DoFill(-25) == -25)
  	// Energieerzeugung
  	DoEnergy(+25);
  else {
  	DoFill(Abs(iChange));
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
		DoFill(Abs(pObj -> DoFill(-pObj -> MaxFill())));
		return 1;
	}
	if(idObj == WBRL) {
		DoFill(Abs(pObj -> BarrelDoFill(-pObj -> BarrelMaxFill())));
		return 1;
	}
	if(idObj == COAL) {
		if(DoFill(1, 2)) {
			Schedule(Format("RemoveObject(Object(%d))", ObjectNumber(pObj)), 1);
			return;
		}
	}
	return 1;
}
