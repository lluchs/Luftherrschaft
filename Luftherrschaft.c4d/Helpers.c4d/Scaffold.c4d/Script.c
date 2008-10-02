/*-- Gerüst --*/

#strict 2

local construct, effectnum, builddir, build;
local lower, bg, onlybg;

protected func Initialize() {
  if(builddir != 1) {
    SetCon(2);
    build = 20;
  }
  BuildUp();
}

public func Init(object pConstruct, object pLower, int iEffectNum, bool bOnlyBg) {
	// Background hat keine Funktion -> wird von Vordergrundgesteuert.
	//Log("%v%v%v%v%v", Par(0), Par(1), Par(2), Par(3), Par(4));
	if(bOnlyBg)
		return onlybg = bOnlyBg;
  construct = pConstruct;
  effectnum = iEffectNum;
  lower = pLower;
  
  // ein Teil in Hintergrund setzen
  bg = CreateObject(GetID(), 0, 0, GetOwner());
  SetGraphics("Bg", bg, GetID(), 0, GFXOV_MODE_Base);
  SetGraphics("Fg", this, GetID(), 0, GFXOV_MODE_Base);
  // in den Hintergrund
  bg->SetCategory(C4D_StaticBack | C4D_Background);
  bg->BuildUp();
  bg->Init(0, 0, 0, true);
}

private func Building() {
  if(!construct && !onlybg)
    BuildDown(3);
  build += builddir * 10;
  
  if(NextBuildStep(build))
    SetCon(build / 10);
  if(GetCon() == 100 && builddir >= 1) {
    construct->~GerustComplete();
    BuildStop();
  }
  else
  {
    if(!Random(6) && builddir != 0)
      Sound("Build*");
    if(!onlybg) {
    	var construction = construct;
    	//if(GetID(construction) == LSSH)
    	//	if(construct->GetConstruction())
    	//  	construction = construct->GetConstruction();
    	if(GetCon(construction) >= 100)
    	  BuildDown(3);
    	if(GetCon() < 5 && lower)
    		lower->BuildDown(3);
    }
  }
}

public func NextBuildStep(int iMille) {
  if(iMille < 340)
    return true;
  if(Inside(iMille, 415, 745))
    return true;
  if(iMille > 790)
    return true;
  return false;
}

public func BuildDown(int speed) {
  if(!speed) speed = 1;
  builddir = -1 * speed;
  if(bg)
  	bg->~BuildDown(speed);
}

public func BuildStop() {
  builddir = 0;
  if(bg)
  	bg->~BuildStop();
}

public func BuildUp(int speed) {
  if(!speed) speed = 1;
  builddir = 1 * speed;
  if(bg)
  	bg->~BuildUp(speed);
}

public func Complete() {
 	return GetCon() >= 100;
}

public func ConstructionCompleted() { BuildDown(); }



