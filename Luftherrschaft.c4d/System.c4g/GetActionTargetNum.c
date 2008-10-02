#strict 2

global func GetActionTargetNum(object pObj,object pOfObject) {
  if(GetActionTarget(0,pOfObject) == pObj) return 0;
  return 1;
}
