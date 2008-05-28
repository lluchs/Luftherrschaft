/*-- Das Seil --*/

#strict 2

#include 1E1E

func DingD() {
  var Target1 = GetRealContainer(GetActionTarget(0));
  var Target2 = GetRealContainer(GetActionTarget(1));
  MoveRope(GetActionTarget(0), GetActionTarget(1));
  if(Target1 == Target2) return 0;
  CalcedLength = CalcLength(Target1, Target2);
  var Difference = CalcedLength - Length;
  Length = Max(Length + GetActionTarget(0)->~RopeAskChangeLength(Difference, this), 3);
  Length = Max(Length + GetActionTarget(1)->~RopeAskChangeLength(Difference, this), 3);
  Difference = CalcedLength - Length;
  if(Difference <= 0) return 0;
  var iMass = GetMass(Target1) + GetMass (Target2);
  Do_Stuff(Target1, Target2, iMass, Difference, 1, Local(2));
  Do_Stuff(Target2, Target1, iMass, Difference, GetVertexNum () - 2, Local(3));
}

func DingS() {
  var Target1 = GetRealContainer(GetActionTarget(0));
  var Target2 = GetRealContainer(GetActionTarget(1));
  // keine Ziele mehr?
  if(!Target1 && !Target2) return 0;
  // Clonk soll nicht "tumbeln"
  if(GetAction(Target1) == "Tumble") Target1->~SetAction("Jump");
  MoveRope(GetActionTarget(0), GetActionTarget(1));
  if(Target1 == Target2) return 0;
  CalcedLength = CalcLength(Target1, Target2);
  var Difference = CalcedLength - Length;
  Length = Max(Length + GetActionTarget(0)->~RopeAskChangeLength(Difference, this), 3);
  Length = Max(Length + GetActionTarget(1)->~RopeAskChangeLength(Difference, this), 3);
  Difference = CalcedLength - Length;
  if(Difference <= 0) return 0;
  Do_Stuff(Target1, Target2, GetMass (Target2), Difference, 1, Local (2));
}

func Ding0() {
  var Target1 = GetRealContainer(GetActionTarget(0));
  var Target2 = GetRealContainer(GetActionTarget(1));
  MoveRope(Target1, Target2);
  if (Target1 == Target2) return 0;
}

public func SetLength(nlen) { return Length=Max(nlen,10); }

public func LineBreak() { return 0; }